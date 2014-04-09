// game_UserInterface.cpp //                       \author Logan Jones
///////////////////////////                            \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "game_UserInterface.h"
#include "gfx.h"
#include "game.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "game_UserInterface.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
game_UserInterface::game_UserInterface():wnd_Manager(),
	m_GuiResolution( 1024, 768 ),
	m_ShiftKeyDown( 0 )
{
}
game_UserInterface::~game_UserInterface()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_UserInterface::BuildButtonPressed() //     \author Logan Jones
/////////////////////////////////////////////        \date 02-09-2003
//
//===================================================================
// Parameters:
//  LPCTSTR strTypeName - Name of buildee
//  bool bWeapon        - Is the buildee a weapon
//  bool bAdd           - Is the buile to be added or removed from the list
//
void game_UserInterface::BuildButtonPressed( LPCTSTR strTypeName, bool bWeapon, bool bAdd )
{
	const int	Amount = ShiftKeyDown() ? 5:1;

	// Check if this is an accumulating build order (factory order or weapon stockpile)
	if( bWeapon || m_ControlBar.m_CurrentSelection->IsStructure() ) {

		// Create a new build oreder list if neccessary
		if( !m_ControlBar.m_CurrentSelection->BuildOrders() )
			m_BuildOrders.push_front( igui_BuildOrders(m_ControlBar.m_CurrentSelection,this) ),
			m_BuildOrders.front().m_ID = m_BuildOrders.begin(),
			m_ControlBar.m_CurrentSelection->BuildOrders() = &m_BuildOrders.front();

		// Add/Sub the order as desired
		if( bAdd ) m_ControlBar.m_CurrentSelection->BuildOrders()->Add( theGame.Units.GetUnitType(strTypeName), Amount );
		else m_ControlBar.m_CurrentSelection->BuildOrders()->Sub( theGame.Units.GetUnitType(strTypeName), Amount );


	// non-accumulating build order
	// let the user handle it just like any other normal unit order
	} else
		m_ControlBar.ResetPressed(),
		m_GameWindow.DoBuildMode( std_NameHash(strTypeName) );
}
// End game_UserInterface::BuildButtonPressed()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_UserInterface::KillBuildOrders() //        \author Logan Jones
//////////////////////////////////////////           \date 03-01-2003
//
//===================================================================
// Parameters:
//  igui_BuildOrders* pOrders - 
//
void game_UserInterface::KillBuildOrders( igui_BuildOrders* pOrders )
{
	pOrders->m_Unit->BuildOrders() = NULL;
	m_BuildOrders.erase( pOrders->m_ID );
}
// End game_UserInterface::KillBuildOrders()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::Update() //                 \author Logan Jones
/////////////////////////////////                       \date 4/1/2002
//               
//====================================================================
//
void game_UserInterface::Update()
{
	std_Point Scroll;

	wnd_Manager::Update();
	gfx->UpdateCursor();

	if( m_Cursor.x==0 )
		Scroll.x = -theGame.Settings.scrollspeed;
	else if( m_Cursor.x==m_GuiResolution.width-1 )
		Scroll.x = theGame.Settings.scrollspeed;
		
	if( m_Cursor.y==0 )
		Scroll.y = -theGame.Settings.scrollspeed;
	else if( m_Cursor.y==m_GuiResolution.height-1 )
		Scroll.y = theGame.Settings.scrollspeed;

	theGame.Terrain.Scroll( Scroll );
}
// End game_UserInterface::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::Render() //                 \author Logan Jones
/////////////////////////////////                       \date 4/1/2002
// Render the game and interface stuff
//====================================================================
//
void game_UserInterface::Render()
{
	std_Vector2 ScreenCursor = m_PreciseCursor;

	gfx->BeginGameScene();

		// Render the game
		m_GameWindow.RenderGame();

		// Render the overlay stuff (windows, ...etc)
		gfx->BeginInterfaceRendering();
			gfx->Set2DProjection( m_GuiResolution, -1024, 256 );
			m_pRootWindow->Render();

			gfx->Set2DProjection( gfx_Resolution_t(640,480), -1024, 256 );
			ScreenCursor.ChangeSpace( m_GuiResolution, gfx_Resolution_t(640,480) );
			gfx->RenderCursor( ScreenCursor );

	gfx->EndGameScene();
}
// End game_UserInterface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::HandleInput() //            \author Logan Jones
//////////////////////////////////////                  \date 4/1/2002
//               
//====================================================================
// Parameters:
//  in_Devices inDevice - 
//  DWORD dwID          - 
//  long lData          - 
//
// Return: BOOL - 
//
BOOL game_UserInterface::HandleInput( in_Devices inDevice, DWORD dwID, long lData )
{
	// Intercept keyboard input
	if( inDevice==in_Keyboard )
		switch( dwID ) {
			case IN_KEY_TILDE:
				if( lData ) m_Console.Toggle();
				return TRUE;

			case IN_KEY_LSHIFT:
			case IN_KEY_RSHIFT:
				if( lData ) ++m_ShiftKeyDown;
				else --m_ShiftKeyDown;
				return TRUE;

			case IN_KEY_LCTRL:
			case IN_KEY_RCTRL:
				if( lData ) ++m_CtrlKeyDown;
				else --m_CtrlKeyDown;
				return TRUE;
		//	default: // Send the rest of the keyboard input to the order panel
		//		m_ControlBar.OnKeyboardButton( dwID, lData & 0x80, 0 );
		//		return TRUE;
		}

	// Let the base class hanlde anything that slips through
	return wnd_Manager::HandleInput( inDevice, dwID, lData );
}
// End game_UserInterface::HandleInput()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_UserInterface.cpp //
/////////////////////////////////
