// std_Point.h //                                  \author Logan Jones
////////////////                                      \date 1/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_POINT_H_
#define _STD_POINT_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// std_Point /////////////////////////////
//
struct std_Point
{
	// The point coordinates
	long		x;
	long		y;

	// Default constructor
	INLINE std_Point();

	// Copy constructor
	INLINE std_Point( const std_Point& pt );

	// Other constructors
	INLINE std_Point( const long lX, const long lY );
	INLINE std_Point( const std_Vector2& vCopy );
	INLINE std_Point( const std_Vector3& vCopy );
#ifdef _WIN32
	INLINE std_Point( const POINT& ptCopy );
#endif
	INLINE std_Point( const DWORD dwCopy );

	// Casting
#ifdef _WIN32
	INLINE operator POINT();
#endif

	// Unary Operators
	INLINE std_Point operator +() const;
	INLINE std_Point operator -() const;

	// Binary Operators
	INLINE std_Point operator +( const std_Point& pt ) const;
	INLINE std_Point operator -( const std_Point& pt ) const;
	INLINE std_Point operator *( const long s ) const;
	INLINE std_Point operator /( const long s ) const;

	// Dot Product
	INLINE long operator *( const std_Point& pt ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_Point& pt ) const;
	INLINE BOOL operator !=( const std_Point& pt ) const;

	// Assignment Operators
	INLINE std_Point& operator +=( const std_Point& pt );
	INLINE std_Point& operator -=( const std_Point& pt );
	INLINE std_Point& operator *=( const long s );
	INLINE std_Point& operator /=( const long s );

	// Operations
	INLINE const bool IsNull() const;
	INLINE void Nullify();
	INLINE void Set( const long lX, const long lY );
	INLINE void Shift( const long lX, const long lY );
	INLINE float Magnitude();
	INLINE long MagnitudeSquared();
	INLINE void ChangeSpace( const std_Size szFrom, const std_Size szTo );

}; // End struct - std_Point
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_POINT_H_)
