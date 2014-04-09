// unit_Object_Commands.cpp //                     \author Logan Jones
/////////////////////////////                         \date 5/13/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"

/*
//////////////////////////////////////////////////////////////////////
// unit_Object::Move() //                          \author Logan Jones
/////////////////////////                              \date 4/15/2002
//               
//====================================================================
// Parameters:
//  float fHowFast       - 
//
void unit_Object::Move( float fHowFast )
{
	if( m_pUnitType->MaxSpeed<fHowFast || m_MoveInfo.Speed==fHowFast )
		return;

	if( m_MoveInfo.Speed<fHowFast )
		m_MoveInfo.Acceleration = m_pUnitType->Acceleration;
	else m_MoveInfo.Acceleration = -m_pUnitType->BrakeRate;

	m_MoveInfo.DesiredSpeed = fHowFast;
	if( !m_MoveInfo.IsMoving ) m_Script->RunModule( "StartMoving" );
	m_MoveInfo.IsMoving |= unit_MoveInfo::MI_MOVING;
}
// End unit_Object::Move()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Turn() //                          \author Logan Jones
/////////////////////////                              \date 4/16/2002
//               
//====================================================================
// Parameters:
//  std_Vector2 vToWhere - 
//
void unit_Object::Turn( std_Vector2 vToWhere )
{
	const std_Vector2 A( m_MoveInfo.Direction );
	const std_Vector2 B( vToWhere );
	const float Cross( A.x * B.y - B.x * A.y );

	if( Cross>=0 ) m_MoveInfo.TurnAngle = m_pUnitType->TurnRate;
	else m_MoveInfo.TurnAngle = -m_pUnitType->TurnRate;

	m_MoveInfo.DesiredDirection = vToWhere;
	if( !m_MoveInfo.IsMoving ) m_Script->RunModule( "StartMoving" );
	m_MoveInfo.IsMoving |= unit_MoveInfo::MI_TURNING;
}
// End unit_Object::Turn()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::StopMoving() //                    \author Logan Jones
/////////////////////////                              \date 4/15/2002
//               
//====================================================================
//
void unit_Object::StopMoving()
{
	if( m_MoveInfo.IsMoving ) m_Script->RunModule( "StopMoving" );

	if( m_MoveInfo.IsMoving & unit_MoveInfo::MI_MOVING )
	{
		m_MoveInfo.Acceleration = -m_pUnitType->BrakeRate;
		m_MoveInfo.DesiredSpeed = 0;
	}

	if( m_MoveInfo.IsMoving & unit_MoveInfo::MI_TURNING )
		m_MoveInfo.IsMoving &= ~unit_MoveInfo::MI_TURNING;
}
// End unit_Object::StopMoving()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// unit_Object::Activate() //                      \author Logan Jones
////////////////////////////                           \date 5/13/2002
//               
//====================================================================
// Parameters:
//  bool bRunScript - 
//
void unit_Object::Activate( bool bRunScript )
{
	if( bRunScript ) m_Script->Activate();
	m_Activation = TRUE;
	OnActivate();
}
// End unit_Object::Activate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Deactivate() //                    \author Logan Jones
//////////////////////////////                         \date 5/13/2002
//               
//====================================================================
// Parameters:
//  bool bRunScript - 
//
void unit_Object::Deactivate( bool bRunScript )
{
	if( bRunScript ) m_Script->Deactivate();
	m_Activation = FALSE;
	OnDeactivate();
}
// End unit_Object::Deactivate()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Object_Commands.cpp //
///////////////////////////////////
