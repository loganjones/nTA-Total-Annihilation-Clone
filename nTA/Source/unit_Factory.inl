// unit_Factory.inl //                             \author Logan Jones
/////////////////////                                  \date 3/5/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Factory::GetUnitInterface() //             \author Logan Jones
/////////////////////////////////////                \date 10/17/2002
//
//===================================================================
// Parameters:
//  UINT32 uiUniqueID - 
//
// Return: unit_Interface* - 
//
unit_Interface* unit_Factory::GetUnitInterface( UINT32 uiUniqueID )
{
	UnitMap_t::iterator	unit = m_Units.find( uiUniqueID );
	if( unit==m_Units.end() ) return NULL;
	else return (*unit).second;
}
// End unit_Factory::GetUnitInterface()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetUnitType() //                  \author Logan Jones
////////////////////////////////                       \date 4/20/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strType - 
//
// Return: const unit_Type* - 
//
const unit_Type* unit_Factory::GetUnitType( LPCTSTR strType ) const
{
	UnitTypeMap_t::const_iterator type( m_UnitTypes.find(std_NameHash(strType)) );
	if( type==m_UnitTypes.end() )
		return NULL;
	else return &(*type).second;
}
// End unit_Factory::GetUnitType()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetUnitType() //                  \author Logan Jones
////////////////////////////////                       \date 8/19/2002
// Quick unit type lookup via key value
//====================================================================
// Parameters:
//  DWORD dwKey - Key value associateds with a unit type
//
// Return: const unit_Type* - Pointer to the associated unit type
//
const unit_Type* unit_Factory::GetUnitType( DWORD dwKey ) const
{
	// Assume a type associated with dwKey exists
	assert( m_UnitTypes.find(dwKey)!=m_UnitTypes.end() );

	// Return the type
	return &(*m_UnitTypes.find(dwKey)).second;
}
// End unit_Factory::GetUnitType()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::CheckUnitRect() //                \author Logan Jones
//////////////////////////////////                     \date 4/20/2002
//               
//====================================================================
// Parameters:
//  const unit_Type* pType  - 
//  const std_Point ptWhere - 
//
// Return: bool - 
//
bool unit_Factory::CheckUnitRect( const unit_Type* pType, const std_Point& ptWhere ) const
{
	std_Vector3 Normal;
	return GetTerrainNormal( ptWhere / 16, pType, Normal );
}
// End unit_Factory::CheckUnitRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetPathInProgress() //            \author Logan Jones
//////////////////////////////////////                  \date 3/5/2002
//               
//====================================================================
// Parameters:
//  unit_Object* pObject - 
//
// Return: const PathFind_t* - 
//
unit_Factory::PathFind_t* unit_Factory::GetPathInProgress( unit_Object* pObject )
{
	PathsInProgress_t::iterator	it = m_PathsInPogress.find( pObject );
	if( it==m_PathsInPogress.end() )
		return NULL;
	else return &((*it).second);
}
// End unit_Factory::GetPathInProgress()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::PQ_Push() //                      \author Logan Jones
//////////////////////////////                          \date 3/2/2002
//               
//====================================================================
// Parameters:
//  PathFindOpenList_t& PQ - 
//  SearchNode_t* pNode    - 
//
void unit_Factory::PQ_Push( PathFindOpenList_t& PQ, SearchNode_t* pNode )
{
	PQ.push_back( pNode );
	push_heap( PQ.begin(), PQ.end(), GreaterSearchNode_t() );
}
// End unit_Factory::PQ_Push()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::PQ_Pop() //                       \author Logan Jones
/////////////////////////////                           \date 3/2/2002
//               
//====================================================================
// Parameters:
//  PathFindOpenList_t& PQ - 
//
// Return: unit_Factory::SearchNode_t* - 
//
unit_Factory::SearchNode_t* unit_Factory::PQ_Pop( PathFindOpenList_t& PQ )
{
	SearchNode_t* pNode = PQ.front();
	pop_heap( PQ.begin(), PQ.end(), GreaterSearchNode_t() );
	PQ.pop_back();
	return pNode;
}
// End unit_Factory::PQ_Pop()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::PQ_Update() //                    \author Logan Jones
////////////////////////////////                        \date 3/2/2002
//               
//====================================================================
// Parameters:
//  PathFindOpenList_t& PQ - 
//  SearchNode_t* pNode    - 
//
void unit_Factory::PQ_Update( PathFindOpenList_t& PQ, SearchNode_t* pNode )
{
//	PQ.remove( pNode );
//	PQ_Push( PQ, pNode );
	PathFindOpenList_t::iterator			it = PQ.begin();
	PathFindOpenList_t::const_iterator	end= PQ.end();
	for( ;it!=end; ++it)
		if( (*it)->Location==pNode->Location )
			push_heap( PQ.begin(), it + 1, GreaterSearchNode_t() );
}
// End unit_Factory::PQ_Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::UpdateUnit() //                   \author Logan Jones
///////////////////////////////                         \date 5/7/2002
//               
//====================================================================
// Parameters:
//  unit_Object* pUnit - 
//
void unit_Factory::UpdateUnit( unit_Object* pUnit )
{
	pUnit->Update();
}
// End unit_Factory::UpdateUnit()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Factory.inl //
///////////////////////////
