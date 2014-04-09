// unit_SoundCategory.h //                         \author Logan Jones
/////////////////////////                            \date 10/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_SOUNDCATEGORY_H_
#define _UNIT_SOUNDCATEGORY_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Unit sound category: Maintains pointers to event sounds for a
// particular sound category for a unit type
struct unit_SoundCategory
{
	LPVOID				Select1;
	LPVOID				Ok1;
	LPVOID				Arrived1;
	LPVOID				Cant1;
	LPVOID				Build;
	LPVOID				Repair;
	LPVOID				Working;
	LPVOID				Activate;
	LPVOID				Deactivate;
	LPVOID				UnderAttack;
	LPVOID				Count5;
	LPVOID				Count4;
	LPVOID				Count3;
	LPVOID				Count2;
	LPVOID				Count1;
	LPVOID				Count0;
	LPVOID				CancelDestruct;
}; // End struct - unit_SoundCategory
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_SOUNDCATEGORY_H_)
