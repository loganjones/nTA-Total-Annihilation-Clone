// unit_MoveInfo.cpp //                            \author Logan Jones
//////////////////////                                \date 4/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_MoveInfo.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "unit_MoveInfo.inl"
#endif // defined( _DEBUG )


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::unit_MoveInfo() //               \author Logan Jones
///////////////////////////////////                    \date 4/16/2002
//               
//====================================================================
//
unit_MoveInfo::unit_MoveInfo():
	IsMoving( 0 ),
	Speed( 0 ),
	Orientation( 0, 0, fPI/2 ),
	Direction( 0, 1 ),
	TurnAngle( 0 ),
	Acceleration( 0 )
{}
// End unit_MoveInfo::unit_MoveInfo()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_MoveInfo.cpp //
////////////////////////////
