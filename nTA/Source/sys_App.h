// sys_App.h //                                    \author Logan Jones
//////////////                                         \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SYS_APP_H_
#define _SYS_APP_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// sys_App //////////////////////////////
//
class sys_App
{

/////////////////////////////////////////////////
// App state flags
public:

	enum sys_App_States
	{
		// Non-Running states
		State_StartUp,
		State_ShutDown,

		// Running states
		State_Active,
		State_Paused,

		FORCE_DWORD( State )
	};


/////////////////////////////////////////////////
// INTERFACE
public:

	// Starting point of the app, called by the OS entry point
	// Usually not overriden, but its possible.
	virtual void Run();

	// Called immediately at startup, most members are INITIALIZED here.
	// This function is not meant for doing start-up processing, that's what Start() is for,
	// Only initialize stuff here, and call the base version
	virtual void Initialize();

	// Start up processing function, override and do all start up stuff here
	virtual BOOL Start() = 0;

	// Creates the application, call from Start()
	// Platform dependent
	virtual BOOL Create( LPCTSTR strSplashPath=NULL );

	// Opposite of Start(), the anti-Start if you will, override and do all shut down stuff here
	virtual void Finish() = 0;

	// Destroys the application, call from Finish()
	// Platform dependent
	virtual void Destroy();

	// Called every frame to do whatever
	virtual void DoFrame() = 0;

	// Called within the app to change the state
	INLINE void ChangeState( sys_App_States DesiredState );

	// Called when the app is about to change its state
	virtual void StateChanging( sys_App_States OldState, sys_App_States NewState )=0;

	// Returns the state that the app is currently in
	INLINE const sys_App_States GetState();

	// Safe way to terminate the app
	// Usually platform dependent so if overidden,
	// call the base version from the override
	virtual void Quit();

	// Method to allow access to the platform specific stuff
	INLINE std_PlatformParameters* PlatformParameters();

	// Returns the exit code of the application to the system.
	// override to provide something more exotic
	virtual int GetExitCode();

	// Creates a popup that informs the user of an error.
	// Default implementation is platform specific
	virtual BOOL ErrorBox( LPCTSTR strTitle, LPCTSTR strErrorMsg, ... );

	//virtual BOOL KeyDown


/////////////////////////////////////////////////
// Data members
protected:

	// Holds the current state information of the app
	sys_App_States				m_CurrentState;

	// Flag that determines if the app is to restart after terminating
	BOOL						m_bRestart;

	// Stores any neccessary platform specific parameters
	std_PlatformParameters		m_PlatformParameters;

	// Value given to the system at exit
	int							m_ExitCode;


/////////////////////////////////////////////////
// Intternal members / methods
protected:

	// Called to check the system for any awaiting messages.
	// Definetely platform dependent.
	BOOL CheckForSysMessage();


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	sys_App();
	virtual ~sys_App();
/////////////////////////////////////////////////

}; // End class - sys_App
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "sys_App.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SYS_APP_H_)
