// unit_MoveInfo.h //                              \author Logan Jones
////////////////////                                  \date 4/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_MOVEINFO_H_
#define _UNIT_MOVEINFO_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Movement info: Anything movement related goes here
struct unit_MoveInfo
{
	// Constructor
	unit_MoveInfo();

	// Queries
	INLINE const bool Moving() const;
	INLINE const float GetSpeed() const;
	INLINE const std_Vector3 GetOrientation() const;
	INLINE const std_Vector2 GetDirection() const;
	INLINE const std_Vector2 GetTilt() const;


/////////////////////////////////////////////////
// Movement methods

	INLINE void UpdateTurn();
	INLINE void UpdateSpeed();


/////////////////////////////////////////////////
// Movement data

	DWORD				IsMoving;
	enum MOVING_FLAGS
	{
		MI_TURNING = 1,
		MI_MOVING  = 2,
	};

	float				Speed;
	std_Vector3			Orientation;

	// Turn stuff
	std_Vector2			Direction;
	float				TurnAngle;
	std_Vector2			DesiredDirection;

	float				Acceleration;
	float				DesiredSpeed;


}; // End struct - unit_MoveInfo
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "unit_MoveInfo.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_MOVEINFO_H_)
