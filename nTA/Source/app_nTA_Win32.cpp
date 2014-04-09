// app_nTA_Win32.cpp //                            \author Logan Jones
//////////////////////                                \date 9/20/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "app.h"
#include "game.h"


//////////////////////////////////////////////////////////////////////
// app_nTA::FindTotalAnnihilation() //             \author Logan Jones
//////////////////////////////////////                 \date 9/20/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_nTA::FindTotalAnnihilation()
{
	LONG	lResult;
	HKEY	hKey;
	DWORD	KeyValueType = REG_SZ;
	DWORD	KeyValueSize = MAX_PATH;

    // Check for the Total Annihilation registry key
    if( lResult=RegOpenKeyEx( HKEY_CURRENT_USER,
                      "Software\\Cavedog Entertainment\\Total Annihilation",
                      0,
                      KEY_QUERY_VALUE,
                      &hKey )!=ERROR_SUCCESS )
    {
		Console.Comment( CT_ERROR, "ERROR: Total Annihilation installation verification step one failed: %d", lResult );
        return FALSE;
    }
	else // Just making sure its there, close the key
		RegCloseKey(hKey);

    // Check again, but look for the uninstall key
    if( lResult=RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                      "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Total Annihilation",
                      0,
                      KEY_QUERY_VALUE,
                      &hKey )!=ERROR_SUCCESS )
    {
		Console.Comment( CT_ERROR, "ERROR: Total Annihilation installation verification step two failed: %d", lResult );
        return FALSE;
    }
	else // Get the directory and close the key
	{
		// Store the directory
		lResult=RegQueryValueEx(
			hKey,
			"Dir",
			0,
			&KeyValueType,
			(BYTE*)m_TotalAnnihilationDirectory,
			&KeyValueSize );

		// Close the uninstall key
		RegCloseKey(hKey);

		// Check if the query failed
		if( lResult!=ERROR_SUCCESS )
		{
			Console.Comment( CT_ERROR, "ERROR: Total Annihilation installation verification step 2.1 failed: %d", lResult );
			return FALSE;
		}
	} // end else // Get the directory and close the key

    // Validation successfull
    return TRUE;
}
// End app_nTA::FindTotalAnnihilation()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::LoadSettings() //                      \author Logan Jones
/////////////////////////////                         \date 11/11/2001
//               
//====================================================================
//
void app_nTA::LoadSettings()
{
	HKEY	hKey = NULL;
	DWORD	KeyValueType;
	DWORD	KeyValueSize = MAX_PATH;
	DWORD	Value;
	BYTE	ValueBuffer[256];
	long	ReturnVal;

    // Open the settings key
    if( ReturnVal=RegOpenKeyEx( HKEY_CURRENT_USER,
                      "Software\\BurnWare\\nTA",
                      0,
                      KEY_QUERY_VALUE,
                      &hKey )!=ERROR_SUCCESS )
    {
		// Just use the defaults
		Console.Comment( CT_NORMAL, "Unable to open the nTA registry key: %d", ReturnVal );
    }
	else
	{
		// Store the renderer
		KeyValueType = REG_SZ;
		RegQueryValueEx( hKey, "Renderer", 0, &KeyValueType, (BYTE*)m_Renderer, &KeyValueSize );

		// Store the renderer resolution
		KeyValueType = REG_DWORD;
		KeyValueSize = sizeof( Value );
		Value = m_RendererResolution.width;
		RegQueryValueEx( hKey, "ResWidth", 0, &KeyValueType, (BYTE*)&Value, &KeyValueSize );
		m_RendererResolution.width = Value;
		Value = m_RendererResolution.height;
		RegQueryValueEx( hKey, "ResHeight", 0, &KeyValueType, (BYTE*)&Value, &KeyValueSize );
		m_RendererResolution.height = Value;
		RegQueryValueEx( hKey, "ResDepth", 0, &KeyValueType, (BYTE*)&m_RendererDepth, &KeyValueSize );

		RegCloseKey( hKey );
	}

// TODO: Create a settings key specificly for nTA.
//       For now just use OTA's settings.
#define GetSettingL( theSETTING, DEFAULT_VALUE ) \
	KeyValueSize = 256; \
	ReturnVal = RegQueryValueEx( hKey, #theSETTING, 0, &KeyValueType, ValueBuffer, &KeyValueSize); \
	if( ReturnVal!=ERROR_SUCCESS ) { theGame.Settings.theSETTING = DEFAULT_VALUE; \
	Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", #theSETTING ); } \
	else theGame.Settings.theSETTING = *((DWORD*)ValueBuffer)
#define GetSettingStr( theSETTING, DEFAULT_VALUE ) \
	KeyValueSize = 256; \
	ReturnVal = RegQueryValueEx( hKey, #theSETTING, 0, &KeyValueType, ValueBuffer, &KeyValueSize); \
	if( ReturnVal!=ERROR_SUCCESS ) { strcpy( theGame.Settings.theSETTING, DEFAULT_VALUE ); \
	Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", #theSETTING ); } \
	else strcpy( theGame.Settings.theSETTING, (LPTSTR)ValueBuffer )
#define GetCategorySettingL( CATEGORY, theSETTING, DEFAULT_VALUE ) \
	KeyValueSize = 256; \
	ReturnVal = RegQueryValueEx( hKey, (#CATEGORY #theSETTING), 0, &KeyValueType, ValueBuffer, &KeyValueSize); \
	if( ReturnVal!=ERROR_SUCCESS ) { theGame.Settings.CATEGORY.theSETTING = DEFAULT_VALUE; \
	Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", (#CATEGORY #theSETTING) ); } \
	else theGame.Settings.CATEGORY.theSETTING = *((DWORD*)ValueBuffer)
#define GetCategorySettingStr( CATEGORY, theSETTING, DEFAULT_VALUE ) \
	KeyValueSize = 256; \
	ReturnVal = RegQueryValueEx( hKey, (#CATEGORY #theSETTING), 0, &KeyValueType, ValueBuffer, &KeyValueSize); \
	if( ReturnVal!=ERROR_SUCCESS ) { strcpy( theGame.Settings.CATEGORY.theSETTING, DEFAULT_VALUE ); \
	Console.Comment( CT_NORMAL, "Unable to retrieve setting, \'%s\'. Using default", (#CATEGORY #theSETTING) ); } \
	else strcpy( theGame.Settings.CATEGORY.theSETTING, (LPTSTR)ValueBuffer )

	RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Cavedog Entertainment\\Total Annihilation", 0, KEY_QUERY_VALUE, &hKey);

	GetSettingL( scrollspeed, 16 );
/*
	GetCategorySettingL( Skirmish, Difficulty, 0 );
	GetCategorySettingL( Skirmish, CommanderDeath, 0 );
	GetCategorySettingL( Skirmish, LineOfSight, 0 );
	GetCategorySettingL( Skirmish, LOSType, 0 );
	GetCategorySettingL( Skirmish, Location, 0 );
	GetCategorySettingL( Skirmish, Mapping, 0 );
	GetCategorySettingStr( Skirmish, Map, "\0Map Not Found In Settings" );

	GetCategorySettingL( Multi, CommanderDeath, 0 );
	GetCategorySettingL( Multi, LineOfSight, 0 );
	GetCategorySettingL( Multi, LOSType, 0 );
	GetCategorySettingL( Multi, Mapping, 0 );
	GetCategorySettingStr( Multi, Map, "\0Map Not Found In Settings" );
*/
	RegCloseKey( hKey );
}
// End app_nTA::LoadSettings()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::DestroyNTASettings() //                \author Logan Jones
//////////////////////////////////                     \date 8/12/2002
//               
//====================================================================
//
void app_nTA::DestroyNTASettings()
{
	RegDeleteKey( HKEY_CURRENT_USER, "Software\\BurnWare\\nTA" );
	RegDeleteKey( HKEY_CURRENT_USER, "Software\\BurnWare" );
}
// End app_nTA::DestroyNTASettings()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::SaveRendererSettings() //              \author Logan Jones
////////////////////////////////////                   \date 8/12/2002
//               
//====================================================================
//
void app_nTA::SaveRendererSettings()
{
	HKEY	hKey = NULL;
	DWORD	Disposition;
	long	ReturnVal;

	// Try to access the key
	if( ReturnVal=RegCreateKeyEx( HKEY_CURRENT_USER,
		"Software\\BurnWare\\nTA", 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL,
		&hKey, &Disposition )!=ERROR_SUCCESS )
	{
		Console.Comment( CT_NORMAL, "Unable to open the nTA registry key for writing: %d", ReturnVal );
		return;
	}

	// Write the values to the registry
	RegSetValueEx( hKey, "Renderer", 0, REG_SZ, (BYTE*)m_Renderer, strlen(m_Renderer) + 1 );
	RegSetValueEx( hKey, "ResWidth", 0, REG_DWORD, (BYTE*)&m_RendererResolution.width, sizeof(m_RendererResolution.width) );
	RegSetValueEx( hKey, "ResHeight",0, REG_DWORD, (BYTE*)&m_RendererResolution.height,sizeof(m_RendererResolution.height) );
	RegSetValueEx( hKey, "ResDepth", 0, REG_DWORD, (BYTE*)&m_RendererDepth, sizeof(m_RendererDepth) );

	// Close the key
	RegCloseKey( hKey );
}
// End app_nTA::SaveRendererSettings()
//////////////////////////////////////////////////////////////////////


#endif // !defined(_WIN32)
/////////////////////////////////////////////////////////////////////
// End - app_nTA_Win32.cpp //
////////////////////////////
