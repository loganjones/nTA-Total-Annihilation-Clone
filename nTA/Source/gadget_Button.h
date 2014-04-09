// gadget_Button.h //                              \author Logan Jones
////////////////////                                  \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GADGET_BUTTON_H_
#define _GADGET_BUTTON_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// gadget_Button ////////////////////////////
//
class gadget_Button : public gui_Gadget
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, ta_gui_Button* pButtonInfo, wnd_Window* pParent );
	virtual BOOL Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent );

	BOOL SetButtonImages( gfx_Surface** pStages, gfx_Surface* pPressed, gfx_Surface* pDisabled, DWORD dwStages=(~0) );
	void SetStage( DWORD dwStage );

	void SetText( LPCTSTR strText, long lStage=-1 );
	LPCTSTR GetText( long lStage=-1 ) const;

	bool IsDown() const {return m_Pressed;}
	DWORD GetStage() const {return m_Stage;}


/////////////////////////////////////////////////
// Event Handlers
public:

	BOOL OnCreate();
	void OnDestroy();

	// Render the button
	void OnRender();

	void OnCursorMove( std_Point_t& ptCursor, DWORD dwFlags );
	void OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Button Events
protected:

	// Button has been created, intitialize the images
	virtual void OnInitButtonImages();

	// The button has gone into the pressed state
	virtual void OnButtonDown();

	// The button has left the pressed state
	virtual void OnButtonUp();

	// Button was pressed
	virtual void OnPressed( DWORD dwButton );


/////////////////////////////////////////////////
// Data members
protected:

	// Button info from the gui file
	ta_gui_Button		m_ButtonInfo;

	// The button is currently being pressed
	bool				m_Pressed;
	DWORD				m_ButtonStates;

	// Currrent stage of the button
	DWORD				m_Stage;

	gfx_Surface**		m_StageImage;
	gfx_Surface*		m_PressedImage;
	gfx_Surface*		m_DisabledImage;

	vector< string >	m_StageText;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_Button();
	virtual ~gadget_Button();
/////////////////////////////////////////////////

}; // End class - gadget_Button
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GADGET_BUTTON_H_)
