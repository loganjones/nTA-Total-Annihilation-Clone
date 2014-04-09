// gadget_TextEdit.h //                            \author Logan Jones
//////////////////////                               \date 04-10-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GADGET_TEXTEDIT_H_
#define _GADGET_TEXTEDIT_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// gadget_TextEdit //////////////////////////
//
class gadget_TextEdit : public gui_Gadget
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, ta_gui_TextEdit* pTextEditInfo, wnd_Window* pParent );

	const char* Text() const {return m_TextBox.Text();}
	void SetText( const char* strText ) {m_TextBox.SetText(strText);}


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();
	void OnDestroy();

	void OnUpdate();

	void OnRender();

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

	// Button info from the gui file
	ta_gui_TextEdit		m_TextEditInfo;

	// Yeah i know this is bad design; so sue me
	wnd_TextBox			m_TextBox;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_TextEdit();
	virtual ~gadget_TextEdit();
/////////////////////////////////////////////////

}; // End class - gadget_TextEdit
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GADGET_TEXTEDIT_H_)
