// igui_ControlBar.cpp //                          \author Logan Jones
////////////////////////                               \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_ControlBar.h"
#include "game.h"
#include "gfx.h"
#include "snd.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "igui_ControlBar.inl"
#endif // defined( _DEBUG )

#define HasAbility( ABILITY )		Abilities & unit_Type::ABILITY
#define ResetCurrentMenu()	if( m_CurrentMenu ) m_CurrentMenu->Detach(); m_CurrentMenu = NULL
#define HandleUnitOrderMode( ORDER, SOUND )	CASE( #ORDER ) \
	sound.PlaySound( SOUND ); \
	if( m_Pressed==pSender ) { m_Pressed = NULL; return 1; } \
	ResetPressed(); \
	m_Pressed = (igui_OrderButton*)pSender; \
	theGame.Interface.GameWindow().DoMode( igui_GameWindow::MODE_##ORDER ); \
	return 1
#define HandleUnitOrder( ORDER, SOUND )	CASE( #ORDER ) \
	sound.PlaySound( SOUND ); \
	ResetPressed(); \
	theGame.Interface.GameWindow().DoOrder( igui_GameWindow::ORDER_##ORDER, (static_cast<gadget_Button*>(pSender))->GetStage() ); \
	return 1

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_ControlBar::igui_ControlBar():wnd_Window(),
	m_CurrentSelection( NULL ),
	m_CurrentMenu( NULL ),
	m_CurrentPage( 0 ),
	m_Pressed( NULL )
{}
igui_ControlBar::~igui_ControlBar()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnCreate() //                  \author Logan Jones
////////////////////////////////                       \date 4/23/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL igui_ControlBar::OnCreate()
{
	// Initialize some stuff
	m_CurrentMenu = NULL;
	m_CurrentPage = 0;
	m_Pressed = NULL;

	// Create the orders menu
	m_OrdersMenu.Create( std_Point(), std_Size(), this );
	m_OrdersMenu.LoadGui( "ArmGen" );

	// Set the initial selection to nothing
	SetSelection( NULL );

	return TRUE;
}
// End igui_ControlBar::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::Destroy() //                   \author Logan Jones
///////////////////////////////                        \date 4/29/2002
//               
//====================================================================
//
void igui_ControlBar::Destroy()
{
	// Make sure a build menu is not in the child list before calling the default
	ResetCurrentMenu();
	wnd_Window::Destroy();
}
// End igui_ControlBar::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnDestroy() //                 \author Logan Jones
/////////////////////////////////                      \date 4/23/2002
//               
//====================================================================
//
void igui_ControlBar::OnDestroy()
{
	{ // Destroy menus
		MenuMap_t::iterator		it = m_UnitMenus.begin();
		MenuMap_t::iterator		end= m_UnitMenus.end();
		for( ; it!=end; ++it)
			((*it).second).Destroy();
		m_UnitMenus.clear();
		m_OrdersMenu.Destroy();
	} // end Destroy menus

	{ // Destroy surfaces
		SurfaceMap_t::iterator		it = m_Surfaces.begin();
		SurfaceMap_t::iterator		end= m_Surfaces.end();
		for( ; it!=end; ++it)
			for( int n=0; n<6; ++n) SAFE_DESTROY( ((*it).second).Surfaces[n] );
		m_Surfaces.clear();
	} // end Destroy surfaces
}
// End igui_ControlBar::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnRender() //                  \author Logan Jones
////////////////////////////////                        \date 4/1/2002
//               
//====================================================================
//
void igui_ControlBar::OnRender()
{
	gfx->PushMatrix();
	gfx->Translate( m_ScreenPosition );
	gfx->RenderSurface( theInterface->Surface(SidePanel) );
	if( m_Size.height>480 )
	{
		for( long y=m_Size.height-480; y>0; y-=480)
		{
			gfx->Translate( std_Vector3(0,480,0) );
			gfx->RenderSurface( theInterface->Surface(SidePanelLower) );
		}
	}
	gfx->PopMatrix();
}
// End igui_ControlBar::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::ResetOrderSelection() //       \author Logan Jones
///////////////////////////////////////////            \date 4/29/2002
//               
//====================================================================
//
void igui_ControlBar::ResetOrderSelection()
{
	ResetPressed();
}
// End igui_ControlBar::ResetOrderSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::SetSelection() //              \author Logan Jones
////////////////////////////////////                   \date 4/25/2002
//               
//====================================================================
// Parameters:
//  unit_Object* pUnit - 
//
// Return: BOOL - 
//
BOOL igui_ControlBar::SetSelection( unit_Object* pUnit )
{
	if( pUnit )
	{
		if( m_CurrentSelection!=pUnit )
		{
			m_CurrentSelection = pUnit;
			SetMenu( pUnit->m_LastMenu );
			SetOrderMenuCommands( &m_OrdersMenu, *m_CurrentSelection->m_pUnitType );
		}
	}
	else
	{
		ResetPressed();
		ResetCurrentMenu();
		m_CurrentSelection = NULL;
	}

	return TRUE;
}
// End igui_ControlBar::SetSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::SetMenu() //                   \author Logan Jones
///////////////////////////////                        \date 4/25/2002
//               
//====================================================================
// Parameters:
//  DWORD dwMenu - 
//
// Return: BOOL - 
//
BOOL igui_ControlBar::SetMenu( DWORD dwMenu )
{
	char					Gui[32];
	MenuMap_t::iterator		it;

	ResetPressed();
	ResetCurrentMenu();

	if( dwMenu==0 )
	{
		m_CurrentMenu = &m_OrdersMenu;
		m_CurrentMenu->Attach( this );
	}
	else
	{
		sprintf( Gui, "%s%d", m_CurrentSelection->m_pUnitType->UnitName, dwMenu );
		it = m_UnitMenus.find( Gui );
		if( it==m_UnitMenus.end() )
		{
			m_CurrentMenu = NULL;
			return FALSE;
		}
		else
		{
			m_CurrentMenu = &(*it).second;
			m_CurrentMenu->Attach( this );
			m_CurrentSelection->m_LastBuildPage = dwMenu;
		}
	}

	m_CurrentPage = dwMenu;
	m_CurrentSelection->m_LastMenu = dwMenu;
	return TRUE;
}
// End igui_ControlBar::SetMenu()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_ControlBar::ResetPressed() //              \author Logan Jones
////////////////////////////////////                 \date 02-09-2003
//
//===================================================================
// Parameters:
//   - 
//
void igui_ControlBar::ResetPressed()
{
	if( m_Pressed )
		m_Pressed->SetPressed( false );
	m_Pressed = NULL;
}
// End igui_ControlBar::ResetPressed()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::LoadPanel() //                 \author Logan Jones
/////////////////////////////////                      \date 4/23/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strPanel      - 
//  gfx_Surface** ppPanel -
//
// Return: BOOL - 
//
BOOL igui_ControlBar::LoadPanel( LPCTSTR strPanel, gfx_Surface** ppPanel )
{
	// Create the panel surface from the CommonGuiBuffer
	gfx->CreateSurfaceFromGAF( guiResources.CommonGuiBuffer, strPanel, 1, ppPanel );

	return TRUE;
}
// End igui_ControlBar::LoadPanel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::LoadOrderButton() //           \author Logan Jones
///////////////////////////////////////                \date 4/23/2002
//               
//====================================================================
// Parameters:
//  igui_OrderButton* pButton - 
//  ta_gui_Common_t& Common   - 
//  ta_gui_Button& Info       - 
//
// Return: BOOL - 
//
BOOL igui_ControlBar::LoadOrderButton( igui_OrderButton* pButton, ta_gui_Common_t& Common, ta_gui_Button& Info )
{
	SurfaceMap_t::iterator it = m_Surfaces.find( Common.Name );
	gfx_Surface**		pSurfaces;

	if( it==m_Surfaces.end() )
	{
		pSurfaces = m_Surfaces[ Common.Name ].Surfaces;
		ZeroMemory( pSurfaces, sizeof(gfx_Surface*) * 6 );
		gfx->CreateSurfacesFromGAF( guiResources.CommonGuiBuffer, Common.Name, 1, 6, pSurfaces );
	}
	else pSurfaces = ((*it).second).Surfaces;

	if( Common.Attribs & GADGET_IS_MULTI_COMMAND ) STRING_SWITCH( Common.Name + 3 )
		CASE_2( "OnOff", "Cloak" )
			pButton->SetButtonImages( pSurfaces, pSurfaces[3], pSurfaces[4] );
			pButton->SetStageCount( 2 );
		CASE_2( "MoveOrd", "FireOrd" )
			pButton->SetButtonImages( pSurfaces, pSurfaces[4], pSurfaces[5] );
			pButton->SetStageCount( 3 );
		END_STRING_SWITCH
	else if( Common.Attribs & GADGET_IS_SELECTION )
	{
		pButton->SetButtonImages( pSurfaces, pSurfaces[1], pSurfaces[2] );
		pButton->SetAutoPress( FALSE );
	}
	else if( stricmp(Common.Name,"IGPATCH")==0 )
	{
		pButton->SetButtonImages( pSurfaces, pSurfaces[0], pSurfaces[0] );
		return TRUE;
	}
	else // Assume this is a normal order button
		pButton->SetButtonImages( pSurfaces, pSurfaces[1], pSurfaces[2] );

	strcpy( Common.Name, Common.Name + 3 );
	return TRUE;
}
// End igui_ControlBar::LoadOrderButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::LoadBuildButton() //           \author Logan Jones
///////////////////////////////////////                \date 4/25/2002
//               
//====================================================================
// Parameters:
//  igui_BuildButton* pButton - 
//  ta_gui_Common_t& Common   - 
//  ta_gui_Button& Info       - 
//
// Return: BOOL - 
//
BOOL igui_ControlBar::LoadBuildButton( igui_BuildButton* pButton, ta_gui_Common_t& Common, ta_gui_Button& Info )
{
	char		Key[32];

	sprintf( Key, "%s.%s", m_KeyPrefix, Common.Name );
	SurfaceMap_t::iterator it = m_Surfaces.find( Key );
	gfx_Surface**		pSurfaces;

	if( it==m_Surfaces.end() )
	{
		pSurfaces = m_Surfaces[ Key ].Surfaces;
		ZeroMemory( pSurfaces, sizeof(gfx_Surface*) * 6 );
		if( bFAILED(gfx->CreateSurfacesFromGAF( m_pFileBuffer, Common.Name, 1, 3, pSurfaces )) )
		{
			pButton->SetButtonImages( NULL, NULL, NULL );
			return TRUE;
		}
	}
	else pSurfaces = ((*it).second).Surfaces;

	pButton->SetButtonImages( pSurfaces, pSurfaces[1], pSurfaces[2] );
	return TRUE;
}
// End igui_ControlBar::LoadBuildButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::CreateButton() //              \author Logan Jones
////////////////////////////////////                   \date 4/27/2002
//               
//====================================================================
// Parameters:
//  gadget_Button** ppButton - 
//  ta_gui_Common_t& Common  - 
//  ta_gui_Button& Info      - 
//  igui_Menu* pParent       - 
//
// Return: BOOL - 
//
BOOL igui_ControlBar::CreateButton( gadget_Button** ppButton, ta_gui_Common_t& Common, ta_gui_Button& Info, igui_Menu* pParent )
{
	SurfaceMap_t::iterator	it;
	gfx_Surface**			pSurfaces;

	if( Common.Attribs & GADGET_IS_BUILD_BUTTON )
	{
		if( stricmp(Common.Name,"IGPATCH")==0 )
		{
			*ppButton = new igui_OrderButton;
			return (*ppButton)->Create( &Common, &Info, pParent );
		}
		else
		{
			*ppButton = new igui_BuildButton;
			return (*ppButton)->Create( &Common, &Info, pParent );
		}
	}
	else if( Common.Attribs & GADGET_IS_ORDER_BUTTON )
	{
		*ppButton = new igui_OrderButton;
		return (*ppButton)->Create( &Common, &Info, pParent );
	}
	else
	{
		*ppButton = new gadget_Button;

		STRING_SWITCH( Common.Name + 3 )
			CASE_3( "Prev", "Next", "Stop" )
				it = m_Surfaces.find( Common.Name );
				if( it==m_Surfaces.end() )
				{
					pSurfaces = m_Surfaces[ Common.Name ].Surfaces;
					ZeroMemory( pSurfaces, sizeof(gfx_Surface*) * 6 );
					gfx->CreateSurfacesFromGAF( guiResources.CommonGuiBuffer, Common.Name, 1, 3, pSurfaces );
				}
				else pSurfaces = ((*it).second).Surfaces;
				strcpy( Common.Name, Common.Name + 3 );
				if( bFAILED((*ppButton)->Create(&Common,&Info,pParent)) )
					return FALSE;
				(*ppButton)->SetButtonImages( pSurfaces, pSurfaces[1], pSurfaces[2] );
				return TRUE;
			STRING_SWITCH_DEFAULT
				if( bFAILED((*ppButton)->Create(&Common,&Info,pParent)) )
					return FALSE;
				(*ppButton)->SetButtonImages( NULL, NULL, NULL );
				return TRUE;
		END_STRING_SWITCH
	}

	return FALSE;
}
// End igui_ControlBar::CreateButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::SetBuildMenuCommands() //      \author Logan Jones
////////////////////////////////////////////           \date 4/28/2002
//               
//====================================================================
// Parameters:
//  igui_Menu* pMenu          - 
//  const unit_Type& UnitType - 
//
void igui_ControlBar::SetBuildMenuCommands( igui_Menu* pMenu, const unit_Type& UnitType )
{
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("Build")))->SetPressed( TRUE );
	pMenu->GetGadget("Move")->SetActive( UnitType.HasAbility(CanMove) );
	pMenu->GetGadget("Stop")->SetActive( UnitType.HasAbility(CanStop) );
	pMenu->GetGadget("Patrol")->SetActive( UnitType.HasAbility(CanPatrol) );
	pMenu->GetGadget("Attack")->SetActive( UnitType.HasAbility(CanAttack) );
	pMenu->GetGadget("Defend")->SetActive( UnitType.HasAbility(CanDefend) );
	pMenu->GetGadget("Blast")->SetActive( UnitType.HasAbility(CanBlast) );
}
// End igui_ControlBar::SetBuildMenuCommands()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::SetOrderMenuCommands() //      \author Logan Jones
////////////////////////////////////////////           \date 4/28/2002
//               
//====================================================================
// Parameters:
//  igui_Menu* pMenu          - 
//  const unit_Type& UnitType - 
//
void igui_ControlBar::SetOrderMenuCommands( igui_Menu* pMenu, const unit_Type& UnitType )
{
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("Orders")))->SetPressed( TRUE );
	pMenu->GetGadget("Build")->SetActive( UnitType.BuildMenuCount>0 );

	pMenu->GetGadget("Move")->SetActive( UnitType.HasAbility(CanMove) );
	pMenu->GetGadget("Stop")->SetActive( UnitType.HasAbility(CanStop) );
	pMenu->GetGadget("Patrol")->SetActive( UnitType.HasAbility(CanPatrol) );
	pMenu->GetGadget("Attack")->SetActive( UnitType.HasAbility(CanAttack) );
	pMenu->GetGadget("Defend")->SetActive( UnitType.HasAbility(CanDefend) );
	if( UnitType.HasAbility(CanLoad) )
	{
		pMenu->GetGadget("Load")->SetActive( TRUE );
		pMenu->GetGadget("Load")->SetVisible( TRUE );
		pMenu->GetGadget("Blast")->SetActive( FALSE );
		pMenu->GetGadget("Blast")->SetVisible( FALSE );
	}
	else
	{
		pMenu->GetGadget("Load")->SetActive( FALSE );
		pMenu->GetGadget("Load")->SetVisible( FALSE );
		pMenu->GetGadget("Blast")->SetActive( UnitType.HasAbility(CanBlast) );
		pMenu->GetGadget("Blast")->SetVisible( TRUE );
	}

	pMenu->GetGadget("FireOrd")->SetActive( UnitType.HasAbility(HasFireOrders) );
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("FireOrd")))->SetStage( m_CurrentSelection->m_FireOrder );
	pMenu->GetGadget("MoveOrd")->SetActive( UnitType.HasAbility(HasMoveOrders) );
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("MoveOrd")))->SetStage( m_CurrentSelection->m_MoveOrder );

	pMenu->GetGadget("OnOff")->SetActive( UnitType.HasAbility(OnOffable) );
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("OnOff")))->SetStage( m_CurrentSelection->m_Activation );
	pMenu->GetGadget("Cloak")->SetActive( UnitType.HasAbility(CanCloak) );
	(static_cast<igui_OrderButton*>(pMenu->GetGadget("Cloak")))->SetStage( m_CurrentSelection->m_Cloaked );

	pMenu->GetGadget("Capture")->SetActive( UnitType.HasAbility(CanCapture) );
	pMenu->GetGadget("Reclaim")->SetActive( UnitType.HasAbility(CanReclaim) );
	pMenu->GetGadget("Repair")->SetActive( UnitType.HasAbility(CanBuild) );
	pMenu->GetGadget("Cloak")->SetActive( UnitType.HasAbility(CanCloak) );
	pMenu->GetGadget("Unload")->SetActive( UnitType.HasAbility(CanLoad) );
}
// End igui_ControlBar::SetOrderMenuCommands()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnCursorMove() //              \author Logan Jones
////////////////////////////////////                   \date 4/27/2002
//               
//====================================================================
// Parameters:
//  std_Point& ptCursor - 
//  DWORD dwFlags       - 
//
void igui_ControlBar::OnCursorMove( std_Point& ptCursor, DWORD dwFlags )
{
	gfx->SetCurrentCursor( guiResources.Cursors.Normal );
	wnd_Window::OnCursorMove( ptCursor, dwFlags );
}
// End igui_ControlBar::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnKeyboardButton() //          \author Logan Jones
////////////////////////////////////////               \date 4/19/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void igui_ControlBar::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	switch( dwButton )
	{
		case IN_KEY_B:
			if( !bDown )
				theInterface->GameWindow().DoBuildMode( std_NameHash("CORLAB") );
			break;

		default:
			theInterface->GameWindow().OnKeyboardButton( dwButton, bDown, dwFlags );
	}
}
// End igui_ControlBar::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ControlBar::OnWndMessage() //              \author Logan Jones
////////////////////////////////////                   \date 4/25/2002
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
DWORD igui_ControlBar::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR	strSender = (LPTSTR)dwParamA;
	DWORD	Page;

	if( dwMessage==gui_msg_ButtonPressed )
	{
		STRING_SWITCH( strSender )

			CASE( "Orders" )
                if( m_CurrentPage ) {
                    sound.PlaySound( guiResources.Sounds.OrdersButton );
					SetMenu( 0 );
                }
				return 1;

			CASE( "Build" )
                if( !m_CurrentPage ) {
                    sound.PlaySound( guiResources.Sounds.BuildButton );
					SetMenu( m_CurrentSelection->m_LastBuildPage );
                }
				return 1;

			CASE( "Next" )
				sound.PlaySound( guiResources.Sounds.NextBuildMenu );
				Page = m_CurrentPage + 1;
				if( Page>m_CurrentSelection->m_pUnitType->BuildMenuCount ) Page = 1;
				if( Page!=m_CurrentPage ) SetMenu( Page );
				return 1;

			CASE( "Prev" )
				sound.PlaySound( guiResources.Sounds.NextBuildMenu );
				Page = m_CurrentPage - 1;
				if( Page==0 ) Page = m_CurrentSelection->m_pUnitType->BuildMenuCount;
				if( Page!=m_CurrentPage ) SetMenu( Page );
				return 1;

			HandleUnitOrderMode( Move,		guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Patrol,	guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Attack,	guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Defend,	guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Blast,		guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Capture,	guiResources.Sounds.SpecialOrders );
			HandleUnitOrderMode( Reclaim,	guiResources.Sounds.SpecialOrders );
			HandleUnitOrderMode( Repair,	guiResources.Sounds.SpecialOrders );
			HandleUnitOrderMode( Load,		guiResources.Sounds.ImmediateOrders );
			HandleUnitOrderMode( Unload,	guiResources.Sounds.SpecialOrders );

			HandleUnitOrder( Stop,			guiResources.Sounds.ImmediateOrders );
			HandleUnitOrder( FireOrd,		guiResources.Sounds.SetFireOrders );
			HandleUnitOrder( MoveOrd,		guiResources.Sounds.SetMoveOrders );
			HandleUnitOrder( OnOff,			guiResources.Sounds.SpecialOrders );
			HandleUnitOrder( Cloak,			guiResources.Sounds.SpecialOrders );

		END_STRING_SWITCH

	} // end if( dwMessage==msg_ButtonPressed )
	else if( dwMessage==gui_msg_InitButtonImages )
		SetButtonImages( static_cast<gadget_Button*>(pSender) );

	return wnd_Window::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End igui_ControlBar::OnWndMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_ControlBar.cpp //
//////////////////////////////
