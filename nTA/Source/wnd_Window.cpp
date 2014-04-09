// wnd_Window.cpp //                               \author Logan Jones
///////////////////                                   \date 9/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "wnd.h"
#include "wnd_Window.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "wnd_Window.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
wnd_Window::wnd_Window():
	m_pWndManager( NULL ),
	m_pParent( NULL ),
	m_bCreated( FALSE ),
	m_bActive( FALSE ),
	m_bVisible( FALSE )
{}
wnd_Window::~wnd_Window()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Create() //                         \author Logan Jones
/////////////////////////                              \date 10/2/2001
//               
//====================================================================
// Parameters:
//  const std_Point_t& ptPos - 
//  const std_Size_t& szDim  - 
//  wnd_Window* pParent      - 
//
// Return: BOOL - 
//
BOOL wnd_Window::Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent )
{
	// Assume this window isn't already being used
	assert( m_bCreated==FALSE );
	assert( m_Children.empty() );

	// Set the parent pointer
	m_pParent = pParent;

	// Set the pointer to the wnd manager
	if( pParent )
	{
		m_pWndManager = pParent->m_pWndManager;
		pParent->AddChild( this );
		m_ScreenPosition = pParent->m_ScreenPosition + ptPos;
	}
	else
	{
		m_pWndManager = NULL;
		m_ScreenPosition = ptPos;
	}

	// Set the initial size and position
	m_Position = ptPos;
	m_Size = szDim;

	m_bCreated = TRUE;
	m_bActive = TRUE;
	m_bVisible = TRUE;

	// No focus
	m_KeyBoardFocus = NULL;

	// Call the creation event
	return OnCreate();
}
// End wnd_Window::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Destroy() //                        \author Logan Jones
//////////////////////////                             \date 10/2/2001
//               
//====================================================================
//
void wnd_Window::Destroy()
{
	// Destroy all our children
	//for_each( m_Children.begin(), m_Children.end(), DestroyChild );
	while( !m_Children.empty() ) m_Children.front()->Destroy();

	m_Children.clear();

	// Remove this window from the parent (if there is one)
	if( m_pParent ) m_pParent->RemoveChild( this );

	m_bCreated = FALSE;
	m_bActive = FALSE;
	m_bVisible = FALSE;

	// Call the destruction event
	OnDestroy();
}
// End wnd_Window::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Update() //                         \author Logan Jones
/////////////////////////                             \date 10/14/2001
//               
//====================================================================
//
void wnd_Window::Update()
{
	// Assume this window is created
	assert( m_bCreated );

	// Window must be active to update
	if( m_bActive )
	{
		// Call the update event
		OnUpdate();

		// Update the children
		for_each( m_Children.begin(), m_Children.end(), UpdateChild );
	}
}
// End wnd_Window::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Render() //                         \author Logan Jones
/////////////////////////                             \date 10/14/2001
//               
//====================================================================
//
void wnd_Window::Render()
{
	// Assume this window is created
	assert( m_bCreated );

	// Window must be visible to render
	if( m_bVisible )
	{
		// Call the render event
		OnRender();

		// Render the children (from back to front)
		for_each( m_Children.rbegin(), m_Children.rend(), RenderChild );
	}
}
// End wnd_Window::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetCapture() //                     \author Logan Jones
/////////////////////////////                         \date 10/15/2001
//               
//====================================================================
//
void wnd_Window::SetCapture()
{
	m_pWndManager->SetCapture( this );
}
// End wnd_Window::SetCapture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ReleaseCapture() //                 \author Logan Jones
/////////////////////////////////                     \date 10/15/2001
//               
//====================================================================
//
void wnd_Window::ReleaseCapture()
{
	if( m_pWndManager->GetCapture()==this )
		m_pWndManager->SetCapture( NULL );
}
// End wnd_Window::ReleaseCapture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SendMessage() //                    \author Logan Jones
//////////////////////////////                        \date 10/18/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pDest  - 
//  DWORD dwMessage - 
//  DWORD dwParamA  - 
//  DWORD dwParamB  - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SendMessage( wnd_Window* pDest, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	if( m_pWndManager==NULL ) return FALSE;

	return m_pWndManager->AddMessage( this, pDest, dwMessage, dwParamA, dwParamB );
}
// End wnd_Window::SendMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_Window.cpp //
/////////////////////////
