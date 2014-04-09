// feature_Factory.inl //                          \author Logan Jones
////////////////////////                              \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::IsInFeatureList() //           \author Logan Jones
///////////////////////////////////////                \date 2/15/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strSearch - 
//
// Return: INLINE BOOL - 
//
INLINE BOOL feature_Factory::IsInFeatureList( LPCTSTR strSearch )
{
	return m_FeatureTypes.find(std_NameHash(strSearch))!=m_FeatureTypes.end();
}
// End feature_Factory::IsInFeatureList()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::AddToLoadList() //             \author Logan Jones
/////////////////////////////////////                  \date 2/12/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strNew - 
//  int iAddedWhen - 
//
void feature_Factory::AddToLoadList( LPCTSTR strNew, int iAddedWhen )
{
	FeatureToLoad_t		Feature;

	// Check if this feature has already been loaded
	if( IsInFeatureList(strNew) )
		return;

	// Check if the feature is already in the list
	if( m_FeaturesToLoad.find(std_NameHash(strNew))!=m_FeaturesToLoad.end() )
		return;

	Feature.SearchAdded = iAddedWhen;
	strcpy( Feature.Name, strNew );
	m_FeaturesToLoad[std_NameHash(strNew)] = Feature;
	++m_ToLoadCount;
	m_NewFeatureAdded = TRUE;
}
// End feature_Factory::AddToLoadList()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::IsInToLoadList() //            \author Logan Jones
//////////////////////////////////////                 \date 2/12/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strSearch - 
//
// Return: BOOL - 
//
BOOL feature_Factory::IsInToLoadList( LPCTSTR strSearch )
{
	FeatureToLoadList_t::iterator		it = m_FeaturesToLoad.find(std_NameHash(strSearch));

	if( it!=m_FeaturesToLoad.end() )
	{
		assert( stricmp((*it).second.Name,strSearch)==0 );
		m_FeaturesToLoad.erase( it );
		return TRUE;
	}
	else return FALSE;
}
// End feature_Factory::IsInToLoadList()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::GetFeatureType() //            \author Logan Jones
//////////////////////////////////////                 \date 2/13/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strType - 
//
// Return: FeatureType_t* - 
//
feature_Factory::FeatureType_t* feature_Factory::GetFeatureType( LPTSTR strType )
{
	FeatureTypeMap_t::iterator	it = m_FeatureTypes.find(std_NameHash(strType));
	if( it==m_FeatureTypes.end() )
		return NULL;
	else return &(*it).second;
}
// End feature_Factory::GetFeatureType()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - feature_Factory.inl //
//////////////////////////////
