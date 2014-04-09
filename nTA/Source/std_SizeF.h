// std_SizeF.h //                                  \author Logan Jones
///////////////                                       \date 9/21/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_SIZEF_H_
#define _STD_SIZEF_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// std_SizeF /////////////////////////////
//
struct std_SizeF
{
	// The size data
	float		width;
	float		height;

	// Default constructor
	INLINE std_SizeF();

	// Copy constructor
	INLINE std_SizeF( const std_SizeF& szCopy );

	// Other constructors
	INLINE std_SizeF( const float newWidth, const float newHeight );
	INLINE std_SizeF( const std_Size& szCopy );
	INLINE std_SizeF( const SIZE szCopy );

	// Casting
	INLINE operator SIZE();

	// Unary Operators
	INLINE std_SizeF operator +() const;
	INLINE std_SizeF operator -() const;
	INLINE float operator *() const;

	// Binary Operators
	INLINE std_SizeF operator +( const std_SizeF& size ) const;
	INLINE std_SizeF operator -( const std_SizeF& size ) const;
	INLINE std_SizeF operator +( const std_Vector2& v ) const;
	INLINE std_SizeF operator -( const std_Vector2& v ) const;
	INLINE std_SizeF operator *( const float s ) const;
	INLINE std_SizeF operator /( const float s ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_SizeF& size ) const;
	INLINE BOOL operator !=( const std_SizeF& size ) const;
	INLINE BOOL operator  <( const std_SizeF& size ) const;
	INLINE BOOL operator <=( const std_SizeF& size ) const;
	INLINE BOOL operator  >( const std_SizeF& size ) const;
	INLINE BOOL operator >=( const std_SizeF& size ) const;

	// Assignment Operators
	INLINE std_SizeF& operator +=( const std_SizeF& size );
	INLINE std_SizeF& operator -=( const std_SizeF& size );
	INLINE std_SizeF& operator +=( const std_Vector2& v );
	INLINE std_SizeF& operator -=( const std_Vector2& v );
	INLINE std_SizeF& operator *=( const float s );
	INLINE std_SizeF& operator /=( const float s );

	// Operations
	INLINE void Set( const float newWidth, const float newHeight );
	INLINE void Shift( const float newWidth, const float newHeight );
	INLINE void ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo );

}; // End struct - std_SizeF
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_SIZEF_H_)
