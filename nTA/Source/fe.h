// fe.h //                                         \author Logan Jones
/////////                                             \date 4/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_H_
#define _FE_H_
/////////////////////////////////////////////////////////////////////


// Include the gui base code and some sound
#include "gui.h"
#include "snd.h"

// Include the fe wnd base classes
#include "fe_Wnd.h"

// Include the fe manager
#include "fe_GuiManager.h"

// Global fe_GuiManager object
// NOTE: Define 'INSTANTIATE_FRONTEND_INTERFACE' in some source file
//       to instantiate the global fe_GuiManager object
#ifdef INSTANTIATE_FRONTEND_INTERFACE
	fe_GuiManager			theFrontEnd;
#else
	extern fe_GuiManager	theFrontEnd;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_H_)
