// wnd_Window.inl //                               \author Logan Jones
///////////////////                                   \date 10/2/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetWndManager() //                  \author Logan Jones
////////////////////////////////                       \date 10/3/2001
//               
//====================================================================
// Return: wnd_Manager* - 
//
wnd_Manager* wnd_Window::GetWndManager()
{
	return m_pWndManager;
}
// End wnd_Window::GetWndManager()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetParentWnd() //                   \author Logan Jones
///////////////////////////////                        \date 10/2/2001
//               
//====================================================================
// Return: wnd_Window* - 
//
wnd_Window* wnd_Window::GetParentWnd()
{
	return m_pParent;
}
// End wnd_Window::GetParentWnd()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetWndPos() //                      \author Logan Jones
////////////////////////////                           \date 10/2/2001
//               
//====================================================================
// Return: const std_Point_t& - 
//
const std_Point_t& wnd_Window::GetWndPos()
{
	return m_Position;
}
// End wnd_Window::GetWndPos()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetWndSize() //                     \author Logan Jones
/////////////////////////////                          \date 10/2/2001
//               
//====================================================================
// Return: const std_Size_t& - 
//
const std_Size_t& wnd_Window::GetWndSize()
{
	return m_Size;
}
// End wnd_Window::GetWndSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::IsCreated() //                      \author Logan Jones
////////////////////////////                          \date 10/17/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_Window::IsCreated() const
{
	return (m_bCreated);
}
// End wnd_Window::IsCreated()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::IsActive() //                       \author Logan Jones
///////////////////////////                           \date 10/17/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_Window::IsActive() const
{
	return (m_bActive);
}
// End wnd_Window::IsActive()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::IsVisible() //                      \author Logan Jones
////////////////////////////                          \date 10/17/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_Window::IsVisible() const
{
	return (m_bVisible);
}
// End wnd_Window::IsVisible()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetWndManager() //                  \author Logan Jones
////////////////////////////////                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  wnd_Manager* pWndManager - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SetWndManager( wnd_Manager* pWndManager )
{
	return ((m_pWndManager = pWndManager)!=NULL);
}
// End wnd_Window::SetWndManager()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetWndPos() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point_t& ptNew - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SetWndPos( const std_Point_t& ptNew )
{
	if( OnPos(ptNew) )
	{
		m_ScreenPosition += ptNew - m_Position;
		m_Position = ptNew;
		return TRUE;
	}
	else return FALSE;
}
// End wnd_Window::SetWndPos()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetWndSize() //                     \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Size_t& szNew - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SetWndSize( const std_Size_t& szNew )
{
	if( OnSize(szNew) )
	{
		m_Size = szNew;
		return TRUE;
	}
	else return FALSE;
}
// End wnd_Window::SetWndSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetActive() //                      \author Logan Jones
////////////////////////////                          \date 10/17/2001
//               
//====================================================================
// Parameters:
//  BOOL bActive - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SetActive( BOOL bActive )
{
	return (m_bActive = bActive);
}
// End wnd_Window::SetActive()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::SetVisible() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  BOOL bVisible - 
//
// Return: BOOL - 
//
BOOL wnd_Window::SetVisible( BOOL bVisible )
{
	return (m_bVisible = bVisible);
}
// End wnd_Window::SetVisible()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Detach() //                         \author Logan Jones
/////////////////////////                              \date 4/27/2002
//               
//====================================================================
//
void wnd_Window::Detach()
{
	if( m_pParent ) m_pParent->RemoveChild( this );
}
// End wnd_Window::Detach()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::Attach() //                         \author Logan Jones
/////////////////////////                              \date 4/27/2002
//               
//====================================================================
// Parameters:
//  wnd_Window* pParent - 
//
void wnd_Window::Attach( wnd_Window* pParent )
{
	m_pParent = pParent;
	m_pParent->AddChild( this );
}
// End wnd_Window::Attach()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ScreenToClient() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Point_t* pPoint - 
//
// Return: const std_Point* - 
//
const std_Point* wnd_Window::ScreenToClient( std_Point_t* pPoint ) const
{
	assert( pPoint );
	*pPoint -= m_ScreenPosition;
	return pPoint;
}
// End wnd_Window::ScreenToClient()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ScreenToClient() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point ptScreen - 
//
// Return: const std_Point - 
//
const std_Point wnd_Window::ScreenToClient( const std_Point ptScreen ) const
{
	return ptScreen - m_ScreenPosition;
}
// End wnd_Window::ScreenToClient()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ScreenToClient() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Rect* pRect - 
//
// Return: const std_Rect* - 
//
const std_Rect* wnd_Window::ScreenToClient( std_Rect* pRect ) const
{
	assert( pRect );
	*pRect -= m_ScreenPosition;
	return pRect;
}
// End wnd_Window::ScreenToClient()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ScreenToClient() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect rctScreen - 
//
// Return: const std_Rect - 
//
const std_Rect wnd_Window::ScreenToClient( const std_Rect rctScreen ) const
{
	return rctScreen - m_ScreenPosition;
}
// End wnd_Window::ScreenToClient()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ClientToScreen() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Point* pPoint - 
//
// Return: const std_Point* - 
//
const std_Point* wnd_Window::ClientToScreen( std_Point* pPoint ) const
{
	assert( pPoint );
	*pPoint += m_ScreenPosition;
	return pPoint;
}
// End wnd_Window::ClientToScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ClientToScreen() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point ptClient - 
//
// Return: const std_Point - 
//
const std_Point wnd_Window::ClientToScreen( const std_Point ptClient ) const
{
	return ptClient + m_ScreenPosition;
}
// End wnd_Window::ClientToScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ClientToScreen() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Rect* pRect - 
//
// Return: const std_Rect* - 
//
const std_Rect* wnd_Window::ClientToScreen( std_Rect* pRect ) const
{
	assert( pRect );
	*pRect += m_ScreenPosition;
	return pRect;
}
// End wnd_Window::ClientToScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::ClientToScreen() //                 \author Logan Jones
/////////////////////////////////                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect rctClient - 
//
// Return: const std_Rect - 
//
const std_Rect wnd_Window::ClientToScreen( const std_Rect rctClient ) const
{
	return rctClient + m_ScreenPosition;
}
// End wnd_Window::ClientToScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetWndRect() //                     \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Rect_t* pRect - 
//
void wnd_Window::GetWndRect( std_Rect_t* pRect )
{
	assert( pRect );

	pRect->left = m_ScreenPosition.x;
	pRect->top = m_ScreenPosition.y;
	pRect->right = m_ScreenPosition.x + m_Size.width;
	pRect->bottom = m_ScreenPosition.y + m_Size.height;
}
// End wnd_Window::GetWndRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetWndFromPoint() //                \author Logan Jones
//////////////////////////////////                     \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& point - 
//
// Return: wnd_Window* - 
//
wnd_Window* wnd_Window::GetWndFromPoint( const std_Point_t& point )
{
	wnd_Window*				pWnd;
	std_Rect_t				rect;
	WndList_t::iterator		it = m_Children.begin();
	WndList_t::iterator		end= m_Children.end();
	for( ; it!=end; ++it)
	{
		pWnd = *it;
		pWnd->GetWndRect( &rect );
		if( rect.PointInRect(point) && pWnd->IsActive() )
			return pWnd->GetWndFromPoint( point );
	}

	return this;
}
// End wnd_Window::GetWndFromPoint()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetChildWndFromPoint() //           \author Logan Jones
///////////////////////////////////////                \date 10/3/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& point - 
//
// Return: wnd_Window* - 
//
wnd_Window* wnd_Window::GetChildWndFromPoint( const std_Point_t& point )
{
	wnd_Window*				pWnd;
	std_Rect_t				rect;
	WndList_t::iterator		it = m_Children.begin();
	WndList_t::iterator		end= m_Children.end();
	for( ; it!=end; ++it)
	{
		pWnd = *it;
		pWnd->GetWndRect( &rect );
		if( rect.PointInRect(point) && pWnd->IsActive() )
			return pWnd->GetWndFromPoint( point );
	}

	return NULL;
}
// End wnd_Window::GetChildWndFromPoint()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::GetInputFocus() //                  \author Logan Jones
////////////////////////////////                       \date 4/19/2002
//               
//====================================================================
// Return: wnd_Window* - 
//
wnd_Window* wnd_Window::GetInputFocus()
{
	if( m_Children.empty() )
		return this;
	else return m_Children.front()->GetInputFocus();
}
// End wnd_Window::GetInputFocus()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::AddChild() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pChild - 
//
void wnd_Window::AddChild( wnd_Window* pChild )
{
	m_Children.push_front( pChild );
	if( !m_KeyBoardFocus ) m_KeyBoardFocus = pChild;
}
// End wnd_Window::AddChild()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::RemoveChild() //                    \author Logan Jones
//////////////////////////////                         \date 10/3/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pChild - 
//
void wnd_Window::RemoveChild( wnd_Window* pChild )
{
	m_Children.remove( pChild );
	if( m_Children.empty() ) m_KeyBoardFocus = NULL;
	else m_KeyBoardFocus = m_Children.back();
}
// End wnd_Window::RemoveChild()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::MoveToTop() //                      \author Logan Jones
////////////////////////////                            \date 5/9/2002
//               
//====================================================================
//
void wnd_Window::MoveToTop()
{
	if( m_pParent )
	{
		m_pParent->m_Children.remove( this );
		m_pParent->m_Children.push_front( this );
	}
}
// End wnd_Window::MoveToTop()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::MoveToBottom() //                   \author Logan Jones
///////////////////////////////                         \date 5/9/2002
//               
//====================================================================
//
void wnd_Window::MoveToBottom()
{
	if( m_pParent )
	{
		m_pParent->m_Children.remove( this );
		m_pParent->m_Children.push_back( this );
	}
}
// End wnd_Window::MoveToBottom()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::DestroyChild() //                   \author Logan Jones
///////////////////////////////                       \date 10/16/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pChild - 
//
void wnd_Window::DestroyChild( wnd_Window* pChild )
{
	pChild->Destroy();
}
// End wnd_Window::DestroyChild()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::UpdateChild() //                    \author Logan Jones
//////////////////////////////                        \date 10/14/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pChild - 
//
void wnd_Window::UpdateChild( wnd_Window* pChild )
{
	pChild->Update();
}
// End wnd_Window::UpdateChild()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Window::RenderChild() //                    \author Logan Jones
//////////////////////////////                        \date 10/14/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pChild - 
//
void wnd_Window::RenderChild( wnd_Window* pChild )
{
	pChild->Render();
}
// End wnd_Window::RenderChild()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_Window.inl //
/////////////////////////
