// ogl_ModelPiece.cpp //                   Author: Logan "Burn" Jones
///////////////////////                              Date: 03-07-2003
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_ModelPiece.h"
#include "ogl_Model.h"
#include "ogl_ModelInstance.h"

/////////////////////////////////////////////////////////////////////
// ogl_ModelPiece::Position() //           Author: Logan "Burn" Jones
///////////////////////////////                      Date: 02-22-2003
//
//===================================================================
// Return: std_Vector3 - 
//
std_Vector3 ogl_ModelPiece::Position() const
{
	//static D3DXVECTOR4	TmpVert;
	//D3DXVec4Transform( &TmpVert, &Origin, Model->Orientation() );
	//return std_Vector3( -TmpVert.x, TmpVert.y, TmpVert.z );
	return std_Vector3();
}
// End ogl_ModelPiece::Position()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelPiece::ScreenPosition() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 02-28-2003
//
//===================================================================
// Return: std_Vector2 - 
//
std_Vector2 ogl_ModelPiece::ScreenPosition() const
{
	//static D3DXVECTOR4	TmpVert;
	//D3DXVec4Transform( &TmpVert, &Origin, Model->Orientation() );
	//return std_Vector2( -TmpVert.x, TmpVert.y - 0.5f * TmpVert.z );
	return std_Vector2();
}
// End ogl_ModelPiece::ScreenPosition()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_ModelPiece.cpp //
/////////////////////////////