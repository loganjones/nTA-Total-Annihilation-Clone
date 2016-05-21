// app_nTA.cpp //                                  \author Logan Jones
////////////////                                       \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"

#define INSTANTIATE_GFX_INTERFACE_POINTER
#define INSTANTIATE_SOUND_INTERFACE
#define INSTANTIATE_INPUT_INTERFACE
#define INSTANTIATE_NET_INTERFACE
#include "gfx.h"
#include "snd.h"
#include "input.h"
#include "net.h"

#define INSTANTIATE_FILETREE_INTERFACE
#define INSTANTIATE_GAME_INTERFACE
#include "app.h"
#include "game.h"

#include "app_nTA.h"
#include <time.h>

#define INSTANTIATE_FRONTEND_INTERFACE
#include "fe.h"

#include "app_nTA_Exposed.inl"
// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "app_nTA.inl"
#endif // defined( _DEBUG )

#define AddConsoleCommand( COMMAND_NAME )	Console.AddCommand( #COMMAND_NAME, COMMAND_NAME )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
app_nTA::app_nTA():sys_App(),
	m_GameState( GS_NOT_RUNNING ),
// Default renderer settings
	m_RendererResolution( 640, 480 ),
	m_RendererDepth( 16 ),
// Full screen by default
	m_bFullScreen( TRUE ),
// Misc
	m_StartCount( 0 ),
	m_RestartFlags( RESTART_All ),
	m_StartFlag( START_NONE )
{
	// I am  Sparticus
	sys = this;

	// Set the default start menu
	SetStartMenu( "Main" );

	// Set the default renderer and game tyype
	strcpy( m_Renderer, DEFUALT_RENDERER );
	strcpy( m_GameType, DEFAULT_GAME_TYPE );

	// Initialize some platform specific stuff
#ifdef _WIN32
	strcpy( m_PlatformParameters.WindowClass, "nTA" );
#endif
    strcpy( m_PlatformParameters.MainWindowName, "nTA" );
    //sprintf( m_PlatformParameters.MainWindowName, "nTA - %s - %s %s", m_GameType, m_Renderer, (m_bFullScreen) ? "(FullScreen)":"(Windowed)" );

	// Process the stuff given in the command line (if any)
	ProcessCommandLine();

	AddConsoleCommand( ft_AddScanDir );
	AddConsoleCommand( ft_AddScanType );
	AddConsoleCommand( ft_AddSubs );
}
app_nTA::~app_nTA()
{}
//
/////////////////////////////////////////////////////////////////////


// Instansiate the application
app_nTA		theApp;


//////////////////////////////////////////////////////////////////////
// app_nTA::Initialize() //                        \author Logan Jones
///////////////////////////                            \date 9/10/2001
// Description:  Initializes theApp data members for start-up
//               
//====================================================================
//
void app_nTA::Initialize()
{
	// Initialize the base class
	sys_App::Initialize();
}
// End app_nTA::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::Start() //                             \author Logan Jones
//////////////////////                                 \date 9/10/2001
// Description:  Create and start all sub-systems
//               
//====================================================================
// Return: BOOL - Return TRUE if successful, FALSE otherwise
//
BOOL app_nTA::Start()
{
	// Setup the log file stuff
	// TODO: Un-hardcode log file set up
	Console.SetLogFile( "Log.txt", CT_ALL, TRUE );

	// Print the strating message
	time_t theTime;
	time( &theTime );
	LPTSTR strTime = ctime( &theTime );
	strTime[ strlen(strTime)-2 ] = '\0';
	Console.Comment( CT_NORMAL, "************************************************************" );
	Console.Comment( CT_NORMAL, "nTA started on %s", strTime );
	Console.Comment( CT_NORMAL, "************************************************************" );
	Console.Comment( CT_NORMAL, "" );

	// Create the application and show the splash screen
	if( m_RestartFlags & RESTART_Subsystems )
    {
		if( bFAILED(Create("nTA Splash.bmp")) )
			return FALSE;
		else Console.Comment( CT_NORMAL, "nTA application object created." );
    }

	// Find Total Annihilation or else
	if( bFAILED(FindTotalAnnihilation()) )
	{
		ErrorBox( "Error -> app_nTA::Start()", "Could not find Total Annihilation." );
		return FALSE;
	}
	else Console.Comment( CT_NORMAL, "Total Annihilation found and verified." );

	// Load the global setting for the game
	Console.AddCommand( "Exec", Exec );
	LoadSettings();
	Console.Comment( CT_NORMAL, "Settings loaded." );

	// Load as specified by the gametype
	if( m_RestartFlags & RESTART_GameType )
	{
		// Load the configuration
		if( bFAILED(LoadConfig()) )
		{
			ErrorBox( "Error -> app_nTA::Start()", "Failed to load the startup config." );
			return FALSE;
		}
		else Console.Comment( CT_NORMAL, "Startup config loaded." );

		// Create the file tree
		if( bFAILED(theFileTree.Create()) )
		{
			ErrorBox( "Error -> app_nTA::Start()", "Failed to create the file tree." );
			return FALSE;
		}
		else Console.Comment( CT_NORMAL, "File tree created." );
	} // end load game type

	// (Re)Create the subsystems
	if( m_RestartFlags & RESTART_Subsystems )
	{
		// Create network
		net.Create();
		Console.Comment( CT_NORMAL, "Network system created." );

		// Create input
		if( bFAILED(input.Create(&m_PlatformParameters)) )
		{
			ErrorBox( "Error -> app_nTA::Start()", "Failed to create the input system." );
			return FALSE;
		}
		else Console.Comment( CT_NORMAL, "Input system created." );

		// Create sound
		if( bFAILED(sound.Create(&m_PlatformParameters)) )
		{
			ErrorBox( "Error -> app_nTA::Start()", "Failed to create the sound system." );
			return FALSE;
		}
		else Console.Comment( CT_NORMAL, "Sound system created." );

		// Get the interface to the gfx system and create it
		if( bFAILED(RetrieveGfxInterface()) ||
			bFAILED(gfx->Create( m_RendererResolution, (int)m_RendererDepth, m_bFullScreen )) )
		{
			ErrorBox( "Error -> app_nTA::Start()", "Failed to create the graphics system." );
			return FALSE;
		}
		else Console.Comment( CT_NORMAL, "Graphics system created." );

	} // end creating subsystems

	Console.AddCommand( "Exit", Exit );
	Console.AddCommand( "Quit", Exit );

	// Load the palette
	if( bFAILED(LoadPalattes()) )
	{
		ErrorBox( "Error -> app_nTA::Start()", "Failed to load the primary palette." );
		return FALSE;
	}
	else Console.Comment( CT_NORMAL, "Primary palettes loaded." );

	theGame.LoadSideData(NULL);

	// Are we supposed to start a game now
	switch( m_StartFlag )
	{
		case START_NONE: // Load the front end
		{
			// Call EndGame() to set the proper game state
			EndGame();

			// Create the front end
			if( bFAILED(theFrontEnd.Create(m_StartMenu)) )
			{
				ErrorBox( "Error -> app_nTA::Start()", "Failed to create the front end." );
				return FALSE;
			}
			break;
		} // end case START_NONE

		case START_SKIRMISH:
		case START_SINGLE:
		case START_MULTI:
		{
			// Do a quick start
			if( bFAILED(theFrontEnd.QuickStart("Skirmish")) )
			{
				ErrorBox( "Error -> app_nTA::Start()", "Quick start failed." );
				return FALSE;
			}

			// Start the game
			StartGame();
			break;
		}

		default: // This shouldn't happen
			assert(!"Unknown start flag");
			ErrorBox( "Error -> app_nTA::Start()", "Unknown start flag." );
			return FALSE;

	} // end switch( m_StartFlag )

	// Startup completed successfully
	Console.Comment( CT_NORMAL, "app_nTA::Start() completed successfully." );
	Console.Comment( CT_NORMAL, "" );
	m_RestartFlags = RESTART_All;
	++m_StartCount;
	return TRUE;
}
// End app_nTA::Start()
//////////////////////////////////////////////////////////////////////

#define GOT_HERE( STR ) \
	Console.Comment( CT_NORMAL, "app_nTA::Finish() " STR "." )
//////////////////////////////////////////////////////////////////////
// app_nTA::Finish() //                            \author Logan Jones
///////////////////////                                \date 9/10/2001
// Description:  Shutdown and destroy all sub-systems
//               
//====================================================================
//
void app_nTA::Finish()
{
	// If a game is running, end it
	EndGame();

	// Destroy the front end
	theFrontEnd.Destroy();

	// Go ahead and destroy the gui stuff
	guiResources.Destroy();

	// Destroy the file tree?
	if( m_RestartFlags & RESTART_GameType )
		theFileTree.Destroy();

	// Destroy subsystems?
	if( m_RestartFlags & RESTART_Subsystems )
	{
		// Destroy the gfx system and unload the DLL
		SAFE_DESTROY( gfx );
		sys_UnloadDll( m_hGfxInterfaceDLL );

		sound.Destroy();
		input.Destroy();
		net.Destroy();

		// Destroy the app
		Destroy();
	}
}
// End app_nTA::Finish()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::DoFrame() //                           \author Logan Jones
////////////////////////                               \date 9/10/2001
// Description:  Called every system frame, our action starting point
//               
//====================================================================
//
void app_nTA::DoFrame()
{
	PROFILE( "Main Loop" )

	BEGIN_PROFILE_BLOCK( "Get and dispatch input" )
	{
		Console.ProcessInput();
	}
	END_PROFILE_BLOCK

	// Do a game frame
	GameFrame();

	// Let the front-end do a frame
	theFrontEnd.OnFrame();
}
// End app_nTA::DoFrame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::StateChanging() //                     \author Logan Jones
//////////////////////////////                         \date 9/29/2001
//               
//====================================================================
// Parameters:
//  sys_App_States OldState - 
//  sys_App_States NewState - 
//
void app_nTA::StateChanging( sys_App_States OldState, sys_App_States NewState )
{
	switch( NewState )
	{
		case State_Active:
			input.AcquireDevices();
			break;

		case State_Paused:
			input.UnacquireDevices();
			break;
            
        default:
            break;

	} // end switch( NewState )
}
// End app_nTA::StateChanging()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::LoadGame() //                          \author Logan Jones
/////////////////////////                             \date 11/17/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_nTA::LoadGame()
{
	// Game is now loading
	m_GameState = GS_LOADING;

	// Destroy the front end
	theFrontEnd.Destroy();

	// Load the game
	return theGame.Load();
}
// End app_nTA::LoadGame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::StartGame() //                         \author Logan Jones
//////////////////////////                            \date 11/17/2001
//               
//====================================================================
//
void app_nTA::StartGame()
{
	m_GameState = GS_START_GAME;
}
// End app_nTA::StartGame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GameFrame() //                         \author Logan Jones
//////////////////////////                            \date 11/17/2001
//               
//====================================================================
//
void app_nTA::GameFrame()
{
	switch( m_GameState )
	{
		// Normal running state
		case GS_RUNNING:
			// UpdateServer();
			//m_pIGInterface->Update();
			theGame.OnFrame();
			return;

		// Game is currently loading
		case GS_LOADING:
			theGame.RenderLoadScreen();
			return;

		// Please load
		case GS_START_GAME:
			if( bFAILED(LoadGame()) )
				EndGame(),
				theFrontEnd.Create( m_StartMenu );
			else m_GameState = GS_RUNNING;
			return;

		// Game hasn't started; must be in front end or something
		case GS_NOT_RUNNING:
			// Do nothing
			return;

	} // end switch( m_GameState )
}
// End app_nTA::GameFrame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::EndGame() //                           \author Logan Jones
////////////////////////                               \date 12/1/2001
//               
//====================================================================
//
void app_nTA::EndGame()
{
	Console.Comment( CT_NORMAL, "Exiting game..." );

	// Destry the game
	theGame.Destroy();

	// The game is now not running
	Console.Comment( CT_NORMAL, "Game destroyed." );
	m_GameState = GS_NOT_RUNNING;
}
// End app_nTA::EndGame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::Restart() //                           \author Logan Jones
///////////////////////                                \date 8/12/2002
//               
//====================================================================
// Parameters:
//  DWORD dwRestartFlags - 
//
void app_nTA::Restart( DWORD dwRestartFlags )
{
	// If no flags were given, assume everything
	if( dwRestartFlags==0 ) dwRestartFlags = RESTART_All;

	// Set the restart flags and "exit"
	m_RestartFlags = dwRestartFlags;
	m_bRestart = TRUE;
	Quit();
}
// End app_nTA::Restart()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::SetStartMenu() //                      \author Logan Jones
/////////////////////////////                         \date 11/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strMenuName - 
//
void app_nTA::SetStartMenu( LPCTSTR strMenuName )
{
	if( strMenuName && (strlen(strMenuName)>0) && (strlen(strMenuName)<31) )
		strcpy( m_StartMenu, strMenuName );

	// It's safe to assume we don't want to start the game at next startup
	m_StartFlag = START_NONE;
}
// End app_nTA::SetStartMenu()
//////////////////////////////////////////////////////////////////////


#if __APPLE__
const int _MakeARGV_macOS(char ***pargv);
void _FreeARGV_macOS(char **argv, const int argc);
#endif

//////////////////////////////////////////////////////////////////////
// app_nTA::ProcessCommandLine() //                \author Logan Jones
///////////////////////////////////                    \date 9/20/2001
//               
//====================================================================
//
void app_nTA::ProcessCommandLine()
{
#if __APPLE__
    char **__argv;
    const int __argc = _MakeARGV_macOS(&__argv);
#endif

    LPTSTR      Value;
	int			x;

#define IsCommand( str )    ( ((str)[0]=='-') || ((str)[0]=='+') || ((str)[0]=='/') )

	// Iterate through each argument
    for( x=1; x<__argc; )
    {
		// Check if this argument is a command
        if( IsCommand( __argv[x] ) )
        {
			// Find out what command this is
			STRING_SWITCH( __argv[x] + 1 )

				CASE( "game" )
					Value = __argv[++x];
					if( !IsCommand(Value) )
					{
						strcpy( m_GameType, __argv[x] );
						++x;
					}

				CASE( "windowed" )
					m_bFullScreen = FALSE;
					++x;

				CASE( "renderer" )
					Value = __argv[++x];
					if( !IsCommand(Value) )
					{
						strcpy( m_Renderer, Value );
						++x;
					}

				CASE( "Direct3D8" )
					strcpy( m_Renderer, "Direct3D8" );
					++x;

				CASE( "OpenGL" )
					strcpy( m_Renderer, "OpenGL" );
					++x;

				CASE( "StartGame" )
					m_StartFlag = START_SKIRMISH;
					++x;
				CASE( "StartSkirmish" )
					m_StartFlag = START_SKIRMISH;
					++x;
				CASE( "StartSingle" )
					m_StartFlag = START_SINGLE;
					++x;
				CASE( "StartMulti" )
					m_StartFlag = START_MULTI;
					++x;

				CASE( "Menu" )
					Value = __argv[++x];
					if( !IsCommand(Value) )
						SetStartMenu( __argv[x++] );

				STRING_SWITCH_DEFAULT
					// Move On
					++x;

			END_STRING_SWITCH
        }
        else // This isn't a command
            ++x; // Try the next argument

    } // End for( Number Of Command-Line Arguments )
    
#if __APPLE__
    _FreeARGV_macOS(__argv, __argc);
#endif
}
// End app_nTA::ProcessCommandLine()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::LoadConfig() //                        \author Logan Jones
///////////////////////////                           \date 11/11/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_nTA::LoadConfig()
{
	char	Command[ 64 ];

	sprintf( Command, "exec %s/Startup.cfg", m_GameType );
	Console.Execute( Command, (DWORD)strlen(Command) );
	return TRUE;
}
// End app_nTA::LoadConfig()
//////////////////////////////////////////////////////////////////////

#if !SYS_GLFW
//////////////////////////////////////////////////////////////////////
// app_nTA::RetrieveGfxInterface() //              \author Logan Jones
/////////////////////////////////////                  \date 9/22/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_nTA::RetrieveGfxInterface()
{
	char						strPath[ MAX_PATH ];
	const char					strDefault[] = "Direct3D8";
	LPRetrieveGfxInterface		RetrieveInterface;

	// Get the path of the desired DLL
	sprintf( strPath, "%s/%s.dll", GFX_DIRECTORY, m_Renderer );

	// Load the DLL desired by the user
	if( !(m_hGfxInterfaceDLL=sys_LoadDll(strPath)) )
	{
		Console.Comment( CT_ERROR, "ERROR: Couldn't find graphics interface \'%s.dll\'. Attempting to load the default, %s.dll", m_Renderer, strDefault );

		// Loading the custom DLL didn't work.
		// Try the default
		sprintf( strPath, "%s/%s.dll", GFX_DIRECTORY, strDefault );
		if( !(m_hGfxInterfaceDLL=sys_LoadDll(strPath)) )
		{
			Console.Comment( CT_ERROR, "ERROR: Default graphics interface, %s.dll, not found", strDefault );
			return FALSE;
		}
	}

	// Get a pointer to the DLL's only function
	RetrieveInterface = (LPRetrieveGfxInterface)sys_RetrieveProc( m_hGfxInterfaceDLL, "RetrieveGfxInterface" );

	// Retrieve the interface pointer
	RetrieveInterface( &gfx );

	// Initialize the interface
	return gfx->Initialize( &m_PlatformParameters, GfxLoadProc, GfxErrorProc );
}
// End app_nTA::RetrieveGfxInterface()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// app_nTA::LoadPalattes() //                      \author Logan Jones
/////////////////////////////                         \date 10/10/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_nTA::LoadPalattes()
{
	PALETTEENTRY*				pEntries;
	DWORD						Size;
	gfx_Interface::LPPALETTE	pPal;

	// Get the primary palette file buffer
	Size = theFileTree.DumpFileToBuffer( "palettes/palette.pal", (BYTE**)&pEntries );
	if( Size==0 )
	{
		Console.Comment( CT_ERROR, "ERROR: Failed to dump \'palettes/palette.pal\' to a buffer." );
		return FALSE;
	}

	// Set all alpha entries to opaque
	for( long i=255; i>=0; --i)
		pEntries[ i ].peFlags = 255;

	// Set some specific alpha values
	pEntries[ PALETTE_CLEAR  ].peFlags = 0;
	pEntries[ PALETTE_CLEAR2 ].peFlags = 0;
	pEntries[ PALETTE_SHADOW ].peFlags = 100;

	// Load the palette
	if( bFAILED(gfx->CreatePalette( pEntries, &pPal )) )
	{
		delete [] pEntries;
		return FALSE;
	}

	delete [] pEntries;
	return TRUE;
}
// End app_nTA::LoadPalattes()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::Exit() //                              \author Logan Jones
/////////////////////                                 \date 11/11/2001
//               
//====================================================================
// Parameters:
//  LPVOID lpParams - 
//
// Return: LRESULT - 
//
LRESULT app_nTA::Exit( LPVOID lpParams )
{
	LPTSTR	strParams = static_cast<LPTSTR>(lpParams);
	char	ExitWhat[ 32 ];

	// Check if there is a parameter
	if( !theApp.Console.RetrieveParam( strParams, app_Console::PT_STRING, (LPTSTR)ExitWhat ) )
	{
		theApp.Quit();
		return ( 1 );
	}
	else STRING_SWITCH( ExitWhat )
		CASE( "?" )
			theApp.Console.Comment( CT_ECHO, "Usage:	exit [game]" );
			return 1;
		CASE( "game" )
			theApp.EndGame();
			theFrontEnd.Create( "Main" );
			return 1;
	END_STRING_SWITCH

	theApp.Console.Comment( CT_ECHO, "Unknown parameter \'%s\'", ExitWhat );
	return 1;
}
// End app_nTA::Exit()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_nTA.cpp //
///////////////////////
