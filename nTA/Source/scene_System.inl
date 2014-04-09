// scene_System.inl //                             \author Logan Jones
/////////////////////                                 \date 5/29/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::TestPoint() //                    \author Logan Jones
//////////////////////////////                         \date 5/29/2002
//               
//====================================================================
// Parameters:
//  const std_Point ptTest - 
//
// Return: scene_Object* - 
//
scene_Object* scene_System::TestPoint( const std_Point ptTest )
{
	SceneryList_t::reverse_iterator	it = m_CurrentScene.rbegin();
	SceneryList_t::reverse_iterator	end= m_CurrentScene.rend();

	for( ; it!=end; ++it)
		if( (*it)->m_VisibleRect.PointInRect(ptTest) )
			return (*it);

	return NULL;
}
// End scene_System::TestPoint()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::SceneListInsert() //              \author Logan Jones
////////////////////////////////////                   \date 5/29/2002
//               
//====================================================================
// Parameters:
//  scene_Object* pObject - 
//
void scene_System::SceneListInsert(  scene_Object* pObject )
{
	SceneryList_t::iterator			it = m_CurrentScene.begin();
	SceneryList_t::const_iterator	end= m_CurrentScene.end();
	for( ; it!=end; ++it)
		if( pObject->m_SceneSortKey<(*it)->m_SceneSortKey )
			break;
	m_CurrentScene.insert( it, pObject );
}
// End scene_System::SceneListInsert()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::AnimateSceneryObject() //         \author Logan Jones
/////////////////////////////////////////              \date 5/29/2002
//               
//====================================================================
// Parameters:
//  scene_Object* pObject - 
//
void scene_System::AnimateSceneryObject( scene_Object* pObject )
{
	pObject->Animate();
}
// End scene_System::AnimateSceneryObject()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::RenderSceneryObject() //          \author Logan Jones
////////////////////////////////////////               \date 5/29/2002
//               
//====================================================================
// Parameters:
//  scene_Object* pObject - 
//
void scene_System::RenderSceneryObject( scene_Object* pObject )
{
	pObject->Render();
}
// End scene_System::RenderSceneryObject()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - scene_System.inl //
///////////////////////////
