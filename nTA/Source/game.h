// game.h //                                       \author Logan Jones
///////////                                            \date 2/6/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "app.h"

// Include game wide definitions
#include "game_Defs.h"

// Include some constants
#include "game_Constants.h"

// Include the Cavedog files
#include "Cavedog/sfxtype.h"
#include "Cavedog/exptype.h"

// This is the interface class for the terrain, almost everything
// terrain related is accessed through this class
#include "game_Terrain.h"

// Include the object base class and all derivatives
#include "object.h"
#include "weapon.h"
#include "feature.h"
#include "unit.h"

// Include the player code
#include "player.h"

// Include the user interface code
#include "igui.h"

// Message names used in client/server communication
#include "game_NetMessages.h"

// Define the game class
#include "game_Main.h"

// Global game_Main object
// NOTE: Define 'INSTANTIATE_GAME_INTERFACE' in some source file
//       to instantiate the global game_Main object
#ifdef INSTANTIATE_GAME_INTERFACE
	game_Main			theGame;
#else
	extern game_Main	theGame;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_H_)
