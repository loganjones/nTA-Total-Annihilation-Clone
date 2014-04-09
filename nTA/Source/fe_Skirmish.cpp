// fe_Skirmish.cpp //                              \author Logan Jones
////////////////////                                 \date 10/16/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_Skirmish.h"
#include "fe_Single.h"
#include "fe_SelectMap.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_Skirmish::fe_Skirmish():fe_Wnd()
{}
fe_Skirmish::~fe_Skirmish()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::OnCreate() //                      \author Logan Jones
////////////////////////////                          \date 10/16/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_Skirmish::OnCreate()
{
	// Load the gui file
	if( bFAILED(LoadGui("Skirmish")) )
		return FALSE;

	// Load the background
	if( bFAILED(LoadPanel("SkirmSetup4x")) )
		return FALSE;

	LoadSettings();

	pMapSelector = NULL;

	return fe_Wnd::OnCreate();
}
// End fe_Skirmish::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::OnDestroy() //                     \author Logan Jones
/////////////////////////////                          \date 7/30/2002
//               
//====================================================================
//
void fe_Skirmish::OnDestroy()
{
	SAFE_DESTROY_AND_DELETE( pMapSelector );
}
// End fe_Skirmish::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::OnWndMessage() //                  \author Logan Jones
////////////////////////////////                      \date 10/16/2001
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
DWORD fe_Skirmish::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;

	if( dwMessage==gui_msg_ButtonPressed )
		if( pMapSelector && pSender==pMapSelector && dwParamA )
			((gadget_String*)GetGadget("MapName"))->SetText( strSender );
		else STRING_SWITCH( strSender )

			CASE( "Start" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				SaveSettings();
				theApp.SetStartMenu( "Skirmish" );
				theApp.StartGame();
				return 1;

			CASE( "PrevMenu" )
				sound.PlaySound( guiResources.Sounds.Previous );
				SWITCH_ROOTS( fe_Single );
				return 1;

			CASE( "SelectMap" )
				if( pMapSelector==NULL ) pMapSelector = new fe_SelectMap;
				pMapSelector->Create(std_Point(),std_Size(),this);
				return 1;

			CASE( "StartLocation" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				return 1;
			CASE( "CommanderDeath" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				return 1;
			CASE( "Mapping" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				return 1;
			CASE( "LineOfSight" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				return 1;
			CASE( "Difficulty" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				return 1;
		END_STRING_SWITCH

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_Skirmish::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_Skirmish.cpp //
//////////////////////////
