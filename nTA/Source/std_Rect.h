// std_Rect.h //                                   \author Logan Jones
///////////////                                       \date 10/3/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_RECT_H_
#define _STD_RECT_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// std_Rect /////////////////////////////
//
struct std_Rect
{
	// The rect data
	long		left;
	long		top;
	long		right;
	long		bottom;

	// Default constructor
	INLINE std_Rect();

	// Copy constructor
	INLINE std_Rect( const std_Rect& rctCopy );

	// Other constructors
	INLINE std_Rect( const long newLeft, const long newTop, const long newRight, const long newBottom );
	INLINE std_Rect( const std_RectF& rctCopy );
#ifdef _WIN32
	INLINE std_Rect( const RECT& rctCopy );
#endif
	INLINE std_Rect( const long newLeft, const long newTop, const std_Size& newSize );
	INLINE std_Rect( const std_Point& newTopLeft, const long newWidth, const long newHeight );
	INLINE std_Rect( const std_Point& newTopLeft, const std_Size& newSize );

    // Casting
#ifdef _WIN32
	INLINE operator RECT();
#endif

	// Unary Operators
	INLINE std_Rect operator +() const;
	INLINE std_Rect operator -() const;

	// Binary Operators
	INLINE std_Rect operator +( const std_Rect& rect ) const;
	INLINE std_Rect operator -( const std_Rect& rect ) const;
	INLINE std_Rect operator +( const std_Point& pt ) const;
	INLINE std_Rect operator -( const std_Point& pt ) const;
	INLINE std_Rect operator +( const std_Size& s ) const;
	INLINE std_Rect operator -( const std_Size& s ) const;
	INLINE std_Rect operator <<( const long s ) const;
	INLINE std_Rect operator >>( const long s ) const;
	INLINE std_Rect operator *( const long s ) const;
	INLINE std_Rect operator /( const long s ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_Rect& rect ) const;
	INLINE BOOL operator !=( const std_Rect& rect ) const;

	// Assignment Operators
	INLINE std_Rect& operator +=( const std_Rect& rect );
	INLINE std_Rect& operator -=( const std_Rect& rect );
	INLINE std_Rect& operator +=( const std_Point& pt );
	INLINE std_Rect& operator -=( const std_Point& pt );
	INLINE std_Rect& operator +=( const std_Size& s );
	INLINE std_Rect& operator -=( const std_Size& s );
	INLINE std_Rect& operator *=( const long s );
	INLINE std_Rect& operator /=( const long s );

	// Operations
	INLINE void Set( const long newLeft, const long newTop, const long newRight, const long newBottom );
	INLINE void Set( const long newLeft, const long newTop, const std_Size& newSize );
	INLINE void Set( const std_Point& newTopLeft, const long newWidth, const long newHeight );
	INLINE void Set( const std_Point& newTopLeft, const std_Size& newSize );
	INLINE void SetX( const long newLeft, const long newTop, const std_Size& newSize );
	INLINE void SetX( const std_Point& newTopLeft, const std_Size& newSize );
	INLINE void Shift( const long shiftHorizontal, const long shiftVertical );

	INLINE void SetPosition( const long newX, const long newY );
	INLINE void SetPosition( const std_Point& newPos );
	INLINE void SetCenter( const long newX, const long newY );
	INLINE void SetCenter( const std_Point& newPos );
	INLINE void SetSize( const long newWidth, const long newHeight );
	INLINE void SetSize( const std_Size& newSize );

	INLINE std_Point Position() const;
	INLINE std_Point Center() const;

	INLINE long Width() const;
	INLINE long Height() const;
	INLINE std_Size Size() const;

	INLINE long WidthX() const;
	INLINE long HeightX() const;
	INLINE std_Size SizeX() const;

	INLINE const bool PointInRect( const std_Point& pt ) const;
	INLINE const bool RectInRect( const std_Rect& rect ) const;

	INLINE void ChangeSpace( const std_Size szFrom, const std_Size szTo );

}; // End struct - std_Rect
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_RECT_H_)
