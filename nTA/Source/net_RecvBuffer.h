// net_RecvBuffer.h //                             \author Logan Jones
/////////////////////                                \date 03-28-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_RECVBUFFER_H_
#define _NET_RECVBUFFER_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// net_RecvBuffer ///////////////////////////
//
class net_RecvBuffer
{
	friend class net_Sockets;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Initialize/Destroy the recv buffer object
	bool Initialize( int iBufferSize );
	void Free();

	// Setup a new sender
	void InitializeSender( net_Client& sender );

	// Receive data from a net_Client
	bool RecvFrom( net_Client& sender );

	// Recieve data from some UDP socket and add to m_Data
	void RecvFrom( SOCKET sender );

	// Inject some data into the recv buffer
	void Inject( const char* pData, UINT32 uiDataSize, UINT32 uiID );

	// Are there any unread bytes in m_Data
	bool BytesToRead();

	UINT32 Marker() const {return ((net_PacketHeader*)m_WorkingPacket)->Marker;}

	INT8 GetInt8() {return *(m_ReadCursor++);}
	UINT8 GetUInt8() {return *(m_ReadCursor++);}
	INT16 GetInt16() {return ntohs(*((u_short*)((m_ReadCursor+=2)-2)));}
	UINT16 GetUInt16() {return ntohs(*((u_short*)((m_ReadCursor+=2)-2)));}
	INT32 GetInt32() {return ntohl(*((u_long*)((m_ReadCursor+=4)-4)));}
	UINT32 GetUInt32() {return ntohl(*((u_long*)((m_ReadCursor+=4)-4)));}
	std_Point GetPoint() {m_ReadCursor+=8; return std_Point(ntohl(*((u_long*)(m_ReadCursor-8))),ntohl(*((u_long*)(m_ReadCursor-4))));}
	const char* GetString() {const char* str=m_ReadCursor; m_ReadCursor+=strlen(m_ReadCursor)+1; return str;}
	const char* GetBytes( unsigned int uiAmount ) {const char* buf=m_ReadCursor; m_ReadCursor+=uiAmount; return buf;}

	// Copy data out of the buffer the istream way
	net_RecvBuffer& operator>>( INT8& Data ) {Data=GetInt8(); return *this;}
	net_RecvBuffer& operator>>( UINT8& Data ) {Data=GetUInt8(); return *this;}
	net_RecvBuffer& operator>>( INT16& Data ) {Data=GetInt16(); return *this;}
	net_RecvBuffer& operator>>( UINT16& Data ) {Data=GetUInt16(); return *this;}
	net_RecvBuffer& operator>>( INT32& Data ) {Data=GetInt32(); return *this;}
	net_RecvBuffer& operator>>( UINT32& Data ) {Data=GetUInt32(); return *this;}
	net_RecvBuffer& operator>>( std_Point& Data ) {Data=GetPoint(); return *this;}
	net_RecvBuffer& operator>>( char* Data ) {while(*m_ReadCursor!='\0') *(Data++)=*(m_ReadCursor++); *(Data++)=*(m_ReadCursor++); return *this;}

	// Get whatever the read cursor is currently at plus a given offset
	INT8& Int8At( long lByteOffset ) {return *(INT8*)(m_ReadCursor + lByteOffset);}
	UINT8& UInt8At( long lByteOffset ) {return *(UINT8*)(m_ReadCursor + lByteOffset);}
	INT16& Int16At( long lByteOffset ) {INT16* mem=(INT16*)(m_ReadCursor+lByteOffset); return (*mem=ntohs(*mem));}
	UINT16& UInt16At( long lByteOffset ) {UINT16* mem=(UINT16*)(m_ReadCursor+lByteOffset); return (*mem=ntohs(*mem));}
	INT32& Int32At( long lByteOffset ) {INT32* mem=(INT32*)(m_ReadCursor+lByteOffset); return (*mem=ntohl(*mem));}
	UINT32& UInt32At( long lByteOffset ) {UINT32* mem=(UINT32*)(m_ReadCursor+lByteOffset); return (*mem=ntohl(*mem));}
	std_Point& PointAt( long lByteOffset ) {INT32* mem=(INT32*)(m_ReadCursor+lByteOffset); mem[0]=ntohl(mem[0]); mem[1]=ntohl(mem[1]); return *((std_Point*)mem);}
	void MoveCursor( long lByteOffset ) {m_ReadCursor+=lByteOffset;}


/////////////////////////////////////////////////
// Data members
private:

	// The actual data buffer consisting of char[m_DataSize]
	char*			m_Data;
	int				m_DataSize;

	// Pointer to where the next recv will be placed in m_Data
	char*			m_NextRecv;

	// Packet that is being read from
	char*			m_WorkingPacket;
	char*			m_EndOfWorkingPacket;

	// Position in m_WorkingPacket from which to read
	char*			m_ReadCursor;

	// Marker value indicating the m_ReadCursor should be wrapped back to m_Data
	const UINT32	m_BackToFront;


/////////////////////////////////////////////////
// Utility methods
private:

	// Start reading a new packet from sender
	bool NewPacket( net_Client& sender );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	net_RecvBuffer();
	virtual ~net_RecvBuffer();
/////////////////////////////////////////////////

}; // End class - net_RecvBuffer
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_RECVBUFFER_H_)
