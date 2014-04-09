// gui_Messages.h //                               \author Logan Jones
///////////////////                                   \date 4/21/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GUI_MESSAGES_H_
#define _GUI_MESSAGES_H_
/////////////////////////////////////////////////////////////////////

// Message types sent by gui_Wnd's and handled by another gui_Wnd's OnWndMessage() method
enum gui_Messages_t {
	gui_msg_InitButtonImages,
	gui_msg_ButtonDown,
	gui_msg_ButtonUp,
	gui_msg_ButtonPressed,
	gui_msg_SliderMoved,
	gui_msg_ListBoxSelectionChanged,
	gui_msg_MESSAGE_COUNT,
};

/////////////////////////////////////////////////////////////////////
#endif // !defined(_GUI_MESSAGES_H_)
