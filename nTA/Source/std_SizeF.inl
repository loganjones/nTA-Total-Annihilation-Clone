// std_SizeF.inl //                                \author Logan Jones
/////////////////                                     \date 9/21/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::std_SizeF() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
//
std_SizeF::std_SizeF():width(0),height(0){}
// End std_SizeF::std_SizeF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::std_SizeF() //                       \author Logan Jones
////////////////////////                               \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& szCopy - 
//
std_SizeF::std_SizeF( const std_SizeF& szCopy ):width(szCopy.width), height(szCopy.height){}
// End std_SizeF::std_SizeF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::std_SizeF() //                       \author Logan Jones
////////////////////////                               \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const float newWidth  - 
//  const float newHeight - 
//
std_SizeF::std_SizeF( const float newWidth, const float newHeight ):width(newWidth), height(newHeight){}
// End std_SizeF::std_SizeF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::std_SizeF() //                       \author Logan Jones
///////////////////////////                            \date 5/10/2002
//               
//====================================================================
// Parameters:
//  const std_Size& szCopy - 
//
std_SizeF::std_SizeF( const std_Size& szCopy ):width(szCopy.width),height(szCopy.height){}
// End std_SizeF::std_SizeF()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_SizeF::std_SizeF() //                       \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const SIZE szCopy - 
//
std_SizeF::std_SizeF( const SIZE szCopy ):width(szCopy.cx), height(szCopy.cy){}
// End std_SizeF::std_SizeF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator SIZE() //                   \author Logan Jones
///////////////////////////////                        \date 10/3/2001
//               
//====================================================================
//
std_SizeF::operator SIZE()
{
	SIZE size;
	size.cx = width;
	size.cy = height;
	return size;
}
// End std_SizeF::operator SIZE()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_SizeF::operator+() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_SizeF - 
//
std_SizeF std_SizeF::operator+() const
{
	return (*this);
}
// End std_SizeF::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator-() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_SizeF - 
//
std_SizeF std_SizeF::operator-() const
{
	return std_SizeF( -width, -height );
}
// End std_SizeF::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator*() //                       \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Return: float  - 
//
float std_SizeF::operator*() const
{
	return ( width * height );
}
// End std_SizeF::operator*()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator+() //                       \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: std_SizeF& - 
//
std_SizeF std_SizeF::operator+( const std_SizeF& size ) const
{
	return std_SizeF( width + size.width, height + size.height );
}
// End std_SizeF::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator-() //                       \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: std_SizeF& - 
//
std_SizeF std_SizeF::operator-( const std_SizeF& size ) const
{
	return std_SizeF( width - size.width, height - size.height );
}
// End std_SizeF::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator+() //                       \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_SizeF& - 
//
std_SizeF std_SizeF::operator+( const std_Vector2& v ) const
{
	return std_SizeF( width + v.x, height + v.y );
}
// End std_SizeF::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator-() //                       \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_SizeF& - 
//
std_SizeF std_SizeF::operator-( const std_Vector2& v ) const
{
	return std_SizeF( width - v.x, height - v.y );
}
// End std_SizeF::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator*() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_SizeF - 
//
std_SizeF std_SizeF::operator*( const float s ) const
{
	return std_SizeF( width * s, height * s );
}
// End std_SizeF::operator*()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator/() //                       \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_SizeF - 
//
std_SizeF std_SizeF::operator/( const float s ) const
{
	return std_SizeF( width / s, height / s );
}
// End std_SizeF::operator/()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator==() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator==( const std_SizeF& size ) const
{
	return ( (width==size.width) && (height==size.height) );
}
// End std_SizeF::operator==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator!=() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator!=( const std_SizeF& size ) const
{
	return ( (width!=size.width) || (height!=size.height) );
}
// End std_SizeF::operator!=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator<() //                       \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator<( const std_SizeF& size ) const
{
	return ( (width<size.width) && (height<size.height) );
}
// End std_SizeF::operator<()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator<=() //                      \author Logan Jones
////////////////////////////                           \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator<=( const std_SizeF& size ) const
{
	return ( (width<=size.width) && (height<=size.height) );
}
// End std_SizeF::operator<=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator>() //                       \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator>( const std_SizeF& size ) const
{
	return ( (width>size.width) && (height>size.height) );
}
// End std_SizeF::operator>()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator>=() //                      \author Logan Jones
////////////////////////////                           \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: BOOL - 
//
BOOL std_SizeF::operator>=( const std_SizeF& size ) const
{
	return ( (width>=size.width) && (height>=size.height) );
}
// End std_SizeF::operator>=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator+=() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator+=( const std_SizeF& size )
{
	width += size.width;
	height+= size.height;
	return (*this);
}
// End std_SizeF::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator-=() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_SizeF& size - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator-=( const std_SizeF& size )
{
	width -= size.width;
	height-= size.height;
	return (*this);
}
// End std_SizeF::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator+=() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator+=( const std_Vector2& v )
{
	width += v.x;
	height+= v.y;
	return (*this);
}
// End std_SizeF::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator-=() //                      \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Vector2& v - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator-=( const std_Vector2& v )
{
	width -= v.x;
	height-= v.y;
	return (*this);
}
// End std_SizeF::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator *=() //                     \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator *=( const float s )
{
	width *= s;
	height*= s;
	return (*this);
}
// End std_SizeF::operator *=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::operator /=() //                     \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const float s - 
//
// Return: std_SizeF& - 
//
std_SizeF& std_SizeF::operator /=( const float s )
{
	width /= s;
	height/= s;
	return (*this);
}
// End std_SizeF::operator /=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::Set() //                             \author Logan Jones
/////////////////////                                  \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const float newWidth  - 
//  const float newHeight - 
//
void std_SizeF::Set( const float newWidth, const float newHeight )
{
	width = newWidth;
	height= newHeight;
}
// End std_SizeF::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::Shift() //                           \author Logan Jones
///////////////////////                                \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const float newWidth  - 
//  const float newHeight - 
//
void std_SizeF::Shift( const float newWidth, const float newHeight )
{
	width += newWidth;
	height+= newHeight;
}
// End std_SizeF::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_SizeF::ChangeSpace() //                     \author Logan Jones
/////////////////////////////                          \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_SizeF szFrom - 
//  const std_SizeF szTo   - 
//
void std_SizeF::ChangeSpace( const std_SizeF szFrom, const std_SizeF szTo )
{
	width *= szTo.width / szFrom.width;
	height*= szTo.height/ szFrom.height;
}
// End std_SizeF::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_SizeF.inl //
///////////////////////
