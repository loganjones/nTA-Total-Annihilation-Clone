// std_Math.h //                                   \author Logan Jones
///////////////                                       \date 10/4/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_MATH_H_
#define _STD_MATH_H_
/////////////////////////////////////////////////////////////////////


#include <math.h>


// Its pi (in float and double form)
#define fPI				( 3.1415926535897932384626433832795f )
#define dPI				( 3.1415926535897932384626433832795d )
#define ldPI			( 3.1415926535897932384626433832795ld )

// #define a square and cube macro macro
#define sqr( A )		( (A) * (A) )
#define cube( A )		( (A) * (A) * (A) )

// Macro to test if A is a power of two
#define math_IsPowerOfTwo( A )          (  (A!=0)&&(!((A)&(A-1)))  )


// Returns the next power of two from a 32 bit value
inline UINT32 math_NextPowerOfTwo( UINT32 uiNum );

inline DWORD std_NameHash( LPCTSTR strName );

// Various operations
inline std_Vector2 math_Normalize( const std_Vector2& V );
inline std_Vector3 math_Normalize( const std_Vector3& V );
inline std_Vector2 math_Truncate( const std_Vector2& V, const float fMax );
inline std_Vector3 math_Truncate( const std_Vector3& V, const float fMax );
inline float math_Determinant( const std_Vector2& vA, const std_Vector2& vB );

inline float math_IIRFilter( const float fInput, const float fSharpness, float& fHistory );


// Define the inline functions
#include "std_Math.inl"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_MATH_H_)
