// app_Defs.h //                                   \author Logan Jones
///////////////                                       \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _APP_DEFS_H_
#define _APP_DEFS_H_
/////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
 #define GFX_DIRECTORY			"gfx_dbg"
#else
 #define GFX_DIRECTORY			"gfx"
#endif

// Defaults

#ifdef _WIN32
 #define DEFUALT_RENDERER		"Direct3D8"
#else
 #define DEFUALT_RENDERER		"OpenGL"
#endif

#define DEFAULT_GAME_TYPE		"OTA"


// Game macros

#define PROFILE( NAME )
#define BEGIN_PROFILE_BLOCK( NAME )		{ PROFILE(NAME) BEGIN_CODE_BLOCK
#define END_PROFILE_BLOCK				END_CODE_BLOCK }


/////////////////////////////////////////////////////////////////////
#endif // !defined(_APP_DEFS_H_)
