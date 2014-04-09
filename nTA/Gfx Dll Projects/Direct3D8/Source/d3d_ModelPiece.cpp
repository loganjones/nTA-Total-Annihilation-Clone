// d3d_ModelPiece.cpp //                   Author: Logan "Burn" Jones
///////////////////////                              Date: 02-22-2003
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_ModelPiece.h"
#include "d3d_Model.h"
#include "d3d_ModelInstance.h"

/////////////////////////////////////////////////////////////////////
// d3d_ModelPiece::Position() //           Author: Logan "Burn" Jones
///////////////////////////////                      Date: 02-22-2003
//
//===================================================================
// Return: std_Vector3 - 
//
std_Vector3 d3d_ModelPiece::Position() const
{
	static D3DXVECTOR4	TmpVert;
	D3DXVec4Transform( &TmpVert, &Origin, Model->Orientation() );
	return std_Vector3( -TmpVert.x, TmpVert.y, TmpVert.z );
}
// End d3d_ModelPiece::Position()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelPiece::ScreenPosition() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 02-28-2003
//
//===================================================================
// Return: std_Vector2 - 
//
std_Vector2 d3d_ModelPiece::ScreenPosition() const
{
	static D3DXVECTOR4	TmpVert;
	D3DXVec4Transform( &TmpVert, &Origin, Model->Orientation() );
	return std_Vector2( -TmpVert.x, TmpVert.y - 0.5f * TmpVert.z );
}
// End d3d_ModelPiece::ScreenPosition()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_ModelPiece.cpp //
/////////////////////////////