// fe_GuiManager.cpp //                            \author Logan Jones
//////////////////////                                \date 10/2/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_GuiManager.h"
#include "fe_MainMenu.h"
#include "fe_Skirmish.h"
#include "fe_SelectGame.h"
#include "fe_MultiplayerSetup.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_GuiManager::fe_GuiManager():wnd_Manager()
{}
fe_GuiManager::~fe_GuiManager()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::Create() //                      \author Logan Jones
////////////////////////////                           \date 10/2/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strStartMenu - 
//
// Return: BOOL - 
//
BOOL fe_GuiManager::Create( LPCTSTR strStartMenu )
{
	DWORD TotalTime = std_Time();

	// Call the default
	if( bFAILED(wnd_Manager::Create()) )
		return FALSE;

	// Initialize the root window
	m_pRootWindow->SetWndManager( this );
	m_pRootWindow->SetWndSize( std_Size_t(640,480) );

	// Load all the images needed for the front end
	if( bFAILED(guiResources.Create()) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to load gui resources." );
		return FALSE;
	}

	if( strStartMenu==NULL )	SwitchRoots( new fe_MainMenu, NULL );
	else STRING_SWITCH( strStartMenu )
		CASE("Main")			SwitchRoots( new fe_MainMenu, NULL );
		CASE("Skirmish")		SwitchRoots( new fe_Skirmish, NULL );
		CASE("SelectGame")		SwitchRoots( new fe_SelectGame, NULL );
		CASE("BattleRoom")		((fe_MultiplayerSetup*)SwitchRoots( new fe_MultiplayerSetup, NULL ))->Init("Test Server","Game Master","dog");
		STRING_SWITCH_DEFAULT	SwitchRoots( new fe_MainMenu, NULL );
	END_STRING_SWITCH

	// Start playing the front end ambient sound
	sound.PlayAmbientSound( guiResources.Sounds.BGM, &m_AmbientSond );

	// Make this class the current input handler
	theApp.Console.SetInputHandler( this );

	// Set the normal cursor
	gfx->SetCurrentCursor( guiResources.Cursors.Normal );

	// Move the cursor to the middle
	SetCursorPosition( std_Point( m_pRootWindow->GetWndSize().width / 2, m_pRootWindow->GetWndSize().height / 2 ) );
	//m_Cursor.Set( m_pRootWindow->GetWndSize().width / 2, m_pRootWindow->GetWndSize().height / 2 );
	SetCursorSpeed( 1.75f );

	theApp.Console.Comment( CT_EXTRA_LOAD, "Front-End load time: %.2f seconds", (std_Time() - TotalTime) * (1/1000.0f) );
	return TRUE;
}
// End fe_GuiManager::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::Destroy() //                     \author Logan Jones
/////////////////////////////                          \date 10/2/2001
//               
//====================================================================
//
void fe_GuiManager::Destroy()
{
	if( m_pRootWindow==NULL ) return;

	theApp.Console.SetInputHandler( NULL );

	// Call the default
	wnd_Manager::Destroy();

	SAFE_DELETE( m_OldRoot );
	SAFE_DELETE( m_CurrentRoot );

	// Stop the ambient sound
	if( m_AmbientSond )
		sound.StopSound( &m_AmbientSond );
}
// End fe_GuiManager::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::OnFrame() //                     \author Logan Jones
/////////////////////////////                         \date 10/30/2001
//               
//====================================================================
//
void fe_GuiManager::OnFrame()
{
	if( m_pRootWindow==NULL ) return ;

	if( (std_Time()-m_UpdateTime)>=75 )
	{
		Update();
		m_UpdateTime = std_Time();
	}

	if( m_pRootWindow )
		Render();
}
// End fe_GuiManager::OnFrame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::Update() //                      \author Logan Jones
////////////////////////////                           \date 10/2/2001
//               
//====================================================================
//
void fe_GuiManager::Update()
{
	wnd_Manager::Update();
	gfx->UpdateCursor();
}
// End fe_GuiManager::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::Render() //                      \author Logan Jones
////////////////////////////                          \date 10/30/2001
//               
//====================================================================
//
void fe_GuiManager::Render()
{
	PALETTEENTRY pe;
	pe.peRed = 0xFF; pe.peGreen = 0x00; pe.peBlue = 0xFF; pe.peFlags = 0xFF;

	gfx->Set2DProjection( gfx_Resolution_t(640,480), -1024, 256 );
	gfx->BeginScene();

	m_pRootWindow->Render();

	gfx->RenderStringAt( std_Point_t(16,16), m_strFPS );

	gfx->RenderCursor( m_PreciseCursor );

	gfx->EndScene();

	if( (std_Time()-m_FrameTime)>500.0f )
	{
		m_FrameCount++;
		sprintf( m_strFPS, "FPS: %.2f",
			     (1000.0f/(std_Time() - m_FrameTime))*m_FrameCount );
		m_FrameCount = 0;
		m_FrameTime = std_Time();
	}
	else m_FrameCount++;
}
// End fe_GuiManager::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::HandleInput() //                 \author Logan Jones
/////////////////////////////////                     \date 10/15/2001
//               
//====================================================================
// Parameters:
//  in_Devices inDevice - 
//  DWORD dwID          - 
//  long lData          - 
//
// Return: BOOL - 
//
//BOOL fe_GuiManager::HandleInput( in_Devices inDevice, DWORD dwID, long lData )
//{
//	return wnd_Manager::HandleInput( inDevice, dwID, lData );
//}
// End fe_GuiManager::HandleInput()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::QuickStart() //                  \author Logan Jones
////////////////////////////////                       \date 7/31/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strGameType - 
//
// Return: BOOL - 
//
BOOL fe_GuiManager::QuickStart( LPCTSTR strGameType )
{
	STRING_SWITCH( strGameType )
		CASE( "Skirmish" ) return fe_Skirmish::QuickStart();
	END_STRING_SWITCH

	return FALSE;
}
// End fe_GuiManager::QuickStart()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_GuiManager::SwitchRoots() //                 \author Logan Jones
/////////////////////////////////                      \date 7/31/2002
//               
//====================================================================
// Parameters:
//  fe_Wnd* pNew - 
//  fe_Wnd* pOld - 
//
// Return: fe_Wnd* - 
//
fe_Wnd* fe_GuiManager::SwitchRoots( fe_Wnd* pNew, fe_Wnd* pOld )
{
	assert( pOld==m_CurrentRoot );
	SAFE_DELETE( m_OldRoot );

	if( bFAILED(pNew->Create( std_Point_t(), m_pRootWindow->GetWndSize(), m_pRootWindow )) )
	{
		delete pNew;
		return NULL;
	}
	else
	{
		m_CurrentRoot = pNew;

		m_OldRoot = pOld;
		SAFE_DESTROY( pOld );

		return pNew;
	}
}
// End fe_GuiManager::SwitchRoots()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_GuiManager.cpp //
////////////////////////////
