// net.h //                                        \author Logan Jones
//////////                                            \date 7/31/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_H_
#define _NET_H_
/////////////////////////////////////////////////////////////////////



// Include the base code
#include "sys.h"

// Network client data
#include "net_Client.h"

// Standard packet header for sent data
#include "net_PacketHeader.h"

// Send/Recv buffer objects
#include "net_SendBuffer.h"
#include "net_RecvBuffer.h"

// Define the network system
#include "net_Sockets.h"

// Global net_Interface object
// NOTE: Define 'INSTANTIATE_NET_INTERFACE' in some source file
//       to instantiate the global net_interface object
#ifdef INSTANTIATE_NET_INTERFACE
	net_Sockets			net;
#else
	extern net_Sockets	net;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_H_)
