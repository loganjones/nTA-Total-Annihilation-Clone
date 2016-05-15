// fe_SelectGame.cpp //                            \author Logan Jones
//////////////////////                                \date 5/13/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_SelectGame.h"
#include "fe_MainMenu.h"
#include "fe_NewMultiplayerGame.h"
#include "fe_MultiplayerSetup.h"
#include "gadget_Button.h"
#include "gadget_ListBox.h"
#include "net.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_SelectGame::fe_SelectGame()
{}
fe_SelectGame::~fe_SelectGame()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectGame::OnCreate() //                    \author Logan Jones
//////////////////////////////                         \date 5/13/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_SelectGame::OnCreate()
{
	// Load the gui file
	if( bFAILED(LoadGui("SelGame")) )
		return FALSE;

	// Load the background
	if( bFAILED(LoadPanel("SelectGame2x")) )
		return FALSE;

	m_GameList = (gadget_ListBox*)GetGadget("GameName");
	m_UpdateButton = (gadget_Button*)GetGadget("Update");
	m_JoinButton = (gadget_Button*)GetGadget("JoinGame");
	m_WatchButton = (gadget_Button*)GetGadget("Watch");
	gadget_TextEdit*	PlayerNameBox=(gadget_TextEdit*)GetGadget("NICKNAME");
	gadget_TextEdit*	PasswordBox = (gadget_TextEdit*)GetGadget("PASSWORD");

	PlayerNameBox->SetText( "Tester" );
	PasswordBox->SetText( "dog" );

	if( !m_GameList || !m_UpdateButton || !m_JoinButton || !m_WatchButton )
		return FALSE;
	m_GameList->SetSort( FALSE );
	m_UpdatingServerList = false;
	return TRUE;
}
// End fe_SelectGame::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectGame::OnDestroy() //                   \author Logan Jones
///////////////////////////////                        \date 5/13/2002
//               
//====================================================================
//
void fe_SelectGame::OnDestroy()
{
	m_UpdatingServerList = false;
}
// End fe_SelectGame::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectGame::OnUpdate() //                    \author Logan Jones
//////////////////////////////                          \date 9/5/2002
//               
//====================================================================
//
void fe_SelectGame::OnUpdate()
{
	char	Name[ 32 ];
//	long	Address;
//	UINT32	Type;
//	UINT32	Addr;
//	char	AddrStr[32];
	in_addr	theAddress;
//	ZeroMemory( &theAddress, sizeof(theAddress) );

	net.SendRecv();

	// Are we updating
	if( m_UpdatingServerList )
	{
		// Check for a response
		//if( net.CheckForQueryResponse(Name,Address) )
		//	m_GameList->AddListBoxItem( Name, (LPVOID)Address );
		while( net.Read().BytesToRead() ) {
			//Type = net.Read().GetUInt32(),
			//Addr = net.Read().GetUInt32();
			//( Type==(~0) )
			//	theAddress.s_addr = Addr,
			//	m_GameList->AddListBoxItem( inet_ntoa(theAddress), (LPVOID)Addr );
			//	m_GameList->AddListBoxItem( net.Read().GetString(), (LPVOID)net.Read().Marker() );
				theAddress.s_addr = net.Read().Marker();
				sprintf( Name, "%s %s", net.Read().GetString(), inet_ntoa(theAddress) );
				m_GameList->AddListBoxItem( Name, (LPVOID)(size_t)net.Read().Marker() );
		}

		// Check for a timeout
		if( (std_Time() - m_UpdateStartTime)>10000 )
			net.StopQuery(),
			m_UpdateButton->SetText( "Update" ),
			m_UpdatingServerList = false;
	}

	// Activate these buttons only if ther is a game to select
	m_JoinButton->SetActive( !m_GameList->IsEmpty() );
	m_WatchButton->SetActive( !m_GameList->IsEmpty() );
}
// End fe_SelectGame::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectGame::OnWndMessage() //                \author Logan Jones
//////////////////////////////////                     \date 5/13/2002
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
DWORD fe_SelectGame::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;
	gadget_TextEdit*	PlayerNameBox,*PasswordBox;
	UINT32				Address;

	if( dwMessage==gui_msg_ButtonPressed ) STRING_SWITCH( strSender )

			CASE( "StartNew" )
				net.StopQuery();
				sound.PlaySound( guiResources.Sounds.BigButton );
				SWITCH_ROOTS( fe_NewMultiplayerGame )->Init( "Test Game", "Test Master", "dog" );
				return 1;

			CASE( "Update" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				gadget_Button* pButton = (gadget_Button*)pSender;

				// Is an update already happening
				if( m_UpdatingServerList )
					// Stop the update
					net.StopQuery(),
					pButton->SetText( "Update" ),
					m_UpdatingServerList = false;
				else // No, do an update
					net.QueryServer(),
					m_GameList->ClearList(),
					pButton->SetText( "Stop" ),
					m_UpdatingServerList = true,
					m_UpdateStartTime = std_Time();
				return 1;

			CASE( "JoinGame" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				PlayerNameBox=(gadget_TextEdit*)GetGadget("NICKNAME");
				PasswordBox = (gadget_TextEdit*)GetGadget("PASSWORD");
				m_GameList->GetCurrentSelection( (void**)&Address );
				if( net.JoinServer(Address) )
					SWITCH_ROOTS( fe_MultiplayerSetup )->Init( NULL, PlayerNameBox->Text(), PasswordBox->Text() );
				return 1;

			CASE( "Watch" )
				// TODO: Implement game watching
				sound.PlaySound( guiResources.Sounds.NotYetImplemented );
				return 1;

			CASE( "PrevMenu" )
				sound.PlaySound( guiResources.Sounds.Previous );
				SWITCH_ROOTS( fe_MainMenu );
				return 1;

	END_STRING_SWITCH

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_SelectGame::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_SelectGame.cpp //
////////////////////////////
