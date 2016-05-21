// net_Sockets.h //                                \author Logan Jones
//////////////////                                     \date 9/3/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_SOCKETS_H_
#define _NET_SOCKETS_H_
/////////////////////////////////////////////////////////////////////


// Neccessary includes (platform dependent)
#ifdef _WIN32
	#include <winsock2.h>
	typedef int ssize_t;
	typedef int socklen_t;
#else
# include <unistd.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#endif


#define net_SystemMarker	(~0)
enum net_SystemMessages {
	net_NewConnection,
	net_ConnectionClosed,
	net_ClientList,
};

//////////////////////////// net_Sockets ////////////////////////////
//
class net_Sockets
{
	typedef map< UINT32, net_Client >		ClientList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	// System creation/destruction
	BOOL Create();
	void Destroy();

	// Start a server
	bool StartServer( char* pQueryResponse, DWORD dwQueryResponseSize );

	// Send and recieve data
	void SendRecv( bool* bQueried=NULL );

	// Send a query to a server (or a broadcast query if no IP is given)
	void QueryServer( const char* strIP=NULL );

	// End a QueryServer()
	void StopQuery();

	// Join a specified server
	bool JoinServer( UINT32 uiServerID );

	// Injects a client lising into the recv buffer (Marker=Sys Msg=2)
	void GetClientList();

	// Closes all connections
	void EndSession();

	// TEMP: Make reading+sending better
	net_RecvBuffer& Read() {return m_RecvBuffer;}
	net_SendBuffer& Write(){return m_SendBuffer;}

	// Query whether the network system is available
	bool IsNetworkAvailable() const {return m_NetworkAvailable;}

	// Query whether the netowrk system is acting as the server
	bool IsServer() const {return m_IsServer;}

	// Query whether the network is actively connected
	bool IsConnected() const {return m_Connected;}

	// Query the amount of active connections
	int ConnectionCount() const {return m_ConnectionCount;}


/////////////////////////////////////////////////
// Data members
protected:

	// Flag indicating the availabiltiy of the network system
	bool			m_NetworkAvailable;

	// Flag indicating if this machine is acting as the server
	bool			m_IsServer;

	// The network is actively connected either as a client or a server
	bool			m_Connected;

	// The server has an active listen socket running
	bool			m_Listening;

	// Primary port for a server
	short			m_nTAPort;

	// List containing connected sockets
	ClientList_t	m_Connections;

	// Current amount of connections in m_Connections
	int				m_ConnectionCount;

	// Keep track of the next possible unique ID for a new connection
	UINT32			m_NextID;

	// Misc members needed for select() stuff
	fd_set			m_Selectables;
	SOCKET			m_fdMax;

	// Send/Recv buffers
	net_SendBuffer	m_SendBuffer;
	net_RecvBuffer	m_RecvBuffer;

	// Pre-Game server-query and listen members
	SOCKET			m_ListenSocket;
	SOCKET			m_QuerySocket;
	short			m_QueryPort;
	char*			m_QueryBuffer;
	DWORD			m_QueryBufferSize;
	char*			m_QueryResponse;
	DWORD			m_QueryResponseSize;


/////////////////////////////////////////////////
// Utility
public:

	// send() data from the send buffer to the stream
	void SendData( net_Client& stream );

	// recv() data from the stream and place it in the recv buffer
	bool RecvData( net_Client& sender );

	bool NewPacket( net_Client& sender );

	// Remove an entry from m_Connections; generate 'client left' message
	void RemoveClient( UINT32 uiClientIndex );

	// Accept a new client into m_Connections; generate 'new client' message
	void AcceptNewClient();

	// Sets up the query and, if a server, the listen socket
	void SetupQuerySockets();

	// Something was sent to the query socket, handle it
	void HandleClientQuery();

	// Close and erase all entries in m_Connections
	static void CloseConnection( const ClientList_t::value_type& pair ) {CloseSocket(pair.second.Socket);}
	void CloseConnections() {for_each(m_Connections.begin(),m_Connections.end(),CloseConnection);m_Connections.clear();m_ConnectionCount=0;}

	// The name of close/closesocket is platform dependent
	static void CloseSocket( SOCKET s ) {
		#ifdef _WIN32
			closesocket( s );
		#else
			close( s );
		#endif
	}


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	net_Sockets();
	virtual ~net_Sockets();
/////////////////////////////////////////////////

}; // End class - net_Sockets
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_SOCKETS_H_)
