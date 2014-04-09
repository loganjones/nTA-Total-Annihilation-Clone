// unit_Project.h //                               \author Logan Jones
///////////////////                                  \date 02-20-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_PROJECT_H_
#define _UNIT_PROJECT_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// unit_Project ///////////////////////////
//
struct unit_Project
{
	unit_Object*		Target;
	const unit_Type*	TargetType;
	std_Point			Location;
	DWORD				WorkerCount;
	float				AppliedTime;

	unit_Project( const unit_Type* pType, const std_Point& ptLoc ):
	Target(NULL),TargetType(pType),Location(ptLoc),WorkerCount(1),AppliedTime(0){}

}; // End struct - unit_Project
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_PROJECT_H_)
