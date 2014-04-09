// game_Defs.h //                                  \author Logan Jones
////////////////                                       \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_DEFS_H_
#define _GAME_DEFS_H_
/////////////////////////////////////////////////////////////////////





/////////////////////////////
// Standard palette indecies

#define PALETTE_CLEAR			(0)
#define PALETTE_CLEAR2			(9)
#define PALETTE_SHADOW			(10)
#define PALETTE_BLACK			(245)
#define PALETTE_WHITE			(255)
#define PALETTE_RED				(249)
#define PALETTE_GREEN			(250)
#define PALETTE_BLUE			(252)
#define PALETTE_YELLOW			(251)
#define PALETTE_LIGHTBLUE		(254)
#define PALETTE_DARKGREY		(248)
#define PALETTE_LIGHTGREY1		(247)
#define PALETTE_LIGHTGREY2		(246)
#define PALETTE_CYAN			(100)


/////////////////////////////
// Player types

enum PlayerTypes_t
{
	PT_LOCAL_USER,				// THE user on this machine
	PT_AI,						// Computer controlled player on this machine
	PT_NETWORK,					// Any player on another machine
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_DEFS_H_)
