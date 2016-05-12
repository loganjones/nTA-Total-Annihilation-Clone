// wnd_Manager.cpp //                              \author Logan Jones
////////////////////                                  \date 9/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "wnd.h"
#include "wnd_Manager.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "wnd_Manager.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
wnd_Manager::wnd_Manager():
	m_Cursor( 0, 0 ),
	m_PreciseCursor( 0, 0 ),
	m_CursorSpeed( 1 ),
	m_pRootWindow( NULL ),
	m_pCapture( NULL ),
	m_Focus( NULL )
{}
wnd_Manager::~wnd_Manager()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::Create() //                        \author Logan Jones
//////////////////////////                             \date 10/2/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_Manager::Create()
{	
	m_pRootWindow = new wnd_Window;
	if( m_pRootWindow==NULL )
		return FALSE;

	if( !m_pRootWindow->Create( std_Point_t(0,0), std_Size_t(0,0), NULL ) )
	{
		SAFE_DELETE( m_pRootWindow );
		return FALSE;
	}

	m_Focus = NULL;

	return TRUE;
}
// End wnd_Manager::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::Destroy() //                       \author Logan Jones
///////////////////////////                            \date 10/2/2001
//               
//====================================================================
//
void wnd_Manager::Destroy()
{
	// Destroy the root window
	SAFE_DESTROY_AND_DELETE( m_pRootWindow );
}
// End wnd_Manager::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::Update() //                        \author Logan Jones
//////////////////////////                             \date 10/3/2001
//               
//====================================================================
//
void wnd_Manager::Update()
{
	// Proccess each message
	for_each( m_Messages.begin(), m_Messages.end(), ProccessMessage );
	m_Messages.clear();

	m_pRootWindow->Update();
}
// End wnd_Manager::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::HandleInput() //                   \author Logan Jones
///////////////////////////////                         \date 4/1/2002
//               
//====================================================================
// Parameters:
//  in_Devices inDevice - 
//  DWORD dwID          - 
//  long lData          - 
//
// Return: BOOL - 
//
BOOL wnd_Manager::HandleInput( in_Devices inDevice, DWORD dwID, long lData )
{
	
	// What device
	switch( inDevice )
	{
		case in_Mouse:
		{
			wnd_Window*		pMouseWnd;
			std_Point		ClientCursor;

			// Check if the cursor needs moving
			switch( dwID )
			{
				case IN_MOUSE_AXIS_X:
					m_Cursor.x = m_PreciseCursor.x += lData * m_CursorSpeed;
					if( m_Cursor.x>=m_pRootWindow->GetWndSize().width )
						m_PreciseCursor.x = m_Cursor.x = m_pRootWindow->GetWndSize().width - 1;
					else if( m_Cursor.x<0 )
						m_PreciseCursor.x = m_Cursor.x = 0;
					break;
				case IN_MOUSE_AXIS_Y:
					m_Cursor.y = m_PreciseCursor.y += lData * m_CursorSpeed;
					if( m_Cursor.y>=m_pRootWindow->GetWndSize().height )
						m_PreciseCursor.y = m_Cursor.y = m_pRootWindow->GetWndSize().height - 1;
					else if( m_Cursor.y<0 )
						m_PreciseCursor.y = m_Cursor.y = 0;
					break;
			} // end switch( dwID )

			// Get the window to send the initial message to
			if( m_pCapture ) pMouseWnd = m_pCapture;
			else pMouseWnd = m_pRootWindow->GetWndFromPoint( m_Cursor );

			// Convert the mouse coordinates
			ClientCursor = m_Cursor;
			pMouseWnd->ScreenToClient( &ClientCursor );

			// What message should be sent
			switch( dwID )
			{
				case IN_MOUSE_AXIS_X:
				case IN_MOUSE_AXIS_Y: pMouseWnd->OnCursorMove( ClientCursor, 0 ); break;
				case IN_MOUSE_AXIS_Z: pMouseWnd->OnMouseWheel( ClientCursor, lData, 0 ); break;

				case IN_MOUSE_BUTTON_8: pMouseWnd->OnMouseButton( ClientCursor, 8, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_7: pMouseWnd->OnMouseButton( ClientCursor, 7, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_6: pMouseWnd->OnMouseButton( ClientCursor, 6, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_5: pMouseWnd->OnMouseButton( ClientCursor, 5, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_4: pMouseWnd->OnMouseButton( ClientCursor, 4, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_3: pMouseWnd->OnMouseButton( ClientCursor, 3, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_2: pMouseWnd->OnMouseButton( ClientCursor, 2, lData & 0x80, 0 ); break;
				case IN_MOUSE_BUTTON_1: pMouseWnd->OnMouseButton( ClientCursor, 1, lData & 0x80, 0 ); break;

				default: // Input not recognized, don't handle it
					return FALSE;
			} // end switch( dwID )

			// Input handled
			return TRUE;

		} // end case in_Mouse

		case in_Keyboard:
		{
			//m_pRootWindow->GetInputFocus()->OnKeyboardButton( dwID, lData & 0x80, 0 );
			if(m_Focus) m_Focus->OnKeyboardButton( dwID, lData & 0x80, 0 );
		}
		// end case in_Keyboard

	} // end switch( inDevice )

	return FALSE;
}
// End wnd_Manager::HandleInput()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::SetCapture() //                    \author Logan Jones
//////////////////////////////                        \date 10/15/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pCaptureWnd - 
//
void wnd_Manager::SetCapture( wnd_Window* pCaptureWnd )
{
	m_pCapture = pCaptureWnd;
}
// End wnd_Manager::SetCapture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::SetCursorPosition() //             \author Logan Jones
/////////////////////////////////////                   \date 4/1/2002
//               
//====================================================================
// Parameters:
//  const std_Point pt - 
//
void wnd_Manager::SetCursorPosition( const std_Point pt )
{
	wnd_Window*		pMouseWnd;
	std_Point		ClientCursor;

	// Set the new point
	m_Cursor = pt;
	m_PreciseCursor = pt;

	// Get the window to send the initial message to
	if( m_pCapture ) pMouseWnd = m_pCapture;
	else pMouseWnd = m_pRootWindow->GetWndFromPoint( m_Cursor );

	// Convert the mouse coordinates
	ClientCursor = m_Cursor;
	pMouseWnd->ScreenToClient( &ClientCursor );

	// Send the message
	pMouseWnd->OnCursorMove( ClientCursor, 0 );
}
// End wnd_Manager::SetCursorPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::AddMessage() //                    \author Logan Jones
//////////////////////////////                        \date 10/18/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pSrc   - 
//  wnd_Window* pDest  - 
//  DWORD dwMessage - 
//  DWORD dwParamA  - 
//  DWORD dwParamB  - 
//
// Return: BOOL - 
//
BOOL wnd_Manager::AddMessage( wnd_Window* pSrc, wnd_Window* pDest, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	Message_t	NewMessage;

	// Fill out the message
	NewMessage.Src = pSrc;
	NewMessage.Dest = pDest;
	NewMessage.Message = dwMessage;
	NewMessage.ParamA = dwParamA;
	NewMessage.ParamB = dwParamB;

	// Add it
	m_Messages.push_back( NewMessage );

	return TRUE;
}
// End wnd_Manager::AddMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_Manager.cpp //
//////////////////////////
