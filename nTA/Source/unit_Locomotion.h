// unit_Locomotion.h //                            \author Logan Jones
//////////////////////                               \date 05-20-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_LOCOMOTION_H_
#define _UNIT_LOCOMOTION_H_
/////////////////////////////////////////////////////////////////////

class unit_Object;
////////////////////////// unit_Locomotion //////////////////////////
//
class unit_Locomotion
{ public:

	// Run the locomotion model one step and return movement
	std_Vector2 Step( const std_Vector2& vSteering ) = 0;

	unit_Locomotion( unit_Object& Unit ){unit=Unit;}

protected:	unit_Object& unit;

}; // End class - unit_Locomotion
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_LOCOMOTION_H_)
