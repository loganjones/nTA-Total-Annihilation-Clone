// feature_Object.cpp //                           \author Logan Jones
///////////////////////                               \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "feature_Object.h"
#include "gfx.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
feature_Object::feature_Object():
	physics_Object( OBJ_Feature ),
	scene_Object( OBJ_Feature )
{}
feature_Object::~feature_Object()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::Create() //                     \author Logan Jones
/////////////////////////////                          \date 2/15/2002
//               
//====================================================================
// Parameters:
//  std_Point ptPosition              - 
//  struct feature_Type* pFeatureType - 
//
// Return: BOOL - 
//
BOOL feature_Object::Create( std_Point ptPosition, struct feature_Type* pFeatureType )
{
	m_pFeatureType = pFeatureType;
	m_BaseRect.Set( ptPosition, m_pFeatureType->Footprint * 16 );
//	m_ColliderFlags = CF_OnTerrainMap;
//	AttachToSystem( true );
	m_pPhysicsSystem->Add( this );

	m_VisibleRect = m_BaseRect - std_Vector2(0, theGame.Terrain.GetElevation(m_BaseRect.Center()) / 2);
	m_RenderPosition = m_VisibleRect.Center();
	m_SceneSortKey = (m_pFeatureType->Flags & FTF_Blocking) ? m_VisibleRect.top : -5000;
	AttachToScene();

	return TRUE;
}
// End feature_Object::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::Animate() //                    \author Logan Jones
//////////////////////////////                         \date 5/29/2002
//               
//====================================================================
//
void feature_Object::Animate()
{
}
// End feature_Object::Animate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::Render() //                     \author Logan Jones
/////////////////////////////                          \date 2/12/2002
//               
//====================================================================
// Parameters:
//   std_Vector2 vOffset - 
//
void feature_Object::Render( std_Vector2 vOffset ) const
{
	const std_RectF	ScreenRect( m_BaseRect - vOffset/* - m_HeightOffset*/ );
	const std_Vector2 ScreenPos( (ScreenRect - m_pFeatureType->ImageRect.Position()).Center() );

	// Image
//	temp_LastZ = ScreenRect.bottom;
	m_pFeatureType->Image->Render( std_Vector3(ScreenPos/*,ScreenRect.bottom*/) );
}
// End feature_Object::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::Render() //                     \author Logan Jones
/////////////////////////////                          \date 5/29/2002
//               
//====================================================================
//
void feature_Object::Render()
{
	// Footprint
	//gfx->DrawRect( ScreenRect, 0xFF0000FF );

	// Image
	m_pFeatureType->Image->Render( m_RenderPosition );
}
// End feature_Object::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::CanCollide() //                 \author Logan Jones
/////////////////////////////////                      \date 2/19/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL feature_Object::CanCollide()
{
	return m_pFeatureType->Flags & FTF_Blocking;
}
// End feature_Object::CanCollide()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Object::IsFlat() //                     \author Logan Jones
/////////////////////////////                          \date 2/20/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL feature_Object::IsFlat()
{
	return (m_pFeatureType->Height<=4);
}
// End feature_Object::IsFlat()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - feature_Object.cpp //
/////////////////////////////
