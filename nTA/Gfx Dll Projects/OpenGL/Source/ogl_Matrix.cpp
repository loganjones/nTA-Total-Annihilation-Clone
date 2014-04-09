// ogl_Matrix.cpp //                       Author: Logan "Burn" Jones
///////////////////                                    Date: 4/2/2002
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "gfx_OpenGL.h"


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::PushMatrix() //              Author: Logan "Burn" Jones
/////////////////////////////                           Date: 4/2/2002
//               
//====================================================================
//
void gfx_OpenGL::PushMatrix()
{
	glPushMatrix();
}
// End gfx_OpenGL::PushMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::PopMatrix() //               Author: Logan "Burn" Jones
////////////////////////////                            Date: 4/2/2002
//               
//====================================================================
//
void gfx_OpenGL::PopMatrix()
{
	glPopMatrix();
}
// End gfx_OpenGL::PopMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::LoadIdentity() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 4/2/2002
//               
//====================================================================
//
void gfx_OpenGL::LoadIdentity()
{
	glLoadIdentity();
}
// End gfx_OpenGL::LoadIdentity()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Translate() //               Author: Logan "Burn" Jones
////////////////////////////                            Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3 vTranslate - 
//
void gfx_OpenGL::Translate( const std_Vector3 vTranslate )
{
	glTranslatef( vTranslate.x, vTranslate.y, vTranslate.z );
}
// End gfx_OpenGL::Translate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Rotate() //                  Author: Logan "Burn" Jones
/////////////////////////                               Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3 vRotate - 
//
void gfx_OpenGL::Rotate( const std_Vector3 vRotate )
{
	glRotatef( vRotate.x, 1,0,0 );
	glRotatef( vRotate.y, 0,1,0 );
	glRotatef( vRotate.z, 0,0,1 );
}
// End gfx_OpenGL::Rotate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RotateX() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_OpenGL::RotateX( const float fRot )
{
	glRotatef( fRot, 1,0,0 );
}
// End gfx_OpenGL::RotateX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RotateY() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_OpenGL::RotateY( const float fRot )
{
	glRotatef( fRot, 0,1,0 );
}
// End gfx_OpenGL::RotateY()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RotateZ() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 4/2/2002
//               
//====================================================================
// Parameters:
//  const float fRot - 
//
void gfx_OpenGL::RotateZ( const float fRot )
{
	glRotatef( fRot, 0,0,1 );
}
// End gfx_OpenGL::RotateZ()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Matrix.cpp //
/////////////////////////
