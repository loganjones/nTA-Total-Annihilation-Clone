// game_NetMessages.h //                        Author: AsmNetMsg.exe
///////////////////////                                Date: 5/4/2004
//
//===================================================================
//
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_NETMESSAGES_H_
#define _GAME_NETMESSAGES_H_
/////////////////////////////////////////////////////////////////////

enum game_NetMessages_t {
	Request_Stop,
	Request_MoveTo,
	Request_Build,
	Request_Activate,
	Request_Attack,
	Request_Unit,
	Notify_SpawnCompleteUnit,
	Notify_PrepareToBuild,
	Notify_SpawnNewUnit,
	NET_MESSAGE_COUNT,
}; // end enum game_NetMessages_t

/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_NETMESSAGES_H_)