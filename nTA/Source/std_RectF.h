// std_RectF.h //                                  \author Logan Jones
///////////////                                       \date 10/3/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_RECTF_H_
#define _STD_RECTF_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// std_RectF /////////////////////////////
//
struct std_RectF
{
	// The rect data
	float		left;
	float		top;
	float		right;
	float		bottom;

	// Default constructor
	INLINE std_RectF();

	// Copy constructor
	INLINE std_RectF( const std_RectF& rctCopy );

	// Other constructors
	INLINE std_RectF( const float newLeft, const float newTop, const float newRight, const float newBottom );
	INLINE std_RectF( const std_Rect& rctCopy );
#ifdef _WIN32
	INLINE std_RectF( const RECT& rctCopy );
#endif
	INLINE std_RectF( const float newLeft, const float newTop, const std_SizeF& newSize );
	INLINE std_RectF( const std_Vector2& newTopLeft, const float newWidth, const float newHeight );
	INLINE std_RectF( const std_Vector2& newTopLeft, const std_SizeF& newSize );

	// Construct a rect large enough to contain rects A and B
	INLINE std_RectF( const std_RectF& rctA, const std_RectF& rctB );

	// Casting
#ifdef _WIN32
	INLINE operator RECT();
#endif

	// Unary Operators
	INLINE std_RectF operator +() const;
	INLINE std_RectF operator -() const;

	// Binary Operators
	INLINE std_RectF operator +( const std_RectF& rect ) const;
	INLINE std_RectF operator -( const std_RectF& rect ) const;
	INLINE std_RectF operator +( const std_Vector2& pt ) const;
	INLINE std_RectF operator -( const std_Vector2& pt ) const;
	INLINE std_RectF operator +( const std_SizeF& s ) const;
	INLINE std_RectF operator -( const std_SizeF& s ) const;
	INLINE std_RectF operator <<( const float s ) const;
	INLINE std_RectF operator >>( const float s ) const;
	INLINE std_RectF operator *( const float s ) const;
	INLINE std_RectF operator /( const float s ) const;

	// Comparison Operators
	INLINE BOOL operator ==( const std_RectF& rect ) const;
	INLINE BOOL operator !=( const std_RectF& rect ) const;

	// Assignment Operators
	INLINE std_RectF& operator +=( const std_RectF& rect );
	INLINE std_RectF& operator -=( const std_RectF& rect );
	INLINE std_RectF& operator +=( const std_Vector2& pt );
	INLINE std_RectF& operator -=( const std_Vector2& pt );
	INLINE std_RectF& operator +=( const std_SizeF& s );
	INLINE std_RectF& operator -=( const std_SizeF& s );
	INLINE std_RectF& operator *=( const float s );
	INLINE std_RectF& operator /=( const float s );

	// Operations
	INLINE void Set( const float newLeft, const float newTop, const float newRight, const float newBottom );
	INLINE void Set( const float newLeft, const float newTop, const std_SizeF& newSize );
	INLINE void Set( const std_Vector2& newTopLeft, const float newWidth, const float newHeight );
	INLINE void Set( const std_Vector2& newTopLeft, const std_SizeF& newSize );
	INLINE void SetX( const float newLeft, const float newTop, const std_SizeF& newSize );
	INLINE void SetX( const std_Vector2& newTopLeft, const std_SizeF& newSize );
	INLINE void Shift( const float shiftHorizontal, const float shiftVertical );

	// Sets the rect object to emcompass both rect A and B
	INLINE void Encompass( const std_RectF& rctA, const std_RectF& rctB );

	// Same as above except that the object is considered as rect A
	INLINE void Encompass( const std_RectF& rctB );

	INLINE void SetPosition( const float newX, const float newY );
	INLINE void SetPosition( const std_Vector2& newPos );
	INLINE void SetCenter( const float newX, const float newY );
	INLINE void SetCenter( const std_Vector2& newPos );
	INLINE void SetSize( const float newWidth, const float newHeight );
	INLINE void SetSize( const std_SizeF& newSize );

	INLINE std_Vector2 Position() const;
	INLINE std_Vector2 Center() const;

	INLINE float Width() const;
	INLINE float Height() const;
	INLINE std_SizeF Size() const;

	INLINE float WidthX() const;
	INLINE float HeightX() const;
	INLINE std_SizeF SizeX() const;

	INLINE const bool PointInRect( const std_Vector2& pt ) const;
	INLINE const bool RectInRect( const std_RectF& rect ) const;

	INLINE void ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo );

}; // End struct - std_RectF
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_RECTF_H_)
