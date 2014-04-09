// unit_Aircraft.h //                              \author Logan Jones
////////////////////                                  \date 11/2/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_AIRCRAFT_H_
#define _UNIT_AIRCRAFT_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// unit_Aircraft //////////////////////////
//
class unit_Aircraft : public unit_Object
{
	enum MoveStates_t;

/////////////////////////////////////////////////
// Inherited physics_Object methods
protected:

	// Initialize position and movement data
	void OnCreate();

	// Specialized aircraft movement
	//bool DoMovement();

	void Locomotion( const std_Vector2& vSteering );

	void Render();

	void OnRequestMoveTo( const std_Point& ptWhere, bool bEnqueue );


/////////////////////////////////////////////////
// Data members
protected:

	// Current movement state for the aircraft
	// This state controls movement behaviour
	MoveStates_t		m_MoveState;

	//std_Vector3			m_Orientation;
	std_Vector2			m_vOrientation[2];

	float				m_DesiredAltitude;
	float				m_GroundLevel;

	float				m_Speed_XZ;
	float				m_Speed_Y;

	float				m_TurnSpeed;
	float				m_BankSpeed;

	// Current velocity
	std_Vector2					m_SteeringForce;
	std_Vector2					m_vSteering;


/////////////////////////////////////////////////
// Type definitions
private:

	// Potential movement states for m_MoveState
	enum MoveStates_t {
		Idle,
		TakingOff,
		Landing,
		Flying,
	};


/////////////////////////////////////////////////
// Utility
private:

	void DoAltitude();
	void TurnTowards( std_Vector2& v, const std_Vector2& vTo, const float& fRate );


/////////////////////////////////////////////////
// Default Constructor
public:
	unit_Aircraft( unit_Factory& Manager ):unit_Object(Manager){}
/////////////////////////////////////////////////

}; // End class - unit_Aircraft
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_AIRCRAFT_H_)
