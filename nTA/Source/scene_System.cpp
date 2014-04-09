// scene_System.cpp //                             \author Logan Jones
/////////////////////                                 \date 5/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "scene_System.h"
#include "ta_TNT.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "scene_System.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
scene_System::scene_System():
	m_SceneryMap( NULL )
{}
scene_System::~scene_System()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::Create() //                       \author Logan Jones
///////////////////////////                            \date 5/23/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL scene_System::Create()
{
	LPTA_TNT_HEADER	pHeader = (LPTA_TNT_HEADER)theGame.Terrain.GetTntBuffer();

	m_SceneryMapSize.Set( pHeader->Width / 16 + 1, pHeader->Height / 16 + 1 );
	m_SceneryMap = new SceneryMapNode_t[ *m_SceneryMapSize ];

	return TRUE;
}
// End scene_System::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::Destroy() //                      \author Logan Jones
////////////////////////////                           \date 5/23/2002
//               
//====================================================================
//
void scene_System::Destroy()
{
	SAFE_DELETE_ARRAY( m_SceneryMap );
}
// End scene_System::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::Update() //                       \author Logan Jones
///////////////////////////                            \date 5/23/2002
//               
//====================================================================
//
void scene_System::Update()
{
	// Update animators
	for_each( m_Animators.begin(), m_Animators.end(), AnimateSceneryObject );

	// Update scene
	m_CurrentScene.clear();
	m_ViewRect = theGame.Terrain.GetRect();
	std_Rect m_MapRect = m_ViewRect / 256;
	SceneryList_t::iterator		it;
	SceneryList_t::iterator		end;

	SceneryMapNode_t* pNode = m_SceneryMap + (m_MapRect.left + (m_MapRect.top * m_SceneryMapSize.width));
	for( long y=m_MapRect.top; y<=m_MapRect.bottom; ++y)
	{
		for( long x=m_MapRect.left,i=0; x<=m_MapRect.right; ++x,++i)
		{
			it = pNode[i].Objects.begin();
			end= pNode[i].Objects.end();
			for( ; it!=end; ++it)
				SceneListInsert( *it );
		}
		pNode += m_SceneryMapSize.width;
	}
}
// End scene_System::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::Render() //                       \author Logan Jones
///////////////////////////                            \date 5/29/2002
//               
//====================================================================
//
void scene_System::Render()
{
	gfx->PushMatrix();
	gfx->Translate( -m_ViewRect.Position() );

	// Render scene
	for_each( m_CurrentScene.begin(), m_CurrentScene.end(), RenderSceneryObject );

	gfx->PopMatrix();
}
// End scene_System::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// scene_System::AddToSceneryMap() //              \author Logan Jones
////////////////////////////////////                   \date 5/29/2002
//               
//====================================================================
// Parameters:
//  scene_Object* pObject - 
//
void scene_System::AddToSceneryMap( scene_Object* pObject )
{
	std_Point Pos( pObject->m_VisibleRect.Position() / 256 );

	if( Pos.x<0 ) Pos.x = 0; else if( Pos.x>=m_SceneryMapSize.width ) Pos.x = m_SceneryMapSize.width - 1;
	if( Pos.y<0 ) Pos.y = 0; else if( Pos.y>=m_SceneryMapSize.height ) Pos.y = m_SceneryMapSize.height - 1;

	pObject->m_SceneryMapHost = &m_SceneryMap[ Pos.x + (Pos.y * m_SceneryMapSize.width) ].Objects;
	pObject->m_SceneryMapHost->push_front( pObject );
	pObject->m_SceneryMapID = pObject->m_SceneryMapHost->begin();
}
// End scene_System::AddToSceneryMap()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - scene_System.cpp //
///////////////////////////
