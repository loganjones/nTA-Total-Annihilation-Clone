// igui_GameWindow.cpp //                          \author Logan Jones
////////////////////////                               \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_GameWindow.h"
#include "gfx.h"
#include "game.h"
#include "snd.h"


#define IsSelectable( pOBJ )		( (pOBJ)->GetSceneryType()==OBJ_Unit )
#define IsUnit( pOBJ )				( (pOBJ)->GetSceneryType()==OBJ_Unit )
#define IsFriendly( pOBJ )			( (static_cast<unit_Object*>(pOBJ))->m_pPlayer==theInterface->m_Player )
#define SmallerRect( __RECT__, N )		std_RectF( __RECT__.left + N, __RECT__.top + N, __RECT__.right - N, __RECT__.bottom - N )
#define MakeRect( __RECT__, ptA, ptB ) {\
		if( ptA.x<ptB.x ) __RECT__.left = ptA.x, __RECT__.right = ptB.x; \
		else m_SelectionRect.left = ptB.x, __RECT__.right = ptA.x; \
		if( ptA.y<ptB.y ) __RECT__.top = ptA.y, __RECT__.bottom = ptB.y; \
		else __RECT__.top = ptB.y, __RECT__.bottom = ptA.y; }


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_GameWindow::igui_GameWindow():wnd_Window(),
//	m_TerrainRes( 640, 480 ),
//	m_NewTerrainRes( m_TerrainRes ),
	m_InterfaceMode( MODE_Select ),
	m_BuildRect_CanBuildColor( 0x00FF00FF ),
	m_BuildRect_CanNotBuildColor( 0xFF0000FF ),
	m_MButton1HoldInterval( 250 ),
	m_SelectionRect_OuterColor( 0xFFFFFFFF ),
	m_SelectionRect_InnerColor( 0x000000FF ),
	m_CurrentSelection( NULL ),
	m_LastSelectionInterval( 100 )
{}
igui_GameWindow::~igui_GameWindow()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::OnCreate() //                  \author Logan Jones
////////////////////////////////                        \date 4/2/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL igui_GameWindow::OnCreate()
{
	std_Size ViewportSize = m_Size;
	ViewportSize.ChangeSpace( theGame.Interface.GuiResolution(), m_TerrainRes );

//	theGame.Terrain.GetGfx()->InitializeViewport( std_Point(), ViewportSize );
//	m_NewTerrainRes = m_TerrainRes;
	theGame.Terrain.InitTerrain( std_Point(), ViewportSize );

	m_MButton1Down = FALSE;

	return TRUE;
}
// End igui_GameWindow::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::Update() //                    \author Logan Jones
//////////////////////////////                          \date 4/2/2002
//               
//====================================================================
//
void igui_GameWindow::Update()
{
	std_Size			ViewportSize = m_Size;
	std_Point			cursor( m_pWndManager->GetCursorPosition() );
	const bool			CursorInWindow( std_Rect(m_ScreenPosition,m_Size).PointInRect(cursor) );
	scene_Object*		pObjectUnderCursor;

	// Convert to client and then terrain space
	ScreenToClient( &cursor );
	cursor.ChangeSpace( theGame.Interface.GuiResolution(), m_TerrainRes );
	cursor += theGame.Terrain.GetPosition();

	// Get the object under the cursor (if any)
	pObjectUnderCursor = theGame.ScenerySystem.TestPoint( cursor );

	switch( m_InterfaceMode )
	{
		case MODE_Select:
			if( pObjectUnderCursor && IsUnit(pObjectUnderCursor) )
				m_CurrentCursor = guiResources.Cursors.Select;
			else m_CurrentCursor = guiResources.Cursors.Normal;
			break;

		case MODE_Default:
		case MODE_Move:
			if( pObjectUnderCursor )
				if( IsUnit(pObjectUnderCursor) )
					//if( IsFriendly(pObjectUnderCursor) ) m_CurrentCursor = guiResources.Cursors.Select;
					//else m_CurrentCursor = guiResources.Cursors.Attack;
					m_CurrentCursor = guiResources.Cursors.Select;
				//else if( pObjectUnderCursor->IsFlat() ) m_CurrentCursor = guiResources.Cursors.Move;
				else m_CurrentCursor = guiResources.Cursors.Normal;
			else m_CurrentCursor = guiResources.Cursors.Move;
			break;

		case MODE_Build:
			m_CurrentCursor = guiResources.Cursors.Green;
			if( CursorInWindow )
			{
				cursor = theGame.Terrain.GetMapPoint(cursor);
				CenterBuildRect( cursor );
				m_BuildHeight = theGame.Terrain.GetHeight( cursor/16 );
				m_CanBuild = theGame.Units.CheckUnitRect( m_BuildType, m_BuildRect.Position() );
				m_BuildInfoHasBeenSet = TRUE;
			}
			break;

		case MODE_Attack:	m_CurrentCursor = guiResources.Cursors.Attack;		break;
		case MODE_Defend:	m_CurrentCursor = guiResources.Cursors.Defend;		break;
		case MODE_Patrol:	m_CurrentCursor = guiResources.Cursors.Patrol;		break;
		case MODE_Blast:	m_CurrentCursor = guiResources.Cursors.Attack;		break;
		case MODE_Capture:	m_CurrentCursor = guiResources.Cursors.Capture;		break;
		case MODE_Reclaim:	m_CurrentCursor = guiResources.Cursors.Reclamate;	break;
		case MODE_Repair:	m_CurrentCursor = guiResources.Cursors.Repair;		break;
		case MODE_Load:		m_CurrentCursor = guiResources.Cursors.Load;		break;
		case MODE_Unload:	m_CurrentCursor = guiResources.Cursors.Unload;		break;

	} // end switch( m_InterfaceMode )

	if( CursorInWindow || m_pWndManager->GetCapture()==this )
	{
		gfx->SetCurrentCursor( m_CurrentCursor );
		theInterface->InfoBar().DisplayObjectInfo( pObjectUnderCursor );
	}

	if( m_MButton1Down && std_Time()-m_MButton1DownTime>m_MButton1HoldInterval )
	{
		MakeRect( m_SelectionRect, m_MButton1DownAt, cursor );
		m_SelectionRectSet = TRUE;
	}
}
// End igui_GameWindow::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::Render() //                    \author Logan Jones
//////////////////////////////                          \date 4/2/2002
//               
//====================================================================
//
void igui_GameWindow::Render()
{
	gfx->PushMatrix();
	gfx->Translate( m_ScreenPosition );

	gfx->Translate( std_Vector3(0,m_Size.height,0) );
	gfx->RenderString( theGame.m_FramesPerSecond, std_Vector3(4,-4), 0xFFFFFFFF, guiResources.Fonts.Standard );

	gfx->PopMatrix();
}
// End igui_GameWindow::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::RenderGame() //                \author Logan Jones
//////////////////////////////////                      \date 4/1/2002
//               
//====================================================================
//
void igui_GameWindow::RenderGame()
{
	std_Point		ScreenPos = m_ScreenPosition;
	const std_Point	ViewPos = theGame.Terrain.GetPosition();

	 m_TerrainRes = theGame.Interface.GuiResolution();
	 m_TerrainRes.ChangeSpace( m_Size, theGame.Terrain.GetViewSize() );

	gfx->PushMatrix();
	gfx->Set2DProjection( m_TerrainRes, -1024, 256 );
	ScreenPos.ChangeSpace( theInterface->GuiResolution(), m_TerrainRes );
	gfx->Translate( ScreenPos );

	theGame.Render();

	if( m_InterfaceMode==MODE_Build && m_BuildInfoHasBeenSet )
		gfx->DrawWireRect(
			m_BuildRect - std_Vector2(0,m_BuildHeight/2) - ViewPos,
			m_CanBuild ? m_BuildRect_CanBuildColor : m_BuildRect_CanNotBuildColor ),
		gfx->DrawWireRect(
			SmallerRect(m_BuildRect,1) - std_Vector2(0,m_BuildHeight/2) - ViewPos,
			m_CanBuild ? m_BuildRect_CanBuildColor : m_BuildRect_CanNotBuildColor );

	if( m_MButton1Down && m_SelectionRectSet )
		gfx->DrawWireRect( m_SelectionRect - ViewPos, m_SelectionRect_OuterColor ),
		gfx->DrawWireRect( SmallerRect(m_SelectionRect,1) - ViewPos, m_SelectionRect_InnerColor );

	gfx->PopMatrix();
}
// End igui_GameWindow::RenderGame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::DoMode() //                    \author Logan Jones
//////////////////////////////                         \date 4/29/2002
//               
//====================================================================
// Parameters:
//  InterafceModes_t Mode - 
//
void igui_GameWindow::DoMode( InterafceModes_t Mode )
{
	assertEx( Mode!=MODE_Select, "Cannot set selection mode extrnaly." );
	assertEx( Mode!=MODE_Default, "Cannot set default mode extrnaly." );
	assertEx( Mode!=MODE_Build, "Build mode must be set by calling DoBuildMode()." );
	m_InterfaceMode = Mode;
}
// End igui_GameWindow::DoMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::DoBuildMode() //               \author Logan Jones
///////////////////////////////////                    \date 4/20/2002
//               
//====================================================================
// Parameters:
//  const DWORD& dwBuildWhat - 
//
void igui_GameWindow::DoBuildMode( const DWORD& dwBuildWhat )
{
	if( m_InterfaceMode!=MODE_Select )
	{
		m_InterfaceMode = MODE_Build;
		m_BuildType = theGame.Units.GetUnitType(dwBuildWhat);
		m_BuildRect.SetSize(std_SizeF(m_BuildType->FootPrint*16));
		m_BuildInfoHasBeenSet = FALSE;
	}
}
// End igui_GameWindow::DoBuildMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::DoOrder() //                   \author Logan Jones
///////////////////////////////                        \date 4/29/2002
//               
//====================================================================
// Parameters:
//  InterafceOrder_t Order - 
//  DWORD dwParam          - 
//
void igui_GameWindow::DoOrder( InterafceOrder_t Order, DWORD dwParam )
{
	switch( Order )
	{
		case ORDER_Stop:
			m_CurrentSelection->RequestStop();
			break;

		case ORDER_OnOff:
			m_CurrentSelection->RequestActivate( dwParam!=0 );
			break;
            
        default:
            break;
	}
	theGame.Interface.ControlBar().ResetOrderSelection();
}
// End igui_GameWindow::DoOrder()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::OnMouseButton() //             \author Logan Jones
/////////////////////////////////////                   \date 4/2/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void igui_GameWindow::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// Point on the map where the button was pressed
	const std_Point	ptMap = theGame.Terrain.GetPoint( ptCursor, theGame.Interface.GuiResolution(), m_TerrainRes );

	switch( dwButton )
	{
		case 1: if( bDown )
		{
			m_MButton1Down = TRUE;
			m_MButton1DownAt = ptMap;
			m_MButton1DownTime = std_Time();
			m_SelectionRectSet = FALSE;
			SetCapture();
		}
		else // end button up
		{
			m_MButton1Down = FALSE;
			ReleaseCapture();

			if( std_Time()-m_MButton1DownTime>m_MButton1HoldInterval )
			{
				MakeRect( m_SelectionRect, m_MButton1DownAt, ptMap );
				DoRectOrder();
			}
			else DoPointOrder( ptMap );
			
		} // end else button up
		break;
		// end case Mouse Button 1

		case 2: if( bDown )
		{
		}
		else // end button up
		{
			if( m_InterfaceMode==MODE_Select ) break;
			else if( m_InterfaceMode==MODE_Default )
			{
				m_CurrentSelection->OnSelect( FALSE, TRUE );
				m_CurrentSelection = NULL;
				m_InterfaceMode = MODE_Select;
				theGame.Interface.ControlBar().SetSelection( NULL );
			}
			else
			{
				theGame.Interface.ControlBar().ResetOrderSelection();
				m_InterfaceMode = MODE_Default;
			}
		} // end else button up
		break;
		// end case Mouse Button 2

	} // end switch( dwButton )
}
// End igui_GameWindow::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::OnMouseWheel() //              \author Logan Jones
////////////////////////////////////                    \date 4/2/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  long lData            - 
//  DWORD dwFlags         - 
//
void igui_GameWindow::OnMouseWheel( const std_Point_t& ptCursor, long lData, DWORD dwFlags )
{
	if( lData<0 )	theGame.Terrain.Zoom( 16, 12 );
	else	theGame.Terrain.Zoom( -16, -12 );
}
// End igui_GameWindow::OnMouseWheel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::OnKeyboardButton() //          \author Logan Jones
////////////////////////////////////////                \date 4/2/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void igui_GameWindow::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	std_Size ViewportSize = m_Size;

	// Check what key
	switch( dwButton )
	{
		case IN_KEY_F9:
			ViewportSize.ChangeSpace( theGame.Interface.GuiResolution(), std_Size(640,480) );
			theGame.Terrain.SetZoom( ViewportSize );
			break;
		case IN_KEY_F10:
			ViewportSize.ChangeSpace( theGame.Interface.GuiResolution(), std_Size(800,600) );
			theGame.Terrain.SetZoom( ViewportSize );
			break;
		case IN_KEY_F11:
			ViewportSize.ChangeSpace( theGame.Interface.GuiResolution(), std_Size(1280,960) );
			theGame.Terrain.SetZoom( ViewportSize );
			break;
		case IN_KEY_F12:
			ViewportSize.ChangeSpace( theGame.Interface.GuiResolution(), std_Size(1600,1200) );
			theGame.Terrain.SetZoom( ViewportSize );
			break;
		case IN_KEY_F8:
			gfx->SaveScreenShot( "TestShot" );
			break;
		case IN_KEY_KP_PLUS:
			if( !bDown ) theGame.Terrain.Zoom( -16, -12 );
			break;
		case IN_KEY_KP_MINUS:
			if( !bDown ) theGame.Terrain.Zoom( 16, 12 );
			break;
	}
}
// End igui_GameWindow::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::CenterBuildRect() //           \author Logan Jones
///////////////////////////////////////                \date 4/20/2002
//               
//====================================================================
// Parameters:
//  const std_Point& ptCursorOnGrid - 
//
void igui_GameWindow::CenterBuildRect( const std_Point& ptCursorOnGrid )
{
	std_Rect rect( 0, 0, m_BuildType->FootPrint );
	rect.SetCenter( ptCursorOnGrid / 16 );
	m_BuildRect.SetPosition( rect.Position() * 16 );
//	m_BuildRect.SetPosition( ptCursorOnGrid );
}
// End igui_GameWindow::CenterBuildRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::DoRectOrder() //               \author Logan Jones
///////////////////////////////////                    \date 5/20/2002
//               
//====================================================================
//
void igui_GameWindow::DoRectOrder()
{
	switch( m_InterfaceMode )
	{
		case MODE_Select:
		case MODE_Default:
			MultiSelection();
			break;
            
        default:
            break;

	} // end switch( m_InterfaceMode )
}
// End igui_GameWindow::DoRectOrder()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::DoPointOrder() //              \author Logan Jones
////////////////////////////////////                   \date 5/20/2002
//               
//====================================================================
// Parameters:
//  const std_Point& ptCursor - Cursor position over the map
//
void igui_GameWindow::DoPointOrder( const std_Point& ptCursor )
{
	scene_Object*	ObjectUnderCursor = theGame.ScenerySystem.TestPoint(ptCursor);
	switch( m_InterfaceMode )
	{
		case MODE_Select:
			if( ObjectUnderCursor && IsSelectable(ObjectUnderCursor) )
				SingleSelection( static_cast<unit_Object*>(ObjectUnderCursor) );
			break;

		case MODE_Default:
		case MODE_Move:
			if( ObjectUnderCursor && IsSelectable(ObjectUnderCursor) )
				SingleSelection( static_cast<unit_Object*>(ObjectUnderCursor) );
			else if( ObjectUnderCursor==NULL )//|| pObject->IsFlat() )
				m_CurrentSelection->RequestMoveTo( theGame.Terrain.GetMapPoint(ptCursor), theInterface->ShiftKeyDown() ),
				theGame.Interface.ControlBar().ResetOrderSelection(),
				m_InterfaceMode = MODE_Default;
			break;

		case MODE_Build:
			if( m_BuildInfoHasBeenSet && m_CanBuild ) {
				m_CurrentSelection->RequestBuild( m_BuildType, m_BuildRect.Position(), theInterface->ShiftKeyDown() );
				if( !theInterface->ShiftKeyDown() )
					theGame.Interface.ControlBar().ResetOrderSelection(),
					m_InterfaceMode = MODE_Default;
			}
			break;

		case MODE_Attack:
			m_CurrentSelection->RequestAttack( ptCursor, theInterface->ShiftKeyDown() );
			m_InterfaceMode = MODE_Default;
			theGame.Interface.ControlBar().ResetOrderSelection();
			break;

		case MODE_Defend:
		case MODE_Patrol:
		case MODE_Blast:
		case MODE_Capture:
		case MODE_Reclaim:
		case MODE_Repair:
		case MODE_Load:
		case MODE_Unload:
			m_InterfaceMode = MODE_Default;
			theGame.Interface.ControlBar().ResetOrderSelection();
			break;

	} // end switch( m_InterfaceMode )
}
// End igui_GameWindow::DoPointOrder()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::SingleSelection() //           \author Logan Jones
///////////////////////////////////////                 \date 5/3/2002
//               
//====================================================================
// Parameters:
//  class unit_Interface* pUnit - 
//
void igui_GameWindow::SingleSelection( class unit_Interface* pUnit )
{
	// Check if there is already a selection
	if( m_CurrentSelection )
    {
		if( m_CurrentSelection==pUnit )
		{
			// TODO: Implement some kind of special handling.
			return;
		}
        else // Unselect the old
        {
			m_CurrentSelection->OnSelect( FALSE, TRUE );
        }
    }

	// Select the new
	m_CurrentSelection = pUnit;
	m_CurrentSelection->OnSelect( TRUE, TRUE );

	// Reset the interface mode and let the order panle know what happened
	m_InterfaceMode = MODE_Default;
	theGame.Interface.ControlBar().SetSelection( static_cast<unit_Object*>(m_CurrentSelection) );
}
// End igui_GameWindow::SingleSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_GameWindow::MultiSelection() //            \author Logan Jones
//////////////////////////////////////                 \date 5/20/2002
//               
//====================================================================
//
void igui_GameWindow::MultiSelection()
{
}
// End igui_GameWindow::MultiSelection()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_GameWindow.cpp //
//////////////////////////////
