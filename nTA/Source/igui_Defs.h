// igui_Defs.h //                                  \author Logan Jones
////////////////                                       \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_DEFS_H_
#define _IGUI_DEFS_H_
/////////////////////////////////////////////////////////////////////


#define theInterface		((game_UserInterface*)m_pWndManager)

/////////////////////////////////////////////////////////////////////
// GUI definitions

/////////////////////////////////////////////////
// ta_gui_Common_t.Attribs

#define GADGET_IS_BUILD_BUTTON		32
#define GADGET_IS_SELECTION			16
#define GADGET_IS_COMMAND			64
#define GADGET_IS_MULTI_COMMAND		256
#define GADGET_IS_ORDER_BUTTON		( GADGET_IS_SELECTION | GADGET_IS_COMMAND | GADGET_IS_MULTI_COMMAND )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_DEFS_H_)
