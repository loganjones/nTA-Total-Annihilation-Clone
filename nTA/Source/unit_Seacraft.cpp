// unit_Seacraft.cpp //                            \author Logan Jones
//////////////////////                                \date 11/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Seacraft.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// unit_Seacraft::OnCreate() //                    \author Logan Jones
//////////////////////////////                       \date 11/10/2002
//
//===================================================================
//
void unit_Seacraft::OnCreate()
{
	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
}
// End unit_Seacraft::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Seacraft::DoMovement() //                  \author Logan Jones
////////////////////////////////                     \date 11/10/2002
//
//===================================================================
// Return: bool - 
//
bool unit_Seacraft::DoMovement()
{
	return false;
}
// End unit_Seacraft::DoMovement()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Seacraft.cpp //
////////////////////////////