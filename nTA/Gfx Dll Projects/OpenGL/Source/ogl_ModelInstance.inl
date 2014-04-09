// ogl_ModelInstance.inl //                Author: Logan "Burn" Jones
//////////////////////////                           Date: 03-07-2003
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetupPieceTransform() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 03-07-2003
//
//===================================================================
// Parameters:
//  const std_Vector3& Origin - 
//  const std_Vector3& Offset - 
//  const std_Vector3& sin    - 
//  const std_Vector3& cos    - 
//  float* M                  - 
//
// Return: const float* - 
//
const float* ogl_ModelInstance::SetupPieceTransform( const std_Vector3& Origin, const std_Vector3& Offset, const std_Vector3& sin, const std_Vector3& cos, float* M )
{
	M[12]=Origin.x-Offset.x; M[13]=Origin.z-Offset.z; M[14]=Origin.y+Offset.y;
	M[0]=cos.y*cos.z; M[1]=(sin.y*cos.x)+(sin.x*cos.y*sin.z); M[2]=(sin.x*sin.y)-(cos.x*cos.y*sin.z);
	M[4]=-sin.y*cos.z; M[5]=(cos.x*cos.y)-(sin.x*sin.y*sin.z); M[6]=(sin.x*cos.y)+(cos.x*sin.y*sin.z);
	M[8]=sin.z; M[9]=-sin.x*cos.z; M[10]=cos.x*cos.z;
	return M;
}
// End ogl_ModelInstance::SetupPieceTransform()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_ModelInstance.inl //
////////////////////////////////