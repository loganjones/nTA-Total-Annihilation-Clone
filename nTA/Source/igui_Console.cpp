// igui_Console.cpp //                             \author Logan Jones
/////////////////////                                  \date 5/9/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_Console.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_Console::igui_Console():wnd_TextBox(),
	m_OpenSpeed( 64 ),
	m_CloseSpeed( 64 ),
	m_BackGround( NULL ),
	m_CaretBlinkRate( 500 )
{}
igui_Console::~igui_Console()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::OnCreate() //                     \author Logan Jones
/////////////////////////////                           \date 5/9/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL igui_Console::OnCreate()
{
	m_OpenPosition = m_Position.y;
	m_Position.y -= m_Size.height;
	m_ScreenPosition.y -= m_Size.height;

	MoveToBottom();
	SetActive( FALSE );
	SetVisible( FALSE );

	m_MoveState = MS_NOT_MOVING;

	m_DrawCaret = TRUE;
	m_CaretBlinkTime = std_Time();

	return wnd_TextBox::OnCreate();
}
// End igui_Console::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::OnDestroy() //                    \author Logan Jones
//////////////////////////////                          \date 5/9/2002
//               
//====================================================================
//
void igui_Console::OnDestroy()
{
	wnd_TextBox::OnDestroy();
}
// End igui_Console::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::Toggle() //                       \author Logan Jones
///////////////////////////                             \date 5/9/2002
//               
//====================================================================
//
void igui_Console::Toggle()
{
	if( m_bActive )
	{
		// The window could still be closing
		if( m_MoveState==MS_CLOSING )
		{
			m_MoveState = MS_OPENING;
		}
		else // The window is open or opening, make it close
		{
			m_MoveState = MS_CLOSING;
		}
	}
	else // The window is not active, make it so
	{
		SetActive( TRUE );
		SetVisible( TRUE );
		MoveToTop();
		m_MoveState = MS_OPENING;
	}
}
// End igui_Console::Toggle()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::OnUpdate() //                     \author Logan Jones
/////////////////////////////                           \date 5/9/2002
//               
//====================================================================
//
void igui_Console::OnUpdate()
{
	long Movement;

	// Check the move state to see if the console is opening or closing
	if( m_MoveState==MS_OPENING )
	{
		Movement = m_OpenSpeed;
		if( (m_Position.y+Movement)>=m_OpenPosition )
		{
			Movement = m_OpenPosition - m_Position.y;
			m_MoveState = MS_NOT_MOVING;
		}
		m_Position.y += Movement;
		m_ScreenPosition.y += Movement;
	}
	else if( m_MoveState==MS_CLOSING )
	{
		Movement = m_CloseSpeed;
		if( (m_Position.y-Movement)<=(m_OpenPosition-m_Size.height) )
		{
			Movement = m_Position.y - (m_OpenPosition-m_Size.height);
			m_MoveState = MS_NOT_MOVING;
			MoveToBottom();
			SetActive( FALSE );
			SetVisible( FALSE );
		}
		m_Position.y -= Movement;
		m_ScreenPosition.y -= Movement;
	}

	if( (std_Time()-m_CaretBlinkTime)>=m_CaretBlinkRate )
	{
		m_DrawCaret = !m_DrawCaret;
		m_CaretBlinkTime = std_Time();
	}

	wnd_TextBox::OnUpdate();
}
// End igui_Console::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::OnRender() //                     \author Logan Jones
/////////////////////////////                           \date 5/9/2002
//               
//====================================================================
//
void igui_Console::OnRender()
{
	app_Console::CommentList_t::reverse_iterator	it = theApp.Console.m_CommentStack.rbegin();
	app_Console::CommentList_t::reverse_iterator	end= theApp.Console.m_CommentStack.rend();
	std_Vector3 Pos( m_ScreenPosition );
	std_Vector3 CommandLineIndent( 4, 0, 0 );
	const long	LineHeight = theInterface->ConsoleFont()->GetHeight() + 2;

	// Draw the backgound
	gfx->DrawRect( std_RectF(m_ScreenPosition,std_SizeF(m_Size)), (DWORD)0x6BA3C3CC );

	// Draw the command line
	Pos.Shift( 4, m_Size.height - 8, 0 );
	CommandLineIndent.x += gfx->RenderString( m_Text, m_TextCaret, Pos + CommandLineIndent, 0xFFFFFFFF, theInterface->ConsoleFont() );
	gfx->RenderString( m_Text + m_TextCaret, Pos + CommandLineIndent, 0xFFFFFFFF, theInterface->ConsoleFont() );
	if( m_DrawCaret ) gfx->RenderString( "|", Pos + CommandLineIndent, 0xFFFFFFFF, theInterface->ConsoleFont() );

	// Iterate through the comments and draw some some to the screen
	for( Pos.y-=LineHeight+4; it!=end; ++it) if( (*it).Type & CT_ALL )
	{
		gfx->RenderString( (*it).Comment.c_str(), Pos, 0xFFFFFFFF, theInterface->ConsoleFont() );
		Pos.y -= LineHeight;
		if( Pos.y<m_ScreenPosition.y ) break;
	} // end for( comments )
}
// End igui_Console::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Console::OnKeyboardButton() //             \author Logan Jones
/////////////////////////////////////                   \date 5/9/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void igui_Console::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	switch( dwButton )
	{
		case IN_KEY_RETURN: if( bDown )
			{
				theApp.Console.Execute( m_Text, m_TextLength );
				ResetText();
			}
			break;

		case IN_KEY_TILDE:
			break;

		default: // Run the default
			wnd_TextBox::OnKeyboardButton( dwButton, bDown, dwFlags );
	} // end switch( dwButton )
}
// End igui_Console::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_Console.cpp //
///////////////////////////
