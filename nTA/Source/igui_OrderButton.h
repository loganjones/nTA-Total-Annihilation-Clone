// igui_OrderButton.h //                           \author Logan Jones
///////////////////////                               \date 4/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_ORDERBUTTON_H_
#define _IGUI_ORDERBUTTON_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// igui_OrderButton //////////////////////////
//
class igui_OrderButton : public gadget_Button
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void SetPressed( bool bPressed=true ) {m_Pressed = bPressed;}
	void SetAutoPress( bool bAutoPress=true ) {m_CanAutoPress = bAutoPress;}

	void SetStageCount( UINT32 uiStageCount );


/////////////////////////////////////////////////
// Event Handlers
public:

	void OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags );
	void OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Button Events
protected:

	// Button has been created, intitialize the images
	void OnInitButtonImages();

	// Button was pressed
	virtual void OnPressed();


/////////////////////////////////////////////////
// Data members
protected:

	// This controls wheteher the button can change its
	// m_Pressed state on its own.
	bool		m_CanAutoPress;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_OrderButton();
	virtual ~igui_OrderButton();
/////////////////////////////////////////////////

}; // End class - igui_OrderButton
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_ORDERBUTTON_H_)
