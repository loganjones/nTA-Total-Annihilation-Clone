// net_SendBuffer.h //                             \author Logan Jones
/////////////////////                                \date 03-31-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_SENDBUFFER_H_
#define _NET_SENDBUFFER_H_
/////////////////////////////////////////////////////////////////////

#define net_SendToAll		(~0)

////////////////////////// net_SendBuffer ///////////////////////////
//
class net_SendBuffer
{
	friend class net_Sockets;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Initialize/Destroy the recv buffer object
	bool Initialize( int iBufferSize );
	void Free();

	// Setup a new target
	void InitializeTarget( net_Client& target );

	// Send data to a net_Client
	void SendTo( net_Client& target );

	// Tries to finish the m_WorkingPacket to a complete state
	void CompletePacket();

	// Prepare the buffer to be written to
	net_SendBuffer& Write( UINT32 uiTarget=net_SendToAll, UINT32 uiSize=32 );/*
	{
		if( ((net_PacketHeader*)m_WorkingPacket)->Marker!=uiTarget || (m_WriteCursor-m_WorkingPacket)>(512-uiSize) )
			NewPacket( uiTarget, uiSize );
		return *this;
	}*/

	// Copy data into the buffer the ostream way
	//template <typename T> net_SendBuffer& operator<<( const T& Data )
	//{memcpy(m_WriteCursor,&Data,sizeof(T)); m_WriteCursor+=sizeof(T); return *this;}
	net_SendBuffer& operator<<( const INT8& Data ) {*(m_WriteCursor++)=Data; return *this;}
	net_SendBuffer& operator<<( const UINT8& Data ) {*(m_WriteCursor++)=Data; return *this;}
	net_SendBuffer& operator<<( const INT16& Data ) { ((INT16*)m_WriteCursor)[0]=htons(Data); m_WriteCursor+=2; return *this;}
	net_SendBuffer& operator<<( const UINT16& Data ) { ((UINT16*)m_WriteCursor)[0]=htons(Data); m_WriteCursor+=2; return *this;}
	net_SendBuffer& operator<<( const INT32& Data ) { ((INT32*)m_WriteCursor)[0]=htonl(Data); m_WriteCursor+=4; return *this;}
	net_SendBuffer& operator<<( const UINT32& Data ) { ((UINT32*)m_WriteCursor)[0]=htonl(Data); m_WriteCursor+=4; return *this;}
	net_SendBuffer& operator<<( const std_Point& Data ) { ((INT32*)m_WriteCursor)[0]=htonl(Data.x); ((INT32*)m_WriteCursor)[1]=htonl(Data.y); m_WriteCursor+=8; return *this;}
	net_SendBuffer& operator<<( const char* Data ) {while(*Data!='\0') *(m_WriteCursor++)=*(Data++); *(m_WriteCursor++)='\0'; return *this;}


/////////////////////////////////////////////////
// Data members
private:

	// The actual data buffer consisting of char[m_DataSize]
	char*			m_Data;
	int				m_DataSize;
	char*			m_EndOfData;

	// Position in m_Data that is to be written to
	char*			m_WriteCursor;

	// Packet that is being written to
	char*			m_WorkingPacket;

	// Size value indicating the m_WriteCursor should be wrapped back to m_Data
	const UINT32	m_BackToFront;


/////////////////////////////////////////////////
// Utility methods
private:

	// Find the next packet (if any) to send to target
	bool NextPacket( net_Client& target );

	// Finish m_WorkingPacket and start a new one
	void NewPacket( UINT32 uiTarget, UINT32 uiSize );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	net_SendBuffer();
	virtual ~net_SendBuffer();
/////////////////////////////////////////////////

}; // End class - net_SendBuffer
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_SENDBUFFER_H_)
