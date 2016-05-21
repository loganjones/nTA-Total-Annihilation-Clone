// fe_SelectConnection.cpp //                      \author Logan Jones
////////////////////////////                          \date 5/13/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_SelectConnection.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_SelectConnection::fe_SelectConnection()
{}
fe_SelectConnection::~fe_SelectConnection()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectConnection::OnCreate() //              \author Logan Jones
////////////////////////////////////                   \date 5/13/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_SelectConnection::OnCreate()
{
	// Load the gui file
	if( bFAILED(LoadGui("SelProv")) )
		return FALSE;

	// Load the background
	if( bFAILED(LoadPanel("SelConnect2")) )
		return FALSE;

	return TRUE;
}
// End fe_SelectConnection::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectConnection::OnDestroy() //             \author Logan Jones
/////////////////////////////////////                  \date 5/13/2002
//               
//====================================================================
//
void fe_SelectConnection::OnDestroy()
{
}
// End fe_SelectConnection::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectConnection::OnWndMessage() //          \author Logan Jones
////////////////////////////////////////               \date 5/13/2002
//               
//====================================================================
// Parameters:
//  wnd_Window* pSender - 
//  DWORD dwMessage     - 
//  DWORD dwParamA      - 
//  DWORD dwParamB      - 
//
// Return: DWORD - 
//
DWORD fe_SelectConnection::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_SelectConnection::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_SelectConnection.cpp //
//////////////////////////////////
