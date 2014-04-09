// std_Vector2.h //                                \author Logan Jones
//////////////////                                    \date 1/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_VECTOR2_H_
#define _STD_VECTOR2_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// std_Vector2 ////////////////////////////
//
struct std_Vector2
{
	// The point coordinates
	float		x;
	float		y;

	// Default constructor
	INLINE std_Vector2();

	// Copy constructor
	INLINE std_Vector2( const std_Vector2& v );

	// Other constructors
	INLINE std_Vector2( const float fX, const float fY );
	INLINE std_Vector2( const std_Point& pt );
	INLINE std_Vector2( const std_Vector3& v );
	INLINE std_Vector2( const POINT& ptCopy );
	INLINE std_Vector2( const DWORD dwCopy );

	// Unary Operators
	INLINE std_Vector2 operator +() const;
	INLINE std_Vector2 operator -() const;

	// Binary Operators
	INLINE std_Vector2 operator +( const std_Vector2& v ) const;
	INLINE std_Vector2 operator -( const std_Vector2& v ) const;
	INLINE std_Vector2 operator *( const float s ) const;
	INLINE std_Vector2 operator /( const float s ) const;

	// Dot Product
	INLINE float operator *( const std_Vector2& v ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_Vector2& v ) const;
	INLINE BOOL operator !=( const std_Vector2& v ) const;

	// Assignment Operators
	INLINE std_Vector2& operator +=( const std_Vector2& v );
	INLINE std_Vector2& operator -=( const std_Vector2& v );
	INLINE std_Vector2& operator *=( const float s );
	INLINE std_Vector2& operator /=( const float s );

	// Operations
	INLINE const bool IsNull() const;
	INLINE void Nullify();
	INLINE void Set( const float fX, const float fY );
	INLINE void SetPolar( const float fRadians, const float fDist );
	INLINE void Shift( const float fX, const float fY );

	INLINE float Angle() const;
	INLINE float Magnitude() const;
	INLINE float MagnitudeSquared() const;

	INLINE void Truncate( float fCap );
	INLINE void Normalize();
	INLINE void Rotate( const float fRadians );
	INLINE void ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo );

}; // End struct - std_Vector2
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_VECTOR2_H_)
