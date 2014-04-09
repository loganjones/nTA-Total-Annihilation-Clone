// scene_System.h //                               \author Logan Jones
///////////////////                                   \date 5/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SCENE_SYSTEM_H_
#define _SCENE_SYSTEM_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// scene_System ///////////////////////////
//
class scene_System
{
	friend class scene_Object;

/////////////////////////////////////////////////
// Type declarations
protected:

	typedef list< scene_Object* >		SceneryList_t;
	struct SceneryMapNode_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create();
	void Destroy();

	void Update();
	void Render();

	INLINE scene_Object* TestPoint( const std_Point ptTest );


/////////////////////////////////////////////////
// Utility Methods
protected:

	void AddToSceneryMap( scene_Object* pObject );

	INLINE void SceneListInsert(  scene_Object* pObject );

	static INLINE void AnimateSceneryObject( scene_Object* pObject );
	static INLINE void RenderSceneryObject( scene_Object* pObject );


/////////////////////////////////////////////////
// Data Members
public:

	// Scenery map
	SceneryMapNode_t*	m_SceneryMap;
	std_Size			m_SceneryMapSize;

	// Scene list
	SceneryList_t		m_CurrentScene;

	// Animators
	SceneryList_t		m_Animators;

	std_Rect			m_ViewRect;


/////////////////////////////////////////////////
// Internal structures
protected:

	struct SceneryMapNode_t
	{
		SceneryList_t				Objects;
	//	ObjectPtrList_t::iterator	Start;		// Flats and stuff like that are at the beginning
	//	ObjectPtrList_t::iterator	Ground;		// Ground level stuff, sorted by its y value in screen space
	//	ObjectPtrList_t::iterator	Air;		// Air level stuff
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	scene_System();
	virtual ~scene_System();
/////////////////////////////////////////////////

}; // End class - scene_System
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "scene_System.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SCENE_SYSTEM_H_)
