// physics_Object.cpp //                           \author Logan Jones
///////////////////////                               \date 5/10/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "physics_Object.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "physics_Object.inl"
#endif // defined( _DEBUG )

// Instansiate the static pointer to the physics system
// NOTE: this should be set somewhere by the game
physics_System* physics_Object::m_pPhysicsSystem = NULL;


/////////////////////////////////////////////////////////////////////
// End - physics_Object.cpp  //
//////////////////////////////
