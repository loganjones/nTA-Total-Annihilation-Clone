// physics_System.inl //                           \author Logan Jones
///////////////////////                               \date 5/30/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_System::Add() //                        \author Logan Jones
//////////////////////////                           \date 10/29/2002
//
//===================================================================
// Parameters:
//  physics_Object* pObject - 
//  bool bStatic            - 
//
void physics_System::Add( physics_Object* pObject, bool bStatic )
{
	pObject->m_IsStatic = bStatic;
	AttachObject( pObject, bStatic ? &m_Statics : m_MoversAndShakers );
}
// End physics_System::Add()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_System::MakeStatic() //                 \author Logan Jones
/////////////////////////////////                    \date 10/29/2002
//
//===================================================================
// Parameters:
//  physics_Object* pObject - 
//
void physics_System::MakeStatic( physics_Object* pObject )
{
	if( pObject->m_IsAttached ) DetachObject( pObject );
	pObject->m_IsStatic = true;
	AttachObject( pObject, &m_Statics );
}
// End physics_System::MakeStatic()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_System::MakeDynamic() //                \author Logan Jones
//////////////////////////////////                   \date 10/29/2002
//
//===================================================================
// Parameters:
//  physics_Object* pObject - 
//
void physics_System::MakeDynamic( physics_Object* pObject )
{
	if( pObject->m_IsAttached ) DetachObject( pObject );
	pObject->m_IsStatic = false;
	AttachObject( pObject, m_MoversAndShakers );
}
// End physics_System::MakeDynamic()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_System::AttachObject() //               \author Logan Jones
///////////////////////////////////                   \date 11/9/2002
//
//===================================================================
// Parameters:
//  physics_Object* pObject - 
//  ObjectList_t* pList     - 
//
void physics_System::AttachObject( physics_Object* pObject, ObjectList_t* pList )
{
	pObject->m_PhysicsID = pList->insert( pList->end(), pObject );
	pObject->m_HostID = pList;
	pObject->m_IsAttached = true;
}
// End physics_System::AttachObject()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// physics_System::DetachObject() //               \author Logan Jones
///////////////////////////////////                   \date 11/9/2002
//
//===================================================================
// Parameters:
//  physics_Object* pObject - 
//
void physics_System::DetachObject( physics_Object* pObject )
{
	pObject->m_HostID->erase( pObject->m_PhysicsID );
	pObject->m_IsAttached = false;
}
// End physics_System::DetachObject()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// physics_System::AxisRegionIntersection() //     \author Logan Jones
/////////////////////////////////////////////          \date 5/30/2002
//               
//====================================================================
// Parameters:
//  const float fA_Low  - 
//  const float fA_High - 
//  const float fB_Low  - 
//  const float fB_High - 
//
// Return: const bool - 
//
const bool physics_System::AxisRegionIntersection( const float fA_Low, const float fA_High, const float fB_Low, const float fB_High )
{
	if( fA_High<fB_Low || fA_Low>fB_High )
		return false;
	else return true;
}
// End physics_System::AxisRegionIntersection()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - physics_System.inl //
/////////////////////////////
