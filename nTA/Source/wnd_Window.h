// wnd_Window.h //                                 \author Logan Jones
/////////////////                                     \date 9/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _WND_WINDOW_H_
#define _WND_WINDOW_H_
/////////////////////////////////////////////////////////////////////


class wnd_Manager;

//////////////////////////// wnd_Window /////////////////////////////
//
class wnd_Window
{

/////////////////////////////////////////////////
// Types
public:

	typedef list< wnd_Window* >	WndList_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	virtual BOOL Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent );
	virtual void Destroy();

	virtual void Update();
	virtual void Render();

	INLINE wnd_Manager* GetWndManager();
	INLINE wnd_Window* GetParentWnd();
	INLINE const std_Point_t& GetWndPos();
	INLINE const std_Size_t& GetWndSize();
	INLINE BOOL IsCreated() const;
	INLINE BOOL IsActive() const;
	INLINE BOOL IsVisible() const;

	INLINE BOOL SetWndManager( wnd_Manager* pWndManager );
	INLINE BOOL SetWndPos( const std_Point_t& ptNew );
	INLINE BOOL SetWndSize( const std_Size_t& szNew );
	INLINE BOOL SetActive( BOOL bActive=TRUE );
	INLINE BOOL SetVisible( BOOL bVisible=TRUE );

	INLINE void Detach();
	INLINE void Attach( wnd_Window* pParent );

	INLINE const std_Point* ScreenToClient( std_Point_t* pPoint ) const;
	INLINE const std_Point ScreenToClient( const std_Point ptScreen ) const;
	INLINE const std_Rect* ScreenToClient( std_Rect* pRect ) const;
	INLINE const std_Rect ScreenToClient( const std_Rect rctScreen ) const;
	INLINE const std_Point* ClientToScreen( std_Point* pPoint ) const;
	INLINE const std_Point ClientToScreen( const std_Point ptClient ) const;
	INLINE const std_Rect* ClientToScreen( std_Rect* pRect ) const;
	INLINE const std_Rect ClientToScreen( const std_Rect rctClient ) const;

	INLINE void GetWndRect( std_Rect_t* pRect );

	INLINE wnd_Window* GetWndFromPoint( std_Point_t& point );
	INLINE wnd_Window* GetChildWndFromPoint( std_Point_t& point );

	INLINE void MoveToTop();
	INLINE void MoveToBottom();
	INLINE wnd_Window* GetInputFocus();

	void SetCapture();
	void ReleaseCapture();

	BOOL SendMessage( wnd_Window* pDest, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB );


/////////////////////////////////////////////////
// Event Handlers
public:

	virtual BOOL OnCreate();
	virtual void OnDestroy();

	virtual void OnUpdate();
	virtual void OnRender();

	virtual BOOL OnPos( const std_Point_t& ptNew );
	virtual BOOL OnSize( const std_Size_t& szNew );

	virtual DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB );

	virtual void OnCursorMove( std_Point_t& ptCursor, DWORD dwFlags );
	virtual void OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );
	virtual void OnMouseWheel( std_Point_t& ptCursor, long lData, DWORD dwFlags );

	virtual void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Internal methods
protected:

	INLINE void AddChild( wnd_Window* pChild );
	INLINE void RemoveChild( wnd_Window* pChild );

	static INLINE void DestroyChild( wnd_Window* pChild );
	static INLINE void UpdateChild( wnd_Window* pChild );
	static INLINE void RenderChild( wnd_Window* pChild );


/////////////////////////////////////////////////
// Data members
protected:

	// Pointer to the window's manager
	wnd_Manager*	m_pWndManager;

	// Pointer to the window's parent (if any)
	wnd_Window*			m_pParent;

	// List of attached child windows
	WndList_t			m_Children;

	// Screen dimensions of the window
	std_Point_t			m_ScreenPosition;
	std_Point_t			m_Position;
	std_Size_t			m_Size;

	// Window attributes
	BOOL				m_bCreated;
	BOOL				m_bActive;
	BOOL				m_bVisible;

	// Child window that has input focus
	wnd_Window*			m_KeyBoardFocus;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	wnd_Window();
	virtual ~wnd_Window();
/////////////////////////////////////////////////

}; // End class - wnd_Window
/////////////////////////////////////////////////////////////////////


// Include INLINE implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "wnd_Window.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_WND_WINDOW_H_)
