// physics_System.cpp //                           \author Logan Jones
///////////////////////                               \date 5/22/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "object.h"
#include "physics_System.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "physics_System.inl"
#endif // defined( _DEBUG )

//////////////////////////////////////////////////////////////////////
// physics_System::Create()  //                    \author Logan Jones
//////////////////////////////                         \date 5/23/2002
//               
//====================================================================
// Return: bool - 
//
bool physics_System::Create()
{
	m_MoversAndShakers = m_Movers + 0;
	m_pMovers = m_Movers + 1;

	return true;
}
// End physics_System::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// physics_System::Destroy()  //                   \author Logan Jones
///////////////////////////////                        \date 5/23/2002
//               
//====================================================================
//
void physics_System::Destroy()
{
}
// End physics_System::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// physics_System::OnFrame()  //                   \author Logan Jones
///////////////////////////////                        \date 5/23/2002
//               
//====================================================================
//
void physics_System::OnFrame()
{
	physics_Object*					pObject;
	bool							bForceCollision;
	ObjectList_t::const_iterator	statics_begin = m_Statics.begin(),
									statics_end= m_Statics.end(),
									it, end;
	physics_Object*					Collidee = NULL;
	ObjectList_t*					temp;

	// Move all moving objects and check for resulting collisions
	while( !m_MoversAndShakers->empty() )
	{
		// Get the front object
		pObject = m_MoversAndShakers->front();
		m_MoversAndShakers->pop_front();
		pObject->m_IsAttached = false;

		// Move a frame
		bForceCollision = pObject->DoMovement();

		// Test against static objects
		for( it=statics_begin; it!=statics_end; ++it)
			if( pObject->m_MovementBox.RectInRect((*it)->m_MovementBox) )
				if( pObject->DetailedStaticCollision(*it) )
					Collidee = *it;

		// Test against dynamics
		it = m_pMovers->begin(),
		end= m_pMovers->end();
		for( ; it!=end; ++it)
			if( pObject->m_MovementBox.RectInRect((*it)->m_MovementBox) )
				if( pObject->DetailedDynamicCollision(*it) )
					Collidee = *it;

		// Was there a confirmed collision
		if( Collidee || bForceCollision )
        {
			if( pObject->OnCollision(Collidee) )
				Collidee = NULL;
			else {Collidee = NULL; continue;}
        }

		// If we got here, the object is still alive.
		// Add it to the other mover list if it is still
		// a mover
		if( !pObject->m_IsStatic )
			AttachObject( pObject, m_pMovers );

	} // end for( m_MoversAndShakers )

	// Switch the mover list pointers
	temp = m_MoversAndShakers;
	m_MoversAndShakers = m_pMovers;
	m_pMovers = temp;
}
// End physics_System::OnFrame()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - physics_System.cpp  //
//////////////////////////////
