// unit_Ground.cpp //                              \author Logan Jones
////////////////////                                  \date 11/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Ground.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// unit_Ground::OnCreate() //                      \author Logan Jones
////////////////////////////                         \date 11/10/2002
//
//===================================================================
//
void unit_Ground::OnCreate()
{
	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
	m_Model->SetOrientation( m_Orientation + std_Vector3(0,0,fPI/2) );
}
// End unit_Ground::OnCreate()
/////////////////////////////////////////////////////////////////////

/*
/////////////////////////////////////////////////////////////////////
// unit_Ground::DoMovement() //                    \author Logan Jones
//////////////////////////////                        \date 11/9/2002
//
//===================================================================
// Return: bool - 
//
bool unit_Ground::DoMovement()
{
	// Save the old base rect
	m_MovementBox = m_BaseRect;
/x
	// Turn
	if( m_MoveInfo.IsMoving & unit_MoveInfo::MI_TURNING )
		m_MoveInfo.UpdateTurn();

	// Update speed
	if( m_MoveInfo.Acceleration )
		m_MoveInfo.UpdateSpeed();

	// Move
	if( m_MoveInfo.IsMoving & unit_MoveInfo::MI_MOVING )
	{
		float			speed = m_MoveInfo.Speed;

		// Tally speed penalties
		if( theGame.Terrain.GetElevation(m_BaseRect.Center()) < theGame.Terrain.GetSeaLevel() )
			speed *= WATER_PENALTY;

		// Calculate velocity and apply it
		m_Velocity = m_MoveInfo.Direction * speed;
		m_BaseRect += m_Velocity;
	}
x/
	std_Vector2 target = Waypoint();
	std_Vector2 position = m_BaseRect.Center();
	std_Vector2	DesiredVelocity = /xposition -x/ target - position;
	//std_Vector2	DesiredVelocity = Waypoint() - m_BaseRect.Center();
	const float Distance = DesiredVelocity.Magnitude();

	const float SlowingDistance = sqr(m_pUnitType->MaxSpeed) / m_pUnitType->BrakeRate;
	const float RampedSpeed = m_pUnitType->MaxSpeed * min( Distance / SlowingDistance, 1.0f );
	DesiredVelocity *= RampedSpeed / Distance;

	//DesiredVelocity *= m_pUnitType->MaxSpeed / Distance;

	std_Vector2	SteeringForce = math_Truncate( DesiredVelocity - m_Velocity, m_pUnitType->Acceleration );
	SteeringForce.Truncate( m_pUnitType->Acceleration );
/x	std_Vector2 NewDir = math_Normalize( m_Velocity + SteeringForce );
	float CosAngle = NewDir * m_MoveInfo.Direction;
	if( acosf(CosAngle)>m_pUnitType->TurnRate )
x/
	m_Velocity += SteeringForce;
	m_Velocity.Truncate( m_pUnitType->MaxSpeed );

	m_BaseRect += m_Velocity;

	m_MoveInfo.Direction = m_Velocity;
	m_MoveInfo.Direction.Normalize();
	m_MoveInfo.Orientation.z = m_MoveInfo.Direction.Angle() + fPI/2;

	if( (Waypoint()-m_BaseRect.Center()).MagnitudeSquared()<=16 )
		m_Velocity.Nullify(),
		m_Script->RunModule( "StopMoving" ),
		MakeMeStatic();

	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
	m_SceneSortKey = m_VisibleRect.top;
	UpdateInScene();

	m_Model->SetOrientation( m_MoveInfo.Orientation );

	// Make a movement box that encompasses the old and new position
	m_MovementBox.Encompass( m_BaseRect );
	return false;
}
// End unit_Ground::DoMovement()
/////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// unit_Ground::Locomotion() //                    \author Logan Jones
//////////////////////////////                       \date 05-28-2003
//
//===================================================================
// Parameters:
//  const std_Vector2& vSteering - 
//
void unit_Ground::Locomotion( const std_Vector2& vSteering )
{
	m_vSteering = vSteering;

	m_SteeringForce = math_Truncate( vSteering, (m_Direction*vSteering)>0 ? m_pUnitType->Acceleration:m_pUnitType->BrakeRate );
	std_Vector2 NewVelocity = math_Truncate( m_Velocity + m_SteeringForce, m_pUnitType->MaxSpeed );
	std_Vector2	NewDirection= math_Normalize( NewVelocity );
	if( (m_Direction*NewDirection)>=cosf(m_pUnitType->TurnRate) )
		m_Velocity = NewVelocity,
		m_Direction = NewDirection;
	else if( math_Determinant(m_Direction,NewDirection)>=0 )
		m_Velocity.SetPolar( m_Direction.Angle() + m_pUnitType->TurnRate, NewVelocity.Magnitude() ),
		m_Direction = math_Normalize( m_Velocity );
	else
		m_Velocity.SetPolar( m_Direction.Angle() - m_pUnitType->TurnRate, NewVelocity.Magnitude() ),
		m_Direction = math_Normalize( m_Velocity );
/*
	const float AccelE = sqrtf( 1 - ((m_pUnitType->TurnRate>m_pUnitType->Acceleration) ? (sqr(m_pUnitType->Acceleration)/sqr(m_pUnitType->TurnRate)):(sqr(m_pUnitType->TurnRate)/sqr(m_pUnitType->Acceleration))) );
	const float BrakeE = sqrtf( 1 - ((m_pUnitType->TurnRate>m_pUnitType->BrakeRate) ? (sqr(m_pUnitType->BrakeRate)/sqr(m_pUnitType->TurnRate)):(sqr(m_pUnitType->TurnRate)/sqr(m_pUnitType->BrakeRate))) );
	const float CosTheta = vSteering * m_Direction;
	float SteeringMagnitude;
	if( CosTheta>0 ) SteeringMagnitude = max(m_pUnitType->TurnRate,m_pUnitType->Acceleration) * sqrt( (1 - sqr(AccelE)) / (1 - sqr(AccelE*CosTheta)) );
	else SteeringMagnitude = max(m_pUnitType->TurnRate,m_pUnitType->BrakeRate) * sqrt( (1 - sqr(BrakeE)) / (1 - sqr(BrakeE*CosTheta)) );
	m_Velocity = math_Truncate( m_Velocity + (vSteering * SteeringMagnitude), m_pUnitType->MaxSpeed );
	m_Direction = math_Normalize( m_Velocity );
	m_SteeringForce = vSteering * SteeringMagnitude;
*/
	m_BaseRect += m_Velocity;

	//m_Direction = math_Normalize( m_Velocity );
	m_Orientation.z = m_Direction.Angle() + fPI/2;

	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
	m_SceneSortKey = m_VisibleRect.top;
	UpdateInScene();

	m_Model->SetOrientation( m_Orientation );
}
// End unit_Ground::Locomotion()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Ground::Render() //                        \author Logan Jones
//////////////////////////                           \date 05-28-2003
//
//===================================================================
//
void unit_Ground::Render()
{
	m_Model->Render( m_VisibleRect.Center(), m_Orientation + std_Vector3(0,0,fPI/2) );

	const std_Vector2	pos = m_VisibleRect.Center();
	gfx->DrawLine( pos, m_Velocity*16 + pos, 0xFFFFFFFF );
	//gfx->DrawLine( pos, m_L3_Acceleration*4 + pos, 0xFFFFFFFF );
	gfx->DrawLine( pos, m_vSteering*16 + pos, 0x00FF00FF );
	gfx->DrawLine( pos, m_SteeringForce*16 + pos, 0xFF0000FF );
/*
	static char str[64];

	sprintf( str, "TurnRate %f Rads, %f degrees, cos=%f", m_pUnitType->TurnRate, m_pUnitType->TurnRate * 180.0f/fPI, cosf(m_pUnitType->TurnRate) );
	gfx->RenderString( str, std_Vector3(0,16,0) + pos, 0xFFFFFFFF );

	const std_Vector2	steer = math_Normalize(m_SteeringForce);
	sprintf( str, "Steering (%f,%f) >> (%f,%f) >> (%f,%f)", m_vSteering.x, m_vSteering.y, m_SteeringForce.x, m_SteeringForce.y, steer.x, steer.y );
	gfx->RenderString( str, std_Vector3(0,32,0) + pos, 0xFFFFFFFF );

	sprintf( str, "(%f,%f) * (%f,%f) = %f", steer.x, steer.y, m_Direction.x, m_Direction.y, steer*m_Direction );
	gfx->RenderString( str, std_Vector3(0,48,0) + pos, 0xFFFFFFFF );*/
}
// End unit_Ground::Render()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Ground.cpp //
//////////////////////////