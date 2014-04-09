// net_Client.h //                                 \author Logan Jones
/////////////////                                    \date 03-24-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_
/////////////////////////////////////////////////////////////////////


struct net_Client {
	SOCKET			Socket;
	int				ID;

	struct SendData{
		char*	Cursor;
		int		BytesLeft;
	}				Send;

	struct RecvData{
		char*	Start;
		char*	Cursor;
		int		BytesLeft;
	}				Recv;
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_CLIENT_H_)
