// std.h //                                        \author Logan Jones
//////////                                            \date 1/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_H_
#define _STD_H_
/////////////////////////////////////////////////////////////////////


// Include some various defines
#include "std_Defs.h"

// Include platform specific stuff and define the platform parameters structure
#include "std_Platform.h"

// Include some other standard stuff
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Disable some warnings
#pragma warning( disable : 4244 )	// conversion warning
#pragma warning( disable : 4018 )	// signed/unsigned mismatch
#pragma warning( disable : 4786 )	// 'truncated to 255 character' warning
#pragma warning( disable : 4503 )	// another truncated warning

// Include some STL stuff by default
#include <vector>
#include <list>
#include <map>
#include <hash_map>
#include <algorithm>
using namespace std;

// Provide time functions
#include "std_Time.h"

// Include some standard types
#include "std_Types.h"

// Include the custom assert function/macro
#include "std_Assert.h"

// Include the custom math macros and routines
#include "std_Math.h"

// Include the standard file access library
#include "std_FileIO.h"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_H_)
