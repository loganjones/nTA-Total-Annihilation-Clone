// std_Types.h //                                  \author Logan Jones
////////////////                                       \date 9/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_
/////////////////////////////////////////////////////////////////////


// Foward declare the structures
struct std_Point;
struct std_Size;
struct std_SizeF;
struct std_Rect;
struct std_RectF;
struct std_Vector2;
struct std_Vector3;

// Include the structure declarations
#include "std_Point.h"
#include "std_Size.h"
#include "std_SizeF.h"
#include "std_Rect.h"
#include "std_RectF.h"
#include "std_Vector2.h"
#include "std_Vector3.h"

// Add some aliases
typedef std_Point			std_Point_t;
typedef std_Size			std_Size_t;
typedef std_Rect			std_Rect_t;
typedef std_Vector2			std_Vertex2;
typedef std_Vector3			std_Vertex3;


/////////////////////////////////////////////////////////////////////
// Comparison structures for STL stuff

// Is A less than B (case insensitive)
struct pchar_less_nocase_t { INLINE bool operator()(const char* strA, const char* strB) const; };

// Is A equal to B (case insensitive)
struct pchar_equal_nocase_t { INLINE bool operator()(const char* strA, const char* strB) const; };

// Is A less than B (case sensitive)
struct pchar_less_case_t { INLINE bool operator()(const char* strA, const char* strB) const; };

// Is A equal to B (case sensitive)
struct pchar_equal_case_t { INLINE bool operator()(const char* strA, const char* strB) const; };

// hash specialization for void*
template<> struct std::hash<LPVOID> {size_t operator()(LPVOID ptr) const { return (size_t)ptr; }};


// Implement inline methods here if this NOT a debug build
#ifndef _DEBUG
 #include "std_Types.inl"
 #include "std_Point.inl"
 #include "std_Size.inl"
 #include "std_SizeF.inl"
 #include "std_Rect.inl"
 #include "std_RectF.inl"
 #include "std_Vector2.inl"
 #include "std_Vector3.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_TYPES_H_)
