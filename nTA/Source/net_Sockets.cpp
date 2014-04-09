// net_Sockets.cpp //                              \author Logan Jones
////////////////////                                   \date 9/3/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "net.h"
#include "net_Sockets.h"
#include "app.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
net_Sockets::net_Sockets():
	m_NetworkAvailable( false ),
	m_IsServer( false ),
	m_Connected( false ),
	m_Listening( false ),
	m_nTAPort( 9119 ),
	m_QueryPort( 9118 ),
	m_ConnectionCount( 0 ),
	m_NextID( 0 ),
	m_QuerySocket( INVALID_SOCKET ),
	m_ListenSocket( INVALID_SOCKET )
{}
net_Sockets::~net_Sockets()
{Destroy();}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::Create() //                        \author Logan Jones
//////////////////////////                              \date 9/3/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL net_Sockets::Create()
{
#ifdef _WIN32
	WSADATA	wsad;

	// Attempt startup
	if( WSAStartup(MAKEWORD(2,2),&wsad)!=0 )
	{
		theApp.Console.Comment( CT_ERROR, "Warning: Winsock 2.2 not available." );
		return FALSE;
	}

	// Verify version
	if( wsad.wVersion!=MAKEWORD(2,2) )
	{
		theApp.Console.Comment( CT_ERROR, "Warning: Winsock 2.2 not available." );
		WSACleanup();
		return FALSE;
	}
#endif

	m_SendBuffer.Initialize( 1024 * 32 );
	m_RecvBuffer.Initialize( 1024 * 32 );
	FD_ZERO( &m_Selectables );

	// Success
	m_NetworkAvailable = true;
	return TRUE;
}
// End net_Sockets::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::Destroy() //                       \author Logan Jones
///////////////////////////                             \date 9/3/2002
//               
//====================================================================
//
void net_Sockets::Destroy()
{
	EndSession();
	m_NetworkAvailable = false;
#ifdef _WIN32
	WSACleanup();
#endif
}
// End net_Sockets::Destroy()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::StartServer() //                   \author Logan Jones
///////////////////////////////                      \date 03-26-2003
//
//===================================================================
// Parameters:
//  char* pQueryResponse      - 
//  DWORD dwQueryResponseSize - 
//
// Return: bool - 
//
bool net_Sockets::StartServer( char* pQueryResponse, DWORD dwQueryResponseSize )
{
	// Make sure the network is not already active
	if( m_Connected )
		return false;

	// Clear connections
	EndSession();

	// Setup the server's query and listen sockets
	m_IsServer = true;
	SetupQuerySockets();

	// Save query response info
	m_QueryResponseSize = sizeof(net_PacketHeader) + dwQueryResponseSize;
	m_QueryResponse = new char[m_QueryResponseSize];
	((net_PacketHeader*)m_QueryResponse)->Marker = net_PacketMarker;
	((net_PacketHeader*)m_QueryResponse)->Size = m_QueryResponseSize;
	memcpy( m_QueryResponse + sizeof(net_PacketHeader), pQueryResponse, dwQueryResponseSize );

	// Complete
	m_Connected = true;
	return true;
}
// End net_Sockets::StartServer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::SendRecv() //                      \author Logan Jones
////////////////////////////                         \date 03-26-2003
//
//===================================================================
//
void net_Sockets::SendRecv( bool* bQueried )
{
	ClientList_t::iterator	it, end=m_Connections.end();
	int			iResult;
	fd_set		Reads = m_Selectables;
	timeval		TimeOut;
	if(bQueried) *bQueried=false;

	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 25 * 1000;

	// Send data to each connection
	for( it=m_Connections.begin(); it!=end; ++it)
		m_SendBuffer.CompletePacket(),
		m_SendBuffer.SendTo( (*it).second );

	// Receive data from each connection
	iResult = select( m_fdMax+1, &Reads, NULL, NULL, &TimeOut );
	//if( iResult==SOCKET_ERROR )
	for( it=m_Connections.begin(); it!=end; ) {
		net_Client&	client = (*it).second;
		++it;

		if( FD_ISSET(client.Socket,&Reads) ) {
			theApp.Console.Comment( CT_DEBUG, "net_Sockets::SendRecv(): Data available on net_Client(%d,%d)", client.ID, client.Socket );
			if( !m_RecvBuffer.RecvFrom(client) ) {
				RemoveClient( client.ID );
				//Reads = m_Selectables;
			}
		}
	}

	if( m_QuerySocket!=INVALID_SOCKET && FD_ISSET(m_QuerySocket,&Reads) ){
		theApp.Console.Comment( CT_DEBUG, "net_Sockets::SendRecv(): Data available on query socket %d", m_QuerySocket );
		if( m_IsServer )
			if(bQueried) HandleClientQuery(),*bQueried=true;
			else HandleClientQuery();
		else m_RecvBuffer.RecvFrom(m_QuerySocket);}

	// Accept a new connection?
	if( m_Listening && FD_ISSET(m_ListenSocket,&Reads) )
		theApp.Console.Comment( CT_DEBUG, "net_Sockets::SendRecv(): Connection available on listen socket %d", m_ListenSocket ),
		AcceptNewClient();
}
// End net_Sockets::SendRecv()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::QueryServer() //                   \author Logan Jones
///////////////////////////////                      \date 04-03-2003
//
//===================================================================
// Parameters:
//  const char* strIP - 
//
void net_Sockets::QueryServer( const char* strIP )
{
	BOOL		Yes = TRUE;
	int			iResult;
	char		Message[ sizeof(net_PacketHeader) + 6 ];
	sockaddr_in	DestAddress;

	SetupQuerySockets();
	setsockopt( m_QuerySocket, SOL_SOCKET, SO_BROADCAST, (char*)&Yes, sizeof(Yes) );

	// Fill out the destination address
	ZeroMemory( &DestAddress, sizeof(DestAddress) );
	DestAddress.sin_family = AF_INET;
	DestAddress.sin_port = htons( m_QueryPort );
	DestAddress.sin_addr.s_addr = (strIP) ? inet_addr(strIP) : INADDR_BROADCAST;//inet_addr("255.255.255.255");

	((net_PacketHeader*)Message)->Marker = net_PacketMarker;
	((net_PacketHeader*)Message)->Size = sizeof(Message);
	strcpy( Message + sizeof(net_PacketHeader), "QUERY" );

	// Send the query
	iResult = sendto( m_QuerySocket, Message, sizeof(Message), 0, (sockaddr*)&DestAddress, sizeof(DestAddress) );
	assert( iResult==sizeof(Message) );
}
// End net_Sockets::QueryServer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::StopQuery() //                     \author Logan Jones
/////////////////////////////                        \date 04-03-2003
//
//===================================================================
//
void net_Sockets::StopQuery()
{
	if( m_QuerySocket!=INVALID_SOCKET ) {
		FD_CLR( m_QuerySocket, &m_Selectables );
		CloseSocket( m_QuerySocket );
		m_QuerySocket = INVALID_SOCKET;
	}

	if( m_IsServer && m_Listening ) {
		FD_CLR( m_ListenSocket, &m_Selectables );
		CloseSocket( m_ListenSocket );
		m_ListenSocket = INVALID_SOCKET;
		m_Listening = false;
	}
}
// End net_Sockets::StopQuery()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::JoinServer() //                    \author Logan Jones
//////////////////////////////                       \date 04-04-2003
//
//===================================================================
// Parameters:
//  UINT32 uiServerID - 
//
// Return: bool - 
//
bool net_Sockets::JoinServer( UINT32 uiServerID )
{
	sockaddr_in	DestAddress;
	int			iResult;

	if( m_Connected )
		return false;

	assert( m_Connections.empty() && m_ConnectionCount==0 );
	net_Client& server = m_Connections[0];
	server.ID = 0;
	server.Socket = socket( AF_INET, SOCK_STREAM, 0 );
	m_SendBuffer.InitializeTarget( server );
	m_RecvBuffer.InitializeSender( server );
	++m_ConnectionCount;

	// Fill out the destination address
	ZeroMemory( &DestAddress, sizeof(DestAddress) );
	DestAddress.sin_family = AF_INET;
	DestAddress.sin_port = htons( m_nTAPort );
	DestAddress.sin_addr.s_addr = uiServerID;

	iResult = connect( server.Socket, (sockaddr*)&DestAddress, sizeof(DestAddress) );
	if( iResult<0 ) {
		theApp.Console.Comment( CT_DEBUG, "net_Sockets::JoinServer(): Failed to join server at %s", inet_ntoa(DestAddress.sin_addr) );
		CloseSocket( server.Socket );
		m_Connections.clear();
		return false;
	} else {
		theApp.Console.Comment( CT_DEBUG, "net_Sockets::JoinServer(): Socket %d connected to server at %s", server.Socket, inet_ntoa(DestAddress.sin_addr) );
		FD_SET( server.Socket, &m_Selectables );
		m_fdMax = max( m_fdMax, server.Socket );
		m_Connected = true;
		return true;
	}
}
// End net_Sockets::JoinServer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::GetClientList() //                 \author Logan Jones
/////////////////////////////////                    \date 04-30-2003
//
//===================================================================
//
void net_Sockets::GetClientList()
{
	char							Message[ 16 * 4 ];
	ClientList_t::const_iterator	it = m_Connections.begin(),
									end= m_Connections.end();

	((UINT32*)Message)[0] = htonl(net_ClientList),
	((UINT32*)Message)[1] = htonl(m_ConnectionCount);
	for( int i=2; it!=end; ++it)
		((UINT32*)Message)[i] = htonl((*it).first);
	m_RecvBuffer.Inject( Message, 8 + m_ConnectionCount*4, net_SystemMarker );
}
// End net_Sockets::GetClientList()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::EndSession() //                    \author Logan Jones
//////////////////////////////                       \date 04-04-2003
//
//===================================================================
//
void net_Sockets::EndSession()
{
	StopQuery();
	CloseConnections();
	m_fdMax = 0;
	FD_ZERO( &m_Selectables );
	m_Connected = false;
	m_IsServer = false;
}
// End net_Sockets::EndSession()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::RemoveClient() //                  \author Logan Jones
////////////////////////////////                     \date 03-27-2003
//
//===================================================================
// Parameters:
//  UINT32 uiClientIndex - 
//
void net_Sockets::RemoveClient( UINT32 uiClientIndex )
{
	char		Message[8];

	theApp.Console.Comment( CT_DEBUG, "net_Sockets::RemoveClient(): Removing net_Client(%d,%d)", m_Connections[uiClientIndex].ID, m_Connections[uiClientIndex].Socket );
	FD_CLR( m_Connections[uiClientIndex].Socket, &m_Selectables );
	CloseSocket( m_Connections[uiClientIndex].Socket );
	m_Connections.erase( uiClientIndex );
	--m_ConnectionCount;

	// Add a 'client left' message to the recv buffer
	//if( m_IsServer )
		((UINT32*)Message)[0] = htonl(net_ConnectionClosed),
		((UINT32*)Message)[1] = htonl(uiClientIndex),
		m_RecvBuffer.Inject( Message, sizeof(Message), net_SystemMarker );
}
// End net_Sockets::RemoveClient()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::AcceptNewClient() //               \author Logan Jones
///////////////////////////////////                  \date 03-27-2003
//
//===================================================================
//
void net_Sockets::AcceptNewClient()
{
	SOCKET		NewClient;
	char		Message[8];

	// Accept the connection
	NewClient = accept( m_ListenSocket, NULL, NULL );
	assert( NewClient!=INVALID_SOCKET );

	// Add the new connection
	net_Client&	stream = m_Connections[m_NextID];
	stream.Socket = NewClient;
	stream.ID = m_NextID;
	m_SendBuffer.InitializeTarget( stream );
	m_RecvBuffer.InitializeSender( stream );
	FD_SET( stream.Socket, &m_Selectables );
	m_fdMax = max( m_fdMax, stream.Socket );
	++m_ConnectionCount;
	++m_NextID;
	theApp.Console.Comment( CT_DEBUG, "net_Sockets::AcceptNewClient(): New net_Client(%d,%d) added", stream.ID, stream.Socket );

	// Add a 'new client' message to the recv buffer
	((UINT32*)Message)[0] = htonl(net_NewConnection);
	((UINT32*)Message)[1] = htonl(stream.ID);
	m_RecvBuffer.Inject( Message, sizeof(Message), net_SystemMarker );
}
// End net_Sockets::AcceptNewClient()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::SetupQuerySockets() //             \author Logan Jones
/////////////////////////////////////                \date 04-03-2003
//
//===================================================================
//
void net_Sockets::SetupQuerySockets()
{
	int			iResult;
	sockaddr_in	LocalAddress;
	BOOL		Yes = TRUE;

	if( m_QuerySocket!=INVALID_SOCKET )
		return;

	// Fill out the local address
	ZeroMemory( &LocalAddress, sizeof(LocalAddress) );
	LocalAddress.sin_family = AF_INET;
	LocalAddress.sin_addr.s_addr = INADDR_ANY;
	LocalAddress.sin_port = m_IsServer ? htons(m_QueryPort) : 0;

	// Setup the query socket
	m_QuerySocket = socket( AF_INET, SOCK_DGRAM, 0 ); assert( m_QuerySocket!=INVALID_SOCKET );

	setsockopt( m_QuerySocket, SOL_SOCKET, SO_REUSEADDR, (char*)&Yes, sizeof(Yes) );
	iResult = bind( m_QuerySocket, (sockaddr*)&LocalAddress, sizeof(LocalAddress) ); assert( iResult==0 );
	FD_SET( m_QuerySocket, &m_Selectables );

	// Setup the listen socket
	if( m_IsServer ) {
		m_ListenSocket = socket( AF_INET, SOCK_STREAM, 0 );
		setsockopt( m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&Yes, sizeof(Yes) );
		LocalAddress.sin_port = htons( m_nTAPort );
		iResult = bind( m_ListenSocket, (sockaddr*)&LocalAddress, sizeof(LocalAddress) ); assert( iResult==0 );
		iResult = listen( m_ListenSocket, 16 ); assert( iResult==0 );
		m_fdMax = max( m_fdMax, m_ListenSocket );
		FD_SET( m_ListenSocket, &m_Selectables );
		m_Listening = true;
	}
	else m_fdMax = max( m_fdMax, m_QuerySocket );
}
// End net_Sockets::SetupQuerySockets()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// net_Sockets::HandleClientQuery() //             \author Logan Jones
/////////////////////////////////////                \date 04-03-2003
//
//===================================================================
//
void net_Sockets::HandleClientQuery()
{
	char		Data[ 32 ];
	int			ByteCount;
	sockaddr_in	SourceAddress;
	int			SourceAddressSize = sizeof( SourceAddress );

	ByteCount = recvfrom( m_QuerySocket, Data, sizeof(Data), 0, (sockaddr*)&SourceAddress, &SourceAddressSize );

	if( ByteCount!=(sizeof(net_PacketHeader)+6) || ((net_PacketHeader*)Data)->Marker!=net_PacketMarker || strncmp(Data+sizeof(net_PacketHeader),"QUERY",5)!=0 )
		return;

	for( ByteCount=0; ByteCount<m_QueryResponseSize; )
		ByteCount += sendto( m_QuerySocket, m_QueryResponse+ByteCount, m_QueryResponseSize-ByteCount, 0, (sockaddr*)&SourceAddress, SourceAddressSize );
}
// End net_Sockets::HandleClientQuery()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - net_Sockets.cpp //
//////////////////////////