// std_Rect.inl //                                 \author Logan Jones
/////////////////                                     \date 10/3/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
/////////////////////////                              \date 5/10/2002
//               
//====================================================================
//
std_Rect::std_Rect():left(0),top(0),right(0),bottom(0){}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rctCopy - 
//
std_Rect::std_Rect( const std_Rect& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long newLeft   - 
//  const long newTop    - 
//  const long newRight  - 
//  const long newBottom - 
//
std_Rect::std_Rect( const long newLeft, const long newTop, const long newRight, const long newBottom ):
left(newLeft),top(newTop),right(newRight),bottom(newBottom)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
/////////////////////////                              \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_RectF& rctCopy - 
//
std_Rect::std_Rect( const std_RectF& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const RECT& rctCopy - 
//
std_Rect::std_Rect( const RECT& rctCopy ):
left(rctCopy.left),top(rctCopy.top),right(rctCopy.right),bottom(rctCopy.bottom)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                              \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const long newLeft          - 
//  const long newTop           - 
//  const std_Size& newSize - 
//
std_Rect::std_Rect( const long newLeft, const long newTop, const std_Size& newSize ):
left(newLeft),top(newTop),right(left+newSize.width),bottom(top+newSize.height)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                              \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Point& newTopLeft - 
//  const std_Point& newTopLeft -
//  const long newWidth              - 
//  const long newHeight             - 
//
std_Rect::std_Rect( const std_Point& newTopLeft, const long newWidth, const long newHeight ):
left(newTopLeft.x),top(newTopLeft.y),right(left+newWidth),bottom(top+newHeight)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::std_Rect() //                         \author Logan Jones
////////////////////////                              \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Point& newTopLeft - 
//  const std_Size& newSize - 
//
std_Rect::std_Rect( const std_Point& newTopLeft, const std_Size& newSize ):
left(newTopLeft.x),top(newTopLeft.y),right(left+newSize.width),bottom(top+newSize.height)
{}
// End std_Rect::std_Rect()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_Rect::operator RECT() //                    \author Logan Jones
///////////////////////////////                        \date 10/3/2001
//               
//====================================================================
//
std_Rect::operator RECT()
{
	RECT rect;
	SetRect( &rect, left, top, right, bottom );
	return rect;
}
// End std_Rect::operator RECT()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_Rect::operator+() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect std_Rect::operator+() const
{
	return (*this);
}
// End std_Rect::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator-() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect std_Rect::operator-() const
{
	return std_Rect( -left, -top, -right, -bottom );
}
// End std_Rect::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator+() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator+( const std_Rect& rect ) const
{
	return std_Rect( left + rect.left, top + rect.top, right + rect.right, bottom + rect.bottom );
}
// End std_Rect::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator-() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator-( const std_Rect& rect ) const
{
	return std_Rect( left - rect.left, top - rect.top, right - rect.right, bottom - rect.bottom );
}
// End std_Rect::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator+() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator+( const std_Point& pt ) const
{
	return std_Rect( left + pt.x, top + pt.y, right + pt.x, bottom + pt.y );
}
// End operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator-() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator-( const std_Point& pt ) const
{
	return std_Rect( left - pt.x, top - pt.y, right - pt.x, bottom - pt.y );
}
// End operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator+() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Size& S - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator+( const std_Size& S ) const
{
	return std_Rect( left + S.width, top + S.height, right + S.width, bottom + S.height );
}
// End operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator-() //                                  \author Logan Jones
////////////////                                       \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Size& S - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator-( const std_Size& S ) const
{
	return std_Rect( left - S.width, top - S.height, right - S.width, bottom - S.height );
}
// End operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator<<() //                                 \author Logan Jones
/////////////////                                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator<<( const long s ) const
{
	return std_Rect( left - s, top, right - s, bottom );
}
// End operator<<()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// operator>>() //                                 \author Logan Jones
/////////////////                                      \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator>>( const long s ) const
{
	return std_Rect( left + s, top, right + s, bottom );
}
// End operator>>()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect std_Rect::operator *() //              \author Logan Jones
//////////////////////////////////////                 \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
std_Rect std_Rect::operator *( const long s ) const
{
	return std_Rect( left * s, top * s, right * s, bottom * s );
}
// End std_Rect std_Rect::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator/() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Rect - 
//
std_Rect std_Rect::operator/( const long s ) const
{
	return std_Rect( left / s, top / s, right / s, bottom / s );
}
// End std_Rect::operator/()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator==() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: BOOL - 
//
BOOL std_Rect::operator==( const std_Rect& rect ) const
{
	return ( (left == rect.left) && (top == rect.top) && (right == rect.right) && (bottom == rect.bottom) );
}
// End std_Rect::operator==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator!=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: BOOL - 
//
BOOL std_Rect::operator!=( const std_Rect& rect ) const
{
	return ( (left != rect.left) || (top != rect.top) || (right != rect.right) || (bottom != rect.bottom) );
}
// End std_Rect::operator!=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator+=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator+=( const std_Rect& rect )
{
	left += rect.left;
	top += rect.top;
	right += rect.right;
	bottom += rect.bottom;
	return (*this);
}
// End std_Rect::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator-=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator-=( const std_Rect& rect )
{
	left -= rect.left;
	top -= rect.top;
	right -= rect.right;
	bottom -= rect.bottom;
	return (*this);
}
// End std_Rect::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator+=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator+=( const std_Point& pt )
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return (*this);
}
// End std_Rect::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator-=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator-=( const std_Point& pt )
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return (*this);
}
// End std_Rect::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator+=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Size& S - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator+=( const std_Size& S )
{
	left += S.width;
	top += S.height;
	right += S.width;
	bottom += S.height;
	return (*this);
}
// End std_Rect::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator-=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Size& S - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator-=( const std_Size& S )
{
	left -= S.width;
	top -= S.height;
	right -= S.width;
	bottom -= S.height;
	return (*this);
}
// End std_Rect::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator*=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator*=( const long s )
{
	left *= s;
	top *= s;
	right *= s;
	bottom *= s;
	return (*this);
}
// End std_Rect::operator*=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::operator/=() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Rect& - 
//
std_Rect& std_Rect::operator/=( const long s )
{
	left /= s;
	top /= s;
	right /= s;
	bottom /= s;
	return (*this);
}
// End std_Rect::operator/=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Set() //                              \author Logan Jones
/////////////////////                                  \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long newLeft   - 
//  const long newTop    - 
//  const long newRight  - 
//  const long newBottom - 
//
void std_Rect::Set( const long newLeft, const long newTop, const long newRight, const long newBottom )
{
	left = newLeft;
	top = newTop;
	right = newRight;
	bottom = newBottom;
}
// End std_Rect::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Set() //                              \author Logan Jones
/////////////////////                                 \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const long newLeft          - 
//  const long newTop           - 
//  const std_Size& newSize - 
//
void std_Rect::Set( const long newLeft, const long newTop, const std_Size& newSize )
{
	left = newLeft;
	top = newTop;
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_Rect::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Set() //                              \author Logan Jones
/////////////////////                                 \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Point& newTopLeft - 
//  const std_Point& newTopLeft - 
//  const long newWidth              - 
//  const long newHeight             - 
//
void std_Rect::Set( const std_Point& newTopLeft, const long newWidth, const long newHeight )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newWidth;
	bottom = top + newHeight;
}
// End std_Rect::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Set() //                              \author Logan Jones
/////////////////////                                 \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Point& newTopLeft - 
//  const std_Size& newSize - 
//
void std_Rect::Set( const std_Point& newTopLeft, const std_Size& newSize )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_Rect::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetX() //                             \author Logan Jones
//////////////////////                                \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const long newLeft          - 
//  const long newTop           - 
//  const std_Size& newSize - 
//
void std_Rect::SetX( const long newLeft, const long newTop, const std_Size& newSize )
{
	left = newLeft;
	top = newTop;
	right = left + newSize.width - 1;
	bottom = top + newSize.height - 1;
}
// End std_Rect::SetX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetX() //                             \author Logan Jones
//////////////////////                                \date 10/13/2001
//               
//====================================================================
// Parameters:
//  const std_Point newTopLeft - 
//  const std_Size& newSize - 
//
void std_Rect::SetX( const std_Point& newTopLeft, const std_Size& newSize )
{
	left = newTopLeft.x;
	top = newTopLeft.y;
	right = left + newSize.width - 1;
	bottom = top + newSize.height - 1;
}
// End std_Rect::SetX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Shift() //                            \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long shiftHorizontal - 
//  const long shiftVertical   - 
//
void std_Rect::Shift( const long shiftHorizontal, const long shiftVertical )
{
	left += shiftHorizontal;
	top += shiftVertical;
	right += shiftHorizontal;
	bottom += shiftVertical;
}
// End std_Rect::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetPosition() //                      \author Logan Jones
/////////////////////////////                         \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const long newX - 
//  const long newY - 
//
void std_Rect::SetPosition( const long newX, const long newY )
{
	const long	width = right - left;
	const long	height= bottom - top;

	left = newX;
	top = newY;
	right = left + width;
	bottom = top + height;
}
// End std_Rect::SetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetPosition() //                      \author Logan Jones
/////////////////////////////                         \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Point& newPos - 
//
void std_Rect::SetPosition( const std_Point& newPos )
{
	const long	width = right - left;
	const long	height= bottom - top;

	left = newPos.x;
	top = newPos.y;
	right = left + width;
	bottom = top + height;
}
// End std_Rect::SetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetCenter() //                        \author Logan Jones
//////////////////////////                             \date 2/25/2002
//               
//====================================================================
// Parameters:
//  const long newX - 
//  const long newY - 
//
void std_Rect::SetCenter( const long newX, const long newY )
{
	const long	width = right - left;
	const long	height= bottom - top;

	left = newX - (width / 2);
	top = newY - (height / 2);
	right = left + width;
	bottom = top + height;
}
// End std_Rect::SetCenter()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetCenter() //                        \author Logan Jones
//////////////////////////                             \date 2/25/2002
//               
//====================================================================
// Parameters:
//  const std_Point& newPos - 
//
void std_Rect::SetCenter( const std_Point& newPos )
{
	const long	width = right - left;
	const long	height= bottom - top;

	left = newPos.x - (width / 2);
	top = newPos.y - (height / 2);
	right = left + width;
	bottom = top + height;
}
// End std_Rect::SetCenter()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetSize() //                          \author Logan Jones
/////////////////////////                             \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const long newWidth  - 
//  const long newHeight - 
//
void std_Rect::SetSize( const long newWidth, const long newHeight )
{
	right = left + newWidth;
	bottom = top + newHeight;
}
// End std_Rect::SetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SetSize() //                          \author Logan Jones
/////////////////////////                             \date 10/27/2001
//               
//====================================================================
// Parameters:
//  const std_Size& newSize - 
//
void std_Rect::SetSize( const std_Size& newSize )
{
	right = left + newSize.width;
	bottom = top + newSize.height;
}
// End std_Rect::SetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Position() //                         \author Logan Jones
//////////////////////////                            \date 11/18/2001
//               
//====================================================================
// Return: std_Point - 
//
std_Point std_Rect::Position() const
{
	return std_Point( left, top );
}
// End std_Rect::Position()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Center() //                           \author Logan Jones
///////////////////////                                \date 2/25/2002
//               
//====================================================================
// Return: std_Point - 
//
std_Point std_Rect::Center() const
{
	return std_Point( left + (Width() / 2), top + (Height() / 2) );
}
// End std_Rect::Center()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Width() //                            \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Return: long - 
//
long std_Rect::Width() const
{
	return ( right - left );
}
// End std_Rect::Width()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Height() //                           \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Return: long - 
//
long std_Rect::Height() const
{
	return ( bottom - top );
}
// End std_Rect::Height()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::Size() //                             \author Logan Jones
//////////////////////                                 \date 10/3/2001
//               
//====================================================================
// Return: std_Size - 
//
std_Size std_Rect::Size() const
{
	return std_Size( right - left ,bottom - top );
}
// End std_Rect::Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::WidthX() //                           \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Return: long - 
//
long std_Rect::WidthX() const
{
	return ( right - left + 1 );
}
// End std_Rect::WidthX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::HeightX() //                          \author Logan Jones
/////////////////////////                              \date 10/3/2001
//               
//====================================================================
// Return: long - 
//
long std_Rect::HeightX() const
{
	return ( bottom - top + 1 );
}
// End std_Rect::HeightX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::SizeX() //                            \author Logan Jones
///////////////////////                                \date 10/3/2001
//               
//====================================================================
// Return: std_Size - 
//
std_Size std_Rect::SizeX() const
{
	return std_Size( right - left + 1 ,bottom - top + 1 );
}
// End std_Rect::SizeX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::PointInRect() //                      \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: const bool - 
//
const bool std_Rect::PointInRect( const std_Point& pt ) const
{
	if( (pt.x < left) || (pt.x > right) || (pt.y < top) || (pt.y > bottom) )
		return false;
	else return true;
}
// End std_Rect::PointInRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::RectInRect() //                       \author Logan Jones
////////////////////////////                           \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const std_Rect& rect - 
//
// Return: const bool - 
//
const bool std_Rect::RectInRect( const std_Rect& rect ) const
{
	if( (rect.right < left) ||
		(rect.left > right) ||
		(rect.bottom < top) ||
		(rect.top > bottom) )
		return false;
	else return true;
}
// End std_Rect::RectInRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Rect::ChangeSpace() //                      \author Logan Jones
////////////////////////////                           \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_Size szFrom - 
//  const std_Size szTo   - 
//
void std_Rect::ChangeSpace( const std_Size szFrom, const std_Size szTo )
{
	left  = (left   * szTo.width) / szFrom.width;
	right = (right  * szTo.width) / szFrom.width;
	top   = (top    * szTo.height)/ szFrom.height;
	bottom= (bottom * szTo.height)/ szFrom.height;
}
// End std_Rect::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Rect.inl //
///////////////////////
