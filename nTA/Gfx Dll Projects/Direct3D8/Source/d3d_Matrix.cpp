// d3d_Matrix.cpp //                       Author: Logan "Burn" Jones
///////////////////                                    Date: 4/2/2002
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "gfx_Direct3D8.h"


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::PushMatrix() //           Author: Logan "Burn" Jones
////////////////////////////////                        Date: 4/2/2002
//               
//====================================================================
//
void gfx_Direct3D8::PushMatrix()
{
	dxPushMatrix();
}
// End gfx_Direct3D8::PushMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::PopMatrix() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 4/2/2002
//               
//====================================================================
//
void gfx_Direct3D8::PopMatrix()
{
	dxPopMatrix();
}
// End gfx_Direct3D8::PopMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::LoadIdentity() //         Author: Logan "Burn" Jones
//////////////////////////////////                      Date: 4/2/2002
//               
//====================================================================
//
void gfx_Direct3D8::LoadIdentity()
{
	dxLoadIdentity();
}
// End gfx_Direct3D8::LoadIdentity()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Translate() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3 vTranslate - 
//
void gfx_Direct3D8::Translate( const std_Vector3 vTranslate )
{
	dxTranslatef( vTranslate );
}
// End gfx_Direct3D8::Translate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Rotate() //               Author: Logan "Burn" Jones
////////////////////////////                            Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3 vRotate - 
//
void gfx_Direct3D8::Rotate( const std_Vector3 vRotate )
{
	dxRotate( vRotate );
}
// End gfx_Direct3D8::Rotate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RotateX() //              Author: Logan "Burn" Jones
/////////////////////////////                           Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_Direct3D8::RotateX( const float fRot )
{
	dxRotateX( fRot );
}
// End gfx_Direct3D8::RotateX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RotateY() //              Author: Logan "Burn" Jones
/////////////////////////////                           Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_Direct3D8::RotateY( const float fRot )
{
	dxRotateY( fRot );
}
// End gfx_Direct3D8::RotateY()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RotateZ() //              Author: Logan "Burn" Jones
/////////////////////////////                           Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_Direct3D8::RotateZ( const float fRot )
{
	dxRotateZ( fRot );
}
// End gfx_Direct3D8::RotateZ()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Matrix.cpp //
/////////////////////////
