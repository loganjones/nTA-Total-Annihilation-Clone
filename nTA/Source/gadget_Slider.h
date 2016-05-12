// gadget_Slider.h //                              \author Logan Jones
///////////////////////                              \date 10/18/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _gadget_Slider_H_
#define _gadget_Slider_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// gadget_Slider //////////////////////////
//
class gadget_Slider : public gui_Gadget
{
	typedef vector<long>	Ticks_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, ta_gui_Slider* pSliderInfo, wnd_Window* pParent );

	// Sets the numerical range for the slider
	INLINE void SetRange( long lMin, long lMax );

	// Sets/gets the position of the knob in relation to the range
	INLINE void SetPosition( long lPos );
	INLINE long GetPosition() const;

	// Sets the frequency at which ticks occur in the range (enables ticker mode)
	INLINE void SetTickFreq( long lFreq );

	// Changes the knob size to a ratio of available space.
	// 1.0 being a knob with maximum size. ie 100%
	INLINE void SetKnobRatio( float fRatio );

	// Attaches the slider to the given window or deattaches the slider if NULL is passed
	INLINE void SetBuddyWnd( wnd_Window* pWnd );


/////////////////////////////////////////////////
// Event Handlers
public:

	void OnDestroy();

	void OnUpdate();
	void OnRender();

	DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB );

	void OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags );
	void OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility methods
protected:

	// Creation/Destruction
	BOOL CreateButtons();
	BOOL CreateStrip();
	void DestroyStrip();
	BOOL CreateKnob();
	void DestroyKnob();

	INLINE void SetKnobPosition( long lPos );

	INLINE void MoveKnobNear( long lWhere );

	INLINE void MoveKnobFoward();
	INLINE void MoveKnobBackward();

	INLINE void SendKnobMovedMessage();


/////////////////////////////////////////////////
// Data members
protected:

	// Info given in the gui file
	ta_gui_Slider		m_SliderInfo;

	// Orientation for the control
	BOOL				m_bVertical;

	// Foward and back buttons for moving the knob
	gadget_Button		m_FowardButton;
	gadget_Button		m_BackButton;

	// Image and position+size for the knob
	gfx_Surface*		m_Knob_Image;
	std_Rect			m_Knob_ImageRect;

	// Length of the knob on its major side
	long				m_Knob_Length;

	// The minimum, maximum, and current point for the top-left corner of the knob.
	// For a vertical knob these are y values, and for a horizontal knob they are x vales
	long				m_Knob_MinPos;
	long				m_Knob_MaxPos;
	long				m_Knob_CurrentPos;

	// Image and position for the background strip
	gfx_Surface*		m_Strip_Image;
	std_Point			m_Strip_ImagePos;

	// Time at which a control button was pressed down
	std_Time_t			m_DownTime;

	// Flag indicating whether the knob is being dragged
	BOOL				m_bDraggingKnob;

	// The given minimum and maximum numerical range for the slider
	long				m_MinRange;
	long				m_MaxRange;

	// Frequency at which ticks will occur in the range
	long				m_TickFreq;
	Ticks_t				m_TickArray;
	Ticks_t				m_PosArray;

	// Current control mode for the control
	enum gadget_Slider_Modes_t {
		MODE_SLIDER,
		MODE_TICKER,
	}					m_Mode;

	// Pointer to a window associated with the slider (if any)
	wnd_Window*			m_pBuddyWnd;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_Slider();
	virtual ~gadget_Slider();
/////////////////////////////////////////////////

}; // End class - gadget_Slider
/////////////////////////////////////////////////////////////////////


// Include INLINE implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "gadget_Slider.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_gadget_Slider_H_)
