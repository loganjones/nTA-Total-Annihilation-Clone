// net_Sockets.inl //                              \author Logan Jones
////////////////////                                   \date 9/3/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::IsNetworkAvailable() //            \author Logan Jones
//////////////////////////////////////                  \date 9/3/2002
//               
//====================================================================
// Return: bool - 
//
bool net_Sockets::IsNetworkAvailable() const
{
	return m_NetworkAvailable;
}
// End net_Sockets::IsNetworkAvailable()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::IsServer() //                      \author Logan Jones
////////////////////////////                           \date 9/15/2002
//
//====================================================================
// Return: bool - 
//
bool net_Sockets::IsServer() const
{
	return m_IsServer;
}
// End net_Sockets::IsServer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::CloseSocket() //                   \author Logan Jones
///////////////////////////////                         \date 9/5/2002
//               
//====================================================================
// Parameters:
//  SOCKET s - 
//
void net_Sockets::CloseSocket( SOCKET s )
{
#ifdef _WIN32
	closesocket( s );
#else
	close( s );
#endif
}
// End net_Sockets::CloseSocket()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// net_Sockets::SendComplete() //                  \author Logan Jones
////////////////////////////////                       \date 9/15/2002
//
//====================================================================
// Parameters:
//  SOCKET s          - 
//  const char* pData - 
//  int iSize         - 
//
// Return: int - 
//
int net_Sockets::SendComplete( SOCKET s, const char* pData, int iSize )
{
	int Result;
	int	Sent = 0;

	while( Sent<iSize )
	{
		Result = send( s, pData + Sent, iSize - Sent, 0 );
		if( Result<0 ) break;
		else Sent += Result;
	}

	return Sent;
}
// End net_Sockets::SendComplete()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - net_Sockets.inl //
//////////////////////////