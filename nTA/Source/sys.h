// sys.h //                                        \author Logan Jones
//////////                                            \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SYS_H_
#define _SYS_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "std.h"

// Define the system application class
#include "sys_App.h"

// Provide an interface for DLLs
#include "sys_DLL.h"

// Global sys_App pointer
// NOTE: Define 'INSTANTIATE_SYS_INTERFACE_POINTER' in some source file
//       to instantiate the global sys_App pointer
#ifdef INSTANTIATE_SYS_INTERFACE_POINTER
	sys_App*			sys = NULL;
#else
	extern sys_App*		sys;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SYS_H_)
