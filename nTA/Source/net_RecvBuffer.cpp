// net_RecvBuffer.cpp //                           \author Logan Jones
///////////////////////                              \date 03-28-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "net.h"
#include "net_RecvBuffer.h"
#include "app.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
net_RecvBuffer::net_RecvBuffer():
	m_Data( NULL ),
	m_DataSize( 0 ),
	m_BackToFront( ~1 )
{}
net_RecvBuffer::~net_RecvBuffer()
{Free();}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::Initialize() //                 \author Logan Jones
/////////////////////////////////                    \date 03-31-2003
//
//===================================================================
// Parameters:
//  int iBufferSize - 
//
// Return: bool - 
//
bool net_RecvBuffer::Initialize( int iBufferSize )
{
	// Safety
	Free();

	// Allocate
	m_Data = new char[ m_DataSize=iBufferSize ];
	if( m_Data==NULL ) return false;

	// Initialize
	m_WorkingPacket = m_Data;
	m_EndOfWorkingPacket = m_Data;
	m_ReadCursor = m_EndOfWorkingPacket;
	m_NextRecv = m_Data;
	((net_PacketHeader*)m_NextRecv)->Size = 0;
	((net_PacketHeader*)m_NextRecv)->Marker = 0;

	return true;
}
// End net_RecvBuffer::Initialize()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::Free() //                       \author Logan Jones
///////////////////////////                          \date 03-31-2003
//
//===================================================================
//
void net_RecvBuffer::Free()
{
	SAFE_DELETE_ARRAY( m_Data );
	m_DataSize = 0;
}
// End net_RecvBuffer::Free()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::InitializeSender() //           \author Logan Jones
///////////////////////////////////////              \date 04-02-2003
//
//===================================================================
// Parameters:
//  net_Client& sender - new sender
//
void net_RecvBuffer::InitializeSender( net_Client& sender )
{
	sender.Recv.BytesLeft = 0;
}
// End net_RecvBuffer::InitializeSender()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::RecvFrom() //                   \author Logan Jones
///////////////////////////////                      \date 03-31-2003
//
//===================================================================
// Parameters:
//  net_Client& sender - 
//
// Return: bool - return true if the sender is still valid
//
bool net_RecvBuffer::RecvFrom( net_Client& sender )
{
	int	BytesReceived;
	theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::RecvFrom(): Recv from net_Client(%d,%d)", sender.ID, sender.Socket );

	// start a new packet?
	if( sender.Recv.BytesLeft==0 )
		if( !NewPacket(sender) ) return false;

	// recv data from sender
	BytesReceived = recv( sender.Socket, sender.Recv.Cursor, sender.Recv.BytesLeft, 0 ),
	sender.Recv.BytesLeft -= BytesReceived,
	sender.Recv.Cursor += BytesReceived;
	theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::RecvFrom(): Recvieved %d bytes", BytesReceived );
	
	// packet complete?
	if( sender.Recv.BytesLeft==0 )
		theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::RecvFrom(): Packet complete" ),
		((net_PacketHeader*)sender.Recv.Start)->Size = sender.Recv.Cursor - sender.Recv.Start;

	// sender valid?
	return BytesReceived>0;
}
// End net_RecvBuffer::RecvFrom()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::RecvFrom() //                   \author Logan Jones
///////////////////////////////                      \date 04-03-2003
//
//===================================================================
// Parameters:
//  SOCKET sender - 
//
void net_RecvBuffer::RecvFrom( SOCKET sender )
{
	ssize_t	BytesReceived;
	sockaddr_in	SourceAddress;
	socklen_t   SourceAddressSize = sizeof( SourceAddress );
	theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::RecvFrom(): Recv from SOCKET %d", sender );

	// Back to front?
	if( 512>((m_Data+m_DataSize)-m_NextRecv) )
		((net_PacketHeader*)m_NextRecv)->Marker = m_BackToFront,
		m_NextRecv = m_Data;

	BytesReceived = recvfrom( sender, m_NextRecv, ((m_Data+m_DataSize)-m_NextRecv), 0, (sockaddr*)&SourceAddress, &SourceAddressSize );
	theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::RecvFrom(): Recvieved %d bytes from %s", BytesReceived, inet_ntoa(SourceAddress.sin_addr) );
	if( BytesReceived && ((net_PacketHeader*)m_NextRecv)->Marker==net_PacketMarker )
		((net_PacketHeader*)m_NextRecv)->Marker = SourceAddress.sin_addr.s_addr,
		m_NextRecv += ((net_PacketHeader*)m_NextRecv)->Size,
		((net_PacketHeader*)m_NextRecv)->Size = 0;
}
// End net_RecvBuffer::RecvFrom()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::Inject() //                     \author Logan Jones
/////////////////////////////                        \date 04-03-2003
//
//===================================================================
// Parameters:
//  const char* pData - 
//  UINT32 uiDataSize - 
//  UINT32 uiID       - 
//
void net_RecvBuffer::Inject( const char* pData, UINT32 uiDataSize, UINT32 uiID )
{
	// Back to front?
	if( uiDataSize>((m_Data+m_DataSize)-m_NextRecv) )
		((net_PacketHeader*)m_NextRecv)->Marker = m_BackToFront,
		m_NextRecv = m_Data;

	((net_PacketHeader*)m_NextRecv)->Marker = uiID;
	((net_PacketHeader*)m_NextRecv)->Size = uiDataSize + sizeof(net_PacketHeader);
	memcpy( m_NextRecv + sizeof(net_PacketHeader), pData, uiDataSize );
	m_NextRecv += ((net_PacketHeader*)m_NextRecv)->Size;
	((net_PacketHeader*)m_NextRecv)->Size = 0;
}
// End net_RecvBuffer::Inject()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::BytesToRead() //                \author Logan Jones
//////////////////////////////////                   \date 04-02-2003
//
//===================================================================
// Return: bool - return true if there is anything to be read
//
bool net_RecvBuffer::BytesToRead()
{
	// Read all of m_WorkingPacket?
	if( m_ReadCursor>=m_EndOfWorkingPacket ) {
		m_WorkingPacket = m_EndOfWorkingPacket;

		// Back to front of m_Data?
		if( ((net_PacketHeader*)m_WorkingPacket)->Marker==m_BackToFront )
			m_WorkingPacket = m_Data,
			m_EndOfWorkingPacket = m_Data,
			m_ReadCursor = m_EndOfWorkingPacket;

		// Next packet ready?
		if( ((net_PacketHeader*)m_WorkingPacket)->Size ) {
			m_ReadCursor = m_WorkingPacket + sizeof(net_PacketHeader),
			m_EndOfWorkingPacket = m_WorkingPacket + ((net_PacketHeader*)m_WorkingPacket)->Size;
			return true;
		} else return false;

	// Keep reading m_WorkingPacket
	} else return true;
}
// End net_RecvBuffer::BytesToRead()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_RecvBuffer::NewPacket() //                  \author Logan Jones
////////////////////////////////                     \date 03-31-2003
//
//===================================================================
// Parameters:
//  net_Client& sender - 
//
// Return: bool - 
//
bool net_RecvBuffer::NewPacket( net_Client& sender )
{
	int					BytesReceived;
	net_PacketHeader	Header;

	//
	BytesReceived = recv( sender.Socket, (char*)&Header, sizeof(Header), 0 );

	// socket closed?
	if( BytesReceived<=0 )
		return false;
//	else if( BytesReceived<0 )
//		{BytesReceived = WSAGetLastError(); assert(0);}

	assert( BytesReceived==sizeof(Header) && Header.Marker==net_PacketMarker && Header.Size<m_DataSize );
	theApp.Console.Comment( CT_DEBUG, "net_RecvBuffer::NewPacket(): New %d byte packet", Header.Size );

	// Back to front?
	if( Header.Size>((m_Data+m_DataSize)-m_NextRecv) )
		((net_PacketHeader*)m_NextRecv)->Marker = m_BackToFront,
		m_NextRecv = m_Data;

	Header.Marker = sender.ID;
	memcpy( m_NextRecv, &Header, sizeof(Header) );

	((net_PacketHeader*)m_NextRecv)->Marker = sender.ID;
	((net_PacketHeader*)m_NextRecv)->Size = 0;
	sender.Recv.Start = m_NextRecv;
	sender.Recv.Cursor = m_NextRecv + sizeof(Header);
	sender.Recv.BytesLeft = Header.Size - sizeof(Header);
	m_NextRecv += Header.Size;
	((net_PacketHeader*)m_NextRecv)->Size = 0;

	return true;
}
// End net_RecvBuffer::NewPacket()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - net_RecvBuffer.cpp //
/////////////////////////////