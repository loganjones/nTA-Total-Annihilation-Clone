// unit_Task.h //                                  \author Logan Jones
////////////////                                     \date 11/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_TASK_H_
#define _UNIT_TASK_H_
/////////////////////////////////////////////////////////////////////

enum TaskType_t {
	TASK_None,
	TASK_Stop,
	TASK_MoveTo,
};

//////////////////////////// unit_Task ////////////////////////////
//
struct unit_Task
{

	TaskType_t		Type;
	std_Point		Waypoint;

	unit_Task( TaskType_t eType ):Type( eType ){}
	unit_Task( TaskType_t eType, const std_Point& ptEnd ):Type( eType ),Waypoint(ptEnd){}

}; // End class - unit_Task
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_TASK_H_)
