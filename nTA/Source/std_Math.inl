// std_Math.inl //                                 \author Logan Jones
/////////////////                                     \date 10/4/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// math_NextPowerOfTwo() //                        \author Logan Jones
//////////////////////////                             \date 10/4/2001
//               
//====================================================================
// Parameters:
//  UINT32 uiNum - 
//
// Return: UINT32 - 
//
UINT32 math_NextPowerOfTwo( UINT32 uiNum )
{
	uiNum -= 1;
	uiNum |= uiNum >> 16;
	uiNum |= uiNum >> 8;
	uiNum |= uiNum >> 4;
	uiNum |= uiNum >> 2;
	uiNum |= uiNum >> 1;
	return uiNum + 1;
}
// End math_NextPowerOfTwo()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_NameHash() //                               \author Logan Jones
///////////////////                                    \date 5/31/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: DWORD - 
//
DWORD std_NameHash( LPCTSTR strName )
{
	DWORD	h;

	for( h=0; *strName; ++strName)
		h = (h << 5) - h + toupper(*strName);

	return h;
}
// End std_NameHash()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_Normalize() //                             \author Logan Jones
/////////////////////                                \date 11/23/2002
//
//===================================================================
// Parameters:
//  const std_Vector2& V - 
//
// Return: std_Vector2 - 
//
std_Vector2 math_Normalize( const std_Vector2& V )
{
	const float factor = 1 / V.Magnitude();
	return std_Vector2( V.x * factor, V.y * factor );
}
// End math_Normalize()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_Normalize() //                             \author Logan Jones
/////////////////////                                \date 11/23/2002
//
//===================================================================
// Parameters:
//  const std_Vector3& V - 
//
// Return: std_Vector3 - 
//
std_Vector3 math_Normalize( const std_Vector3& V )
{
	const float factor = 1 / V.Magnitude();
	return std_Vector3( V.x * factor, V.y * factor, V.z * factor );
}
// End math_Normalize()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_Truncate() //                              \author Logan Jones
////////////////////                                 \date 11/23/2002
//
//===================================================================
// Parameters:
//  const std_Vector2& V - 
//  const float fMax     - 
//
// Return: std_Vector2 - 
//
std_Vector2 math_Truncate( const std_Vector2& V, const float fMax )
{
	float factor = V.MagnitudeSquared();
	if( factor>sqr(fMax) ) {
		factor = fMax / sqrtf(factor);
		return std_Vector2( V.x * factor, V.y * factor );
	} else return V;
}
// End math_Truncate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_Truncate() //                              \author Logan Jones
////////////////////                                 \date 11/23/2002
//
//===================================================================
// Parameters:
//  const std_Vector3& V - 
//  const float fMax     - 
//
// Return: std_Vector3 - 
//
std_Vector3 math_Truncate( const std_Vector3& V, const float fMax )
{
	float factor = V.MagnitudeSquared();
	if( factor>sqr(fMax) ) {
		factor = fMax / sqrtf(factor);
		return std_Vector3( V.x * factor, V.y * factor, V.z * factor );
	} else return V;
}
// End math_Truncate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_Determinant() //                           \author Logan Jones
///////////////////////                               \date 1/21/2003
//
//===================================================================
// Parameters:
//  const std_Vector2& vA - 
//  const std_Vector2& vB - 
//
// Return: float - 
//
float math_Determinant( const std_Vector2& vA, const std_Vector2& vB )
{
	return (vA.x * vB.y) - (vA.y * vB.x);
}
// End math_Determinant()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// math_IIRFilter() //                             \author Logan Jones
/////////////////////                                \date 06-02-2003
//
//===================================================================
// Parameters:
//  const float fInput     - 
//  const float fSharpness - 
//  float& fHistory        - 
//
// Return: float - 
//
float math_IIRFilter( const float fInput, const float fSharpness, float& fHistory )
{
	return fHistory = (fInput * fSharpness) + (fHistory * (1 - fSharpness));
}
// End math_IIRFilter()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Math.inl //
///////////////////////
