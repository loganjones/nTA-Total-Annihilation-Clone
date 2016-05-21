// std_Size.h //                                   \author Logan Jones
///////////////                                       \date 9/21/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_SIZE_H_
#define _STD_SIZE_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// std_Size /////////////////////////////
//
struct std_Size
{
	// The size data
	long		width;
	long		height;

	// Default constructor
	INLINE std_Size();

	// Copy constructor
	INLINE std_Size( const std_Size& szCopy );

	// Other constructors
	INLINE std_Size( const long newWidth, const long newHeight );
	INLINE std_Size( const std_SizeF& szCopy );
#ifdef _WIN32
	INLINE std_Size( const SIZE szCopy );
#endif

	// Casting
#ifdef _WIN32
	INLINE operator SIZE();
#endif

	// Unary Operators
	INLINE std_Size operator +() const;
	INLINE std_Size operator -() const;
	INLINE long operator *() const;

	// Binary Operators
	INLINE std_Size operator +( const std_Size& size ) const;
	INLINE std_Size operator -( const std_Size& size ) const;
	INLINE std_Size operator +( const std_Point& pt ) const;
	INLINE std_Size operator -( const std_Point& pt ) const;
	INLINE std_Size operator *( const long s ) const;
	INLINE std_Size operator /( const long s ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_Size& size ) const;
	INLINE BOOL operator !=( const std_Size& size ) const;
	INLINE BOOL operator  <( const std_Size& size ) const;
	INLINE BOOL operator <=( const std_Size& size ) const;
	INLINE BOOL operator  >( const std_Size& size ) const;
	INLINE BOOL operator >=( const std_Size& size ) const;

	// Assignment Operators
	INLINE std_Size& operator +=( const std_Size& size );
	INLINE std_Size& operator -=( const std_Size& size );
	INLINE std_Size& operator +=( const std_Point& pt );
	INLINE std_Size& operator -=( const std_Point& pt );
	INLINE std_Size& operator *=( const long s );
	INLINE std_Size& operator /=( const long s );

	// Operations
	INLINE void Set( const long newWidth, const long newHeight );
	INLINE void Shift( const long newWidth, const long newHeight );
	INLINE void ChangeSpace( const std_Size szFrom, const std_Size szTo );

}; // End struct - std_Size
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_SIZE_H_)
