// physics_System.h  //                            \author Logan Jones
//////////////////////                                \date 5/22/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _PHYSICS_SYSTEM_H_
#define _PHYSICS_SYSTEM_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// physics_System //////////////////////////
//
class physics_System
{
	friend class game_Main;
	friend class physics_Object;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Creation and destruction of the system
	bool Create();
	void Destroy();

	// Called every frame to update the system
	void OnFrame();

	// Adds a new object to the system
	INLINE void Add( physics_Object* pObject, bool bStatic=true );


/////////////////////////////////////////////////
// Data Members
private:

// TODO: Make real collision data structures

	typedef list< physics_Object* >		ObjectList_t;

	// Collision map
	ObjectList_t			m_Statics;

	// Dynamic list
	ObjectList_t*			m_MoversAndShakers;
	ObjectList_t*			m_pMovers;
	ObjectList_t			m_Movers[2];

	// Recorded Positions
	std_RectF				m_PrevBaseRect;


/////////////////////////////////////////////////
// Utility Methods
protected:

	// Set an objects dynamic state
	INLINE void MakeStatic( physics_Object* pObject );
	INLINE void MakeDynamic( physics_Object* pObject );

	static INLINE void AttachObject( physics_Object* pObject, ObjectList_t* pList );
	static INLINE void DetachObject( physics_Object* pObject );

	static INLINE const bool AxisRegionIntersection( const float fA_Low, const float fA_High, const float fB_Low, const float fB_High );


}; // End class - physics_System
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "physics_System.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_PHYSICS_SYSTEM_H_)
