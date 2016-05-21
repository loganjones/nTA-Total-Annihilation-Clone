// fe_NewMultiplayerGame.cpp //                    \author Logan Jones
//////////////////////////////                        \date 7/31/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_NewMultiplayerGame.h"
#include "fe_SelectGame.h"
#include "fe_MultiplayerSetup.h"
#include "net.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_NewMultiplayerGame::fe_NewMultiplayerGame()
{}
fe_NewMultiplayerGame::~fe_NewMultiplayerGame()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_NewMultiplayerGame::Init() //                \author Logan Jones
//////////////////////////////////                   \date 04-24-2003
//
//===================================================================
// Parameters:
//  const char* strGameName   - 
//  const char* strPlayerName - 
//  const char* strPassword   - 
//
void fe_NewMultiplayerGame::Init( const char* strGameName, const char* strPlayerName, const char* strPassword )
{
	gadget_TextEdit*	GameNameBox,*PlayerNameBox,*PasswordBox;
	GameNameBox = (gadget_TextEdit*)GetGadget("GAMENAME");
	PlayerNameBox=(gadget_TextEdit*)GetGadget("NICKNAME");
	PasswordBox = (gadget_TextEdit*)GetGadget("PASSWORD");

	if( GameNameBox )
		if( strGameName )
			GameNameBox->SetText( strGameName );

	if( PlayerNameBox )
		if( strPlayerName )
			PlayerNameBox->SetText( strPlayerName );

	if( PasswordBox )
		if( strPassword )
			PasswordBox->SetText( strPassword );
}
// End fe_NewMultiplayerGame::Init()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_NewMultiplayerGame::OnCreate() //            \author Logan Jones
//////////////////////////////////////                 \date 7/31/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_NewMultiplayerGame::OnCreate()
{
	// Load the gui file
	if( bFAILED(LoadGui("NewMulti")) )
		return FALSE;

	// Load the background
	if( bFAILED(LoadPanel("CreateNew")) )
		return FALSE;

	return fe_Wnd::OnCreate();
}
// End fe_NewMultiplayerGame::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_NewMultiplayerGame::OnWndMessage() //        \author Logan Jones
//////////////////////////////////////////             \date 7/31/2002
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
DWORD fe_NewMultiplayerGame::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;
	gadget_TextEdit*	GameNameBox,*PlayerNameBox,*PasswordBox;

	if( dwMessage==gui_msg_ButtonPressed ) STRING_SWITCH( strSender )

			CASE( "Ok" )
				sound.PlaySound( "Button1" );
				GameNameBox = (gadget_TextEdit*)GetGadget("GAMENAME");
				PlayerNameBox=(gadget_TextEdit*)GetGadget("NICKNAME");
				PasswordBox = (gadget_TextEdit*)GetGadget("PASSWORD");
				SWITCH_ROOTS( fe_MultiplayerSetup )->Init( GameNameBox->Text(), PlayerNameBox->Text(), PasswordBox->Text() );
				return 1;

			CASE( "Cancel" )
				sound.PlaySound( guiResources.Sounds.Previous );
				SWITCH_ROOTS( fe_SelectGame );
				return 1;

	END_STRING_SWITCH

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_NewMultiplayerGame::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_NewMultiplayerGame.cpp //
////////////////////////////////////