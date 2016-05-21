// unit_Aircraft.cpp //                            \author Logan Jones
//////////////////////                                \date 11/2/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Aircraft.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// unit_Aircraft::OnCreate() //                    \author Logan Jones
//////////////////////////////                       \date 11/10/2002
//
//===================================================================
//
void unit_Aircraft::OnCreate()
{
	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);

	m_Speed_Y = 0;
	m_MoveState = Idle;
	m_DesiredAltitude = m_GroundLevel = m_Elevation;
	m_Orientation.z = m_Direction.Angle() + fPI/2;
	m_Model->SetOrientation( m_Orientation/* + std_Vector3(0,0,fPI/2)*/ );
}
// End unit_Aircraft::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Aircraft::Locomotion() //                  \author Logan Jones
////////////////////////////////                     \date 05-28-2003
//
//===================================================================
// Parameters:
//  const std_Vector2& vSteering - 
//
void unit_Aircraft::Locomotion( const std_Vector2& vSteering )
{
	m_vSteering = vSteering;

	if( m_MoveState==Flying ) {

		m_SteeringForce = math_Truncate( vSteering, (m_Velocity*vSteering)>0 ? m_pUnitType->Acceleration:m_pUnitType->BrakeRate );
		m_Velocity = math_Truncate( m_Velocity + m_SteeringForce, m_pUnitType->MaxSpeed );
		TurnTowards( m_Direction, math_Normalize(m_Velocity), m_pUnitType->TurnRate );

		m_BaseRect += m_Velocity;

		m_Orientation.z = m_Direction.Angle() + fPI/2;

	//	Bank( m_SteeringForce );
	//	m_Orientation.y = fPI/6.0f;

		m_GroundLevel = theGame.Terrain.GetElevationAboveSeaLevel( m_BaseRect.Center() );
		m_DesiredAltitude = m_GroundLevel + m_pUnitType->CruiseAltitude;

		//if( (Waypoint()-m_BaseRect.Center()).MagnitudeSquared()<=16 )
		//	m_MoveState = Landing,
		//	m_DesiredAltitude = m_GroundLevel,
		//	m_Script->Deactivate(),
		//	m_Velocity.Nullify();
	}

	DoAltitude();
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
	m_SceneSortKey = m_VisibleRect.top;
	UpdateInScene();
	m_Model->SetShadowOffset( (m_Elevation - m_GroundLevel)/2 );
	m_Model->SetOrientation( m_Orientation /*+ std_Vector3(0,0,fPI/2)*/ );
}
// End unit_Aircraft::Locomotion()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Aircraft::DoAltitude() //                  \author Logan Jones
////////////////////////////////                     \date 12/18/2002
//
//===================================================================
//
void unit_Aircraft::DoAltitude()
{
	if( m_DesiredAltitude>m_Elevation )
		m_Elevation += min( m_DesiredAltitude - m_Elevation, 1.0f );
	else m_Elevation -= min( m_Elevation - m_DesiredAltitude, 1.0f );

	if( m_Elevation<m_GroundLevel && m_MoveState==Flying )
		m_Elevation = m_GroundLevel;

	if( m_Elevation==m_DesiredAltitude )
    {
		if( m_MoveState==Landing )
			m_MoveState = Idle,
			MakeMeStatic();
		else if( m_MoveState==TakingOff )
			m_MoveState = Flying;
    }
}
// End unit_Aircraft::DoAltitude()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Aircraft::TurnTowards() //                 \author Logan Jones
/////////////////////////////////                     \date 1/21/2003
//
//===================================================================
// Parameters:
//  std_Vector2& v         - 
//  const std_Vector2& vTo - 
//  const float& fRate     - 
//
void unit_Aircraft::TurnTowards( std_Vector2& v, const std_Vector2& vTo, const float& fRate )
{
	const float sharpness = 0.1f;
	const float max_bank = fPI/6.0f;

	if( (v*vTo)>=cosf(fRate) )
		v = vTo,
		math_IIRFilter( 0, sharpness, m_Orientation.y );
	else if( math_Determinant(v,vTo)>=0 )
		v.Rotate( fRate ),
		math_IIRFilter( -max_bank, sharpness, m_Orientation.y );
	else v.Rotate( -fRate ),
		math_IIRFilter( max_bank, sharpness, m_Orientation.y );
}
// End unit_Aircraft::TurnTowards()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Aircraft::Render() //                      \author Logan Jones
////////////////////////////                          \date 1/18/2003
//
//===================================================================
//
void unit_Aircraft::Render()
{
	//const DWORD Flags = (m_bSelected ? gfx_Model::Selected : 0) |
	//					(m_Elevation < theGame.Terrain.GetSeaLevel() ? gfx_Model::InWater : 0) |
	//					(TRUE ? gfx_Model::SubmergedVisible : 0);

	//m_pUnitType->Model->Render(
	//	m_Script->GetPieces(),
	//	m_VisibleRect.Center(),
	//	m_MoveInfo.GetOrientation() + std_Vector3(0,0,fPI/2),
	//	(m_Elevation - m_GroundLevel)/2,
	//	theGame.Terrain.GetSeaLevel() - m_Elevation,
	//	Flags,
	//	this );
	m_Model->Render( m_VisibleRect.Center(), m_Orientation + std_Vector3(0,0,fPI/2) );

	const std_Vector2	pos = m_VisibleRect.Center();
	gfx->DrawLine( pos, m_Velocity*16 + pos, 0xFFFFFFFF );
	gfx->DrawLine( pos, m_vSteering*16 + pos, 0x00FF00FF );
	gfx->DrawLine( pos, m_SteeringForce*16 + pos, 0xFF0000FF );

	static char str[64];

	sprintf( str, "Orientation (%f, %f, %f)", m_Orientation.x, m_Orientation.y, m_Orientation.z );
	gfx->RenderString( str, std_Vector3(0,16,0) + pos, 0xFFFFFFFF );
}
// End unit_Aircraft::Render()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Aircraft::OnRequestMoveTo() //             \author Logan Jones
/////////////////////////////////////                 \date 1/18/2003
// Proccess some stuff before proccessing the request
//===================================================================
// Parameters:
//  const std_Point& ptWhere - Desired location
//  bool bEnqueue            - Add to order queue?
//
void unit_Aircraft::OnRequestMoveTo( const std_Point& ptWhere, bool bEnqueue )
{
	// If the aircraft is idle, set the state so that the we move only
	// when we rise to the desired altitude
	if( m_MoveState==Idle || m_MoveState==Landing )
		m_MoveState = TakingOff,
		m_DesiredAltitude = m_GroundLevel + m_pUnitType->CruiseAltitude,
		m_Script->Activate();

	// Not idle, set to flying state so th aircraft will start moving.
	// The desired altitude will be reached in flight.
	else m_MoveState = Flying;

	// Call the default to handle the request
	unit_Object::OnRequestMoveTo( ptWhere, bEnqueue );
}
// End unit_Aircraft::OnRequestMoveTo()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Aircraft.cpp //
////////////////////////////