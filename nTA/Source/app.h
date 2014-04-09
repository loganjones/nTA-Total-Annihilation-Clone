// app.h //                                        \author Logan Jones
//////////                                            \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _APP_H_
#define _APP_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "sys.h"

// Define some app-wide stuff
#include "app_Defs.h"

// The file tree object
#include "app_FileTree.h"

#include "app_Console.h"
#include "app_nTA.h"

// Global app_FileTree object
// NOTE: Define 'INSTANTIATE_FILETREE_INTERFACE' in some source file
//       to instantiate the global app_FileTree object
#ifdef INSTANTIATE_FILETREE_INTERFACE
	app_FileTree			theFileTree;
#else
	extern app_FileTree		theFileTree;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_APP_H_)
