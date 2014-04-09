// sys_App.cpp //                                  \author Logan Jones
////////////////                                       \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "sys.h"
#include "sys_App.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "sys_App.inl"
#endif // defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
sys_App::sys_App():
	m_ExitCode( 0 )
{}
sys_App::~sys_App()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Initialize() //                        \author Logan Jones
//////////////////////////                             \date 9/10/2001
// Description:  Initializes theApp data members for start-up
//               
//====================================================================
//
void sys_App::Initialize()
{
	// The app is now starting
	m_CurrentState = State_StartUp;

	// Set (reset) the restart flag
	m_bRestart = FALSE;
}
// End sys_App::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Run() //                               \author Logan Jones
///////////////////                                    \date 9/10/2001
// Description:  Starting point of the application. The app's root.
//               
//====================================================================
//
void sys_App::Run()
{
	// Start running the app
	do {

		// Initialize data members
		Initialize();

		if(bFAILED( Start() ))
		{
			// Start failed; clean up and skip the loop
			ChangeState( State_ShutDown );
			Finish();
		}
		else // Continue with the app loop
		{
			ChangeState( State_Active );

			while( (m_CurrentState==State_Active) || (m_CurrentState==State_Paused) )
			{
				// If a message is proccessed, reevaluate the loop condition
				if( CheckForSysMessage() ) continue;

				DoFrame();

			} // end while( State_Running )

			// Loop is done, shutdown the app
			Finish();

		} // end else // Continue with the app loop

	// If the restart flag is set, start all over again
	} while( m_bRestart );
}
// End sys_App::Run()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::GetExitCode() //                       \author Logan Jones
///////////////////////////                            \date 9/10/2001
// Description:  Returns a value to the system at exit
//               
//====================================================================
// Return: int - The exit code
//
int sys_App::GetExitCode()
{
	// Just return the exit code member
	return ( m_ExitCode );
}
// End sys_App::GetExitCode()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - sys_App.cpp //
//////////////////////