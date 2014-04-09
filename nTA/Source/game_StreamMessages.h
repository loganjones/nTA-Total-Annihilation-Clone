// game_StreamMessages.h //                        \author Logan Jones
//////////////////////////                            \date 9/17/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_STREAMMESSAGES_H_
#define _GAME_STREAMMESSAGES_H_
/////////////////////////////////////////////////////////////////////


// Message names for game_Main::Request() and game_Main::Notify()
enum game_StreamMessages_t
{
	REQ_Stop,
	REQ_MoveTo,
	REQ_Build,
	REQ_Attack,
	REQ_Activate,

	NOTE_Spawn = 128,
	NOTE_Moving,
	NOTE_Stopped,
	NOTE_StartBuilding,
	NOTE_StopBuilding,
	NOTE_Hit,
	NOTE_Damage,
	NOTE_Dead,
}; // end enum game_StreamMessages_t

#define MAX_STREAM_MESSAGES		256


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_STREAMMESSAGES_H_)
