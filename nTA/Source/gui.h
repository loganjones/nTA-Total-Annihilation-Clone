// gui.h //                                        \author Logan Jones
//////////                                            \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GUI_H_
#define _GUI_H_
/////////////////////////////////////////////////////////////////////


// Include some base code
#include "app.h"

// Include the 'g' and the 'ui' for the gui
#include "gfx.h"
#include "wnd.h"

// Provide a means to proccess gui files
#include "ta_GUI.h"

// Define the resource class
#include "gui_Resources.h"

// Global gui_Resources object
// NOTE: Define 'INSTANTIATE_GUI_RESOURCES_INTERFACE' in some source file
//       to instantiate the global gui_Resources object
#ifdef INSTANTIATE_GUI_RESOURCES_INTERFACE
	gui_Resources			guiResources;
#else
	extern gui_Resources	guiResources;
#endif

// Define some messages that the gadgets and other windows will send/recieve
#include "gui_Messages.h"

// Define the base gadget class
#include "gui_Gadget.h"

// Include the gadget controls
#include "gadget_Button.h"
#include "gadget_ListBox.h"
#include "gadget_TextEdit.h"
#include "gadget_Slider.h"
#include "gadget_String.h"
#include "gadget_Pic.h"

// Define a base window that proccess a gui file and owns the resulting gadgets
#include "gui_Window.h"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GUI_H_)
