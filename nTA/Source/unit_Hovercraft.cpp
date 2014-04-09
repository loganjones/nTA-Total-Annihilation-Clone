// unit_Hovercraft.cpp //                          \author Logan Jones
////////////////////////                              \date 11/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Hovercraft.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// unit_Hovercraft::OnCreate() //                  \author Logan Jones
////////////////////////////////                     \date 11/10/2002
//
//===================================================================
//
void unit_Hovercraft::OnCreate()
{
	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
}
// End unit_Hovercraft::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Hovercraft::DoMovement() //                \author Logan Jones
//////////////////////////////////                   \date 11/10/2002
//
//===================================================================
// Return: bool - 
//
bool unit_Hovercraft::DoMovement()
{
	return false;
}
// End unit_Hovercraft::DoMovement()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Hovercraft.cpp //
//////////////////////////////