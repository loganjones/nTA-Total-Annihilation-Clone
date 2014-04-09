// object_Manager.cpp //                   Author: Logan "Burn" Jones
///////////////////////                               Date: 1/29/2002
//
/////////////////////////////////////////////////////////////////////
/*
#include "game.h"
#include "object_Manager.h"
#include "ta_TNT.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "object_Manager.inl"
#endif // defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
object_Manager::object_Manager():
	m_InterfaceMap( NULL )
{}
object_Manager::~object_Manager()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::Initialize() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 2/15/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntFileBuffer - 
//
void object_Manager::Initialize( BYTE* pTntFileBuffer )
{
	LPTA_TNT_HEADER			pHeader;
	LPTA_TNT_EXT_HEADER		pMapHeader;

	// Get the header
	pHeader = (LPTA_TNT_HEADER)pTntFileBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(pTntFileBuffer + sizeof(TA_TNT_HEADER));

	m_WorldSize.Set( pHeader->Width, pHeader->Height );
	m_WorldRect.Set( 0, 0, pHeader->Width * 16, pHeader->Height * 16 );

	m_InterfaceMapSize = (m_WorldSize / 16) + std_Size(1,1);
	m_InterfaceMap = new InterfaceMapNode_t[ *m_InterfaceMapSize ];
}
// End object_Manager::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::Destroy() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 4/11/2002
//               
//====================================================================
//
void object_Manager::Destroy()
{
	SAFE_DELETE_ARRAY( m_InterfaceMap );
}
// End object_Manager::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::InsertObject() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 1/29/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//  BOOL bAddToPathGrid  - 
//
// Return: BOOL - 
//
BOOL object_Manager::InsertObject( game_Object* pObject, BOOL bAddToPathGrid )
{
	// Insert into all maps
	//if( !TestCollsion(pObject) )
		m_CollisionMap.push_back( pObject );

	pObject->SetHeight( theGame.Terrain.GetElevation(pObject->m_BaseRect.Center()) );
	//const std_Point Pos = pObject->m_BaseRect.Position() / 16;
	//pObject->SetHeight( ((BYTE*)(m_HeightMap + (Pos.x + (Pos.y * m_WorldSize.width))))[0] );

	const std_Point Pos = pObject->m_BaseRect.Position() / 16;
	game_Terrain::MapNode* pNode = theGame.Terrain.GetMapNode(Pos);
	const long pitch = theGame.Terrain.GetMapPitch();
	if( bAddToPathGrid && !pObject->IsFlat() )//&& theGame.Terrain.MapPointIsValid(Pos + std_Point(pObject->m_BaseRect.Width()/16 - 1,pObject->m_BaseRect.Height()/16 - 1)) )
		for( long y=0; y<pObject->m_BaseRect.Height()/16; ++y)
		{
			for( long x=0; x<pObject->m_BaseRect.Width()/16; ++x)
				++pNode[0].OccupationCount_Static;
			pNode += pitch;
		}

	// Interface map
	m_InterfaceMap[ GetInterfaceMapIndex(pObject) ].Objects.push_back( pObject );

	return FALSE;
}
// End object_Manager::InsertObject()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::RemoveObject() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 1/29/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
void object_Manager::RemoveObject( game_Object* pObject )
{
	m_CollisionMap.remove( pObject );
	m_InterfaceMap[ GetInterfaceMapIndex(pObject) ].Objects.remove( pObject );
}
// End object_Manager::RemoveObject()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::Update() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 1/29/2002
//               
//====================================================================
//
void object_Manager::Update( std_RectF rctViewPort )
{
	// Update the moving and colliding objects
	MoversMap_t::iterator	it  = m_MoversAndShakers.begin();
	MoversMap_t::iterator	next= it;
	MoversMap_t::iterator	end = m_MoversAndShakers.end();
	for( ; it!=end; it = next)
	{
		game_Object* pObject = (*it).first;
		next = it;
		++next;

		// Have the object update its stuff
		pObject->DoMovement();

		// Check for collisions
		if( CheckCollisions(pObject)==FALSE )
		{
		}

	} // end for( m_MoversAndShakers )

	// Create a new scene list
	m_SceneList.clear();
	{
	std_Rect						View;
	ObjectPtrList_t::iterator		it;
	ObjectPtrList_t::iterator		end;
//	game_Object*					pObject;

	View.Set( rctViewPort.Position() / 256, std_Size(rctViewPort.Size() / 256) );
	if( View.left>0 ) --View.left; if( View.right<(m_InterfaceMapSize.width-1) ) ++View.right;
	if( View.top>0 ) --View.top; if( View.bottom<(m_InterfaceMapSize.height-1) ) ++View.bottom;

	InterfaceMapNode_t* pNode = m_InterfaceMap + (View.left + (View.top * m_InterfaceMapSize.width));
	for( long y=View.top; y<=View.bottom; ++y)
	{
		for( long x=View.left,i=0; x<=View.right; ++x,++i)
		{
			it = pNode[i].Objects.begin();
			end= pNode[i].Objects.end();
			for( ; it!=end; ++it)
			{
				SceneListInsert( *it );
				//pObject = *it;
				//if( rctViewPort.RectInRect( pObject->m_BaseRect - pObject->m_HeightOffset ) )
				//	pObject->Render( rctViewPort.Position() );
			}
		}
		pNode += m_InterfaceMapSize.width;
	}
	}
}
// End object_Manager::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::Render() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 1/29/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctViewPort - 
//
void object_Manager::Render( std_RectF rctViewPort )
{
//	ObjectPtrList_t::iterator		it = m_CollisionMap.begin();
//	ObjectPtrList_t::const_iterator	end = m_CollisionMap.end();
	std_Vector2						Offset( rctViewPort.Position() );
//	game_Object*					pObject;
/*	std_Rect						View;
	ObjectPtrList_t::iterator		it;
	ObjectPtrList_t::iterator		end;
	game_Object*					pObject;

	View.Set( rctViewPort.Position() / 256, std_Size(rctViewPort.Size() / 256) );
	if( View.left>0 ) --View.left; if( View.right<(m_InterfaceMapSize.width-1) ) ++View.right;
	if( View.top>0 ) --View.top; if( View.bottom<(m_InterfaceMapSize.height-1) ) ++View.bottom;

	InterfaceMapNode_t* pNode = m_InterfaceMap + (View.left + (View.top * m_InterfaceMapSize.width));
	for( long y=View.top; y<=View.bottom; ++y)
	{
		for( long x=View.left,i=0; x<=View.right; ++x,++i)
		{
			it = pNode[i].Objects.begin();
			end= pNode[i].Objects.end();
			for( ; it!=end; ++it)
			{
				pObject = *it;
				//if( rctViewPort.RectInRect( pObject->m_BaseRect - pObject->m_HeightOffset ) )
					pObject->Render( rctViewPort.Position() );
			}
		}
		pNode += m_InterfaceMapSize.width;
	}
*
//	ClearRenderList();
//	for( ; it!=end; ++it)
//	{
//		pObject = *it;
//		if( rctViewPort.RectInRect( pObject->m_BaseRect - pObject->m_HeightOffset ) )
//		/*	if( pObject->IsFlat() )* pObject->Render( Offset );
//		///*	else* AddToRenderList( pObject );
//	}

	//for( RenderNode_t* pNode = m_pRenderList; pNode; pNode=pNode->Next )
	//	pNode->pObject->Render( Offset );
	SceneList_t::iterator		it = m_SceneList.begin();
	SceneList_t::const_iterator	end= m_SceneList.end();
	for( ; it!=end; ++it)
		(*it)->Render( Offset );
}
// End object_Manager::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::TestPoint() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 1/12/2002
//               
//====================================================================
// Parameters:
//  const std_Point_t ptTest - 
//
// Return: game_Object* - 
//
game_Object* object_Manager::TestPoint( const std_Point_t ptTest )
{
	ObjectPtrList_t::iterator		it = m_CollisionMap.begin();
	ObjectPtrList_t::const_iterator	end = m_CollisionMap.end();

	for( ; it!=end; ++it)
		if( ((*it)->m_BaseRect  - std_Vector2(0,(*it)->m_Height/2)).PointInRect( ptTest ) )
			return (*it);

	return NULL;
}
// End object_Manager::TestPoint()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::TestCollsion() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 1/30/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
// Return: game_Object* - 
//
game_Object* object_Manager::TestCollsion( game_Object* pObject )
{
	ObjectPtrList_t::iterator		it = m_CollisionMap.begin();
	ObjectPtrList_t::const_iterator	end = m_CollisionMap.end();
/*
	if( pObject->m_BaseRect.left<m_WorldRect.left ) return pObject;
	else if( pObject->m_BaseRect.right>m_WorldRect.right ) return pObject;
	if( pObject->m_BaseRect.top<m_WorldRect.top ) return pObject;
	else if( pObject->m_BaseRect.bottom>m_WorldRect.bottom ) return pObject;
*
	for( ; it!=end; ++it)
		if( pObject!=(*it) && (*it)->CanCollide() && (*it)->m_BaseRect.RectInRect( pObject->m_BaseRect ) )
			return (*it);

	return NULL;
}
// End object_Manager::TestCollsion()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::CheckCollisions() //     Author: Logan "Burn" Jones
//////////////////////////////////////                 Date: 4/17/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
// Return: BOOL - 
//
BOOL object_Manager::CheckCollisions( game_Object* pObject )
{
	RemoveObject( pObject );
	ObjectPtrList_t::iterator		it = m_CollisionMap.begin();
	ObjectPtrList_t::const_iterator	end = m_CollisionMap.end();
	for( ; it!=end; ++it)
	{
		game_Object* pTestee = *it;

		if( pObject!=pTestee && pTestee->CanCollide() && pTestee->m_BaseRect.RectInRect( pObject->m_BaseRect + pObject->m_Velocity ) )
		{
			// Handle the collision
			if( pObject->OnCollision( pTestee ) )
				return TRUE;
		}
	} // end for( units )

	pObject->m_BaseRect += pObject->m_Velocity;
	pObject->m_Velocity.Nullify();
	InsertObject( pObject, FALSE );

	return FALSE;
}
// End object_Manager::CheckCollisions()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// object_Manager::GetInterfaceMapIndex() // Author: Logan "Burn" Jones
///////////////////////////////////////////            Date: 4/11/2002
//               
//====================================================================
// Parameters:
//  game_Object* pObject - 
//
// Return: DWORD - 
//
DWORD object_Manager::GetInterfaceMapIndex( game_Object* pObject )
{
	std_Point Pos( (pObject->m_BaseRect.Position() - pObject->m_HeightOffset) / 256 );

	if( Pos.x<0 ) Pos.x = 0; else if( Pos.x>=m_InterfaceMapSize.width ) Pos.x = m_InterfaceMapSize.width - 1;
	if( Pos.y<0 ) Pos.y = 0; else if( Pos.y>=m_InterfaceMapSize.height ) Pos.y = m_InterfaceMapSize.height - 1;

	return (Pos.x + (Pos.y * m_InterfaceMapSize.width));
}
// End object_Manager::GetInterfaceMapIndex()
//////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// End - object_Manager.cpp //
/////////////////////////////
