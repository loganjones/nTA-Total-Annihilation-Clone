// net_PacketHeader.h //                           \author Logan Jones
///////////////////////                              \date 03-31-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _NET_PACKETHEADER_H_
#define _NET_PACKETHEADER_H_
/////////////////////////////////////////////////////////////////////


// Header prepended to every packet sent across the network
struct net_PacketHeader {
	UINT32		Marker;
	UINT32		Size;
};

// Standard marker
const UINT32	net_PacketMarker = 0x3141546E;


/////////////////////////////////////////////////////////////////////
#endif // !defined(_NET_PACKETHEADER_H_)
