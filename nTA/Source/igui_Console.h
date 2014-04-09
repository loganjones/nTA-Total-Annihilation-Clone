// igui_Console.h //                               \author Logan Jones
///////////////////                                    \date 5/9/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_CONSOLE_H_
#define _IGUI_CONSOLE_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// igui_Console ////////////////////////////
//
class igui_Console : public wnd_TextBox
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Toggle();


/////////////////////////////////////////////////
// Event Handlers
public:

	BOOL OnCreate();
	void OnDestroy();

	void OnUpdate();
	void OnRender();

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility
protected:


/////////////////////////////////////////////////
// Data members
protected:

	long			m_OpenPosition;
	long			m_OpenSpeed;
	long			m_CloseSpeed;

	enum MoveStates_t {
		MS_NOT_MOVING,
		MS_OPENING,
		MS_CLOSING,
	}				m_MoveState;

	gfx_Surface*	m_BackGround;

	BOOL			m_DrawCaret;
	std_Time_t		m_CaretBlinkTime;
	std_Time_t		m_CaretBlinkRate;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_Console();
	virtual ~igui_Console();
/////////////////////////////////////////////////

}; // End class - igui_Console
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_CONSOLE_H_)
