// fe_MultiplayerSetup.cpp //                      \author Logan Jones
////////////////////////////                          \date 7/31/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_MultiplayerSetup.h"
#include "fe_SelectGame.h"
#include "fe_NewMultiplayerGame.h"
#include "net.h"
#include "game.h"
#include "gadget_Button.h"
#include "gadget_String.h"
#include "gadget_ListBox.h"
#include "gadget_Pic.h"
#include "ta_TDF.h"
#include "fe_MS_PlayerSelection.h"


UINT32	NextPlayer;
UINT32	PlayerCount;
typedef map< UINT32, fe_MS_PlayerSelection >		PlayerInfoMap_t;
PlayerInfoMap_t		Players;

enum fe_MultiplayerSetupMenuMessages {
	fems_ClientLogin,
	fems_Resync,
	fems_NewPlayer,
	fems_RemovePlayer,
	fems_Go,
};

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_MultiplayerSetup::fe_MultiplayerSetup()
{}
fe_MultiplayerSetup::~fe_MultiplayerSetup()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::Init() //                  \author Logan Jones
////////////////////////////////                     \date 04-24-2003
//
//===================================================================
// Parameters:
//  const char* strGameName   - 
//  const char* strPlayerName - 
//  const char* strPassword   - 
//
void fe_MultiplayerSetup::Init( const char* strGameName, const char* strPlayerName, const char* strPassword )
{
	if( strGameName ) // Start the server
		strcpy( m_GameName, strGameName ),
		strcpy( m_Password, strPassword ),
		//AddNewPlayer( NextPlayer++, ~0, strPlayerName ),
		Players[NextPlayer].Create( strPlayerName, NextPlayer, ~0, NULL, GetColor(), this ),
		net.StartServer( m_GameName, strlen(m_GameName) + 1 ),
		++NextPlayer,++PlayerCount,
		GetGadget("SYNCHING")->SetVisible(FALSE),
		GetGadget("battlestart")->SetVisible(FALSE),
		LoadSettings();
	else // Login to the server
		m_GameName[0] = '\0',
		GetGadget("battlestart")->SetVisible(),
		GetGadget("Start")->SetActive(FALSE),
		net.Write().Write( 0, 4 + strlen(strPlayerName) + 1 )
			<< (UINT32)fems_ClientLogin
			<< (UINT32)std_NameHash(strPassword)
			<< strPlayerName;
}
// End fe_MultiplayerSetup::Init()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::GetColor() //              \author Logan Jones
////////////////////////////////////                 \date 05-06-2003
//
//===================================================================
// Parameters:
//  UINT8 uiStart      - 
//  bool bReplaceStart - 
//  bool bFoward       - 
//
// Return: UINT8 - 
//
UINT8 fe_MultiplayerSetup::GetColor( UINT8 uiStart, bool bReplaceStart, bool bFoward )
{
	if( bReplaceStart ) {
		m_ColorsAvailable |= (1<<uiStart);
		if( bFoward ) (++uiStart==10) && (uiStart=0);
		else (uiStart--==0) && (uiStart=9);
	}

	if( bFoward ) while( !(m_ColorsAvailable & (1<<uiStart)) ) (++uiStart==10) && (uiStart=0);
	else while( !(m_ColorsAvailable & (1<<uiStart)) ) (uiStart--==0) && (uiStart=9);
	m_ColorsAvailable &= ~(1<<uiStart);
	return uiStart;
}
// End fe_MultiplayerSetup::GetColor()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OpenSlot() //              \author Logan Jones
////////////////////////////////////                 \date 05-06-2003
//
//===================================================================
// Return: std_Point - 
//
std_Point fe_MultiplayerSetup::OpenSlot()
{
	std_Point pt = m_SlotsOpen.front();
	m_SlotsOpen.pop_front();
	return pt;
}
// End fe_MultiplayerSetup::OpenSlot()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::Replace() //               \author Logan Jones
///////////////////////////////////                  \date 05-06-2003
//
//===================================================================
// Parameters:
//  UINT8 uiColor    - 
//  std_Point ptSlot - 
//
void fe_MultiplayerSetup::Replace( UINT8 uiColor, std_Point ptSlot )
{
	m_ColorsAvailable |=(1<<uiColor);
	
	list<std_Point>::iterator	it = m_SlotsOpen.begin(),
								end= m_SlotsOpen.end();
	for(; it!=end && ptSlot.y>(*it).y; ++it);
	m_SlotsOpen.insert( it, ptSlot );
}
// End fe_MultiplayerSetup::Replace()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnCreate() //              \author Logan Jones
////////////////////////////////////                   \date 7/31/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_MultiplayerSetup::OnCreate()
{
	gadget_Button*	pButton;
	gadget_Pic*		pPic;
	BYTE*			pBuf = NULL;
	DWORD			Size;

	// Load the gui file
	if( bFAILED(LoadGui("Lounge2")) )
		return FALSE;

	// Load the background
	if( bFAILED(LoadPanel("BattleRoom")) )
		return FALSE;

	// Load up the custom image gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/Lounge2.gaf", &pBuf );
	if( (pBuf!=NULL) && (Size!=0) ) {
		//if( pButton = (gadget_Button*)GetGadget("SIDEx") )
		//	gfx->CreateSurfacesFromGAF( pBuf, "SIDEx", 1, 5, SideSurfaces ),
		//	pButton->SetButtonImages( SideSurfaces, SideSurfaces[3], SideSurfaces[4] ),
		//	pButton->SetActive( TRUE ), pButton->SetVisible(TRUE);
		if( (pPic = (gadget_Pic*)GetGadget("battlestart")) )
			gfx->CreateSurfacesFromGAF( pBuf, "battlestart", 1, 9, m_BattleStartAnim ),
			pPic->SetFrames( m_BattleStartAnim, 9 );
		delete [] pBuf;
	}

	// Load up the custom image gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/Logos.gaf", &pBuf );
	if( (pBuf!=NULL) && (Size!=0) ) {
		gfx->CreateSurfacesFromGAF( pBuf, "32xlogos", 1, 10, m_Logos );
		delete [] pBuf;
	}

	gadget_ListBox* pBox = (gadget_ListBox*)GetGadget("Output");
	pBox->SetSort( FALSE );

	pButton = (gadget_Button*)GetGadget("PLAYERx");
	m_FirstPos = pButton->GetWndPos();

	ResetPlayers();
	return TRUE;
}
// End fe_MultiplayerSetup::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnDestroy() //             \author Logan Jones
/////////////////////////////////////                  \date 7/31/2002
//               
//====================================================================
//
void fe_MultiplayerSetup::OnDestroy()
{
	DestroyPlayers();
}
// End fe_MultiplayerSetup::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnUpdate() //              \author Logan Jones
////////////////////////////////////                    \date 9/5/2002
//               
//====================================================================
//
void fe_MultiplayerSetup::OnUpdate()
{
	bool			bQueried;
	UINT32			Msg;
	gadget_ListBox*	pBox = (gadget_ListBox*)GetGadget("Output");

	net.SendRecv( &bQueried );

	if( bQueried )
		pBox->AddListBoxItem( "Something pinged at me" );

	while( net.Read().BytesToRead() )
		if( net.Read().Marker()==net_SystemMarker ) OnSystemMessage();
		else { net.Read() >> Msg; theApp.Console.Comment( CT_LOAD, "fe_MultiplayerSetup::OnUpdate(): Handling message, %d", Msg );switch(Msg){
			case fems_ClientLogin: OnClientLogin(); break;
			case fems_Resync: OnResync(); break;
			case fems_NewPlayer: OnNewPlayer(); break;
			case fems_RemovePlayer: DestroyPlayer(net.Read().GetUInt32()); break;
			case fems_Go: sound.PlaySound( guiResources.Sounds.BigButton ); SaveSettings(); theApp.SetStartMenu( "SelectGame" ); theApp.StartGame(); break;
			default:theApp.Console.Comment( CT_LOAD, "fe_MultiplayerSetup::OnUpdate(): Unknown message, %d", Msg );
		}}
}
// End fe_MultiplayerSetup::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnWndMessage() //          \author Logan Jones
////////////////////////////////////////               \date 7/31/2002
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
DWORD fe_MultiplayerSetup::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;
	UINT32				Who;

	if( dwMessage==gui_msg_ButtonPressed )
    {
		if( strSender[0]=='#' )
        {
            Who=atoi(strSender+1);
            STRING_SWITCH( strSender+5 )

			CASE( "Side" )
				sound.PlaySound( guiResources.Sounds.Options );
				fe_MS_PlayerSelection&		Player = Players[Who];
				if( Player.Side==theGame.SideData.end() )
					Player.Side = theGame.SideData.begin();
				else Player.Side++;
				Player.SideSel->SetText( Player.Side==theGame.SideData.end() ? "WATCH":(*Player.Side).name );
				return 1;

			CASE( "Color" )
				sound.PlaySound( guiResources.Sounds.Options );
				fe_MS_PlayerSelection&		Player = Players[Who];
				Player.Color = GetColor( Player.Color, true, dwParamB==1 );
				Player.ColorSel->SetFrame( Player.Color );
				return 1;
		
            END_STRING_SWITCH
        }
        else STRING_SWITCH( strSender )

			CASE( "PrevMenu" )
				net.EndSession();
				sound.PlaySound( guiResources.Sounds.Previous );
				if( m_GameName[0]=='\0' ) SWITCH_ROOTS( fe_SelectGame );
				else SWITCH_ROOTS( fe_NewMultiplayerGame )->Init( m_GameName, Players[0].Name, m_Password );
				return 1;

			CASE( "RESTRICTIONS" )
				sound.PlaySound( guiResources.Sounds.Options );
				//((gadget_Pic*)GetGadget("battlestart"))->Animate();
				//GetGadget("SYNCHING")->SetActive();
				return 1;

			CASE( "START" )
				sound.PlaySound( guiResources.Sounds.BigButton );
				net.StopQuery();
				net.Write().Write(~0,4) << (UINT32)fems_Go;
				SaveSettings();
				theApp.SetStartMenu( "BattleRoom" );
				theApp.StartGame();
				return 1;

        END_STRING_SWITCH
    }

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_MultiplayerSetup::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::ResetPlayers() //          \author Logan Jones
////////////////////////////////////////             \date 05-06-2003
//
//===================================================================
//
void fe_MultiplayerSetup::ResetPlayers()
{
	DestroyPlayers();
	NextPlayer = 0;
	PlayerCount = 0;
	m_ColorsAvailable = ~0;

	std_Point Pos = m_FirstPos;
	for( int n=0; n<16; ++n,Pos.y+=20) m_SlotsOpen.push_back( Pos );
}
// End fe_MultiplayerSetup::ResetPlayers()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::DestroyPlayers() //        \author Logan Jones
//////////////////////////////////////////           \date 05-06-2003
//
//===================================================================
//
void fe_MultiplayerSetup::DestroyPlayers()
{
	PlayerInfoMap_t::iterator	it = Players.begin(),
								end= Players.end(),del;
	while( it!=end )
		del = it++,
		(*del).second.Destroy(this),
		--PlayerCount,
		Players.erase( del );
}
// End fe_MultiplayerSetup::DestroyPlayers()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::DestroyPlayer() //         \author Logan Jones
/////////////////////////////////////////            \date 05-06-2003
//
//===================================================================
// Parameters:
//  UINT32 uiWho - 
//
void fe_MultiplayerSetup::DestroyPlayer( UINT32 uiWho )
{
	Players[uiWho].Destroy(this);
	Players.erase( uiWho );
	--PlayerCount;
}
// End fe_MultiplayerSetup::DestroyPlayer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::RemovePlayers() //         \author Logan Jones
/////////////////////////////////////////            \date 05-06-2003
//
//===================================================================
// Parameters:
//  UINT32 uiWho - 
//
void fe_MultiplayerSetup::RemovePlayers( UINT32 uiWho )
{
	PlayerInfoMap_t::iterator	it = Players.begin(),
								end= Players.end(),next;
	while( it!=end )
		if( ((*it).second).NetID==uiWho )
			next=it,++next,
			net.Write().Write(~0,8) << (UINT32)fems_RemovePlayer << (*it).first,
			(*it).second.Destroy(this),
			--PlayerCount,
			Players.erase(it),it=next;
		else ++it;
}
// End fe_MultiplayerSetup::RemovePlayers()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnSystemMessage() //       \author Logan Jones
///////////////////////////////////////////          \date 04-24-2003
//
//===================================================================
//
void fe_MultiplayerSetup::OnSystemMessage()
{
	UINT32			Msg,Who;
	char			Str[64];
	gadget_ListBox*	pBox = (gadget_ListBox*)GetGadget("Output");

	net.Read() >> Msg >> Who;

	switch( Msg ) {
		case net_NewConnection:
			if( net.IsServer() )
				sprintf( Str, "**Player %d arrived.", Who ),
				pBox->AddListBoxItem( Str );
			break;
		case net_ConnectionClosed:
			if( net.IsServer() )
				RemovePlayers( Who ),
				sprintf( Str, "**Player %d left.", Who ),
				pBox->AddListBoxItem( Str );
			else SendMessage( this, gui_msg_ButtonPressed, (DWORD)"PrevMenu", 0 );
			break;
	}
}
// End fe_MultiplayerSetup::OnSystemMessage()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::ResyncClient() //          \author Logan Jones
////////////////////////////////////////             \date 04-24-2003
//
//===================================================================
// Parameters:
//  UINT32 uiWho - 
//
void fe_MultiplayerSetup::ResyncClient( UINT32 uiWho )
{
	net.Write().Write(uiWho) << (UINT32)fems_Resync
		<< ((gadget_String*)GetGadget("MapName"))->GetText()
		<< PlayerCount;
	PlayerInfoMap_t::iterator	it = Players.begin(),
								end= Players.end();
	for(; it!=end; ++it)
		((*it).second).Send( uiWho, (*it).first );
}
// End fe_MultiplayerSetup::ResyncClient()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnClientLogin() //         \author Logan Jones
/////////////////////////////////////////            \date 04-24-2003
//
//===================================================================
//
void fe_MultiplayerSetup::OnClientLogin()
{
	UINT32		ClientPassword;
	UINT32		Who = net.Read().Marker();
	char		Str[64];
	gadget_ListBox*	pBox = (gadget_ListBox*)GetGadget("Output");

	net.Read() >> ClientPassword >> Str;

	if( ClientPassword==std_NameHash(m_Password) )
		Players[NextPlayer].Create( Str, NextPlayer, Who, NULL, GetColor(), this ),
		Players[NextPlayer].Send( ~0, NextPlayer, fems_NewPlayer ),
		++PlayerCount,
		ResyncClient( Who ),
		++NextPlayer;

	else // Invalid password
		Players.erase(Who),
		sprintf( Str, "Bad Password; %X != %X", ClientPassword, std_NameHash(m_Password) ),
		pBox->AddListBoxItem( Str ),
		net.RemoveClient( Who );
}
// End fe_MultiplayerSetup::OnClientLogin()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnResync() //              \author Logan Jones
////////////////////////////////////                 \date 05-06-2003
//
//===================================================================
//
void fe_MultiplayerSetup::OnResync()
{
	UINT32	c,index,count;
	UINT8	local, color;
	char	name[32];
	char	side[16];

	net.Read() >> name >> count;
	((gadget_String*)GetGadget("MapName"))->SetText( name );

	ResetPlayers();
	theApp.Console.Comment( CT_LOAD, "fe_MultiplayerSetup::OnResync(): Loading %d players", count );
	for( c=0; c<count; ++c) {
		net.Read() >> index >> local >> name >> side >> color;
		theApp.Console.Comment( CT_LOAD, "fe_MultiplayerSetup::OnResync(): Player %d %s (%s)", index, name, local ? "local":"non-local" );
		Players[index].Create( name, index, local ? ~0:0, side, color, this ),
		++PlayerCount;
	}
	GetGadget("SYNCHING")->SetVisible(FALSE);
}
// End fe_MultiplayerSetup::OnResync()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::OnNewPlayer() //           \author Logan Jones
///////////////////////////////////////              \date 05-06-2003
//
//===================================================================
//
void fe_MultiplayerSetup::OnNewPlayer()
{
	UINT32	index;
	UINT8	local, color;
	char	name[32];
	char	side[16];

	net.Read() >> index >> local >> name >> side >> color;
	if( PlayerCount==0 )
		theApp.Console.Comment( CT_LOAD, "fe_MultiplayerSetup::OnNewPlayer(): Ignoring" );
	else
		Players[index].Create( name, index, local ? ~0:0, side, color, this ),
		++PlayerCount;
}
// End fe_MultiplayerSetup::OnNewPlayer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MultiplayerSetup::SaveSettings() //          \author Logan Jones
////////////////////////////////////////             \date 05-06-2003
//
//===================================================================
//
void fe_MultiplayerSetup::SaveSettings()
{
	//theGame.Settings.Difficulty = ((gadget_Button*)GetGadget("Difficulty"))->GetStage();
	theGame.Settings.CommanderDeath = ((gadget_Button*)GetGadget("Commander"))->GetStage();
	theGame.Settings.LineOfSight = ((gadget_Button*)GetGadget("LOSType"))->GetStage();
	theGame.Settings.Location = ((gadget_Button*)GetGadget("FixedLoc"))->GetStage();
	theGame.Settings.Mapping = ((gadget_Button*)GetGadget("Mapping"))->GetStage();
	strcpy( theGame.Settings.Map, ((gadget_String*)GetGadget("MapName"))->GetText() );

	theGame.SetPlayerCount( PlayerCount );
	PlayerInfoMap_t::iterator	it = Players.begin(),
								end= Players.end();
	for( DWORD i=0; it!=end; ++it,++i)
		theGame.SetPlayer( i, (((*it).second).NetID==~0) ? PT_LOCAL_USER:PT_NETWORK, (*((*it).second).Side).name, ((*it).second).Color, 1000, 1000 );
}
// End fe_MultiplayerSetup::SaveSettings()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_MultiplayerSetup.cpp //
//////////////////////////////////