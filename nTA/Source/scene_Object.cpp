// scene_Object.cpp   //                           \author Logan Jones
///////////////////////                               \date 5/10/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "scene_Object.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "scene_Object.inl"
#endif // defined( _DEBUG )

// Instansiate the static pointer to the physics system
// NOTE: this should be set somewhere by the game
scene_System* scene_Object::m_pSceneSystem = NULL;

//////////////////////////////////////////////////////////////////////
// scene_Object::MakeAnimator()   //               \author Logan Jones
///////////////////////////////////                    \date 5/29/2002
//               
//====================================================================
// Parameters:
//  const bool bAnimate - 
//
void scene_Object::MakeAnimator( const bool bAnimate )
{
	if( bAnimate && m_IsAnimator==false )
	{
		theGame.ScenerySystem.m_Animators.push_front( this );
		m_AnimatorID = theGame.ScenerySystem.m_Animators.begin();
		m_IsAnimator = true;
	}
	else if( !bAnimate && m_IsAnimator==true )
	{
		theGame.ScenerySystem.m_Animators.erase( m_AnimatorID );
		m_IsAnimator = false;
	}
}
// End scene_Object::MakeAnimator()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - scene_Object.cpp   //
/////////////////////////////
