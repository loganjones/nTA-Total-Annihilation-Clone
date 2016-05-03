// igui_OrderButton.cpp //                         \author Logan Jones
/////////////////////////                             \date 4/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_OrderButton.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_OrderButton::igui_OrderButton():gadget_Button(),
	m_CanAutoPress( true )
{}
igui_OrderButton::~igui_OrderButton()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_OrderButton::OnInitButtonImages() //       \author Logan Jones
///////////////////////////////////////////           \date 11/7/2002
//
//===================================================================
//
void igui_OrderButton::OnInitButtonImages()
{
	theGame.Interface.ControlBar().LoadOrderButton( this, m_CommonData, m_ButtonInfo );
}
// End igui_OrderButton::OnInitButtonImages()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_OrderButton::SetStageCount() //            \author Logan Jones
//////////////////////////////////////                 \date 4/25/2002
//               
//====================================================================
// Parameters:
//  UINT32 uiStageCount - 
//
void igui_OrderButton::SetStageCount( UINT32 uiStageCount )
{
	m_ButtonInfo.Stages = uiStageCount;
}
// End igui_OrderButton::SetStageCount()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_OrderButton::OnCursorMove() //             \author Logan Jones
/////////////////////////////////////                  \date 4/25/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwFlags         - 
//
void igui_OrderButton::OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags )
{
	// Skip the gadget_Button implementation
	gui_Gadget::OnCursorMove(ptCursor,dwFlags);
}
// End igui_OrderButton::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_OrderButton::OnMouseButton() //            \author Logan Jones
//////////////////////////////////////                 \date 4/25/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void igui_OrderButton::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// If a primary button was pressed then do our stuff
	if( (dwButton==1 || dwButton==2) && bDown )
			OnPressed();

	// Skip the gadget_Button implementation
	gui_Gadget::OnMouseButton(ptCursor,dwButton,bDown,dwFlags);
}
// End igui_OrderButton::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_OrderButton::OnPressed() //                \author Logan Jones
//////////////////////////////////                     \date 4/25/2002
//               
//====================================================================
//
void igui_OrderButton::OnPressed()
{
	if( m_ButtonInfo.Stages )
		m_Stage = ((m_Stage+1) >= m_ButtonInfo.Stages) ? 0: m_Stage+1;
	else // No stages
		if( m_CanAutoPress ) m_Pressed = !m_Pressed;

	// Send the pressed message
	SendMessage( m_pParent, gui_msg_ButtonPressed, (DWORD)(LPTSTR(m_CommonData.Name)), (DWORD)this );
}
// End igui_OrderButton::OnPressed()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_OrderButton.cpp //
///////////////////////////////
