// game_UserInterface_Load.cpp //                  \author Logan Jones
////////////////////////////////                      \date 11/5/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "game.h"
#include "ta_TDF.h"


//////////////////////////////////////////////////////////////////////
// game_UserInterface::Create() //                 \author Logan Jones
/////////////////////////////////                       \date 4/1/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL game_UserInterface::Create()
{
	// Call the default
	if( bFAILED(wnd_Manager::Create()) )
		return FALSE;

	m_GuiResolution = *gfx->GetScreenResolution();
	m_GameWindow.m_TerrainRes = m_GuiResolution;

	// Initialize the root window
	m_pRootWindow->SetWndManager( this );
	m_pRootWindow->SetWndSize( m_GuiResolution );

	// Create the console window
	if( bFAILED(m_Console.Create( std_Point(0,0), std_Size(m_GuiResolution.width,m_GuiResolution.height*0.75f), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	// Create the game window
	if( bFAILED(m_GameWindow.Create( std_Point(128,32), std_Size(m_GuiResolution.width-128,m_GuiResolution.height-64), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	// Create the resource bar
	if( bFAILED(m_ResourceBar.Create( std_Point(128,0), std_Size(m_GuiResolution.width-128,32), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	// Create the info bar
	if( bFAILED(m_InfoBar.Create( std_Point(128,m_GuiResolution.height-32), std_Size(m_GuiResolution.width-128,32), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	// Create the order panel
	if( bFAILED(m_ControlBar.Create( std_Point(0,0), std_Size(128,m_GuiResolution.height), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	// Create the mini-map
	if( bFAILED(m_MiniMap.Create( std_Point(0,0), std_Size(128,128), m_pRootWindow )) )
	{
		Destroy();
		return FALSE;
	}

	SetFocus( &m_ControlBar );
	return TRUE;
}
// End game_UserInterface::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::Initialize() //             \author Logan Jones
/////////////////////////////////////                  \date 4/27/2002
//               
//====================================================================
// Parameters:
//  class player_Local* pPlayer  - 
//  std_Point StartPosition      - 
//  struct SideData_t* pSideData - 
//
// Return: BOOL - 
//
BOOL game_UserInterface::Initialize( class player_Local* pPlayer, std_Point StartPosition, struct SideData_t* pSideData )
{
	char	Path[128];
    BYTE*	FileBuf;

	// Make this class the current input handler
	theApp.Console.SetInputHandler( this );

	// Set the player pointer
	m_Player = pPlayer;

	// Create the fonts
	sprintf( Path, "fonts/%s.fnt", pSideData->font );
	if( theFileTree.DumpFileToBuffer( Path, &FileBuf )==0 ||
		bFAILED(gfx->CreateFontFromFNT( FileBuf, &m_Font )) )
		m_Font = guiResources.Fonts.StandardSmall;
	delete [] FileBuf;
	sprintf( Path, "fonts/%s.fnt", pSideData->fontgui );
	if( theFileTree.DumpFileToBuffer( Path, &FileBuf )==0 ||
		bFAILED(gfx->CreateFontFromFNT( FileBuf, &m_GuiFont )) )
		m_GuiFont = guiResources.Fonts.Standard;
	delete [] FileBuf;
	if( theFileTree.DumpFileToBuffer( "fonts/console.fnt", &FileBuf )==0 ||
		bFAILED(gfx->CreateFontFromFNT( FileBuf, &m_ConsoleFont )) )
		m_GuiFont = guiResources.Fonts.Standard;
	delete [] FileBuf;

	// Get the gui surfaces
	sprintf( Path, "anims/%s.gaf", pSideData->intgaf );
	if( theFileTree.DumpFileToBuffer( Path, &FileBuf )==0 )
		return FALSE;
	gfx->CreateSurfaceFromGAF( FileBuf, "PANELTOP", 0, std_Size(512,32), &m_Surfaces[TopPanel] );
	gfx->CreateSurfaceFromGAF( FileBuf, "PANELBOT", 0, std_Size(512,32), &m_Surfaces[BottomPanel] );
	gfx->CreateSurfaceFromGAF( FileBuf, "PANELSIDE2", 0, std_Size(128,480), &m_Surfaces[SidePanel] );
	gfx->CreateSurfaceFromGAF( FileBuf, "PANELSIDE", 0, std_Size(128,480), &m_Surfaces[SidePanelLower] );
    delete [] FileBuf;

	// Initialize the control bars with the side data
	m_ResourceBar.Initialize( pSideData );
	m_InfoBar.Initialize( pSideData );

	// Initialize the cursor
	SetCursorPosition( std_Point(m_GuiResolution.width/2,m_GuiResolution.height/2) );
	SetCursorSpeed( 1.75f );

	// Set the terrain view
	theGame.Terrain.SetCenter( StartPosition );

	// return success
	return TRUE;
}
// End game_UserInterface::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::Destroy() //                \author Logan Jones
//////////////////////////////////                      \date 4/1/2002
//               
//====================================================================
//
void game_UserInterface::Destroy()
{
	// We no longer want to handle input
	theApp.Console.SetInputHandler( NULL );

	// Destroy any created surfaces
	SAFE_DESTROY( m_Surfaces[TopPanel] );
	SAFE_DESTROY( m_Surfaces[BottomPanel] );
	SAFE_DESTROY( m_Surfaces[SidePanel] );
	SAFE_DESTROY( m_Surfaces[SidePanelLower] );

	// Call the default to destroy all the child windows
	wnd_Manager::Destroy();
}
// End game_UserInterface::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::CreateBuildMenu() //        \author Logan Jones
//////////////////////////////////////////             \date 4/27/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strUnitName - 
//  DWORD& dwMenuCount  - 
//
// Return: BOOL - 
//
BOOL game_UserInterface::CreateBuildMenu( unit_Type& UnitType )
{
	DownloadMenus_t::iterator				it = m_Downloads.find( UnitType.Key );
	game_UserInterface::DownloadMenuPages_t*	pPages;	
	game_UserInterface::DownloadMenuPage_t*		pPage;
	long									PageCount=0, count;
	char									MenuName[ 32 ];
	char									Path[ 32 ];
	char									Key[ 32 ];
	LPVOID									hGui;
	LPVOID									hGaf;
	igui_Menu*								pMenu;
	ta_gui_Common_t							CommonInfo;
	ta_gui_Button							ButtonInfo;

// Macro that sets the position of a download entry
#define GET_ENTRY_POS( INDEX ) \
	switch( INDEX ) { \
		case 0: CommonInfo.XPos = 0; CommonInfo.YPos = 27; break; \
		case 1: CommonInfo.XPos = 64;CommonInfo.YPos = 27; break; \
		case 2: CommonInfo.XPos = 0; CommonInfo.YPos = 91; break; \
		case 3: CommonInfo.XPos = 64;CommonInfo.YPos = 91; break; \
		case 4: CommonInfo.XPos = 0; CommonInfo.YPos = 155;break; \
		case 5: CommonInfo.XPos = 64;CommonInfo.YPos = 155;break; \
		default:CommonInfo.XPos = 0; CommonInfo.YPos = 27; }

	// Inititalize the order panel members
	m_ControlBar.m_pFileBuffer = NULL;
	m_ControlBar.m_KeyPrefix = Key;

	// Check if any download entries were given
	if( it==m_Downloads.end() ) pPages = NULL;
	else {
		pPages = &(*it).second;
		PageCount = pPages->size();
	}

	// Initialize the download button info
	ZeroMemory( &CommonInfo, sizeof(ta_gui_Common_t) );
	ZeroMemory( &ButtonInfo, sizeof(ta_gui_Button) );
	CommonInfo.ID = GADGET_Button;
	CommonInfo.Attribs = 32;
	CommonInfo.Assoc = 77;
	CommonInfo.Active = 1;
	CommonInfo.CommonAttribs = 4;
	CommonInfo.Width = CommonInfo.Height = 64;

	// Start at zero and loop until there are no more guis (if there were any)
	for( UnitType.BuildMenuCount=count=0; ; ++UnitType.BuildMenuCount,++count)
	{
		// Configure the gui/gaf name we should be looking for
		sprintf( MenuName, "%s%d", UnitType.UnitName, UnitType.BuildMenuCount + 1 );

		// Try to open the gui and the gaf
		sprintf( Path, "guis/%s.gui", MenuName );
		hGui = theFileTree.OpenFile( Path );
		sprintf( Path, "anims/%s.gaf", MenuName );
		hGaf = theFileTree.OpenFile( Path );

		// If anything went worng while getting the files, break out of the loop
		if( hGui==NULL || hGaf==NULL ||
			0>=theFileTree.DumpFileToBuffer((app_FileTree::HFILEENTRY)hGaf,&m_ControlBar.m_pFileBuffer) )
			break;

		// Configure the key name
		sprintf( Key, "%s.", MenuName );

		// Get a new menu assocciated to the key and set it up
		pMenu = &m_ControlBar.m_UnitMenus[ MenuName ];
		pMenu->Create( std_Point(), std_Size(), &m_ControlBar );
		pMenu->LoadGui( MenuName );

		// Set the button states for this menu page and delete the file buffer
		m_ControlBar.SetBuildMenuCommands( pMenu, UnitType );
		SAFE_DELETE_ARRAY( m_ControlBar.m_pFileBuffer );

		// Are there any download entries for this page
		if( pPages && count<PageCount && !(*pPages)[count].empty() )
		{
			// Save a pointer to this page
			pPage = &(*pPages)[count];
			DownloadMenuPage_t::iterator		it = pPage->begin();
			DownloadMenuPage_t::const_iterator	end= pPage->end();
			for( ; it!=end; ++it)
			{
				strcpy( CommonInfo.Name, ((*it).second).c_str() );
				GET_ENTRY_POS( (*it).first );
				sprintf( Path, "anims/%s_gadget.gui", ((*it).second).c_str() );
				theFileTree.DumpFileToBuffer( Path, &m_ControlBar.m_pFileBuffer );
				pMenu->CreateButton( CommonInfo, ButtonInfo );
				SAFE_DELETE_ARRAY( m_ControlBar.m_pFileBuffer );
			}
		} // end if( downloads )
	} // end for( gui pages )

	for( ; count<PageCount; ++count) if( !(*pPages)[count].empty() )
	{
		// Save a pointer to this page
		pPage = &(*pPages)[count];
		sprintf( MenuName, "%s%d", UnitType.UnitName, UnitType.BuildMenuCount + 1 );
		sprintf( Key, "%s.", MenuName );
		pMenu = &m_ControlBar.m_UnitMenus[ MenuName ];
		pMenu->Create( std_Point(), std_Size(), &m_ControlBar );
		pMenu->LoadGui( "Generic Menu" );

		DownloadMenuPage_t::iterator		it;
		for( int i=0; i<6; ++i) if( (it=pPage->find(i))!=pPage->end() )
		{
			strcpy( CommonInfo.Name, ((*it).second).c_str() );
			GET_ENTRY_POS( i );
			sprintf( Path, "anims/%s_gadget.gui", ((*it).second).c_str() );
			theFileTree.DumpFileToBuffer( Path, &m_ControlBar.m_pFileBuffer );
			pMenu->CreateButton( CommonInfo, ButtonInfo );
			SAFE_DELETE_ARRAY( m_ControlBar.m_pFileBuffer );
		}
		else
		{
			strcpy( CommonInfo.Name, "IGPATCH" );
			GET_ENTRY_POS( i );
			pMenu->CreateButton( CommonInfo, ButtonInfo );
		}

		m_ControlBar.SetBuildMenuCommands( pMenu, UnitType );
		++UnitType.BuildMenuCount;
	}

	return TRUE;
}
// End game_UserInterface::CreateBuildMenu()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_UserInterface_Load.cpp //
//////////////////////////////////////