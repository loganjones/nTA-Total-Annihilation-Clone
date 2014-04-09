// unit_Requests.cpp //                            \author Logan Jones
//////////////////////                                \date 8/22/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "game.h"


// TODO: Implement proper request sending.
//       For now just route through old command code.


//////////////////////////////////////////////////////////////////////
// unit_Object::Request_Stop() //                  \author Logan Jones
////////////////////////////////                       \date 9/11/2002
//
//====================================================================
//
void unit_Object::Request_Stop()
{
	Stop();
}
// End unit_Object::Request_Stop()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Request_MoveTo() //                \author Logan Jones
//////////////////////////////////                     \date 9/11/2002
//
//====================================================================
// Parameters:
//  const std_Point& ptDestination - 
//  bool bEnqueue                  - 
//
void unit_Object::Request_MoveTo( const std_Point& ptDestination, bool bEnqueue )
{
	MoveTo( ptDestination, bEnqueue );
}
// End unit_Object::Request_MoveTo()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Request_Build() //                 \author Logan Jones
/////////////////////////////////                      \date 9/11/2002
//
//====================================================================
// Parameters:
//  struct unit_Type* pWhat  - 
//  const std_Point& ptWhere - 
//  bool bEnqueue            - 
//
void unit_Object::Request_Build( struct unit_Type* pWhat, const std_Point& ptWhere, bool bEnqueue )
{
	Build( pWhat, ptWhere, bEnqueue );
}
// End unit_Object::Request_Build()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Request_Activate() //              \author Logan Jones
////////////////////////////////////                   \date 9/11/2002
//
//====================================================================
// Parameters:
//  bool bActivate - 
//
void unit_Object::Request_Activate( bool bActivate )
{
	SetActive( bActivate );
}
// End unit_Object::Request_Activate()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Requests.cpp //
////////////////////////////