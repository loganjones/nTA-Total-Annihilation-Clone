// wnd_TextBox.h //                                \author Logan Jones
//////////////////                                     \date 5/9/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _WND_TEXTBOX_H_
#define _WND_TEXTBOX_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// wnd_TextBox ////////////////////////////
//
class wnd_TextBox : public wnd_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Clear() {m_Text[0]='\0'; m_TextLength=m_TextCaret=m_KeyDown=0;}

	const char* Text() const {return m_Text;}
	void SetText( LPCTSTR strText ) {for(m_TextLength=0; strText[m_TextLength]!='\0'; ++m_TextLength) m_Text[m_TextLength]=strText[m_TextLength]; m_Text[m_TextLength]='\0'; m_TextCaret=0;}
	const long Caret() const {return m_TextCaret;}


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();

	void OnUpdate();

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility
protected:

	void InsertCharAtCaret( char chInsert );
	void DoBackspace();
	void DoDelete();
	void ResetText();

	char TranslateKeyCode( DWORD dwKey );


/////////////////////////////////////////////////
// Data members
protected:

	char		m_Text[ 512 ];
	long		m_TextLength;
	long		m_TextCaret;

	DWORD		m_KeyDown;
	std_Time_t	m_KeyDownTime;
	std_Time_t	m_TypematicRateDelay;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	wnd_TextBox();
	virtual ~wnd_TextBox();
/////////////////////////////////////////////////

}; // End class - wnd_TextBox
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_WND_TEXTBOX_H_)
