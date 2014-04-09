// std_Vector3.h //                                \author Logan Jones
//////////////////                                    \date 1/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_VECTOR3_H_
#define _STD_VECTOR3_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// std_Vector3 ////////////////////////////
//
struct std_Vector3
{
	// The point coordinates
	float		x;
	float		y;
	float		z;

	// Default constructor
	INLINE std_Vector3();

	// Copy constructor
	INLINE std_Vector3( const std_Vector3& v );

	// Other constructors
	INLINE std_Vector3( const float fX, const float fY, const float fZ=0 );
	INLINE std_Vector3( const std_Point& pt, const float fZ=0 );
	INLINE std_Vector3( const std_Vector2& v, const float fZ=0 );

	// Unary Operators
	INLINE std_Vector3 operator +() const;
	INLINE std_Vector3 operator -() const;

	// Binary Operators
	INLINE std_Vector3 operator +( const std_Vector3& v ) const;
	INLINE std_Vector3 operator -( const std_Vector3& v ) const;
	INLINE std_Vector3 operator *( const float s ) const;
	INLINE std_Vector3 operator /( const float s ) const;

	// Member access
	INLINE float& operator[]( DWORD dwIndex );

	// Dot Product
	INLINE float operator *( const std_Vector3& v ) const;

	// Cross Product
	INLINE std_Vector3 operator %( const std_Vector3& v ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_Vector3& v ) const;
	INLINE BOOL operator !=( const std_Vector3& v ) const;

	// Assignment Operators
	INLINE std_Vector3& operator +=( const std_Vector3& v );
	INLINE std_Vector3& operator -=( const std_Vector3& v );
	INLINE std_Vector3& operator *=( const float s );
	INLINE std_Vector3& operator /=( const float s );
	INLINE std_Vector3& operator %=( const std_Vector3& v );

	// Operations
	INLINE const bool IsNull() const;
	INLINE void Nullify();
	INLINE void Set( const float fX, const float fY, const float fZ );
	INLINE void Shift( const float fX, const float fY, const float fZ );
	INLINE float Magnitude() const;
	INLINE float MagnitudeSquared() const;
	INLINE void Truncate( float fCap );
	INLINE void Normalize();
	INLINE void ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo );

}; // End struct - std_Vector3
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_VECTOR3_H_)
