// std_Vector.h //                                 \author Logan Jones
/////////////////                                      \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_VECTOR_H_
#define _STD_VECTOR_H_
/////////////////////////////////////////////////////////////////////


template <class T>
struct VECTOR_2
{
	// The point coordinates
	T		x;
	T		y;

	// Copy (and conversion) constructor
	template <class T_in>
	VECTOR_2( const VECTOR_2<T_in>& V );

	// Default constructor
	VECTOR_2( const T newX=0, const T newY=0 );

	// Other constructors
	VECTOR_2( const POINT ptCopy );
	VECTOR_2( const POINT& ptCopy );
	VECTOR_2( const DWORD dwCopy );

	// Casting
	operator T*();
	operator const T*() const;
	operator POINT();

	// Unary Operators
	inline VECTOR_2<T> operator +() const;
	inline VECTOR_2<T> operator -() const;

	// Binary Operators
	inline VECTOR_2<T> operator +( const VECTOR_2<T>& V ) const;
	inline VECTOR_2<T> operator -( const VECTOR_2<T>& V ) const;
	inline VECTOR_2<T> operator *( const T s ) const;
	inline VECTOR_2<T> operator /( const T s ) const;

	// Dot Product
	inline T operator *( const VECTOR_2<T>& V ) const;

	// Comparison Operators
	inline BOOL operator ==( const VECTOR_2<T>& V ) const;
	inline BOOL operator !=( const VECTOR_2<T>& V ) const;

	// Assignment Operators
	inline VECTOR_2<T>& operator =( const VECTOR_2<T>& V );
	inline VECTOR_2<T>& operator +=( const VECTOR_2<T>& V );
	inline VECTOR_2<T>& operator -=( const VECTOR_2<T>& V );
	inline VECTOR_2<T>& operator *=( const T s );
	inline VECTOR_2<T>& operator /=( const T s );

	// Operations
	inline void Set( const T newX, const T newY );
	inline void Shift( const T newX, const T newY );
	inline T Magnitude();
	inline T MagnitudeSquared();
};


template <class T>
struct VECTOR_3
{
	// The point coordinates
	T		x;
	T		y;
	T		z;

	// Copy constructor
	VECTOR_3( const VECTOR_3<T>& V );

	// Default constructor
	VECTOR_3( const T newX=0, const T newY=0, const T newZ=0 );

	// Casting
	operator T*();
	operator const T*() const;

	// Unary Operators
	inline VECTOR_3<T> operator +() const;
	inline VECTOR_3<T> operator -() const;

	// Binary Operators
	inline VECTOR_3<T> operator +( const VECTOR_3<T>& V ) const;
	inline VECTOR_3<T> operator -( const VECTOR_3<T>& V ) const;
	inline VECTOR_3<T> operator *( const T s ) const;
	inline VECTOR_3<T> operator /( const T s ) const;

	// Dot Product
	inline T operator *( const VECTOR_3<T>& V ) const;

	// Cross Product
	inline VECTOR_3<T> operator %( const VECTOR_3<T>& V ) const;

	// Comparison Operators
	inline BOOL operator ==( const VECTOR_2<T>& V ) const;
	inline BOOL operator !=( const VECTOR_2<T>& V ) const;

	// Assignment Operators
	inline VECTOR_3<T>& operator =( const VECTOR_3<T>& V );
	inline VECTOR_3<T>& operator +=( const VECTOR_3<T>& V );
	inline VECTOR_3<T>& operator -=( const VECTOR_3<T>& V );
	inline VECTOR_3<T>& operator *=( const T s );
	inline VECTOR_3<T>& operator /=( const T s );
	inline VECTOR_3<T>& operator %=( const VECTOR_3<T>& V );

	// Operations
	inline void Set( const T newX, const T newY, const T newZ );
	inline void Shift( const T newX, const T newY, const T newZ );
	inline T Magnitude();
	inline T MagnitudeSquared();
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_VECTOR_H_)
