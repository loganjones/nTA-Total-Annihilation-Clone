// object_Manager.inl //                   Author: Logan "Burn" Jones
///////////////////////                               Date: 2/12/2002
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::MakeMover() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 4/17/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
void object_Manager::MakeMover( game_Object* pObject )
{
	m_MoversAndShakers[pObject] = 0;
}
// End object_Manager::MakeMover()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::UnmakeMover() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 4/17/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
void object_Manager::UnmakeMover( game_Object* pObject )
{
	m_MoversAndShakers.erase( pObject );
}
// End object_Manager::UnmakeMover()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::SceneListInsert() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 5/1/2002
//               
//====================================================================
// Parameters:
//  const game_Object* pObject - 
//
// Return: void - 
//
void object_Manager::SceneListInsert( const game_Object* pObject )
{
	SceneList_t::iterator		it = m_SceneList.begin();
	SceneList_t::const_iterator	end= m_SceneList.end();
	for( ; it!=end; ++it)
		if( pObject->m_SceneLevel<(*it)->m_SceneLevel ||
			pObject->m_BaseRect.top<(*it)->m_BaseRect.top )
			break;
	m_SceneList.insert( it, pObject );
//	m_SceneList.push_back( pObject );
}
// End object_Manager::SceneListInsert()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::ClearRenderList() //     Author: Logan "Burn" Jones
//////////////////////////////////////                 Date: 2/12/2002
//               
//====================================================================
//
void object_Manager::ClearRenderList()
{
	m_pRenderList = NULL;
	m_RenderListIndex = 0;
}
// End object_Manager::ClearRenderList()
//////////////////////////////////////////////////////////////////////


inline bool unit_less( game_Object* pA, game_Object* pB )
{
	return (pA->m_BaseRect.bottom < pB->m_BaseRect.bottom);
}


//////////////////////////////////////////////////////////////////////
// object_Manager::AddToRenderList() //     Author: Logan "Burn" Jones
//////////////////////////////////////                 Date: 2/12/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
void object_Manager::AddToRenderList( game_Object* pObject )
{

	if( m_pRenderList==NULL )
	{
		m_RenderListNodeBank[0].pObject = pObject;
		m_RenderListNodeBank[0].Next = NULL;
		m_pRenderList = m_RenderListNodeBank;
		++m_RenderListIndex;
	}
	else if( unit_less(pObject,m_pRenderList->pObject) )
	{
		m_RenderListNodeBank[m_RenderListIndex].pObject = pObject;
		m_RenderListNodeBank[m_RenderListIndex].Next = m_pRenderList;
		m_pRenderList = m_RenderListNodeBank + m_RenderListIndex++;
	}
	else
	{
		RenderNode_t* pPrev = m_pRenderList;
		RenderNode_t* pNode = m_pRenderList->Next;
		for( ; pNode && unit_less(pNode->pObject,pObject); pPrev=pNode, pNode=pNode->Next );
		m_RenderListNodeBank[m_RenderListIndex].pObject = pObject;
		m_RenderListNodeBank[m_RenderListIndex].Next = pNode;
		pPrev->Next = m_RenderListNodeBank + m_RenderListIndex++;
	}
}
// End object_Manager::AddToRenderList()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - object_Manager.inl //
/////////////////////////////
