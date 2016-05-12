// wnd_Window_Events.cpp //                        \author Logan Jones
//////////////////////////                            \date 10/2/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "sys.h"
#include "wnd_Window.h"


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnCreate() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_Window::OnCreate()
{
	return TRUE;
}
// End wnd_Window::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnDestroy() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
//
void wnd_Window::OnDestroy()
{
}
// End wnd_Window::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnUpdate() //                       \author Logan Jones
///////////////////////////                           \date 10/17/2001
//               
//====================================================================
//
void wnd_Window::OnUpdate()
{
}
// End wnd_Window::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnRender() //                       \author Logan Jones
///////////////////////////                           \date 10/17/2001
//               
//====================================================================
//
void wnd_Window::OnRender()
{
}
// End wnd_Window::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnPos() //                          \author Logan Jones
////////////////////////                               \date 10/2/2001
//               
//====================================================================
// Parameters:
//  const std_Point_t& ptNew - 
//
// Return: BOOL - 
//
BOOL wnd_Window::OnPos( const std_Point_t& ptNew )
{
	return TRUE;
}
// End wnd_Window::OnPos()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnSize() //                         \author Logan Jones
/////////////////////////                              \date 10/2/2001
//               
//====================================================================
// Parameters:
//  const std_Size_t& szNew - 
//
// Return: BOOL - 
//
BOOL wnd_Window::OnSize( const std_Size_t& szNew )
{
	return TRUE;
}
// End wnd_Window::OnSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnWndMessage() //                   \author Logan Jones
///////////////////////////////                       \date 10/15/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pSender - 
//  DWORD dwMessage  - 
//  DWORD dwParamA   - 
//  DWORD dwParamB   - 
//
// Return: DWORD - 
//
DWORD wnd_Window::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	// No parent, no point going on
	if( m_pParent==NULL ) return 1;

	// Send the event down
	return m_pParent->OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End wnd_Window::OnWndMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnCursorMove() //                   \author Logan Jones
///////////////////////////////                        \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwFlags         - 
//
void wnd_Window::OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags )
{
	// No parent, no point going on
	if( m_pParent==NULL ) return;
/*
	// Copy the cursor position
	std_Point_t		ptParentCursor( ptCursor + m_ScreenPosition - m_pParent->m_ScreenPosition );

	// Convert to screen space
	ClientToScreen( &ptParentCursor );

	// Convert to the parent's client space
	m_pParent->ScreenToClient( &ptParentCursor );

	// Send the event down
	m_pParent->OnCursorMove( ptParentCursor, dwFlags );
*/
	// Send the event down
	m_pParent->OnCursorMove( ptCursor + m_ScreenPosition - m_pParent->m_ScreenPosition, dwFlags );
}
// End wnd_Window::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnMouseButton() //                  \author Logan Jones
////////////////////////////////                      \date 10/15/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void wnd_Window::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// No parent, no point going on
	if( m_pParent==NULL ) return;

	// Send the event down
	m_pParent->OnMouseButton( ptCursor + m_ScreenPosition - m_pParent->m_ScreenPosition, dwButton, bDown, dwFlags );
}
// End wnd_Window::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnMouseWheel() //                   \author Logan Jones
///////////////////////////////                       \date 10/15/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  long lData            - 
//  DWORD dwFlags         - 
//
void wnd_Window::OnMouseWheel( const std_Point_t& ptCursor, long lData, DWORD dwFlags )
{
	// No parent, no point going on
	if( m_pParent==NULL ) return;

	// Send the event down
	m_pParent->OnMouseWheel( ptCursor + m_ScreenPosition - m_pParent->m_ScreenPosition, lData, dwFlags );
}
// End wnd_Window::OnMouseWheel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::OnKeyboardButton() //               \author Logan Jones
///////////////////////////////////                   \date 10/15/2001
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void wnd_Window::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// No parent, no point going on
	if( m_pParent==NULL ) return;

	// Send the event down
	m_pParent->OnKeyboardButton( dwButton, bDown, dwFlags );
}
// End wnd_Window::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_Window_Events.cpp //
////////////////////////////////
