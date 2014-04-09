// unit_Structure.cpp //                           \author Logan Jones
///////////////////////                               \date 11/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Structure.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// unit_Structure::OnCreate() //                   \author Logan Jones
///////////////////////////////                      \date 11/10/2002
//
//===================================================================
//
void unit_Structure::OnCreate()
{
	m_Elevation = theGame.Terrain.GetElevation( m_BaseRect.Center() );
	m_VisibleRect = m_BaseRect - std_Vector2(0, m_Elevation / 2);
	m_Model->SetOrientation( m_Orientation + std_Vector3(0,0,fPI/2) );
}
// End unit_Structure::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Structure.cpp //
/////////////////////////////