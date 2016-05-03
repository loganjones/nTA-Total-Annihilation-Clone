// net_SendBuffer.cpp //                           \author Logan Jones
///////////////////////                              \date 03-31-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "net.h"
#include "net_SendBuffer.h"
#include "app.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
net_SendBuffer::net_SendBuffer():
	m_Data( NULL ),
	m_DataSize( 0 ),
	m_BackToFront( ~0 )
{}
net_SendBuffer::~net_SendBuffer()
{Free();}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::Initialize() //                 \author Logan Jones
/////////////////////////////////                    \date 03-31-2003
//
//===================================================================
// Parameters:
//  int iBufferSize - 
//
// Return: bool - 
//
bool net_SendBuffer::Initialize( int iBufferSize )
{
	// Safety
	Free();

	// Allocate
	m_Data = new char[ m_DataSize=iBufferSize ];
	if( m_Data==NULL ) return false;

	// Initialize
	m_EndOfData = m_Data + m_DataSize;
	m_WorkingPacket = m_Data;
	((net_PacketHeader*)m_WorkingPacket)->Marker = (~1);
	((net_PacketHeader*)m_WorkingPacket)->Size = 0;
	m_WriteCursor = m_WorkingPacket;

	return true;
}
// End net_SendBuffer::Initialize()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::Free() //                       \author Logan Jones
///////////////////////////                          \date 03-31-2003
//
//===================================================================
//
void net_SendBuffer::Free()
{
	SAFE_DELETE_ARRAY( m_Data );
	m_DataSize = 0;
}
// End net_SendBuffer::Free()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::InitializeTarget() //           \author Logan Jones
///////////////////////////////////////              \date 04-02-2003
//
//===================================================================
// Parameters:
//  net_Client& target - new target
//
void net_SendBuffer::InitializeTarget( net_Client& target )
{
	target.Send.BytesLeft = 0;
	target.Send.Cursor = m_WorkingPacket;
}
// End net_SendBuffer::InitializeTarget()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::SendTo() //                     \author Logan Jones
/////////////////////////////                        \date 03-31-2003
//
//===================================================================
// Parameters:
//  net_Client& target - send target
//
void net_SendBuffer::SendTo( net_Client& target )
{
	ssize_t BytesSent;
	UINT32	Destination;

	// Send data that remains in the packet
	if( target.Send.BytesLeft )
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::SendTo(): Sending %d bytes to net_Client(%d,%d)", target.Send.BytesLeft, target.ID, target.Socket ),
		BytesSent = send( target.Socket, target.Send.Cursor, target.Send.BytesLeft, 0 ),
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::SendTo(): Sent %d bytes", BytesSent ),
		target.Send.BytesLeft -= BytesSent,
		target.Send.Cursor += BytesSent;

	// or find a new packet and send it (note that the net_PacketMarker is added)
	else if( NextPacket(target) )
		Destination = ((net_PacketHeader*)target.Send.Cursor)->Marker,
		((net_PacketHeader*)target.Send.Cursor)->Marker = net_PacketMarker,
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::SendTo(): Sending %d bytes to net_Client(%d,%d)", target.Send.BytesLeft, target.ID, target.Socket ),
		BytesSent = send( target.Socket, target.Send.Cursor, target.Send.BytesLeft, 0 ),
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::SendTo(): Sent %d bytes", BytesSent ),
		target.Send.BytesLeft -= BytesSent,
		((net_PacketHeader*)target.Send.Cursor)->Marker = Destination,
		target.Send.Cursor += BytesSent;
}
// End net_SendBuffer::SendTo()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::CompletePacket() //             \author Logan Jones
/////////////////////////////////////                \date 04-02-2003
//
//===================================================================
//
void net_SendBuffer::CompletePacket()
{
	if( (m_WriteCursor - m_WorkingPacket)>sizeof(net_PacketHeader) )
		((net_PacketHeader*)m_WorkingPacket)->Size = m_WriteCursor - m_WorkingPacket,
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::CompletePacket(): %d byte packet to %d completed", ((net_PacketHeader*)m_WorkingPacket)->Size, ((net_PacketHeader*)m_WorkingPacket)->Marker ),
		m_WorkingPacket = m_WriteCursor,
		((net_PacketHeader*)m_WorkingPacket)->Marker = ~1;
}
// End net_SendBuffer::CompletePacket()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::NextPacket() //                 \author Logan Jones
/////////////////////////////////                    \date 04-02-2003
//
//===================================================================
// Parameters:
//  net_Client& target - send target
//
// Return: bool - return true if a packet to send was found
//
bool net_SendBuffer::NextPacket( net_Client& target )
{
	// Iterate until we encounter the working packet
	while( target.Send.Cursor!=m_WorkingPacket )

		// Size == m_BackToFront if we are supposed to loop back
		// to the start of the buffer
		if( ((net_PacketHeader*)target.Send.Cursor)->Size==m_BackToFront )
			target.Send.Cursor = m_Data;

		// If the marker matches then send this packet
		else if( ((net_PacketHeader*)target.Send.Cursor)->Marker==net_SendToAll ||
			((net_PacketHeader*)target.Send.Cursor)->Marker==target.ID )
		{
			target.Send.BytesLeft = ((net_PacketHeader*)target.Send.Cursor)->Size;
			return true;
		}

		// Move to next packet
		else target.Send.Cursor += ((net_PacketHeader*)target.Send.Cursor)->Size;

	// Nothing sendable found
	return false;
}
// End net_SendBuffer::NextPacket()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_SendBuffer::NewPacket() //                  \author Logan Jones
////////////////////////////////                     \date 04-02-2003
//
//===================================================================
// Parameters:
//  UINT32 uiTarget - 
//  UINT32 uiSize   - 
//
void net_SendBuffer::NewPacket( UINT32 uiTarget, UINT32 uiSize )
{
	((net_PacketHeader*)m_WorkingPacket)->Size = m_WriteCursor - m_WorkingPacket;
	theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::NewPacket( %d, %u ); Old(%d,%u)", uiTarget, uiSize, ((net_PacketHeader*)m_WorkingPacket)->Marker, ((net_PacketHeader*)m_WorkingPacket)->Size ),
	m_WorkingPacket = m_WriteCursor;

	if( (m_EndOfData-m_WorkingPacket)<max(uiSize,512) )
		theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::NewPacket(): Back to front" ),
		((net_PacketHeader*)m_WorkingPacket)->Size = m_BackToFront,
		m_WorkingPacket = m_Data;

	((net_PacketHeader*)m_WorkingPacket)->Marker = uiTarget;
	((net_PacketHeader*)m_WorkingPacket)->Size = 0;
	m_WriteCursor = m_WorkingPacket + sizeof(net_PacketHeader);
}
// End net_SendBuffer::NewPacket()
/////////////////////////////////////////////////////////////////////


net_SendBuffer& net_SendBuffer::Write( UINT32 uiTarget, UINT32 uiSize )
{
	if( ((net_PacketHeader*)m_WorkingPacket)->Marker!=uiTarget || (m_WriteCursor-m_WorkingPacket)>(512-uiSize) )
		NewPacket( uiTarget, uiSize );
	else theApp.Console.Comment( CT_DEBUG, "net_SendBuffer::Write(): Appending to packet( %d, 0x%X )", ((net_PacketHeader*)m_WorkingPacket)->Marker, m_WorkingPacket );
	return *this;
}


/////////////////////////////////////////////////////////////////////
// End - net_SendBuffer.cpp //
/////////////////////////////