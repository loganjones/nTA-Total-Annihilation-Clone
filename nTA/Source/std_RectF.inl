// std_RectF.inl //                                \author Logan Jones
/////////////////                                     \date 10/3/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
//
std_RectF::std_RectF():left(0),top(0),right(0),bottom(0){}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rctCopy - 
//
std_RectF::std_RectF( const std_RectF& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float newLeft   - 
//  const float newTop    - 
//  const float newRight  - 
//  const float newBottom - 
//
std_RectF::std_RectF( const float newLeft, const float newTop, const float newRight, const float newBottom ):
left(newLeft),top(newTop),right(newRight),bottom(newBottom)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
///////////////////////////                            \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Rect& rctCopy - 
//
std_RectF::std_RectF( const std_Rect& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const RECT& rctCopy - 
//
std_RectF::std_RectF( const RECT& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                              \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const float newLeft          - 
//  const float newTop           - 
//  const std_SizeF& newSize - 
//
std_RectF::std_RectF( const float newLeft, const float newTop, const std_SizeF& newSize ):
left(newLeft),top(newTop),right(left+newSize.width),bottom(top+newSize.height)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                              \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& newTopLeft - 
//  const std_Vector2& newTopLeft -
//  const float newWidth              - 
//  const float newHeight             - 
//
std_RectF::std_RectF( const std_Vector2& newTopLeft, const float newWidth, const float newHeight ):
left(newTopLeft.x),top(newTopLeft.y),right(left+newWidth),bottom(top+newHeight)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
////////////////////////                              \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& newTopLeft - 
//  const std_SizeF& newSize - 
//
std_RectF::std_RectF( const std_Vector2& newTopLeft, const std_SizeF& newSize ):
left(newTopLeft.x),top(newTopLeft.y),right(left+newSize.width),bottom(top+newSize.height)
{}
// End std_RectF::std_RectF()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// std_RectF::std_RectF() //                       \author Logan Jones
///////////////////////////                           \date 11/9/2002
// Construct a rect large enough to contain rects A and B
//===================================================================
// Parameters:
//  const std_RectF& rctA - 
//  const std_RectF& rctB - 
//
std_RectF::std_RectF( const std_RectF& rctA, const std_RectF& rctB )
{
    if( rctA.left<rctB.left ) {
        left = rctA.left;
		right = rctB.right;
    }
    else {
        left = rctB.left;
		right = rctA.right;
    }
    if( rctA.top<rctB.top ) {
        top = rctA.top;
		bottom = rctB.bottom;
    }
    else {
        top = rctB.top;
		bottom = rctA.bottom;
    }
}
// End std_RectF::std_RectF()
/////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// operator float*() //                            \author Logan Jones
//////////////////                                     \date 10/3/2001
//               
//====================================================================
//
std_RectF::operator float*()
{
	return (&left);
}
// End operator float*()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator const float*() //           \author Logan Jones
///////////////////////////////////                    \date 10/3/2001
//               
//====================================================================
//
std_RectF::operator const float*() const
{
	return (&left);
}
// End std_RectF::operator const float*()
//////////////////////////////////////////////////////////////////////
*/
#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_RectF::operator RECT() //                   \author Logan Jones
///////////////////////////////                        \date 10/3/2001
//               
//====================================================================
//
std_RectF::operator RECT()
{
	RECT rect;
	SetRect( &rect, left, top, right, bottom );
	return rect;
}
// End std_RectF::operator RECT()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_RectF::operator+() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_RectF - 
//
std_RectF std_RectF::operator+() const
{
	return (*this);
}
// End std_RectF::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator-() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_RectF - 
//
std_RectF std_RectF::operator-() const
{
	return std_RectF( -left, -top, -right, -bottom );
}
// End std_RectF::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator+() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator+( const std_RectF& rect ) const
{
	return std_RectF( left + rect.left, top + rect.top, right + rect.right, bottom + rect.bottom );
}
// End std_RectF::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator-() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator-( const std_RectF& rect ) const
{
	return std_RectF( left - rect.left, top - rect.top, right - rect.right, bottom - rect.bottom );
}
// End std_RectF::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator+() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& pt - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator+( const std_Vector2& pt ) const
{
	return std_RectF( left + pt.x, top + pt.y, right + pt.x, bottom + pt.y );
}
// End operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator-() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& pt - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator-( const std_Vector2& pt ) const
{
	return std_RectF( left - pt.x, top - pt.y, right - pt.x, bottom - pt.y );
}
// End operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator+() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& S - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator+( const std_SizeF& S ) const
{
	return std_RectF( left + S.width, top + S.height, right + S.width, bottom + S.height );
}
// End operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator-() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& S - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator-( const std_SizeF& S ) const
{
	return std_RectF( left - S.width, top - S.height, right - S.width, bottom - S.height );
}
// End operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator<<() //                                 \author Logan Jones
/////////////////                                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator<<( const float s ) const
{
	return std_RectF( left - s, top, right - s, bottom );
}
// End operator<<()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator>>() //                                 \author Logan Jones
/////////////////                                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator>>( const float s ) const
{
	return std_RectF( left + s, top, right + s, bottom );
}
// End operator>>()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF std_RectF::operator *() //            \author Logan Jones
//////////////////////////////////////                 \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
std_RectF std_RectF::operator *( const float s ) const
{
	return std_RectF( left * s, top * s, right * s, bottom * s );
}
// End std_RectF std_RectF::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator/() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_RectF - 
//
std_RectF std_RectF::operator/( const float s ) const
{
	return std_RectF( left / s, top / s, right / s, bottom / s );
}
// End std_RectF::operator/()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator==() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: BOOL - 
//
BOOL std_RectF::operator==( const std_RectF& rect ) const
{
	return ( (left == rect.left) && (top == rect.top) && (right == rect.right) && (bottom == rect.bottom) );
}
// End std_RectF::operator==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator!=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: BOOL - 
//
BOOL std_RectF::operator!=( const std_RectF& rect ) const
{
	return ( (left != rect.left) || (top != rect.top) || (right != rect.right) || (bottom != rect.bottom) );
}
// End std_RectF::operator!=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator+=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator+=( const std_RectF& rect )
{
	left += rect.left;
	top += rect.top;
	right += rect.right;
	bottom += rect.bottom;
	return (*this);
}
// End std_RectF::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator-=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator-=( const std_RectF& rect )
{
	left -= rect.left;
	top -= rect.top;
	right -= rect.right;
	bottom -= rect.bottom;
	return (*this);
}
// End std_RectF::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator+=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& pt - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator+=( const std_Vector2& pt )
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return (*this);
}
// End std_RectF::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator-=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& pt - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator-=( const std_Vector2& pt )
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return (*this);
}
// End std_RectF::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator+=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& S - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator+=( const std_SizeF& S )
{
	left += S.width;
	top += S.height;
	right += S.width;
	bottom += S.height;
	return (*this);
}
// End std_RectF::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator-=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& S - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator-=( const std_SizeF& S )
{
	left -= S.width;
	top -= S.height;
	right -= S.width;
	bottom -= S.height;
	return (*this);
}
// End std_RectF::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator*=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator*=( const float s )
{
	left *= s;
	top *= s;
	right *= s;
	bottom *= s;
	return (*this);
}
// End std_RectF::operator*=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::operator/=() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_RectF& - 
//
std_RectF& std_RectF::operator/=( const float s )
{
	left /= s;
	top /= s;
	right /= s;
	bottom /= s;
	return (*this);
}
// End std_RectF::operator/=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Set() //                             \author Logan Jones
/////////////////////                                  \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float newLeft   - 
//  const float newTop    - 
//  const float newRight  - 
//  const float newBottom - 
//
void std_RectF::Set( const float newLeft, const float newTop, const float newRight, const float newBottom )
{
	left = newLeft;
	top = newTop;
	right = newRight;
	bottom = newBottom;
}
// End std_RectF::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Set() //                             \author Logan Jones
/////////////////////                                 \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const float newLeft          - 
//  const float newTop           - 
//  const std_SizeF& newSize - 
//
void std_RectF::Set( const float newLeft, const float newTop, const std_SizeF& newSize )
{
	left = newLeft;
	top = newTop;
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_RectF::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Set() //                             \author Logan Jones
/////////////////////                                 \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& newTopLeft - 
//  const std_Vector2& newTopLeft - 
//  const float newWidth              - 
//  const float newHeight             - 
//
void std_RectF::Set( const std_Vector2& newTopLeft, const float newWidth, const float newHeight )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newWidth;
	bottom = top + newHeight;
}
// End std_RectF::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Set() //                             \author Logan Jones
/////////////////////                                 \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& newTopLeft - 
//  const std_SizeF& newSize - 
//
void std_RectF::Set( const std_Vector2& newTopLeft, const std_SizeF& newSize )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_RectF::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetX() //                            \author Logan Jones
//////////////////////                                \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const float newLeft          - 
//  const float newTop           - 
//  const std_SizeF& newSize - 
//
void std_RectF::SetX( const float newLeft, const float newTop, const std_SizeF& newSize )
{
	left = newLeft;
	top = newTop;
	right = left + newSize.width - 1;
	bottom = top + newSize.height - 1;
}
// End std_RectF::SetX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetX() //                            \author Logan Jones
//////////////////////                                \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2 newTopLeft - 
//  const std_SizeF& newSize - 
//
void std_RectF::SetX( const std_Vector2& newTopLeft, const std_SizeF& newSize )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newSize.width - 1;
	bottom = top + newSize.height - 1;
}
// End std_RectF::SetX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Shift() //                           \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float shiftHorizontal - 
//  const float shiftVertical   - 
//
void std_RectF::Shift( const float shiftHorizontal, const float shiftVertical )
{
	left += shiftHorizontal;
	top += shiftVertical;
	right += shiftHorizontal;
	bottom += shiftVertical;
}
// End std_RectF::Shift()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// std_RectF::Encompass() //                       \author Logan Jones
///////////////////////////                           \date 11/9/2002
//
//===================================================================
// Parameters:
//  const std_RectF& rctA - 
//  const std_RectF& rctB - 
//
void std_RectF::Encompass( const std_RectF& rctA, const std_RectF& rctB )
{
    if( rctA.left<rctB.left ) {
        left = rctA.left;
		right = rctB.right;
    }
    else {
        left = rctB.left;
		right = rctA.right;
    }
    if( rctA.top<rctB.top ) {
        top = rctA.top;
		bottom = rctB.bottom;
    }
    else {
        top = rctB.top;
		bottom = rctA.bottom;
    }
}
// End std_RectF::Encompass()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// std_RectF::Encompass() //                       \author Logan Jones
///////////////////////////                           \date 11/9/2002
//
//===================================================================
// Parameters:
//  const std_RectF& rctB - 
//
void std_RectF::Encompass( const std_RectF& rctB )
{
	if( left<rctB.left )
		right = rctB.right;
	else		
		left = rctB.left;

	if( top<rctB.top )
		bottom = rctB.bottom;
	else		
		top = rctB.top;
}
// End std_RectF::Encompass()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetPosition() //                     \author Logan Jones
/////////////////////////////                         \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const float newX - 
//  const float newY - 
//
void std_RectF::SetPosition( const float newX, const float newY )
{
	const float	width = right - left;
	const float	height= bottom - top;

	left = newX;
	top = newY;
	right = left + width;
	bottom = top + height;
}
// End std_RectF::SetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetPosition() //                     \author Logan Jones
/////////////////////////////                         \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& newPos - 
//
void std_RectF::SetPosition( const std_Vector2& newPos )
{
	const float	width = right - left;
	const float	height= bottom - top;

	left = newPos.x;
	top = newPos.y;
	right = left + width;
	bottom = top + height;
}
// End std_RectF::SetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetCenter() //                       \author Logan Jones
///////////////////////////                            \date 2/25/2002
//               
//====================================================================
// Parameters:
//  const float newX - 
//  const float newY - 
//
void std_RectF::SetCenter( const float newX, const float newY )
{
	const float	width = right - left;
	const float	height= bottom - top;

	left = newX - (width / 2);
	top = newY - (height / 2);
	right = left + width;
	bottom = top + height;
}
// End std_RectF::SetCenter()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetCenter() //                       \author Logan Jones
///////////////////////////                            \date 2/25/2002
//               
//====================================================================
// Parameters:
//  const std_Vector2& newPos - 
//
void std_RectF::SetCenter( const std_Vector2& newPos )
{
	const float	width = right - left;
	const float	height= bottom - top;

	left = newPos.x - (width / 2);
	top = newPos.y - (height / 2);
	right = left + width;
	bottom = top + height;
}
// End std_RectF::SetCenter()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetSize() //                         \author Logan Jones
/////////////////////////                             \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const float newWidth  - 
//  const float newHeight - 
//
void std_RectF::SetSize( const float newWidth, const float newHeight )
{
	right = left + newWidth;
	bottom = top + newHeight;
}
// End std_RectF::SetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SetSize() //                         \author Logan Jones
/////////////////////////                             \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& newSize - 
//
void std_RectF::SetSize( const std_SizeF& newSize )
{
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_RectF::SetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Position() //                        \author Logan Jones
//////////////////////////                            \date 11/18/2001
//               
//====================================================================
// Return: std_Vector2 - 
//
std_Vector2 std_RectF::Position() const
{
	return std_Vector2( left, top );
}
// End std_RectF::Position()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Center() //                          \author Logan Jones
////////////////////////                               \date 2/25/2002
//               
//====================================================================
// Return: std_Vector2 - 
//
std_Vector2 std_RectF::Center() const
{
	return std_Vector2( left + (Width() / 2), top + (Height() / 2) );
}
// End std_RectF::Center()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Width() //                           \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Return: float - 
//
float std_RectF::Width() const
{
	return ( right - left );
}
// End std_RectF::Width()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Height() //                          \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Return: float - 
//
float std_RectF::Height() const
{
	return ( bottom - top );
}
// End std_RectF::Height()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::Size() //                            \author Logan Jones
//////////////////////                                 \date 10/3/2001
//               
//====================================================================
// Return: std_SizeF - 
//
std_SizeF std_RectF::Size() const
{
	return std_SizeF( right - left ,bottom - top );
}
// End std_RectF::Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::WidthX() //                          \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Return: float - 
//
float std_RectF::WidthX() const
{
	return ( right - left + 1 );
}
// End std_RectF::WidthX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::HeightX() //                         \author Logan Jones
/////////////////////////                              \date 10/3/2001
//               
//====================================================================
// Return: float - 
//
float std_RectF::HeightX() const
{
	return ( bottom - top + 1 );
}
// End std_RectF::HeightX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::SizeX() //                           \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Return: std_SizeF - 
//
std_SizeF std_RectF::SizeX() const
{
	return std_SizeF( right - left + 1 ,bottom - top + 1 );
}
// End std_RectF::SizeX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::PointInRect() //                     \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& pt - 
//
// Return: const bool - 
//
const bool std_RectF::PointInRect( const std_Vector2& pt ) const
{
	if( (pt.x < left) || (pt.x >= right) || (pt.y < top) || (pt.y >= bottom) )
		return false;
	else return true;
}
// End std_RectF::PointInRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::RectInRect() //                      \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_RectF& rect - 
//
// Return: const bool - 
//
const bool std_RectF::RectInRect( const std_RectF& rect ) const
{
	if( (rect.right < left) ||
		(rect.left >= right) ||
		(rect.bottom < top) ||
		(rect.top >= bottom) )
		return false;
	else return true;
}
// End std_RectF::RectInRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_RectF::ChangeSpace() //                     \author Logan Jones
/////////////////////////////                          \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_SizeF szFrom - 
//  const std_SizeF szTo   - 
//
void std_RectF::ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo )
{
	left  *= szTo.width / szFrom.width;
	right *= szTo.width / szFrom.width;
	top   *= szTo.height/ szFrom.height;
	bottom*= szTo.height/ szFrom.height;
}
// End std_RectF::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_RectF.inl //
///////////////////////
