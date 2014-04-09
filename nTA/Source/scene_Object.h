// scene_Object.h //                               \author Logan Jones
/////////////////////                                 \date 5/10/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
/////////////////////////////////////////////////////////////////////


enum SceneryID_t;

/////////////////////////// scene_Object ///////////////////////////
//
class scene_Object
{
	friend class game_Main;
	friend class scene_System;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Returns the objects derived type
	INLINE object_Type_t GetSceneryType() const;


/////////////////////////////////////////////////
// System methods called by derivations or the system itself
protected:

	// Constructor called by derived objects
	scene_Object( const object_Type_t eType ):m_SceneryType( eType ),
		m_SceneryMapHost( NULL ),
		m_IsAnimator( false ){}

	// The action
	virtual void Animate() = 0;
	virtual void Render() = 0;

	INLINE void AttachToScene();
	INLINE void UpdateInScene();
	INLINE void DetachFromScene();

	void MakeAnimator( const bool bAnimate=true );


/////////////////////////////////////////////////
// Data members
protected:

	std_RectF						m_VisibleRect;
	long							m_SceneSortKey;

	// Runtime type information
	const object_Type_t				m_SceneryType;

	// Pointer to the controlling system object
	static class scene_System*		m_pSceneSystem;


/////////////////////////////////////////////////
// Private data
private:

	list<scene_Object*>*			m_SceneryMapHost;
	list<scene_Object*>::iterator	m_SceneryMapID;

	bool							m_IsAnimator;
	list<scene_Object*>::iterator	m_AnimatorID;

}; // End class - scene_Object
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "scene_Object.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_SCENE_OBJECT_H_)
