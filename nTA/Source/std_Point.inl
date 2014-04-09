// std_Point.inl //                                \author Logan Jones
//////////////////                                    \date 1/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
//
std_Point::std_Point():x(0),y(0){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
std_Point::std_Point( const std_Point& pt ):x(pt.x),y(pt.y){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long lX - 
//  const long lY - 
//
std_Point::std_Point( const long lX, const long lY ):x(lX),y(lY){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
///////////////////////////                            \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Vector2& vCopy - 
//
std_Point::std_Point( const std_Vector2& vCopy ):x(vCopy.x), y(vCopy.y){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
///////////////////////////                            \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3& vCopy - 
//
std_Point::std_Point( const std_Vector3& vCopy ):x(vCopy.x), y(vCopy.y){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const POINT& ptCopy - 
//
std_Point::std_Point( const POINT& ptCopy ):x(ptCopy.x),y(ptCopy.y){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::std_Point() //                       \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const DWORD dwCopy - 
//
std_Point::std_Point( const DWORD dwCopy ):x(LOWORD(dwCopy)),y(HIWORD(dwCopy)){}
// End std_Point::std_Point()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operatorPOINT() //                   \author Logan Jones
/////////////////////////////////                       \date 9/8/2001
//               
//====================================================================
//
std_Point::operator POINT()
{
	POINT pt;

	pt.x = x;
	pt.y = y;

	return pt;
}
// End std_Point::operatorPOINT()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator +() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Return: std_Point - 
//
std_Point std_Point::operator +() const
{
	return (*this);
}
// End std_Point::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator -() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Return: std_Point - 
//
std_Point std_Point::operator -() const
{
	return std_Point( -x, -y );
}
// End std_Point::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator +() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& AV - 
//
// Return: std_Point - 
//
std_Point std_Point::operator +( const std_Point& pt ) const
{
	return std_Point( x + pt.x, y + pt.y );
}
// End std_Point::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator -() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Point - 
//
std_Point std_Point::operator -( const std_Point& pt ) const
{
	return std_Point( x - pt.x, y - pt.y );
}
// End std_Point::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator *() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Point - 
//
std_Point std_Point::operator *( const long s ) const
{
	return std_Point( x * s, y * s );
}
// End std_Point::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator /() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Point - 
//
std_Point std_Point::operator /( const long s ) const
{
	return std_Point( x / s, y / s );
}
// End std_Point::operator /()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator *() //                      \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: long - 
//
long std_Point::operator *( const std_Point& pt ) const
{
	return ( (x * pt.x) + (y * pt.y) );
}
// End std_Point::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator ==() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: BOOL - 
//
BOOL std_Point::operator ==( const std_Point& pt ) const
{
	return ( (x==pt.x) && (y==pt.y) );
}
// End std_Point::operator ==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator !=() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: BOOL - 
//
BOOL std_Point::operator !=( const std_Point& pt ) const
{
	return ( (x!=pt.x) || (y!=pt.y) );
}
// End std_Point::operator !=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator +=() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Point& - 
//
std_Point& std_Point::operator +=( const std_Point& pt )
{
	x += pt.x;
	y += pt.y;

	return (*this);
}
// End std_Point::operator +=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator -=() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Point& - 
//
std_Point& std_Point::operator -=( const std_Point& pt )
{
	x -= pt.x;
	y -= pt.y;

	return (*this);
}
// End std_Point::operator -=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator *=() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Point& - 
//
std_Point& std_Point::operator *=( const long s )
{
	x *= s;
	y *= s;

	return (*this);
}
// End std_Point::operator *=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::operator /=() //                     \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Point& - 
//
std_Point& std_Point::operator /=( const long s )
{
	x /= s;
	y /= s;

	return (*this);
}
// End std_Point::operator /=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::IsNull() //                          \author Logan Jones
////////////////////////                               \date 6/13/2002
//               
//====================================================================
// Return: const bool - 
//
const bool std_Point::IsNull() const
{
	return (x==0 && y==0);
}
// End std_Point::IsNull()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::Nullify() //                         \author Logan Jones
/////////////////////////                              \date 6/13/2002
//               
//====================================================================
//
void std_Point::Nullify()
{
	x = y = 0;
}
// End std_Point::Nullify()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::Set() //                             \author Logan Jones
///////////////////////                                 \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long lX - 
//  const long lY - 
//
void std_Point::Set( const long lX, const long lY )
{
	x = lX;
	y = lY;
}
// End std_Point::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::Shift() //                           \author Logan Jones
/////////////////////////                               \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const long lX - 
//  const long lY - 
//
void std_Point::Shift( const long lX, const long lY )
{
	x += lX;
	y += lY;
}
// End std_Point::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::Magnitude() //                       \author Logan Jones
/////////////////////////////                          \date 1/20/2002
//               
//====================================================================
// Return: float - 
//
float std_Point::Magnitude()
{
	return sqrt( (float)((x * x) + (y * y)) );
}
// End std_Point::Magnitude()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::MagnitudeSquared() //                \author Logan Jones
////////////////////////////////////                   \date 1/20/2002
//               
//====================================================================
// Return: long - 
//
long std_Point::MagnitudeSquared()
{
	return (x * x) + (y * y);
}
// End std_Point::MagnitudeSquared()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Point::ChangeSpace() //                     \author Logan Jones
/////////////////////////////                          \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_Size szFrom - 
//  const std_Size szTo   - 
//
void std_Point::ChangeSpace( const std_Size szFrom, const std_Size szTo )
{
	x = (x * szTo.width) / szFrom.width;
	y = (y * szTo.height)/ szFrom.height;
}
// End std_Point::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Point.inl //
////////////////////////
