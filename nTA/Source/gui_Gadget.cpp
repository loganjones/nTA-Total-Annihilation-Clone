// gui_Gadget.cpp //                               \author Logan Jones
///////////////////                                   \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gui_Gadget.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "gui_Gadget.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gui_Gadget::gui_Gadget():wnd_Window()
{}
gui_Gadget::~gui_Gadget()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Gadget::Create() //                         \author Logan Jones
/////////////////////////                             \date 10/14/2001
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData - 
//  wnd_Window* pParent          - 
//
// Return: BOOL - 
//
BOOL gui_Gadget::Create( ta_gui_Common_t* pCommonData, wnd_Window* pParent )
{
	std_Point_t		Position;

	// Sanity check
	assert( pCommonData );

	// Copy the common data
	memcpy( &m_CommonData, pCommonData, sizeof(m_CommonData) );

	Position.Set( m_CommonData.XPos, m_CommonData.YPos );
	ClientToScreen( &Position );

	// Call the default Create() method
	if( bFAILED(wnd_Window::Create( Position ,std_Size_t(m_CommonData.Width,m_CommonData.Height),pParent)) )
	{
		return FALSE;
	}

	return TRUE;
}
// End gui_Gadget::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Gadget::Render() //                         \author Logan Jones
/////////////////////////                              \date 4/20/2002
//               
//====================================================================
//
void gui_Gadget::Render()
{
	// Assume this window is created
	assert( m_bCreated );

	// Window must be visible to render
	if( m_bVisible )
	{
		// Call the render event
	//	gfx->PushMatrix();
	//	gfx->Translate( m_ScreenPosition );
		OnRender();
	//	gfx->PopMatrix();

		for_each( m_Children.begin(), m_Children.end(), RenderChild );
	}
}
// End gui_Gadget::Render()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gui_Gadget::OnMouseButton() //                  \author Logan Jones
////////////////////////////////                     \date 04-23-2003
//
//===================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void gui_Gadget::OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	m_pWndManager->SetFocus( this );
	wnd_Window::OnMouseButton( ptCursor, dwButton, bDown, dwFlags );
}
// End gui_Gadget::OnMouseButton()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gui_Gadget.cpp //
/////////////////////////
