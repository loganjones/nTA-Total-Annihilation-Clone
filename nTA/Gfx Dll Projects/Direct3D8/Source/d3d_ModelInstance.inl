// d3d_ModelInstance.inl //                Author: Logan "Burn" Jones
//////////////////////////                           Date: 02-23-2003
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetupPieceTransform() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 02-23-2003
//
//===================================================================
// Parameters:
//  const std_Vector3& Origin - 
//  const std_Vector3& Offset - 
//  const std_Vector3& sin    - 
//  const std_Vector3& cos    - 
//  D3DXMATRIX& M             - 
//
// Return: D3DXMATRIX* - 
//
D3DXMATRIX* d3d_ModelInstance::SetupPieceTransform( const std_Vector3& Origin, const std_Vector3& Offset, const std_Vector3& sin, const std_Vector3& cos, D3DXMATRIX& M )
{
	M._41=Origin.x-Offset.x; M._42=Origin.z-Offset.z; M._43=Origin.y+Offset.y; M._44=1;
	M._11=cos.y*cos.z; M._12=(sin.y*cos.x)+(sin.x*cos.y*sin.z); M._13=(sin.x*sin.y)-(cos.x*cos.y*sin.z); M._14=0;
	M._21=-sin.y*cos.z; M._22=(cos.x*cos.y)-(sin.x*sin.y*sin.z); M._23=(sin.x*cos.y)+(cos.x*sin.y*sin.z); M._24=0;
	M._31=sin.z; M._32=-sin.x*cos.z; M._33=cos.x*cos.z; M._34=0;
	return &M;
}
// End d3d_ModelInstance::SetupPieceTransform()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_ModelInstance.inl //
////////////////////////////////