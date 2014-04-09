// physics_Object.h //                             \author Logan Jones
//////////////////////                                \date 5/10/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// physics_Object ///////////////////////////
//
class physics_Object
{
	friend class game_Main;
	friend class physics_System;

/////////////////////////////////////////////////
// System methods called by derivations or the system itself
protected:

	// Constructor called by derived objects
	physics_Object( const object_Type_t eType ):m_PhysicalType( eType ){}

	// Initialize the object into the physics system
	INLINE void AddToPhysicsSystem( bool bStartStatic=true );

	// Remove the object from the physics system
	INLINE void RemoveFromPhysicsSystem();

	// Request a change of the object's state, return wheather the state was changed
	INLINE bool MakeMeStatic();
	INLINE bool MakeMeDynamic();

	// Returns the objects derived type
	INLINE object_Type_t PhysicalType() const {return m_PhysicalType;}


/////////////////////////////////////////////////
// Derivable events
protected:

	// Do a frame of movement. The return value indicates whether to force
	// a collision event
	virtual bool DoMovement(){return false;}

	// A possible collision was detected by the system.
	// Do a more detailed check and return whether a collsion occured
	virtual bool DetailedStaticCollision( physics_Object* pStatic ){return false;}
	virtual bool DetailedDynamicCollision( physics_Object* pObject ){return false;}

	// Handle a collision event. Returns whether the object is still valid mover
	virtual bool OnCollision( physics_Object* pCollidee ){return true;}


/////////////////////////////////////////////////
// Data members
protected:

	// The physical dimensions of the object as known by the system
	std_RectF						m_BaseRect;
	std_RectF						m_MovementBox;

	std_RectF						m_Collision_Rect;
	float							m_Collision_Low;
	float							m_Collision_High;

	// Runtime type information
	const object_Type_t				m_PhysicalType;

	// Pointer to the controlling system object
	static class physics_System*	m_pPhysicsSystem;


/////////////////////////////////////////////////
// Private data members used by the physics_System
private:

	// Flag maintaining the movement state of the object
	bool							m_IsStatic;
	bool							m_IsAttached;

	// ID used by the system
	list<physics_Object*>::iterator	m_PhysicsID;
	list<physics_Object*>*			m_HostID;


}; // End class - physics_Object
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "physics_Object.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_PHYSICS_OBJECT_H_)
