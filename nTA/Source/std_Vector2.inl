// std_Vector2.inl //                              \author Logan Jones
////////////////////                                   \date 9/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
///////////////////////////////                        \date 5/10/2002
//               
//====================================================================
//
std_Vector2::std_Vector2():x(0),y(0){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
std_Vector2::std_Vector2( const std_Vector2& v ):x(v.x),y(v.y){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//
std_Vector2::std_Vector2( const float fX, const float fY ):x(fX),y(fY){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
///////////////////////////////                        \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
std_Vector2::std_Vector2( const std_Point& pt ):x(pt.x),y(pt.y){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
///////////////////////////////                        \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
std_Vector2::std_Vector2( const std_Vector3& v ):x(v.x),y(v.y){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const POINT& ptCopy - 
//
std_Vector2::std_Vector2( const POINT& ptCopy ):x(ptCopy.x),y(ptCopy.y){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_Vector2::std_Vector2() //                   \author Logan Jones
////////////////////////////                            \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const DWORD dwCopy - 
//
std_Vector2::std_Vector2( const DWORD dwCopy ):x(LOWORD(dwCopy)),y(HIWORD(dwCopy)){}
// End std_Vector2::std_Vector2()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// std_Vector2::operatorPOINT() //                 \author Logan Jones
/////////////////////////////////                       \date 9/8/2001
//               
//====================================================================
//
std_Vector2::operator POINT()
{
	POINT pt;

	pt.x = x;
	pt.y = y;

	return pt;
}
// End std_Vector2::operatorPOINT()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// std_Vector2::operator +() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator +() const
{
	return (*this);
}
// End std_Vector2::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator -() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator -() const
{
	return std_Vector2( -x, -y );
}
// End std_Vector2::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator +() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& AV - 
//
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator +( const std_Vector2& v ) const
{
	return std_Vector2( x + v.x, y + v.y );
}
// End std_Vector2::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator -() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator -( const std_Vector2& v ) const
{
	return std_Vector2( x - v.x, y - v.y );
}
// End std_Vector2::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator *() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator *( const float s ) const
{
	return std_Vector2( x * s, y * s );
}
// End std_Vector2::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator /() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector2 - 
//
std_Vector2 std_Vector2::operator /( const float s ) const
{
	return std_Vector2( x / s, y / s );
}
// End std_Vector2::operator /()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator *() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: float - 
//
float std_Vector2::operator *( const std_Vector2& v ) const
{
	return ( (x * v.x) + (y * v.y) );
}
// End std_Vector2::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator ==() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: BOOL - 
//
BOOL std_Vector2::operator ==( const std_Vector2& v ) const
{
	return ( (x==v.x) && (y==v.y) );
}
// End std_Vector2::operator ==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator !=() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: BOOL - 
//
BOOL std_Vector2::operator !=( const std_Vector2& v ) const
{
	return ( (x!=v.x) || (y!=v.y) );
}
// End std_Vector2::operator !=()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// std_Vector2::operator =() //                    \author Logan Jones
//////////////////////////////                          \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_Vector2& - 
//
std_Vector2& std_Vector2::operator =( const std_Vector2& v )
{
	x = v.x;
	y = v.y;

	return (*this);
}
// End std_Vector2::operator =()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// std_Vector2::operator +=() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_Vector2& - 
//
std_Vector2& std_Vector2::operator +=( const std_Vector2& v )
{
	x += v.x;
	y += v.y;

	return (*this);
}
// End std_Vector2::operator +=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator -=() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_Vector2& - 
//
std_Vector2& std_Vector2::operator -=( const std_Vector2& v )
{
	x -= v.x;
	y -= v.y;

	return (*this);
}
// End std_Vector2::operator -=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator *=() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector2& - 
//
std_Vector2& std_Vector2::operator *=( const float s )
{
	x *= s;
	y *= s;

	return (*this);
}
// End std_Vector2::operator *=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::operator /=() //                   \author Logan Jones
///////////////////////////////                         \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector2& - 
//
std_Vector2& std_Vector2::operator /=( const float s )
{
	x /= s;
	y /= s;

	return (*this);
}
// End std_Vector2::operator /=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::IsNull() //                        \author Logan Jones
//////////////////////////                             \date 5/10/2002
//               
//====================================================================
// Return: const bool - 
//
const bool std_Vector2::IsNull() const
{
	return (x!=0 || y!=0);
}
// End std_Vector2::IsNull()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Nullify() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
//
void std_Vector2::Nullify()
{
	x = y = 0;
}
// End std_Vector2::Nullify()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Set() //                           \author Logan Jones
///////////////////////                                 \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//
void std_Vector2::Set( const float fX, const float fY )
{
	x = fX;
	y = fY;
}
// End std_Vector2::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::SetPolar() //                      \author Logan Jones
////////////////////////////                           \date 2/27/2002
//               
//====================================================================
// Parameters:
//  const float fRadians - 
//  const float fDist    - 
//
void std_Vector2::SetPolar( const float fRadians, const float fDist )
{
	x = cosf( fRadians ) * fDist;
	y = sinf( fRadians ) * fDist;
}
// End std_Vector2::SetPolar()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Shift() //                         \author Logan Jones
/////////////////////////                               \date 9/8/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//
void std_Vector2::Shift( const float fX, const float fY )
{
	x += fX;
	y += fY;
}
// End std_Vector2::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Angle() //                         \author Logan Jones
/////////////////////////                              \date 2/27/2002
//               
//====================================================================
// Return: float - 
//
float std_Vector2::Angle() const
{
	if( y>=0 ) return acosf( x );
	else return -acosf( x );
}
// End std_Vector2::Angle()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Magnitude() //                     \author Logan Jones
/////////////////////////////                          \date 1/20/2002
//               
//====================================================================
// Return: float - 
//
float std_Vector2::Magnitude() const
{
	return sqrtf( (x * x) + (y * y) );
}
// End std_Vector2::Magnitude()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::MagnitudeSquared() //              \author Logan Jones
////////////////////////////////////                   \date 1/20/2002
//               
//====================================================================
// Return: float - 
//
float std_Vector2::MagnitudeSquared() const
{
	return (x * x) + (y * y);
}
// End std_Vector2::MagnitudeSquared()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// std_Vector2::Truncate() //                      \author Logan Jones
////////////////////////////                         \date 11/17/2002
//
//===================================================================
// Parameters:
//  float fCap - 
//
void std_Vector2::Truncate( float fCap )
{
	float Magnitude = MagnitudeSquared();

	if( Magnitude>(fCap*fCap) ) {
		Magnitude = fCap / sqrtf( Magnitude );
		x *= Magnitude;
		y *= Magnitude;
	}
}
// End std_Vector2::Truncate()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Normalize() //                     \author Logan Jones
/////////////////////////////                          \date 2/20/2002
//               
//====================================================================
//
void std_Vector2::Normalize()
{
	const float Scale = 1.0f / Magnitude();
	x *= Scale;
	y *= Scale;
}
// End std_Vector2::Normalize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::Rotate() //                        \author Logan Jones
//////////////////////////                             \date 2/27/2002
//               
//====================================================================
// Parameters:
//  const float fRadians - 
//
void std_Vector2::Rotate( const float fRadians )
{	
	const float cos = cosf( fRadians );
	const float sin = sinf( fRadians );
	const float temp = x;

	x = (x * cos) + (y * -sin);
	y = (temp * sin) + (y * cos);
}
// End std_Vector2::Rotate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector2::ChangeSpace() //                   \author Logan Jones
///////////////////////////////                        \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_SizeF szFrom - 
//  const std_SizeF szTo   - 
//
void std_Vector2::ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo )
{
	x *= szTo.width / szFrom.width;
	y *= szTo.height/ szFrom.height;
}
// End std_Vector2::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Vector2.inl //
//////////////////////////
