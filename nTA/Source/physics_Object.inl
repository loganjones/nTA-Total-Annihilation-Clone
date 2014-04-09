// physics_Object.inl //                           \author Logan Jones
///////////////////////                              \date 10/29/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "physics_System.h"


/////////////////////////////////////////////////////////////////////
// physics_Object::AddToPhysicsSystem() //         \author Logan Jones
/////////////////////////////////////////            \date 01-30-2003
// Initialize the object into the physics system
//===================================================================
// Parameters:
//  bool bStartStatic - Initialize this object as static if true
//
void physics_Object::AddToPhysicsSystem( bool bStartStatic )
{
	m_pPhysicsSystem->Add( this, bStartStatic );
}
// End physics_Object::AddToPhysicsSystem()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_Object::RemoveFromPhysicsSystem() //    \author Logan Jones
//////////////////////////////////////////////       \date 01-30-2003
// Remove the object from the physics system
//===================================================================
//
void physics_Object::RemoveFromPhysicsSystem()
{
	m_pPhysicsSystem->DetachObject( this );
}
// End physics_Object::RemoveFromPhysicsSystem()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_Object::MakeMeStatic() //               \author Logan Jones
///////////////////////////////////                  \date 10/29/2002
// Make this object phyisically static (ie non-moving)
//===================================================================
//
bool physics_Object::MakeMeStatic()
{
	if( m_IsStatic ) return false;
	else m_pPhysicsSystem->MakeStatic( this );
	return true;
}
// End physics_Object::MakeMeStatic()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_Object::MakeMeDynamic() //              \author Logan Jones
////////////////////////////////////                 \date 10/29/2002
// Make this object phyisically dynamic (ie moving)
//===================================================================
//
bool physics_Object::MakeMeDynamic()
{
	if( !m_IsStatic ) return false;
	else m_pPhysicsSystem->MakeDynamic( this );
	return true;
}
// End physics_Object::MakeMeDynamic()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - physics_Object.inl //
/////////////////////////////