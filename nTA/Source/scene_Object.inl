// scene_Object.inl //                             \author Logan Jones
/////////////////////                                \date 10/29/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "scene_System.h"

/////////////////////////////////////////////////////////////////////
// scene_Object::GetSceneryType() //               \author Logan Jones
///////////////////////////////////                  \date 10/29/2002
//
//===================================================================
// Return: object_Type_t - 
//
object_Type_t scene_Object::GetSceneryType() const
{
	return m_SceneryType;
}
// End scene_Object::GetSceneryType()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_Object::AttachToScene()   //              \author Logan Jones
////////////////////////////////////                   \date 5/22/2002
//               
//====================================================================
//
void scene_Object::AttachToScene()
{
	m_pSceneSystem->AddToSceneryMap(this);
}
// End scene_Object::AttachToScene()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// scene_Object::UpdateInScene() //                \author Logan Jones
//////////////////////////////////                   \date 10/29/2002
//
//===================================================================
//
void scene_Object::UpdateInScene()
{
	assert( m_SceneryMapHost );

	// Remove from scene map
	m_SceneryMapHost->erase( m_SceneryMapID );
	m_SceneryMapHost = NULL;

	// Add again
	m_pSceneSystem->AddToSceneryMap(this);
}
// End scene_Object::UpdateInScene()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_Object::DetachFromScene()   //            \author Logan Jones
//////////////////////////////////////                 \date 5/22/2002
//               
//====================================================================
//
void scene_Object::DetachFromScene()
{
	assert( m_SceneryMapHost );

	// Remove from scene map
	m_SceneryMapHost->erase( m_SceneryMapID );
	m_SceneryMapHost = NULL;
}
// End scene_Object::DetachFromScene()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - scene_Object.inl //
///////////////////////////