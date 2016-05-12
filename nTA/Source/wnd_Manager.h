// wnd_Manager.h //                                \author Logan Jones
//////////////////                                    \date 9/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _WND_MANAGER_H_
#define _WND_MANAGER_H_
/////////////////////////////////////////////////////////////////////


class wnd_Window;

//////////////////////////// wnd_Manager ////////////////////////////
//
class wnd_Manager : public in_Handler
{
	struct Message_t
	{
		wnd_Window*		Src;
		wnd_Window*		Dest;
		DWORD			Message;
		DWORD			ParamA;
		DWORD			ParamB;
	};
	typedef list<Message_t>	MessageList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	virtual BOOL Create();
	virtual void Destroy();

	virtual void Update();

	// Inherited input handler. Proccess generic input here.
	virtual BOOL HandleInput( in_Devices inDevice, DWORD dwID, long lData );

	virtual void SetCapture( wnd_Window* pCaptureWnd );
	inline wnd_Window* GetCapture() {return m_pCapture;}

	wnd_Window* SetFocus( wnd_Window* pNewFocus ) { wnd_Window* pOld=m_Focus; m_Focus=pNewFocus; return pOld;}
	wnd_Window* Focus() const {return m_Focus;}

	virtual void SetCursorPosition( const std_Point pt );
	INLINE void SetCursorSpeed( const float fSpeed );
	INLINE const std_Point GetCursorPosition() const;

	BOOL AddMessage( wnd_Window* pSrc, wnd_Window* pDest, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB );


/////////////////////////////////////////////////
// Data members
protected:

	std_Point			m_Cursor;
	std_Vector2			m_PreciseCursor;
	float				m_CursorSpeed;

	wnd_Window*			m_pRootWindow;

	wnd_Window*			m_pCapture;

	wnd_Window*			m_Focus;

	MessageList_t		m_Messages;


/////////////////////////////////////////////////
// Internal methods
protected:

	static INLINE void ProccessMessage( const Message_t& Message );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	wnd_Manager();
	virtual ~wnd_Manager();
/////////////////////////////////////////////////

}; // End class - wnd_Manager
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "wnd_Manager.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_WND_MANAGER_H_)
