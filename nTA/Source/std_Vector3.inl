// std_Vector3.inl //                              \author Logan Jones
////////////////////                                   \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::std_Vector3() //                   \author Logan Jones
///////////////////////////////                        \date 5/10/2002
//               
//====================================================================
//
std_Vector3::std_Vector3():x(0),y(0),z(0){}
// End std_Vector3::std_Vector3()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::std_Vector3() //                   \author Logan Jones
////////////////////////////                            \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//

std_Vector3::std_Vector3( const std_Vector3& v ):x(v.x),y(v.y),z(v.z){}
// End std_Vector3::std_Vector3()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::std_Vector3() //                   \author Logan Jones
////////////////////////////                            \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//  const float fZ - 
//
std_Vector3::std_Vector3( const float fX, const float fY, const float fZ ):x(fX),y(fY),z(fZ){}
// End std_Vector3::std_Vector3()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::std_Vector3() //                   \author Logan Jones
///////////////////////////////                        \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//  const float fZ      - 
//
std_Vector3::std_Vector3( const std_Point& pt, const float fZ ):x(pt.x),y(pt.y),z(fZ){}
// End std_Vector3::std_Vector3()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::std_Vector3() //                   \author Logan Jones
///////////////////////////////                        \date 1/26/2002
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//  const float fZ       - 
//
std_Vector3::std_Vector3( const std_Vector2& v, const float fZ ):x(v.x),y(v.y),z(fZ){}
// End std_Vector3::std_Vector3()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator +() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator +() const
{
	return (*this);
}
// End std_Vector3::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator -() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator -() const
{
	return std_Vector3( -x, -y, -z);
}
// End std_Vector3::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator +() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator +( const std_Vector3& v ) const
{
	return std_Vector3( x + v.x, y + v.y, z + v.z);
}
// End std_Vector3::operator +()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator -() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator -( const std_Vector3& v ) const
{
	return std_Vector3( x - v.x, y - v.y, z - v.z);
}
// End std_Vector3::operator -()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator *() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator *( const float s ) const
{
	return std_Vector3( x * s, y * s, z * s);
}
// End std_Vector3::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator /() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator /( const float s ) const
{
	return std_Vector3( x / s, y / s, z / s);
}
// End std_Vector3::operator /()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator *() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: float - 
//

float std_Vector3::operator *( const std_Vector3& v ) const
{
	return ( (x * v.x) + (y * v.y) +(z * v.z) );
}
// End std_Vector3::operator *()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator %() //                    \author Logan Jones
//////////////////////////////                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3 - 
//

std_Vector3 std_Vector3::operator %( const std_Vector3& v ) const
{
	return std_Vector3( (y*v.z) - (v.y*z), (v.x*z) - (x*v.z), (x*v.y) - (v.x*y) );
}
// End std_Vector3::operator %()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator ==() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: BOOL - 
//

BOOL std_Vector3::operator ==( const std_Vector3& v ) const
{
	return ( (x==v.x) && (y==v.y) && (z==v.z) );
}
// End std_Vector3::operator ==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator !=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: BOOL - 
//

BOOL std_Vector3::operator !=( const std_Vector3& v ) const
{
	return ( (x!=v.x) || (y!=v.y) || (z!=v.z) );
}
// End std_Vector3::operator !=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator +=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3& - 
//

std_Vector3& std_Vector3::operator +=( const std_Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;

	return (*this);
}
// End std_Vector3::operator +=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator -=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3& - 
//

std_Vector3& std_Vector3::operator -=( const std_Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return (*this);
}
// End std_Vector3::operator -=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator *=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector3& - 
//

std_Vector3& std_Vector3::operator *=( const float s )
{
	x *= s;
	y *= s;
	z *= s;

	return (*this);
}
// End std_Vector3::operator *=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator /=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_Vector3& - 
//

std_Vector3& std_Vector3::operator /=( const float s )
{
	x /= s;
	y /= s;
	z /= s;

	return (*this);
}
// End std_Vector3::operator /=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator %=() //                   \author Logan Jones
///////////////////////////////                         \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const std_Vector3& v - 
//
// Return: std_Vector3& - 
//

std_Vector3& std_Vector3::operator %=( const std_Vector3& v )
{
	return ((*this) = ((*this) % v));
}
// End std_Vector3::operator %=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::operator[]() //                    \author Logan Jones
//////////////////////////////                         \date 4/17/2002
//               
//====================================================================
// Parameters:
//  const DWORD dwIndex - 
//
// Return: float& - 
//
float& std_Vector3::operator[]( DWORD dwIndex )
{
	return (&x)[dwIndex];
}
// End std_Vector3::operator[]()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::IsNull() //                        \author Logan Jones
//////////////////////////                             \date 5/10/2002
//               
//====================================================================
// Return: const bool - 
//
const bool std_Vector3::IsNull() const
{
	return (x!=0 || y!=0 || z!=0);
}
// End std_Vector3::IsNull()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::Nullify() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
//
void std_Vector3::Nullify()
{
	x = y = z = 0;
}
// End std_Vector3::Nullify()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::Set() //                           \author Logan Jones
///////////////////////                                 \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//  const float fZ - 
//

void std_Vector3::Set( const float fX, const float fY, const float fZ )
{
	x = fX;
	y = fY;
	z = fZ;
}
// End std_Vector3::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::Shift() //                         \author Logan Jones
/////////////////////////                               \date 9/9/2001
//               
//====================================================================
// Parameters:
//  const float fX - 
//  const float fY - 
//  const float fZ - 
//

void std_Vector3::Shift( const float fX, const float fY, const float fZ )
{
	x += fX;
	y += fY;
	z += fZ;
}
// End std_Vector3::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::Magnitude() //                     \author Logan Jones
/////////////////////////////                          \date 1/20/2002
//               
//====================================================================
// Return: float - 
//

float std_Vector3::Magnitude() const
{
	return sqrtf( (x * x) + (y * y) + (z * z) );
}
// End std_Vector3::Magnitude()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::MagnitudeSquared() //              \author Logan Jones
////////////////////////////////////                   \date 1/20/2002
//               
//====================================================================
// Return: float - 
//

float std_Vector3::MagnitudeSquared() const
{
	return (x * x) + (y * y) + (z * z);
}
// End std_Vector3::MagnitudeSquared()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// std_Vector3::Truncate() //                      \author Logan Jones
////////////////////////////                         \date 11/17/2002
//
//===================================================================
// Parameters:
//  float fCap - 
//
void std_Vector3::Truncate( float fCap )
{
	float Magnitude = MagnitudeSquared();

	if( Magnitude>(fCap*fCap) ) {
		Magnitude = fCap / sqrtf( Magnitude );
		x *= Magnitude;
		y *= Magnitude;
		z *= Magnitude;
	}
}
// End std_Vector3::Truncate()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::Normalize() //                     \author Logan Jones
/////////////////////////////                          \date 2/20/2002
//               
//====================================================================
//
void std_Vector3::Normalize()
{
	const float Scale = 1.0f / Magnitude();
	x *= Scale;
	y *= Scale;
	z *= Scale;
}
// End std_Vector3::Normalize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Vector3::ChangeSpace() //                   \author Logan Jones
///////////////////////////////                        \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_SizeF szFrom - 
//  const std_SizeF szTo   - 
//
void std_Vector3::ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo )
{
	x *= szTo.width / szFrom.width;
	y *= szTo.height/ szFrom.height;
}
// End std_Vector3::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Vector3.inl //
//////////////////////////
