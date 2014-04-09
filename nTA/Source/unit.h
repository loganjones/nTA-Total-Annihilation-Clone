// unit.h //                                       \author Logan Jones
///////////                                           \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_H_
#define _UNIT_H_
/////////////////////////////////////////////////////////////////////


// Include some base code
#include "object.h"

// Define some stuff
#include "unit_Defs.h"

// Include the script machine
#include "script.h"

// Structure holding unit movement data
#include "unit_MoveInfo.h"

// Low level data for every unit type
#include "unit_Type.h"

// Data pertaining to a unit's active job and the project it references
#include "unit_Project.h"
#include "unit_Job.h"

// Base class for a unit or group object.
// Provides an interface with which to interact with the server-side object
#include "unit_Interface.h"

// Order queue
#include "unit_Orders.h"

// Include the derived object
#include "unit_Object.h"

// Include the factory class
#include "unit_Factory.h"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_H_)
