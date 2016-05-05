// input.h //                                      \author Logan Jones
////////////                                          \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "sys.h"

// Include some definitions
#include "in_Defs.h"

// Define the input interface (platform dependent)
#ifdef _WIN32
 #include "in_DInput8.h"
 typedef in_DInput8			input_Interface;
#else // Other platforms go here
 #include "in_Mac.hpp"
 typedef in_Mac             input_Interface;
#endif

// Global input_Interface object
// NOTE: Define 'INSTANTIATE_INPUT_INTERFACE' in some source file
//       to instantiate the global input_Interface object
#ifdef INSTANTIATE_INPUT_INTERFACE
	input_Interface			input;
#else
	extern input_Interface	input;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_INPUT_H_)
