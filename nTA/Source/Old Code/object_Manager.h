// object_Manager.h //                     Author: Logan "Burn" Jones
/////////////////////                                 Date: 1/29/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////// object_Manager ///////////////////////////
//
class object_Manager
{
public:

	typedef list< game_Object* >			ObjectPtrList_t;
	typedef hash_map< game_Object*, DWORD, hash<LPVOID> >	MoversMap_t;

	struct InterfaceMapNode_t;

	typedef list< const game_Object* >		SceneList_t;
	typedef SceneList_t::iterator			SceneListPtr_t;

	struct RenderNode_t
	{
		game_Object*	pObject;
		RenderNode_t*	Next;
	};


/////////////////////////////////////////////////
// INTERFACE
public:

	void Initialize( BYTE* pTntFileBuffer );

	void Destroy();

	// Updates the object system
	void Update( std_RectF rctViewPort );

	void Render( std_RectF rctViewPort );

	BOOL InsertObject( game_Object* pObject, BOOL bAddToPathGrid=TRUE );
	void RemoveObject( game_Object* pObject );

	INLINE void MakeMover( game_Object* pObject );
	INLINE void UnmakeMover( game_Object* pObject );

	game_Object* TestPoint( const std_Point_t ptTest );
	game_Object* TestCollsion( game_Object* pObject );


/////////////////////////////////////////////////
// Utility Methods
protected:

	BOOL CheckCollisions( game_Object* pObject );

	DWORD GetInterfaceMapIndex( game_Object* pObject );

	INLINE void SceneListInsert( const game_Object* pObject );

	INLINE void ClearRenderList();
	INLINE void AddToRenderList( game_Object* pObject );


/////////////////////////////////////////////////
// Data Members
public:

	std_Size					m_WorldSize;
	std_RectF					m_WorldRect;

	// Collision Map
	ObjectPtrList_t				m_CollisionMap;		// TEMP
	// Interface Map
	InterfaceMapNode_t*			m_InterfaceMap;
	std_Size					m_InterfaceMapSize;

	// List containing objects that are moving and can collide
	MoversMap_t					m_MoversAndShakers;

	// Scene list
	SceneList_t					m_SceneList;
	SceneListPtr_t				m_SL_pStart;
	SceneListPtr_t				m_SL_pGround;
	SceneListPtr_t				m_SL_pAir;

	RenderNode_t				m_RenderListNodeBank[8192];
	DWORD						m_RenderListIndex;
	RenderNode_t*				m_pRenderList;


/////////////////////////////////////////////////
// Internal structures
public:

	struct InterfaceMapNode_t
	{
		ObjectPtrList_t				Objects;
		ObjectPtrList_t::iterator	Start;		// Flats and stuff like that are at the beginning
		ObjectPtrList_t::iterator	Ground;		// Ground level stuff, sorted by its y value in screen space
		ObjectPtrList_t::iterator	Air;		// Air level stuff
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	object_Manager();
	virtual ~object_Manager();
/////////////////////////////////////////////////

}; // End class - object_Manager
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "object_Manager.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OBJECT_MANAGER_H_)
