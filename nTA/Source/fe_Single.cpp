// fe_Single.cpp //                                \author Logan Jones
//////////////////                                   \date 10/16/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_Single.h"
#include "gadget_Button.h"
#include "fe_MainMenu.h"
#include "fe_Skirmish.h"
#include "fe_Options.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_Single::fe_Single():fe_Wnd()
{}
fe_Single::~fe_Single()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Single::OnCreate() //                        \author Logan Jones
//////////////////////////                            \date 10/16/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_Single::OnCreate()
{
	gadget_Button*		pButton;
	BYTE*				pBuf = NULL;
	DWORD				Size;

	// Load the gui file
	if( bFAILED(LoadGui("Single")) )
	{
		return FALSE;
	}

	// Load the background
	if( bFAILED(LoadPanel("SingleBG")) )
	{
		return FALSE;
	}

	// Load up the custom image gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/Single.gaf", &pBuf );
	if( (pBuf!=NULL) && (Size!=0) )
	{
		// Change the default images for the Credits button
		if( pButton = (gadget_Button*)GetGadget( "AnyMsn" ) )
		{			
			gfx->CreateSurfacesFromGAF( pBuf, "AnyMsn", 1, 5, m_Surfaces );
			pButton->SetButtonImages( m_Surfaces, m_Surfaces[1], m_Surfaces[2] );
		}

		// Delete the gaf buffer
		SAFE_DELETE_ARRAY( pBuf );
	}

	return fe_Wnd::OnCreate();
}
// End fe_Single::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Single::OnDestroy() //                       \author Logan Jones
///////////////////////////                           \date 10/17/2001
//               
//====================================================================
//
void fe_Single::OnDestroy()
{
	// Destroy the custom images
	for( int n=0; n<5; ++n) SAFE_DESTROY( m_Surfaces[n] );

	fe_Wnd::OnDestroy();
}
// End fe_Single::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Single::OnWndMessage() //                    \author Logan Jones
//////////////////////////////                        \date 10/16/2001
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
DWORD fe_Single::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;

	if( dwMessage==gui_msg_ButtonPressed )
	{
		STRING_SWITCH( strSender )

			CASE( "NewCamp" )
				sound.PlaySound( guiResources.Sounds.NotYetImplemented );
				return 1;

			CASE( "LoadGame" )
				sound.PlaySound( guiResources.Sounds.NotYetImplemented );
				return 1;

			CASE( "Skirmish" )
				sound.PlaySound( guiResources.Sounds.Skirmish );
				SWITCH_ROOTS( fe_Skirmish );
				return 1;

			CASE( "PrevMenu" )
				sound.PlaySound( guiResources.Sounds.Previous );
				SWITCH_ROOTS( fe_MainMenu );
				return 1;

			CASE( "Options" )
				sound.PlaySound( guiResources.Sounds.Options );
				SWITCH_ROOTS( fe_Options );
				return 1;
				

		END_STRING_SWITCH

	} // end if( dwMessage==msg_ButtonPressed )

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_Single::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_Single.cpp //
////////////////////////
