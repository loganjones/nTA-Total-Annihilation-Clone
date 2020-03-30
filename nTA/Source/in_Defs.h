// in_Defs.h //                                    \author Logan Jones
//////////////                                       \date 10/15/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IN_DEFS_H_
#define _IN_DEFS_H_
/////////////////////////////////////////////////////////////////////


#ifdef _WIN32

#define IN_MOUSE_AXIS_X			DIMOFS_X
#define IN_MOUSE_AXIS_Y			DIMOFS_Y
#define IN_MOUSE_AXIS_Z			DIMOFS_Z
#define IN_MOUSE_BUTTON_1		DIMOFS_BUTTON0
#define IN_MOUSE_BUTTON_2		DIMOFS_BUTTON1
#define IN_MOUSE_BUTTON_3		DIMOFS_BUTTON2
#define IN_MOUSE_BUTTON_4		DIMOFS_BUTTON3
#define IN_MOUSE_BUTTON_5		DIMOFS_BUTTON4
#define IN_MOUSE_BUTTON_6		DIMOFS_BUTTON5
#define IN_MOUSE_BUTTON_7		DIMOFS_BUTTON6
#define IN_MOUSE_BUTTON_8		DIMOFS_BUTTON7

#define IN_KEY_ESCAPE			DIK_ESCAPE
#define IN_KEY_1				DIK_1
#define IN_KEY_2				DIK_2
#define IN_KEY_3				DIK_3
#define IN_KEY_4				DIK_4
#define IN_KEY_5				DIK_5
#define IN_KEY_6				DIK_6
#define IN_KEY_7				DIK_7
#define IN_KEY_8				DIK_8
#define IN_KEY_9				DIK_9
#define IN_KEY_0				DIK_0
#define IN_KEY_MINUS			DIK_MINUS
#define IN_KEY_EQUALS			DIK_EQUALS
#define IN_KEY_BACKSPACE		DIK_BACK
#define IN_KEY_TAB				DIK_TAB
#define IN_KEY_Q				DIK_Q
#define IN_KEY_W				DIK_W
#define IN_KEY_E				DIK_E
#define IN_KEY_R				DIK_R
#define IN_KEY_T				DIK_T
#define IN_KEY_Y				DIK_Y
#define IN_KEY_U				DIK_U
#define IN_KEY_I				DIK_I
#define IN_KEY_O				DIK_O
#define IN_KEY_P				DIK_P
#define IN_KEY_LBRACKET			DIK_LBRACKET
#define IN_KEY_RBRACKET			DIK_RBRACKET
#define IN_KEY_ENTER			DIK_RETURN
#define IN_KEY_RETURN			DIK_RETURN
#define IN_KEY_LCTRL			DIK_LCONTROL
#define IN_KEY_LCONTROL			DIK_LCONTROL
#define IN_KEY_A				DIK_A
#define IN_KEY_S				DIK_S
#define IN_KEY_D				DIK_D
#define IN_KEY_F				DIK_F
#define IN_KEY_G				DIK_G
#define IN_KEY_H				DIK_H
#define IN_KEY_J				DIK_J
#define IN_KEY_K				DIK_K
#define IN_KEY_L				DIK_L
#define IN_KEY_SEMICOLON		DIK_SEMICOLON
#define IN_KEY_APOSTROPHE		DIK_APOSTROPHE
#define IN_KEY_TILDE			DIK_GRAVE
#define IN_KEY_GRAVE			DIK_GRAVE
#define IN_KEY_LSHIFT			DIK_LSHIFT
#define IN_KEY_BACKSLASH		DIK_BACKSLASH
#define IN_KEY_Z				DIK_Z
#define IN_KEY_X				DIK_X
#define IN_KEY_C				DIK_C
#define IN_KEY_V				DIK_V
#define IN_KEY_B				DIK_B
#define IN_KEY_N				DIK_N
#define IN_KEY_M				DIK_M
#define IN_KEY_COMMA			DIK_COMMA
#define IN_KEY_PERIOD			DIK_PERIOD
#define IN_KEY_SLASH			DIK_SLASH
#define IN_KEY_RSHIFT			DIK_RSHIFT
#define IN_KEY_KP_MULTIPLY		DIK_MULTIPLY
#define IN_KEY_LALT				DIK_LMENU
#define IN_KEY_SPACE			DIK_SPACE
#define IN_KEY_CAPSLOCK			DIK_CAPITAL
#define IN_KEY_F1				DIK_F1
#define IN_KEY_F2				DIK_F2
#define IN_KEY_F3				DIK_F3
#define IN_KEY_F4				DIK_F4
#define IN_KEY_F5				DIK_F5
#define IN_KEY_F6				DIK_F6
#define IN_KEY_F7				DIK_F7
#define IN_KEY_F8				DIK_F8
#define IN_KEY_F9				DIK_F9
#define IN_KEY_F10				DIK_F10
#define IN_KEY_NUMLOCK			DIK_NUMLOCK
#define IN_KEY_SCROLLLOCK		DIK_SCROLL
#define IN_KEY_KP_7				DIK_NUMPAD7
#define IN_KEY_KP_8				DIK_NUMPAD8
#define IN_KEY_KP_9				DIK_NUMPAD9
#define IN_KEY_KP_MINUS			DIK_SUBTRACT
#define IN_KEY_KP_SUBTRACT		DIK_SUBTRACT
#define IN_KEY_KP_4				DIK_NUMPAD4
#define IN_KEY_KP_5				DIK_NUMPAD5
#define IN_KEY_KP_6				DIK_NUMPAD6
#define IN_KEY_KP_PLUS			DIK_ADD
#define IN_KEY_KP_ADD			DIK_ADD
#define IN_KEY_KP_1				DIK_NUMPAD1
#define IN_KEY_KP_2				DIK_NUMPAD2
#define IN_KEY_KP_3				DIK_NUMPAD3
#define IN_KEY_KP_0				DIK_NUMPAD0
#define IN_KEY_KP_DECIMAL		DIK_DECIMAL
#define IN_KEY_KP_PERIOD		DIK_DECIMAL
#define IN_KEY_F11				DIK_F11
#define IN_KEY_F12				DIK_F12
#define IN_KEY_F13				DIK_F13
#define IN_KEY_F14				DIK_F14
#define IN_KEY_F15				DIK_F15
#define IN_KEY_KP_EQUALS		DIK_NUMPADEQUALS
#define IN_KEY_RCTRL			DIK_RCONTROL
#define IN_KEY_KP_COMMA			DIK_NUMPADCOMMA
#define IN_KEY_KP_DIVIDE		DIK_DIVIDE
#define IN_KEY_KP_SLASH			DIK_DIVIDE
#define IN_KEY_RALT				DIK_RMENU
#define IN_KEY_PAUSE			DIK_PAUSE
#define IN_KEY_INSERT			DIK_INSERT
#define IN_KEY_DELETE			DIK_DELETE
#define IN_KEY_HOME				DIK_HOME
#define IN_KEY_UP				DIK_UP
#define IN_KEY_PGUP				DIK_PRIOR
#define IN_KEY_LEFT				DIK_LEFT
#define IN_KEY_RIGHT			DIK_RIGHT
#define IN_KEY_END				DIK_END
#define IN_KEY_DOWN				DIK_DOWN
#define IN_KEY_PGDOWN			DIK_NEXT
#define IN_KEY_KP_INS			DIK_INSERT
#define IN_KEY_KP_DEL			DIK_DELETE
#define IN_KEY_LWIN				DIK_LWIN
#define IN_KEY_RWIN				DIK_RWIN
#define IN_KEY_APPMENU			DIK_APPS


#elif SYS_GLFW

enum in_MouseAxis : DWORD {
    IN_MOUSE_AXIS_X = 10000,
    IN_MOUSE_AXIS_Y,
    IN_MOUSE_AXIS_Z,
};

//#define IN_MOUSE_AXIS_X			DIMOFS_X
//#define IN_MOUSE_AXIS_Y			DIMOFS_Y
//#define IN_MOUSE_AXIS_Z			DIMOFS_Z
#define IN_MOUSE_BUTTON_1		GLFW_MOUSE_BUTTON_1
#define IN_MOUSE_BUTTON_2		GLFW_MOUSE_BUTTON_2
#define IN_MOUSE_BUTTON_3		GLFW_MOUSE_BUTTON_3
#define IN_MOUSE_BUTTON_4		GLFW_MOUSE_BUTTON_4
#define IN_MOUSE_BUTTON_5		GLFW_MOUSE_BUTTON_5
#define IN_MOUSE_BUTTON_6		GLFW_MOUSE_BUTTON_6
#define IN_MOUSE_BUTTON_7		GLFW_MOUSE_BUTTON_7
#define IN_MOUSE_BUTTON_8		GLFW_MOUSE_BUTTON_8

#define IN_KEY_ESCAPE			GLFW_KEY_ESCAPE
#define IN_KEY_1				GLFW_KEY_1
#define IN_KEY_2				GLFW_KEY_2
#define IN_KEY_3				GLFW_KEY_3
#define IN_KEY_4				GLFW_KEY_4
#define IN_KEY_5				GLFW_KEY_5
#define IN_KEY_6				GLFW_KEY_6
#define IN_KEY_7				GLFW_KEY_7
#define IN_KEY_8				GLFW_KEY_8
#define IN_KEY_9				GLFW_KEY_9
#define IN_KEY_0				GLFW_KEY_0
#define IN_KEY_MINUS			GLFW_KEY_MINUS
#define IN_KEY_EQUALS			GLFW_KEY_EQUAL
#define IN_KEY_BACKSPACE		GLFW_KEY_BACKSPACE
#define IN_KEY_TAB				GLFW_KEY_TAB
#define IN_KEY_Q				GLFW_KEY_Q
#define IN_KEY_W				GLFW_KEY_W
#define IN_KEY_E				GLFW_KEY_E
#define IN_KEY_R				GLFW_KEY_R
#define IN_KEY_T				GLFW_KEY_T
#define IN_KEY_Y				GLFW_KEY_Y
#define IN_KEY_U				GLFW_KEY_U
#define IN_KEY_I				GLFW_KEY_I
#define IN_KEY_O				GLFW_KEY_O
#define IN_KEY_P				GLFW_KEY_P
#define IN_KEY_LBRACKET			GLFW_KEY_LEFT_BRACKET
#define IN_KEY_RBRACKET			GLFW_KEY_RIGHT_BRACKET
#define IN_KEY_ENTER			GLFW_KEY_ENTER
#define IN_KEY_RETURN			GLFW_KEY_KP_ENTER
#define IN_KEY_LCTRL			GLFW_KEY_LEFT_CONTROL
#define IN_KEY_LCONTROL			GLFW_KEY_LEFT_CONTROL
#define IN_KEY_A				GLFW_KEY_A
#define IN_KEY_S				GLFW_KEY_S
#define IN_KEY_D				GLFW_KEY_D
#define IN_KEY_F				GLFW_KEY_F
#define IN_KEY_G				GLFW_KEY_G
#define IN_KEY_H				GLFW_KEY_H
#define IN_KEY_J				GLFW_KEY_J
#define IN_KEY_K				GLFW_KEY_K
#define IN_KEY_L				GLFW_KEY_L
#define IN_KEY_SEMICOLON		GLFW_KEY_SEMICOLON
#define IN_KEY_APOSTROPHE		GLFW_KEY_APOSTROPHE
#define IN_KEY_TILDE			GLFW_KEY_GRAVE_ACCENT
#define IN_KEY_GRAVE			GLFW_KEY_GRAVE_ACCENT
#define IN_KEY_LSHIFT			GLFW_KEY_LEFT_SHIFT
#define IN_KEY_BACKSLASH		GLFW_KEY_BACKSLASH
#define IN_KEY_Z				GLFW_KEY_Z
#define IN_KEY_X				GLFW_KEY_X
#define IN_KEY_C				GLFW_KEY_C
#define IN_KEY_V				GLFW_KEY_V
#define IN_KEY_B				GLFW_KEY_B
#define IN_KEY_N				GLFW_KEY_N
#define IN_KEY_M				GLFW_KEY_M
#define IN_KEY_COMMA			GLFW_KEY_COMMA
#define IN_KEY_PERIOD			GLFW_KEY_PERIOD
#define IN_KEY_SLASH			GLFW_KEY_SLASH
#define IN_KEY_RSHIFT			GLFW_KEY_RIGHT_SHIFT
#define IN_KEY_KP_MULTIPLY		GLFW_KEY_MULTIPLY
#define IN_KEY_LALT				GLFW_KEY_LMENU
#define IN_KEY_SPACE			GLFW_KEY_SPACE
#define IN_KEY_CAPSLOCK			GLFW_KEY_CAPITAL
#define IN_KEY_F1				GLFW_KEY_F1
#define IN_KEY_F2				GLFW_KEY_F2
#define IN_KEY_F3				GLFW_KEY_F3
#define IN_KEY_F4				GLFW_KEY_F4
#define IN_KEY_F5				GLFW_KEY_F5
#define IN_KEY_F6				GLFW_KEY_F6
#define IN_KEY_F7				GLFW_KEY_F7
#define IN_KEY_F8				GLFW_KEY_F8
#define IN_KEY_F9				GLFW_KEY_F9
#define IN_KEY_F10				GLFW_KEY_F10
#define IN_KEY_NUMLOCK			GLFW_KEY_NUMLOCK
#define IN_KEY_SCROLLLOCK		GLFW_KEY_SCROLL
#define IN_KEY_KP_7				GLFW_KEY_KP_7
#define IN_KEY_KP_8				GLFW_KEY_KP_8
#define IN_KEY_KP_9				GLFW_KEY_KP_9
#define IN_KEY_KP_MINUS			GLFW_KEY_KP_SUBTRACT
#define IN_KEY_KP_SUBTRACT		GLFW_KEY_KP_SUBTRACT
#define IN_KEY_KP_4				GLFW_KEY_KP_4
#define IN_KEY_KP_5				GLFW_KEY_KP_5
#define IN_KEY_KP_6				GLFW_KEY_KP_6
#define IN_KEY_KP_PLUS			GLFW_KEY_KP_ADD
#define IN_KEY_KP_ADD			GLFW_KEY_KP_ADD
#define IN_KEY_KP_1				GLFW_KEY_KP_1
#define IN_KEY_KP_2				GLFW_KEY_KP_2
#define IN_KEY_KP_3				GLFW_KEY_KP_3
#define IN_KEY_KP_0				GLFW_KEY_KP_0
#define IN_KEY_KP_DECIMAL		GLFW_KEY_KP_DECIMAL
#define IN_KEY_KP_PERIOD		GLFW_KEY_KP_DECIMAL
#define IN_KEY_F11				GLFW_KEY_F11
#define IN_KEY_F12				GLFW_KEY_F12
#define IN_KEY_F13				GLFW_KEY_F13
#define IN_KEY_F14				GLFW_KEY_F14
#define IN_KEY_F15				GLFW_KEY_F15
#define IN_KEY_KP_EQUALS		GLFW_KEY_KP_EQUAL
#define IN_KEY_RCTRL			GLFW_KEY_RIGHT_CONTROL
#define IN_KEY_KP_COMMA			GLFW_KEY_NUMPADCOMMA
#define IN_KEY_KP_DIVIDE		GLFW_KEY_DIVIDE
#define IN_KEY_KP_SLASH			GLFW_KEY_DIVIDE
#define IN_KEY_RALT				GLFW_KEY_RMENU
#define IN_KEY_PAUSE			GLFW_KEY_PAUSE
#define IN_KEY_INSERT			GLFW_KEY_INSERT
#define IN_KEY_DELETE			GLFW_KEY_DELETE
#define IN_KEY_HOME				GLFW_KEY_HOME
#define IN_KEY_UP				GLFW_KEY_UP
#define IN_KEY_PGUP				GLFW_KEY_PAGE_UP
#define IN_KEY_LEFT				GLFW_KEY_LEFT
#define IN_KEY_RIGHT			GLFW_KEY_RIGHT
#define IN_KEY_END				GLFW_KEY_END
#define IN_KEY_DOWN				GLFW_KEY_DOWN
#define IN_KEY_PGDOWN			GLFW_KEY_PAGE_DOWN
#define IN_KEY_KP_INS			GLFW_KEY_INSERT
#define IN_KEY_KP_DEL			GLFW_KEY_DELETE
#define IN_KEY_LWIN				GLFW_KEY_LWIN
#define IN_KEY_RWIN				GLFW_KEY_RWIN
#define IN_KEY_APPMENU			GLFW_KEY_APPS


#elif __APPLE__

#include "std_Platform.h"

enum in_MouseAxis : DWORD {
    IN_MOUSE_AXIS_X = 10000,
    IN_MOUSE_AXIS_Y,
    IN_MOUSE_AXIS_Z,
};

enum in_MouseButton : DWORD {
    IN_MOUSE_BUTTON_1 = 10100,
    IN_MOUSE_BUTTON_2,
    IN_MOUSE_BUTTON_3,
    IN_MOUSE_BUTTON_4,
    IN_MOUSE_BUTTON_5,
    IN_MOUSE_BUTTON_6,
    IN_MOUSE_BUTTON_7,
    IN_MOUSE_BUTTON_8,
};

enum in_KeyboardButton : DWORD {
    IN_KEY_ESCAPE = 1000,
    IN_KEY_1,
    IN_KEY_2,
    IN_KEY_3,
    IN_KEY_4,
    IN_KEY_5,
    IN_KEY_6,
    IN_KEY_7,
    IN_KEY_8,
    IN_KEY_9,
    IN_KEY_0,
    IN_KEY_MINUS,
    IN_KEY_EQUALS,
    IN_KEY_BACKSPACE,
    IN_KEY_TAB,
    IN_KEY_Q,
    IN_KEY_W,
    IN_KEY_E,
    IN_KEY_R,
    IN_KEY_T,
    IN_KEY_Y,
    IN_KEY_U,
    IN_KEY_I,
    IN_KEY_O,
    IN_KEY_P,
    IN_KEY_LBRACKET,
    IN_KEY_RBRACKET,
    IN_KEY_ENTER,
    IN_KEY_RETURN,
    IN_KEY_LCTRL,
    IN_KEY_LCONTROL,
    IN_KEY_A,
    IN_KEY_S,
    IN_KEY_D,
    IN_KEY_F,
    IN_KEY_G,
    IN_KEY_H,
    IN_KEY_J,
    IN_KEY_K,
    IN_KEY_L,
    IN_KEY_SEMICOLON,
    IN_KEY_APOSTROPHE,
    IN_KEY_TILDE,
    IN_KEY_GRAVE,
    IN_KEY_LSHIFT,
    IN_KEY_BACKSLASH,
    IN_KEY_Z,
    IN_KEY_X,
    IN_KEY_C,
    IN_KEY_V,
    IN_KEY_B,
    IN_KEY_N,
    IN_KEY_M,
    IN_KEY_COMMA,
    IN_KEY_PERIOD,
    IN_KEY_SLASH,
    IN_KEY_RSHIFT,
    IN_KEY_KP_MULTIPLY,
    IN_KEY_LALT,
    IN_KEY_SPACE,
    IN_KEY_CAPSLOCK,
    IN_KEY_F1,
    IN_KEY_F2,
    IN_KEY_F3,
    IN_KEY_F4,
    IN_KEY_F5,
    IN_KEY_F6,
    IN_KEY_F7,
    IN_KEY_F8,
    IN_KEY_F9,
    IN_KEY_F10,
    IN_KEY_NUMLOCK,
    IN_KEY_SCROLLLOCK,
    IN_KEY_KP_7,
    IN_KEY_KP_8,
    IN_KEY_KP_9,
    IN_KEY_KP_MINUS,
    IN_KEY_KP_SUBTRACT,
    IN_KEY_KP_4,
    IN_KEY_KP_5,
    IN_KEY_KP_6,
    IN_KEY_KP_PLUS,
    IN_KEY_KP_ADD,
    IN_KEY_KP_1,
    IN_KEY_KP_2,
    IN_KEY_KP_3,
    IN_KEY_KP_0,
    IN_KEY_KP_DECIMAL,
    IN_KEY_KP_PERIOD,
    IN_KEY_F11,
    IN_KEY_F12,
    IN_KEY_F13,
    IN_KEY_F14,
    IN_KEY_F15,
    IN_KEY_KP_EQUALS,
    IN_KEY_RCTRL,
    IN_KEY_KP_COMMA,
    IN_KEY_KP_DIVIDE,
    IN_KEY_KP_SLASH,
    IN_KEY_RALT,
    IN_KEY_PAUSE,
    IN_KEY_INSERT,
    IN_KEY_DELETE,
    IN_KEY_HOME,
    IN_KEY_UP,
    IN_KEY_PGUP,
    IN_KEY_LEFT,
    IN_KEY_RIGHT,
    IN_KEY_END,
    IN_KEY_DOWN,
    IN_KEY_PGDOWN,
    IN_KEY_KP_INS,
    IN_KEY_KP_DEL,
    IN_KEY_LWIN,
    IN_KEY_RWIN,
    IN_KEY_APPMENU,
};

#else
#  error "Unknown Platform"
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IN_DEFS_H_)