// fe_MultiplayerSetup_Win32.cpp //                \author Logan Jones
//////////////////////////////////                   \date 05-07-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "fe.h"
#include "fe_MultiplayerSetup.h"


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


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::LoadSettings() //          \author Logan Jones
////////////////////////////////////////             \date 05-07-2003
//
//===================================================================
//
void fe_MultiplayerSetup::LoadSettings()
{
	HKEY	hKey = NULL;
	DWORD	KeyValueType;
	DWORD	KeyValueSize = MAX_PATH;
	long	lResult;
	long	Value;
	char	Map[64];

	// Get the skirmish settings
	RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Cavedog Entertainment\\Total Annihilation", 0, KEY_QUERY_VALUE, &hKey);

//	GetSetting_Int( "MultiDifficulty", Value, 0 );
//	((gadget_Button*)GetGadget("Difficulty"))->SetStage( Value );

	GetSetting_Int( "MultiCommanderDeath", Value, 0 );
	((gadget_Button*)GetGadget("Commander"))->SetStage( Value );

	GetSetting_Int( "MultiLineOfSight", Value, 0 );
	((gadget_Button*)GetGadget("LOSType"))->SetStage( Value );

	GetSetting_Int( "FixedLocations", Value, 0 );
	((gadget_Button*)GetGadget("FixedLoc"))->SetStage( Value );

	GetSetting_Int( "MultiMapping", Value, 0 );
	((gadget_Button*)GetGadget("Mapping"))->SetStage( Value );

	GetSetting_pChr("MultiMap", Map, "Coast To Coast" );
	((gadget_String*)GetGadget("MapName"))->SetText( Map );

	RegCloseKey(hKey);
}
// End fe_MultiplayerSetup::LoadSettings()
/////////////////////////////////////////////////////////////////////


#endif //defined(_WIN32)
/////////////////////////////////////////////////////////////////////
// End - fe_MultiplayerSetup_Win32.cpp //
////////////////////////////////////////