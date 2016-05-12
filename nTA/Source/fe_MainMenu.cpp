// fe_MainMenu.cpp //                              \author Logan Jones
////////////////////                                 \date 10/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_MainMenu.h"
#include "gadget_Button.h"
#include "gadget_String.h"
#include "net.h"

#include "fe_Single.h"
#include "fe_SelectGame.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_MainMenu::fe_MainMenu():fe_Wnd()
{}
fe_MainMenu::~fe_MainMenu()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MainMenu::OnCreate() //                      \author Logan Jones
////////////////////////////                          \date 10/14/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_MainMenu::OnCreate()
{
	gadget_String*	pString;
	gadget_Button*	pButton;
	BYTE*			pBuf = NULL;
	DWORD			Size;

	// Load the gui file
	if( bFAILED(LoadGui("MainMenu")) )
	{
		return FALSE;
	}

	// Load the background
	if( bFAILED(LoadPanel("FrontendX")) )
	{
		return FALSE;
	}

	// Change the text of the debug string to show the version string
	if( (pString = (gadget_String*)GetGadget( "DebugString" )) )
		pString->SetText( "nTA 0.01" );
		//pString->SetText( "v3.1" );

	// Load up the custom image gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/MainMenu.gaf", &pBuf );
	if( (pBuf!=NULL) && (Size!=0) )
	{
		// Change the default images for the Credits button
		if( (pButton = (gadget_Button*)GetGadget( "Credits" )) )
		{			
			gfx->CreateSurfacesFromGAF( pBuf, "Credits", 1, 5, m_Surfaces );
			pButton->SetButtonImages( m_Surfaces, m_Surfaces[1], m_Surfaces[2] );
		}

		// Delete the gaf buffer
		SAFE_DELETE_ARRAY( pBuf );
	}

	// Disable the multiplayer button if the network is not available
	if( net.IsNetworkAvailable()==false && (pButton=(gadget_Button*)GetGadget("Multi"))!=NULL )
		pButton->SetActive( FALSE );

	return fe_Wnd::OnCreate();
}
// End fe_MainMenu::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MainMenu::OnDestroy() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
//
void fe_MainMenu::OnDestroy()
{
	// Destroy the custom images
	for( int n=0; n<5; ++n) SAFE_DESTROY( m_Surfaces[n] );

	fe_Wnd::OnDestroy();
}
// End fe_MainMenu::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MainMenu::OnWndMessage() //                  \author Logan Jones
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
DWORD fe_MainMenu::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;

	if( dwMessage==gui_msg_ButtonPressed )
	{
		STRING_SWITCH( strSender )

			CASE( "Single" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				SWITCH_ROOTS( fe_Single );
				return 1;

			CASE( "Intro" )
				sound.PlaySound( guiResources.Sounds.NotYetImplemented );
				return 1;

			CASE( "Multi" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				SWITCH_ROOTS( fe_SelectGame );
				return 1;

			CASE( "Exit" )
				sound.PlaySound( guiResources.Sounds.Exit );
				Destroy();
				theApp.Quit();
				return 1;

			CASE( "Credits" )
				sound.PlaySound( "CDoggy" );
				return 1;
				

		END_STRING_SWITCH

	} // end if( dwMessage==msg_ButtonPressed )

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_MainMenu::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_MainMenu.cpp //
//////////////////////////
