// fe_Skirmish_Win32.cpp //                        \author Logan Jones
//////////////////////////                            \date 4/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "fe.h"
#include "fe_Skirmish.h"
#include "gadget_Button.h"
#include "gadget_String.h"
#include "game.h"


#define GetSetting_Int( SETTING, DESTINATION, DEFAULT ) \
	KeyValueSize = sizeof( DESTINATION ); \
	KeyValueType = REG_DWORD; \
	lResult = RegQueryValueEx( hKey, SETTING, 0, &KeyValueType, (BYTE*)(&(DESTINATION)), &KeyValueSize ); \
	if( lResult!=ERROR_SUCCESS ) { (DESTINATION) = (DEFAULT); \
	theApp.Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", SETTING ); }

#define GetSetting_pChr( SETTING, DESTINATION, DEFAULT ) \
	KeyValueSize = MAX_PATH; \
	KeyValueType = REG_SZ; \
	lResult = RegQueryValueEx( hKey, SETTING, 0, &KeyValueType, (BYTE*)(DESTINATION), &KeyValueSize ); \
	if( lResult!=ERROR_SUCCESS ) { strcpy( (DESTINATION) , (DEFAULT) ); \
	theApp.Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", SETTING ); }


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::QuickStart() //                    \author Logan Jones
//////////////////////////////                         \date 4/14/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_Skirmish::QuickStart()
{
	HKEY	hKey = NULL;
	DWORD	KeyValueType;
	DWORD	KeyValueSize = MAX_PATH;
	long	lResult;

	// Get the skirmish settings
	RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Cavedog Entertainment\\Total Annihilation", 0, KEY_QUERY_VALUE, &hKey);
	GetSetting_Int( "SkirmishDifficulty", theGame.Settings.Difficulty, 0 );
	GetSetting_Int( "SkirmishCommanderDeath", theGame.Settings.CommanderDeath, 0 );
	GetSetting_Int( "SkirmishLineOfSight", theGame.Settings.LineOfSight, 0 );
	GetSetting_Int( "SkirmishLOSType", theGame.Settings.LOSType, 0 );
	GetSetting_Int( "SkirmishLocation", theGame.Settings.Location, 0 );
	GetSetting_Int( "SkirmishMapping", theGame.Settings.Mapping, 0 );
	GetSetting_pChr("SkirmishMap", theGame.Settings.Map, "Coast To Coast" );
	RegCloseKey(hKey);

	// Get the player settings
	// TODO: Use the actual settings
	theGame.SetPlayerCount( 2 );
	theGame.SetPlayer( 0, PT_LOCAL_USER, "CORE", 0, 1000, 1000 );
	theGame.SetPlayer( 1, PT_AI, "ARM", 1, 1000, 1000 );

	return TRUE;
}
// End fe_Skirmish::QuickStart()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::LoadSettings() //                  \author Logan Jones
////////////////////////////////                       \date 4/21/2002
//               
//====================================================================
//
void fe_Skirmish::LoadSettings()
{
	HKEY	hKey = NULL;
	DWORD	KeyValueType;
	DWORD	KeyValueSize = MAX_PATH;
	long	lResult;
	long	Value;
	char	Map[64];

	// Get the skirmish settings
	RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Cavedog Entertainment\\Total Annihilation", 0, KEY_QUERY_VALUE, &hKey);

	GetSetting_Int( "SkirmishDifficulty", Value, 0 );
	((gadget_Button*)GetGadget("Difficulty"))->SetStage( Value );

	GetSetting_Int( "SkirmishCommanderDeath", Value, 0 );
	((gadget_Button*)GetGadget("CommanderDeath"))->SetStage( Value );

	GetSetting_Int( "SkirmishLineOfSight", Value, 0 );
	((gadget_Button*)GetGadget("LineOfSight"))->SetStage( Value );

//	GetSetting_Int( "SkirmishLOSType", Value, 0 );
//	((gadget_Button*)GetGadget("CommanderDeath"))->SetStage( Value );

	GetSetting_Int( "SkirmishLocation", Value, 0 );
	((gadget_Button*)GetGadget("StartLocation"))->SetStage( Value );

	GetSetting_Int( "SkirmishMapping", Value, 0 );
	((gadget_Button*)GetGadget("Mapping"))->SetStage( Value );

	GetSetting_pChr("SkirmishMap", Map, "Coast To Coast" );
	((gadget_String*)GetGadget("MapName"))->SetText( Map );

	RegCloseKey(hKey);
}
// End fe_Skirmish::LoadSettings()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::SaveSettings() //                  \author Logan Jones
////////////////////////////////                       \date 4/21/2002
//               
//====================================================================
//
void fe_Skirmish::SaveSettings()
{
	theGame.Settings.Difficulty = ((gadget_Button*)GetGadget("Difficulty"))->GetStage();
	theGame.Settings.CommanderDeath = ((gadget_Button*)GetGadget("CommanderDeath"))->GetStage();
	theGame.Settings.LineOfSight = ((gadget_Button*)GetGadget("LineOfSight"))->GetStage();
//	theGame.Settings.LOSType = ((gadget_Button*)GetGadget("Difficulty"))->GetStage();
	theGame.Settings.Location = ((gadget_Button*)GetGadget("StartLocation"))->GetStage();
	theGame.Settings.Mapping = ((gadget_Button*)GetGadget("Mapping"))->GetStage();
	strcpy( theGame.Settings.Map, ((gadget_String*)GetGadget("MapName"))->GetText() );

	// Get the player settings
	// TODO: Use the actual settings
	theGame.SetPlayerCount( 2 );
	theGame.SetPlayer( 0, PT_LOCAL_USER, "CORE", 0, 1000, 1000 );
	theGame.SetPlayer( 1, PT_AI, "ARM", 1, 1000, 1000 );
}
// End fe_Skirmish::SaveSettings()
//////////////////////////////////////////////////////////////////////

#endif //defined(_WIN32)
/////////////////////////////////////////////////////////////////////
// End - fe_Skirmish_Win32.cpp //
////////////////////////////////
