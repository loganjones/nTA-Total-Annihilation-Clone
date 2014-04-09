// unit_MoveInfo.inl //                            \author Logan Jones
//////////////////////                                \date 4/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::Moving() //                      \author Logan Jones
////////////////////////////                           \date 4/16/2002
//               
//====================================================================
// Return: const bool - 
//
const bool unit_MoveInfo::Moving() const
{
	return (IsMoving > 0);
}
// End unit_MoveInfo::Moving()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::GetSpeed() //                    \author Logan Jones
//////////////////////////////                         \date 4/16/2002
//               
//====================================================================
// Return: const float - 
//
const float unit_MoveInfo::GetSpeed() const
{
	return Speed;
}
// End unit_MoveInfo::GetSpeed()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::GetOrientation() //              \author Logan Jones
////////////////////////////////////                   \date 4/16/2002
//               
//====================================================================
// Return: const std_Vector3 - 
//
const std_Vector3 unit_MoveInfo::GetOrientation() const
{
	return Orientation;
}
// End unit_MoveInfo::GetOrientation()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::GetDirection() //                \author Logan Jones
//////////////////////////////////                     \date 4/16/2002
//               
//====================================================================
// Return: const std_Vector2 - 
//
const std_Vector2 unit_MoveInfo::GetDirection() const
{
	return std_Vector2( cosf(Orientation.z), sinf(Orientation.z) );
}
// End unit_MoveInfo::GetDirection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::GetTilt() //                     \author Logan Jones
/////////////////////////////                          \date 4/16/2002
//               
//====================================================================
// Return: const std_Vector2 - 
//
const std_Vector2 unit_MoveInfo::GetTilt() const
{
	return Orientation;
}
// End unit_MoveInfo::GetTilt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::UpdateTurn() //                  \author Logan Jones
////////////////////////////////                       \date 4/16/2002
//               
//====================================================================
//
void unit_MoveInfo::UpdateTurn()
{
	Direction.Rotate( TurnAngle );

	if( (Direction*DesiredDirection)>0.99f )
	{
		Direction = DesiredDirection;
		TurnAngle = 0;
		IsMoving &= ~unit_MoveInfo::MI_TURNING;
	}

	Orientation.z = Direction.Angle();
}
// End unit_MoveInfo::UpdateTurn()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_MoveInfo::UpdateSpeed() //                 \author Logan Jones
/////////////////////////////////                      \date 4/16/2002
//               
//====================================================================
//
void unit_MoveInfo::UpdateSpeed()
{
	Speed += Acceleration;
	if( (Acceleration>0 && Speed>=DesiredSpeed) || (Acceleration<0 && Speed<=DesiredSpeed) )
	{
		Acceleration = 0;
		Speed = DesiredSpeed;
		if( Speed==0 ) IsMoving &= ~unit_MoveInfo::MI_MOVING;
	}
}
// End unit_MoveInfo::UpdateSpeed()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_MoveInfo.inl //
////////////////////////////
