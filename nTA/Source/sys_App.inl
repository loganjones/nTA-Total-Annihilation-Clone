// sys_App.inl //                                  \author Logan Jones
////////////////                                      \date 9/10/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::ChangeState() //                       \author Logan Jones
///////////////////////////                            \date 9/10/2001
// Description:  Called to change the app's state
//               
//====================================================================
// Parameters:
//  sys_App_States DesiredState - New state for the app
//
void sys_App::ChangeState( sys_App_States DesiredState )
{
	// Check for redundancy
	if( DesiredState==m_CurrentState )
	{
		//assertEx( 0, "Redundant chage of app state" );
		return;
	}

	// Make it known that the state will change
	StateChanging( m_CurrentState, DesiredState );

	// Change the state
	m_CurrentState = DesiredState;
}
// End sys_App::ChangeState()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::GetState() //                          \author Logan Jones
////////////////////////                               \date 9/10/2001
// Description:  Returns a value of the enumerated type sys_App::sys_App_States
//               that indicates the current state of the app
//====================================================================
// Return: const sys_App::sys_App_States - The current state
//
const sys_App::sys_App_States sys_App::GetState()
{
	// Return the current state of the app
	return ( m_CurrentState );
}
// End sys_App::GetState()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::PlatformParameters() //                \author Logan Jones
//////////////////////////////////                     \date 9/10/2001
// Description:  Gives access to the platform paramaterts via a pointer to the member
//               
//====================================================================
// Return: std_PlatformParameters* - A pointer to the m_PlatformParameters member of sys_app
//
std_PlatformParameters* sys_App::PlatformParameters()
{
	// Just return the address
	return ( &m_PlatformParameters );
}
// End sys_App::PlatformParameters()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - sys_App.inl //
//////////////////////