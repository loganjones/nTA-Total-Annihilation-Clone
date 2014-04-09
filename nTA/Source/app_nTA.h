// app_nTA.h //                                    \author Logan Jones
//////////////                                         \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _APP_NTA_H_
#define _APP_NTA_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// app_nTA //////////////////////////////
//
class app_nTA : public sys_App
{

/////////////////////////////////////////////////
// sys_App overrides
public:

	// Initailize data members
	void Initialize();

	// Startup proccessing
	BOOL Start();

	// Shutdown proeccessing
	void Finish();

	// The action
	void DoFrame();

	// App state change handling
	void StateChanging( sys_App_States OldState, sys_App_States NewState );


/////////////////////////////////////////////////
// Interface
public:

	// Starts the game loading and running process
	void StartGame();

	// Processes a frame for the current game
	void GameFrame();

	// Ends the game
	void EndGame();

	void DestroyNTASettings();
	void SaveRendererSettings();

	// Restarts desired parts of the app
	void Restart( DWORD dwRestartFlags=0 );

	// Sets the m_StartMenu data member
	void SetStartMenu( LPCTSTR strMenuName );

	// Access to m_Renderer info
	INLINE LPCTSTR GetRenderer() const;
	INLINE std_Size	GetResolution() const;
	INLINE long GetColorDepth() const;
	INLINE void	SetRenderer( LPCTSTR strRenderer );
	INLINE void	SetResolution( std_Size szResolution );
	INLINE void SetColorDepth( long lDepth );


/////////////////////////////////////////////////
// Data members
protected:

	// Current state of the game
	enum GameStates_t
	{
		GS_NOT_RUNNING,
		GS_RUNNING,
		GS_START_GAME,
		GS_LOADING,

	}					m_GameState;

	DWORD				m_RestartFlags;

	DWORD				m_StartCount;

	char				m_StartMenu[ 32 ];

	// The game or mod that is going to be played ie "OTA".
	// nTA looks for a subdirectory of the same name and
	// loads all its files from there.
	char				m_GameType[ 32 ];

	// Total Annihilation's actual path on disk
	char				m_TotalAnnihilationDirectory[ MAX_PATH ];

	// Name, resolution and depth of the gfx renderer
	char				m_Renderer[ 32 ];
	std_Size			m_RendererResolution;
	long				m_RendererDepth;

	// Handle to the currently loaded graphics DLL
	HDLL				m_hGfxInterfaceDLL;

	// Full screen flag
	BOOL				m_bFullScreen;

	// Start game flag
	enum {
		START_NONE,
		START_SKIRMISH,
		START_SINGLE,
		START_MULTI,
	}					m_StartFlag;


/////////////////////////////////////////////////
// Public data members
public:

	// Console used for the game
	app_Console			Console;

	// Restart flags
	enum RestartFlags_t
	{
		RESTART_None				=	0,
		RESTART_Subsystems			=	1,
		RESTART_GameType			=	2,
		RESTART_All					=	RESTART_Subsystems | RESTART_GameType,
	};


/////////////////////////////////////////////////
// Utility
protected:

	// Searches the system for Total Annihilation(tm)
	// Platform specific
	BOOL FindTotalAnnihilation();

	// Loads the game's global settings
	// Platform specific
	void LoadSettings();

	// Processes the arguments given in the command line
	void ProcessCommandLine();

	// Executes the startup cfg for the current game type
	BOOL LoadConfig();

	// Loads the gfx DLL and initializes the interface
	BOOL RetrieveGfxInterface();

	// Loads any palettes needed into the gfx system
	BOOL LoadPalattes();

	// Loads a game session
	BOOL LoadGame();


/////////////////////////////////////////////////
// Exposed functions
public:

	static LRESULT CALLBACK GfxLoadProc( LPCTSTR strComment, ... );
	static LRESULT CALLBACK GfxErrorProc( LPCTSTR strComment, ... );
	static LRESULT CALLBACK Exit( LPVOID lpParams );
	static LRESULT CALLBACK ft_AddScanDir( LPVOID lpParams );
	static LRESULT CALLBACK ft_AddScanType( LPVOID lpParams );
	static LRESULT CALLBACK ft_AddSubs( LPVOID lpParams );
	static LRESULT CALLBACK Exec( LPVOID lpParams );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	app_nTA();
	virtual ~app_nTA();
/////////////////////////////////////////////////

}; // End class - app_nTA
/////////////////////////////////////////////////////////////////////


// Game wide access to the application object
extern app_nTA		theApp;


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "app_nTA.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_APP_NTA_H_)
