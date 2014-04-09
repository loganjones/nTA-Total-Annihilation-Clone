// game_Settings.h //                              \author Logan Jones
////////////////////                                  \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_
/////////////////////////////////////////////////////////////////////


struct game_Settings
{
	std_Size	MapViewportSize;
	long		scrollspeed;

	long			Difficulty;
	long			LineOfSight;
	long			LOSType;
	long			CommanderDeath;
	long			Location;
	long			Mapping;
	char			Map[128];
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_SETTINGS_H_)
