// wnd_TextBox.cpp //                              \author Logan Jones
////////////////////                                   \date 5/9/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "wnd.h"
#include "wnd_TextBox.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
wnd_TextBox::wnd_TextBox():wnd_Window(),
	m_TypematicRateDelay( 300 )
{}
wnd_TextBox::~wnd_TextBox()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::OnCreate() //                      \author Logan Jones
////////////////////////////                            \date 5/9/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL wnd_TextBox::OnCreate()
{
	// Initialize
	Clear();

	// Return the default
	return wnd_Window::OnCreate();
}
// End wnd_TextBox::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::OnUpdate() //                      \author Logan Jones
////////////////////////////                            \date 5/9/2002
//               
//====================================================================
//
void wnd_TextBox::OnUpdate()
{
	// Check if a key is down and enough time has passed
	if( m_KeyDown &&((std_Time()-m_KeyDownTime) >= m_TypematicRateDelay) )
	{
		// Try to get a char from the key code
		char newChar = TranslateKeyCode( m_KeyDown );

		if( newChar ) InsertCharAtCaret( newChar );
		else switch( m_KeyDown )
		{
			case IN_KEY_BACKSPACE: DoBackspace(); break;
			case IN_KEY_DELETE: DoDelete(); break;

			case IN_KEY_LEFT:
				if( --m_TextCaret<0 ) m_TextCaret = 0;
				break;
			case IN_KEY_RIGHT:
				if( ++m_TextCaret>m_TextLength ) m_TextCaret = m_TextLength;
				break;
		} // end switch( m_KeyDown )

	} // end if( m_KeyDown && m_TypematicRateDelay )

	// Run the default
	wnd_Window::OnUpdate();
}
// End wnd_TextBox::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::OnKeyboardButton() //              \author Logan Jones
////////////////////////////////////                    \date 5/9/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void wnd_TextBox::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
#define SetKeyDown( KEY_CODE )	m_KeyDown = KEY_CODE; m_KeyDownTime = std_Time()
	char newChar = TranslateKeyCode( dwButton );

	if( bDown )
	{
		// Check the key is a charaacter
		if( newChar ) InsertCharAtCaret( newChar );
		else switch( dwButton )
		{
			case IN_KEY_BACKSPACE: DoBackspace(); break;
			case IN_KEY_DELETE: DoDelete(); break;

			case IN_KEY_LEFT:
				if( --m_TextCaret<0 ) m_TextCaret = 0;
				break;
			case IN_KEY_RIGHT:
				if( ++m_TextCaret>m_TextLength ) m_TextCaret = m_TextLength;
				break;
/*
			case IN_KEY_LSHIFT:
			case IN_KEY_RSHIFT:
				m_iShiftState++;
				m_KeyDown = 0;
				return 1;
*/
			case IN_KEY_HOME:
				m_TextCaret = 0;
				m_KeyDown = 0;
				return;
			case IN_KEY_END:
				m_TextCaret = m_TextLength;
				m_KeyDown = 0;
				return;

			default: // Run the default
				wnd_Window::OnKeyboardButton( dwButton, bDown, dwFlags );
		} // end switch( dwButton )

		// Record what button was pressed
		SetKeyDown( dwButton );
	}
	else // Key was released
	{
		// Reset the key dwon record if neccessary
		if( m_KeyDown==dwButton ) m_KeyDown = 0;

		if( !newChar ) switch( dwButton )
		{
			case IN_KEY_BACKSPACE:
			case IN_KEY_DELETE:
			case IN_KEY_LEFT:
			case IN_KEY_RIGHT:
			case IN_KEY_HOME:
			case IN_KEY_END:
				break;
/*
			case IN_KEY_LSHIFT:
			case IN_KEY_RSHIFT:
				m_iShiftState--;
				if( m_iShiftState<0 ) m_iShiftState=0;
				return 1;
*/
			default: // Run the default
				wnd_Window::OnKeyboardButton( dwButton, bDown, dwFlags );
		} // end switch( dwButton )
	} // end else // Key was released
}
// End wnd_TextBox::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::InsertCharAtCaret() //             \author Logan Jones
/////////////////////////////////////                  \date 7/27/2001
// Description:  
//               
//====================================================================
// Parameters:
//  char chInsert - 
//
void wnd_TextBox::InsertCharAtCaret( char chInsert )
{
	// If the string is empty
	if( m_TextLength==0 )
	{
		// Just put the charater at the first index
		m_Text[ 0 ] = chInsert;
		m_Text[ 1 ] = '\0';
		m_TextCaret = 1;
		m_TextLength = 1;
	}
	// If the caret is at the end of the string
	else if( m_TextCaret==m_TextLength )
	{
		// Insert the character at the end and increment the caret and length
		m_Text[ m_TextCaret++ ] = chInsert;
		m_Text[ m_TextCaret ] = '\0';
		++m_TextLength;
	}
	else // Insert the character at the caret
	{
		// Move each character after and including the caret back one space
		for( int n=m_TextLength+1; n>m_TextCaret; --n)
			m_Text[ n ] = m_Text[ n-1 ];

		// Place the new character at the caret
		m_Text[ m_TextCaret++ ] = chInsert;

		// Increment the string's length
		++m_TextLength;

	} // end else

}
// End wnd_TextBox::InsertCharAtCaret()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::DoBackspace() //                   \author Logan Jones
///////////////////////////////                        \date 7/27/2001
// Description:  
//               
//====================================================================
//
void wnd_TextBox::DoBackspace()
{
	// Make sure there are characters in the string
	// and that the caret is not at the beginning
	if( (m_TextLength>0)&&(m_TextCaret>0) )
	{
		// Move all characters after (and including) the cursor up one space
		for( int n=(m_TextCaret-1); n<=(m_TextLength-1); n++)
			m_Text[ n ] = m_Text[ n+1 ];

		// Move the caret back one space
		--m_TextCaret;

		// The string is now one space shorter
		--m_TextLength;

	} // end if( (m_TextLength>0)&&(m_TextCaret>0) )

}
// End wnd_TextBox::DoBackspace()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::DoDelete() //                      \author Logan Jones
////////////////////////////                           \date 7/27/2001
// Description:  
//               
//====================================================================
//
void wnd_TextBox::DoDelete()
{
	// Make sure there are characters in the string
	// and that the caret is not at the end
	if( (m_TextLength>0)&&(m_TextCaret<m_TextLength) )
	{
		// Move all characters after the cursor up one space
		for( int n=m_TextCaret; n<=(m_TextLength-1); ++n)
			m_Text[ n ] = m_Text[ n+1 ];

		// The string is now one space shorter
		--m_TextLength;

	} // end if( (m_TextLength>0)&&(m_TextCaret<m_TextLength) )

}
// End wnd_TextBox::DoDelete()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::ResetText() //                     \author Logan Jones
/////////////////////////////                           \date 5/9/2002
//               
//====================================================================
//
void wnd_TextBox::ResetText()
{
	m_Text[0] = '\0';
	m_TextLength = 0;
	m_TextCaret = 0;
	m_KeyDown = 0;
}
// End wnd_TextBox::ResetText()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_TextBox::TranslateKeyCode() //              \author Logan Jones
////////////////////////////////////                    \date 5/9/2002
//               
//====================================================================
// Parameters:
//  DWORD dwKey - 
//
// Return: char - 
//
char wnd_TextBox::TranslateKeyCode( DWORD dwKey )
{
	const bool m_iShiftState = false;
	const bool m_bCapsLock = false;

	switch( dwKey )
	{
		case IN_KEY_1: return ( m_iShiftState ? '!':'1' );
		case IN_KEY_2: return ( m_iShiftState ? '@':'2' );
		case IN_KEY_3: return ( m_iShiftState ? '#':'3' );
		case IN_KEY_4: return ( m_iShiftState ? '$':'4' );
		case IN_KEY_5: return ( m_iShiftState ? '%':'5' );
		case IN_KEY_6: return ( m_iShiftState ? '^':'6' );
		case IN_KEY_7: return ( m_iShiftState ? '&':'7' );
		case IN_KEY_8: return ( m_iShiftState ? '*':'8' );
		case IN_KEY_9: return ( m_iShiftState ? '(':'9' );
		case IN_KEY_0: return ( m_iShiftState ? ')':'0' );
		case IN_KEY_MINUS: return ( m_iShiftState ? '_':'-' );
		case IN_KEY_EQUALS: return ( m_iShiftState ? '+':'=' );
		case IN_KEY_Q: return ( (m_iShiftState ^ m_bCapsLock) ? 'Q':'q' );
		case IN_KEY_W: return ( (m_iShiftState ^ m_bCapsLock) ? 'W':'w' );
		case IN_KEY_E: return ( (m_iShiftState ^ m_bCapsLock) ? 'E':'e' );
		case IN_KEY_R: return ( (m_iShiftState ^ m_bCapsLock) ? 'R':'r' );
		case IN_KEY_T: return ( (m_iShiftState ^ m_bCapsLock) ? 'T':'t' );
		case IN_KEY_Y: return ( (m_iShiftState ^ m_bCapsLock) ? 'Y':'y' );
		case IN_KEY_U: return ( (m_iShiftState ^ m_bCapsLock) ? 'U':'u' );
		case IN_KEY_I: return ( (m_iShiftState ^ m_bCapsLock) ? 'I':'i' );
		case IN_KEY_O: return ( (m_iShiftState ^ m_bCapsLock) ? 'O':'o' );
		case IN_KEY_P: return ( (m_iShiftState ^ m_bCapsLock) ? 'P':'p' );
		case IN_KEY_LBRACKET: return ( m_iShiftState ? '{':'[' );
		case IN_KEY_RBRACKET: return ( m_iShiftState ? '}':']' );
		case IN_KEY_A: return ( (m_iShiftState ^ m_bCapsLock) ? 'A':'a' );
		case IN_KEY_S: return ( (m_iShiftState ^ m_bCapsLock) ? 'S':'s' );
		case IN_KEY_D: return ( (m_iShiftState ^ m_bCapsLock) ? 'D':'d' );
		case IN_KEY_F: return ( (m_iShiftState ^ m_bCapsLock) ? 'F':'f' );
		case IN_KEY_G: return ( (m_iShiftState ^ m_bCapsLock) ? 'G':'g' );
		case IN_KEY_H: return ( (m_iShiftState ^ m_bCapsLock) ? 'H':'h' );
		case IN_KEY_J: return ( (m_iShiftState ^ m_bCapsLock) ? 'J':'j' );
		case IN_KEY_K: return ( (m_iShiftState ^ m_bCapsLock) ? 'K':'k' );
		case IN_KEY_L: return ( (m_iShiftState ^ m_bCapsLock) ? 'L':'l' );
		case IN_KEY_SEMICOLON: return ( m_iShiftState ? ':':';' );
		case IN_KEY_APOSTROPHE: return ( m_iShiftState ? '\"':'\'' );
		case IN_KEY_BACKSLASH: return ( m_iShiftState ? '|':'\\' );
		case IN_KEY_Z: return ( (m_iShiftState ^ m_bCapsLock) ? 'Z':'z' );
		case IN_KEY_X: return ( (m_iShiftState ^ m_bCapsLock) ? 'X':'x' );
		case IN_KEY_C: return ( (m_iShiftState ^ m_bCapsLock) ? 'C':'c' );
		case IN_KEY_V: return ( (m_iShiftState ^ m_bCapsLock) ? 'V':'v' );
		case IN_KEY_B: return ( (m_iShiftState ^ m_bCapsLock) ? 'B':'b' );
		case IN_KEY_N: return ( (m_iShiftState ^ m_bCapsLock) ? 'N':'n' );
		case IN_KEY_M: return ( (m_iShiftState ^ m_bCapsLock) ? 'M':'m' );
		case IN_KEY_COMMA: return ( m_iShiftState ? '<':',' );
		case IN_KEY_PERIOD: return ( m_iShiftState ? '>':'.' );
		case IN_KEY_SLASH: return ( m_iShiftState ? '?':'/' );
		case IN_KEY_SPACE: return (' ');
		default: return 0;
	} // end switch( uiKey )
}
// End wnd_TextBox::TranslateKeyCode()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_TextBox.cpp //
//////////////////////////
