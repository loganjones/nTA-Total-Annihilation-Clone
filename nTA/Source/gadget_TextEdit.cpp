// gadget_TextEdit.cpp //                          \author Logan Jones
////////////////////////                             \date 04-10-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_TextEdit.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_TextEdit::gadget_TextEdit()
{}
gadget_TextEdit::~gadget_TextEdit()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::Create() //                    \author Logan Jones
//////////////////////////////                       \date 04-10-2003
//
//===================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData   - 
//  ta_gui_TextEdit* pTextEditInfo - 
//  wnd_Window* pParent            - 
//
// Return: BOOL - 
//
BOOL gadget_TextEdit::Create( ta_gui_Common_t* pCommonData, ta_gui_TextEdit* pTextEditInfo, wnd_Window* pParent )
{
	// Sanity check
	assert( pTextEditInfo );

	// Copy the common data
	memcpy( &m_TextEditInfo, pTextEditInfo, sizeof(m_TextEditInfo) );

	// Call the default Create() method
	return gui_Gadget::Create(pCommonData,pParent);
}
// End gadget_TextEdit::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::OnCreate() //                  \author Logan Jones
////////////////////////////////                     \date 04-10-2003
//
//===================================================================
// Return: BOOL - 
//
BOOL gadget_TextEdit::OnCreate()
{
	return m_TextBox.Create(m_Position,m_Size,this);
}
// End gadget_TextEdit::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::OnDestroy() //                 \author Logan Jones
/////////////////////////////////                    \date 04-10-2003
//
//===================================================================
//
void gadget_TextEdit::OnDestroy()
{
	m_TextBox.Destroy();
}
// End gadget_TextEdit::OnDestroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::OnUpdate() //                  \author Logan Jones
////////////////////////////////                     \date 04-10-2003
//
//===================================================================
//
void gadget_TextEdit::OnUpdate()
{
	m_TextBox.OnUpdate();
}
// End gadget_TextEdit::OnUpdate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::OnRender() //                  \author Logan Jones
////////////////////////////////                     \date 04-10-2003
//
//===================================================================
//
void gadget_TextEdit::OnRender()
{
	std_Vector3	Pos( m_ScreenPosition.x + 4, m_ScreenPosition.y + m_Size.height - 8, 0 );

	Pos.x += gfx->RenderString( m_TextBox.Text(), (DWORD)m_TextBox.Caret(), Pos, 0xFFFFFFFF, guiResources.Fonts.Standard );
	gfx->RenderString( m_TextBox.Text() + m_TextBox.Caret(), Pos, 0xFFFFFFFF, guiResources.Fonts.Standard );
	gfx->RenderString( "|", Pos, 0xFFFFFFFF, guiResources.Fonts.Standard );
}
// End gadget_TextEdit::OnRender()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_TextEdit::OnKeyboardButton() //          \author Logan Jones
////////////////////////////////////////             \date 04-10-2003
//
//===================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void gadget_TextEdit::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	m_TextBox.OnKeyboardButton( dwButton, bDown, dwFlags );
}
// End gadget_TextEdit::OnKeyboardButton()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_TextEdit.cpp //
//////////////////////////////