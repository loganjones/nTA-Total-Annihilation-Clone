// game_Load.cpp //                                \author Logan Jones
//////////////////                                    \date 3/28/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "gfx.h"
#include "snd.h"
#include "net.h"
#include "fe.h"
#include "ta_OTA.h"


struct LoadGameData_t {
	class gfx_Surface*	Background;
	class gfx_Surface*	LightBar;
	class gfx_Surface*	ConsolePanels[ 9 ];
	int					Progress[ 8 ];
	list<UINT32>		Clients;
	bool				ReadyToPlay;

	LoadGameData_t():Background(NULL),LightBar(NULL),ReadyToPlay(false){
		BYTE* pBuf = NULL; DWORD Size = theFileTree.DumpFileToBuffer( "bitmaps/LoadGame2bg.pcx", &pBuf );
		gfx->CreateSurfaceFromPCX( pBuf, Size, GFX_FORCE_CURRENT_PALETTE, &Background ); delete [] pBuf;
		gfx->CreateSurfaceFromGAF( guiResources.CommonGuiBuffer, "LightBar", 1, &LightBar );
		gfx->CreateSurfacesFromGAF( guiResources.CommonGuiBuffer, "ListBox", 1, 9, ConsolePanels );
	}
	~LoadGameData_t(){
		SAFE_DESTROY( Background );
		SAFE_DESTROY( LightBar );
		for( int n=0; n<9; ++n ) SAFE_DESTROY( ConsolePanels[n] );
	}
}	*pLoadGameData = NULL;

enum LoadGameNetMessages {
	LGNM_ClientReady = NET_MESSAGE_COUNT,
	LGNM_Go,
};


/////////////////////////////////////////////////////////////////////
// game_LoadFacilitator implementation
//
game_LoadFacilitator::game_LoadFacilitator( int iCategory ):Category(iCategory){}
void game_LoadFacilitator::Progress( int iProgress )
{
	pLoadGameData->Progress[Category] = iProgress;
	theApp.DoFrame();
}
void game_LoadFacilitator::Refresh()
{
	theApp.DoFrame();
}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Main::LoadSideData() //                    \author Logan Jones
//////////////////////////////                       \date 04-28-2003
//
//===================================================================
// Parameters:
//  const char* strSetRestrictions - 
//
// Return: bool - 
//
bool game_Main::LoadSideData( const char* strSetRestrictions )
{
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;
	BOOL							bSearching;
	DWORD							FileSize;
	BYTE*							FileBuffer;

	SideData.clear();

	// Proccess downloads
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "tdf", "gamedata/sides", &FindInfo );
	for( bSearching=hFind!=NULL; bSearching; bSearching=theFileTree.ContinueFind(hFind,&FindInfo) ){
		// Get the TDF file buffer and proccess
		FileSize = theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(FindInfo.hFile), &FileBuffer );
		ta_tdf_GetSideData( FileBuffer, FileSize, SideData );
		SAFE_DELETE_ARRAY( FileBuffer );
	} // end while( tdf files )
	theFileTree.CloseFind( hFind );

	return SideData.size()>0;
}
// End game_Main::LoadSideData()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::Load() //                            \author Logan Jones
//////////////////////                                 \date 3/28/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL game_Main::Load()
{
	BOOL						bLoadSuccessfull = FALSE;
	BYTE*						pBuf = NULL;
	DWORD						Size;
	std_Time_t					t;
	char						FilePath[ MAX_PATH ];
	ta_ota_GlobalHeader_t		Ota;
	ta_ota_Schema_t				Schema;
#define ERROR_AND_DELAY( SECONDS_TO_DELAY, SOUND_STRING ) {\
	sound.PlaySound( SOUND_STRING ); \
	t = std_Time(); \
	while( (std_Time()-t)<(SECONDS_TO_DELAY * 1000) ) theApp.DoFrame(); \
	EXIT_CODE_BLOCK }

	// Make sure the gui stuff is loaded
	guiResources.Create();

	// Game is now loading
	Tick = 0;
	LoadGameData_t	LoadGameData;
	pLoadGameData = &LoadGameData;

	// Start loading
	BEGIN_CODE_BLOCK

	// Get a list of loading clients
	if( net.IsConnected() && net.IsServer() )
		net.GetClientList();

	// Set the game server flag
	IsGameServer = net.IsConnected() ? net.IsServer() : true;

	// Proccess the ota file
	sprintf( FilePath, "maps/%s.ota", theGame.Settings.Map );
	if( 0>=(Size = theFileTree.DumpFileToBuffer( FilePath, &pBuf )) ||
		bFAILED(ta_ota_GetSchema( pBuf, Size, "Network 1",&Schema, &Ota )) )
	{
		theApp.Console.Comment( CT_LOAD, "ERROR: Failed to retrieve the terrain description file, \'%s\'.", FilePath );
		ERROR_AND_DELAY( 3, "CantDo2" );
	}
	delete [] pBuf;

	// Load the terrain
	theApp.Console.Comment( CT_LOAD, "Loading terrain (%s)...", theGame.Settings.Map );
	theApp.DoFrame();
	{
		// Create the terrain interface
		sprintf( FilePath, "maps/%s.tnt", theGame.Settings.Map );
		if( bFAILED(Terrain.Create( FilePath, game_LoadFacilitator(0) )) )
		{
			ERROR_AND_DELAY( 3, "CantDo2" );
		}
	} // end Load the terrain

	// Create the interface
	if( bFAILED(theGame.Interface.Create()) )
		ERROR_AND_DELAY( 3, "CantDo2" );

	if( bFAILED(ColliderSystem.Create()) )
		ERROR_AND_DELAY( 3, "CantDo2" );
	if( bFAILED(ScenerySystem.Create()) )
		ERROR_AND_DELAY( 3, "CantDo2" );

	// Load the units
	theApp.Console.Comment( CT_LOAD, "Loading units..." );
	theApp.DoFrame();
	{

		theGame.Units.Create( game_LoadFacilitator(/*this,*/1) );
		pLoadGameData->Progress[1] = 100;
	} // end Load the units

	// We dont really need the CommonGui file buffer any more
	SAFE_DELETE_ARRAY( guiResources.CommonGuiBuffer );

	// Load the features
	theApp.Console.Comment( CT_LOAD, "Loading features..." );
	theApp.DoFrame();
	{
		LPCTSTR World;
		STRING_SWITCH( Ota.Planet )
			CASE( "green planet" )
				World = "green";
			CASE( "Archipelago" )
				World = "archi";
			CASE( "Water World" )
				World = "water";
			STRING_SWITCH_DEFAULT
				World = Ota.Planet;
		END_STRING_SWITCH

		theGame.Features.Create( Terrain.GetTntBuffer(), World, game_LoadFacilitator(/*this,*/2) );
		pLoadGameData->Progress[2] = 100;
	} // end Load the features

	// Load the weapons
	theApp.Console.Comment( CT_LOAD, "Loading weapons..." );
	theApp.DoFrame();
	{

		theGame.Weapons.Create( /*game_LoadFacilitator(1)*/ );
	} // end Load the weapons

	// What are we loading objects from
	if( false/*theGame.SaveFile*/ ){/*Load saved game*/}
	else {
		theGame.Units.ProccessSchema(&Schema);
		for( PlayerList_t::iterator it=Players.begin(),end=Players.end(); it!=end; ++it) (*it)->Initialize(&Schema);
	}
	ClearSideData();
	theApp.DoFrame();

	// Load complete
	bLoadSuccessfull = TRUE;
	END_CODE_BLOCK

	// This client is ready
	if( net.IsConnected()==false )
		LoadGameData.ReadyToPlay = true;
	else if( net.IsServer() )
		theApp.Console.Comment( CT_LOAD, "Waiting for clients..." );
	else theApp.Console.Comment( CT_LOAD, "Waiting for others..." ),
		net.Write().Write(0,1)<<(UINT8)LGNM_ClientReady;

	// Wait unitl everyone is loaded
	while( !LoadGameData.ReadyToPlay )
		theApp.DoFrame();

	// Go clients go
	if( net.IsServer() )
		net.Write().Write(~0,1) << (UINT8)LGNM_Go,
		theApp.DoFrame();

	if( bLoadSuccessfull )
		theApp.Console.Comment( CT_LOAD, "Game loaded successfully" );
	return bLoadSuccessfull;
}
// End game_Main::Load()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::RenderLoadScreen() //                \author Logan Jones
//////////////////////////////////                     \date 3/28/2002
//               
//====================================================================
//
void game_Main::RenderLoadScreen()
{
	gfx->Set2DProjection( gfx_Resolution_t(640,480), -1024, 256 );
	gfx->BeginScene();

	gfx->RenderSurface( pLoadGameData->Background );

	int count;
	const int NumLoadBars       = 6;
    const int ProgressWidth     = 351;
    const int ProgressHeight    = 21;
    const int LoadBarX          = 204;
    const int LoadBarY          = 136;
    const int NextLoadBarOffset = 42;
	const int StringX			= 90;
	const int StringY			= 144;
    const int NextStringOffset	= 43;
	char Strings[NumLoadBars][32] = {"Terrain","Units","Features","SFX","Other","Stuff"};

    // Draw the progress bars
	std_Point_t		ptBar( LoadBarX, LoadBarY );
	std_Size_t		szBar( ProgressWidth, ProgressHeight );
	PALETTEENTRY	peBar; peBar.peRed = 0xFF; peBar.peBlue = 0x00; peBar.peFlags = 0xFF;
	for( count=0; count<NumLoadBars; ++count, ptBar.y+=NextLoadBarOffset )
	{
        // Only draw if there is any progress
		if( pLoadGameData->Progress[count]!=0 )
		{
            // If 100% complete darw a full yellow bar
			if( pLoadGameData->Progress[count]>=100 )
			{
				peBar.peGreen = 0xFF;
				szBar.width = ProgressWidth;
			}
			else // Draw a red bar with % complete length
			{
				peBar.peGreen = 0x00;
				szBar.width = ProgressWidth * pLoadGameData->Progress[count] * 0.01f;
			}
			gfx->DrawRect( std_Rect_t(ptBar, szBar), peBar );
        } // end if( pLoadGameData->Progress[count]>=100 )

    } // end for( LoadBars )

    // Draw the lighbars over the progress bars
	for( count=0; count<NumLoadBars; ++count)
		gfx->RenderSurfaceEx( std_Point_t(LoadBarX,LoadBarY + NextLoadBarOffset*count), pLoadGameData->LightBar );

    // Draw the strings over the background
	gfx->SetCurrentFont( guiResources.Fonts.Standard );
	for( count=0; count<NumLoadBars; ++count)
		gfx->RenderStringAt( std_Point_t(StringX,StringY + NextStringOffset*count), Strings[count] );
	
	// Draw the console window
	{
		std_Rect_t	ConsoleWindowRect( std_Point_t(288,16), std_Size_t(320,96) );
		std_Point_t	Pos( ConsoleWindowRect.Position() );
		long x,y;

		Pos.y = ConsoleWindowRect.top + pLoadGameData->ConsolePanels[4]->GetSize().height;
		for( y=(ConsoleWindowRect.Size().height / pLoadGameData->ConsolePanels[4]->GetSize().height) - 2; y>0; --y, Pos.y += pLoadGameData->ConsolePanels[4]->GetSize().height)
		{
			Pos.x = ConsoleWindowRect.left + pLoadGameData->ConsolePanels[4]->GetSize().width;
			for( x=(ConsoleWindowRect.Size().width / pLoadGameData->ConsolePanels[4]->GetSize().width) - 2; x>0; --x, Pos.x += pLoadGameData->ConsolePanels[4]->GetSize().width)
				gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[4] );
		}

		Pos.x = ConsoleWindowRect.left + pLoadGameData->ConsolePanels[1]->GetSize().width;
		Pos.y = ConsoleWindowRect.top;
		for( x=(ConsoleWindowRect.Size().width / pLoadGameData->ConsolePanels[1]->GetSize().width) - 2; x>0; --x, Pos.x += pLoadGameData->ConsolePanels[1]->GetSize().width)
		{
			gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[1] );
			gfx->RenderSurfaceEx( Pos + std_Point_t(0,ConsoleWindowRect.Size().height - pLoadGameData->ConsolePanels[7]->GetSize().height), pLoadGameData->ConsolePanels[7] );
		}

		Pos.x = ConsoleWindowRect.left;
		Pos.y = ConsoleWindowRect.top + pLoadGameData->ConsolePanels[3]->GetSize().height;
		for( y=(ConsoleWindowRect.Size().height / pLoadGameData->ConsolePanels[3]->GetSize().height) - 2; y>0; --y, Pos.y += pLoadGameData->ConsolePanels[3]->GetSize().height)
		{
			gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[3] );
			gfx->RenderSurfaceEx( Pos + std_Point_t(ConsoleWindowRect.Size().width - pLoadGameData->ConsolePanels[5]->GetSize().width,0), pLoadGameData->ConsolePanels[5] );
		}

		gfx->RenderSurfaceEx( ConsoleWindowRect.Position(), pLoadGameData->ConsolePanels[0] );

		Pos.x = ConsoleWindowRect.right - pLoadGameData->ConsolePanels[2]->GetSize().width;
		Pos.y = ConsoleWindowRect.top;
		gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[2] );

		Pos.x = ConsoleWindowRect.left;
		Pos.y = ConsoleWindowRect.bottom - pLoadGameData->ConsolePanels[6]->GetSize().height;
		gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[6] );

		Pos.x = ConsoleWindowRect.right - pLoadGameData->ConsolePanels[8]->GetSize().width;
		Pos.y = ConsoleWindowRect.bottom - pLoadGameData->ConsolePanels[8]->GetSize().height;
		gfx->RenderSurfaceEx( Pos, pLoadGameData->ConsolePanels[8] );

		gfx->SetCurrentFont( guiResources.Fonts.StandardSmall );
		Pos.x = ConsoleWindowRect.left + 4;
		Pos.y = ConsoleWindowRect.bottom - 4;
		app_Console::CommentList_t::reverse_iterator	it = theApp.Console.m_CommentStack.rbegin();
		app_Console::CommentList_t::reverse_iterator	end= theApp.Console.m_CommentStack.rend();
		const long fheight = guiResources.Fonts.StandardSmall->GetHeight();
		for( ; it!=end; ++it )
		{
			if( CT_ALL & (*it).Type )
			{
				gfx->RenderStringAt( Pos, (*it).Comment.c_str() );
				Pos.y -= fheight + 2;
				if( (Pos.y - fheight) <= (ConsoleWindowRect.top) )
					break;
			}
		}

	} // end drawing the console window

	gfx->EndScene();

	net.SendRecv(),
	ProccessNetMessages();
}
// End game_Main::RenderLoadScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::SetPlayerCount() //                  \author Logan Jones
////////////////////////////////                       \date 4/14/2002
//               
//====================================================================
// Parameters:
//  DWORD dwCount - 
//
// Return: bool - 
//
bool game_Main::SetPlayerCount( DWORD dwCount )
{
	// Delete and clear the current players
	PlayerList_t::iterator			it = Players.begin();
	PlayerList_t::iterator	end= Players.end();
	for( ; it!=end; ++it) delete (*it);
	Players.clear();

	// If that was all we were supposed to do, return success
	if( dwCount==0 ) return true;

	// Set the new count and initialize the player pointers
	Players.resize( dwCount );	
	for( it=Players.begin(),end=Players.end(); it!=end; ++it)
		(*it) = NULL;

	return true;
}
// End game_Main::SetPlayerCount()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::SetPlayer() //                       \author Logan Jones
///////////////////////////                            \date 4/14/2002
//               
//====================================================================
// Parameters:
//  DWORD dwIndex      - 
//  PlayerTypes_t Type - 
//  LPCTSTR strSide    - 
//
// Return: bool - 
//
bool game_Main::SetPlayer( DWORD dwIndex, PlayerTypes_t Type, LPCTSTR strSide, DWORD dwColor, float fMaxMetal, float fMaxEnergy, UINT32 uiNetID )
{
	// Sanity check
	assert( dwIndex<Players.size() );

	switch( Type )
	{
		case PT_LOCAL_USER:
			Players[ dwIndex ] = new player_Local;
			break;
		case PT_AI:
			Players[ dwIndex ] = new player_AI;
			break;
		case PT_NETWORK:
			Players[ dwIndex ] = new player_Network;
			break;
		default: // We shouldn't get here
			return false;
	}

	Players[ dwIndex ]->SetSide( strSide, dwColor );
	Players[ dwIndex ]->Update( Metal, Maximum, fMaxMetal );
	Players[ dwIndex ]->Update( Energy,Maximum, fMaxEnergy);
	Players[ dwIndex ]->m_Index = dwIndex;
	return true;
}
// End game_Main::SetPlayer()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Main::LoadMessage() //                     \author Logan Jones
/////////////////////////////                        \date 05-08-2003
//
//===================================================================
// Parameters:
//  UINT8 uiMsg  - 
//  bool bSysMsg - 
//
void game_Main::LoadMessage( UINT8 uiMsg, bool bSysMsg )
{
	UINT32	i,n;

	if( bSysMsg ) {
		i = net.Read().GetUInt32();
		theApp.Console.Comment( CT_LOAD, "game_Main::LoadMessage(): System message, %d", i );
		switch( i ){
			case net_ConnectionClosed:
				pLoadGameData->Clients.remove(net.Read().GetUInt32());
				break;
			case net_ClientList: net.Read() >> n;
				for( i=0; i<n; ++i)
					pLoadGameData->Clients.push_back( net.Read().GetUInt32() );
				pLoadGameData->ReadyToPlay = n==0;
				break;
		}
	} else switch( uiMsg ){
		case LGNM_ClientReady:
			theApp.Console.Comment( CT_LOAD, "game_Main::LoadMessage(): Load message, LGNM_ClientReady" );
			pLoadGameData->Clients.remove(net.Read().Marker());
			pLoadGameData->ReadyToPlay = pLoadGameData->Clients.empty();
			break;
		case LGNM_Go:
			theApp.Console.Comment( CT_LOAD, "game_Main::LoadMessage(): Load message, LGNM_Go" );
			pLoadGameData->ReadyToPlay = true;
			break;
		default:
			theApp.Console.Comment( CT_LOAD, "game_Main::LoadMessage(): Undefined load message, %d", uiMsg );
	}
}
// End game_Main::LoadMessage()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Load.cpp //
////////////////////////
