// std_Size.inl //                                 \author Logan Jones
/////////////////                                     \date 9/21/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::std_Size() //                         \author Logan Jones
/////////////////////////                              \date 5/10/2002
//               
//====================================================================
//
std_Size::std_Size():width(0), height(0){}
// End std_Size::std_Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::std_Size() //                         \author Logan Jones
////////////////////////                               \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& szCopy - 
//
std_Size::std_Size( const std_Size& szCopy ):width(szCopy.width), height(szCopy.height){}
// End std_Size::std_Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::std_Size() //                         \author Logan Jones
////////////////////////                               \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const long newWidth  - 
//  const long newHeight - 
//
std_Size::std_Size( const long newWidth, const long newHeight ):width(newWidth), height(newHeight){}
// End std_Size::std_Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::std_Size() //                         \author Logan Jones
/////////////////////////                              \date 5/10/2002
//               
//====================================================================
// Parameters:
//  const std_SizeF& szCopy - 
//
std_Size::std_Size( const std_SizeF& szCopy ):width(szCopy.width), height(szCopy.height){}
// End std_Size::std_Size()
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////
// std_Size::std_Size() //                         \author Logan Jones
////////////////////////                               \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const SIZE szCopy - 
//
std_Size::std_Size( const SIZE szCopy ):width(szCopy.cx), height(szCopy.cy){}
// End std_Size::std_Size()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator SIZE() //                    \author Logan Jones
///////////////////////////////                        \date 10/3/2001
//               
//====================================================================
//
std_Size::operator SIZE()
{
	SIZE size;
	size.cx = width;
	size.cy = height;
	return size;
}
// End std_Size::operator SIZE()
//////////////////////////////////////////////////////////////////////
#endif

//////////////////////////////////////////////////////////////////////
// std_Size::operator+() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_Size - 
//
std_Size std_Size::operator+() const
{
	return (*this);
}
// End std_Size::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator-() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Return: std_Size - 
//
std_Size std_Size::operator-() const
{
	return std_Size( -width, -height );
}
// End std_Size::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator*() //                        \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Return: long  - 
//
long std_Size::operator*() const
{
	return ( width * height );
}
// End std_Size::operator*()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator+() //                        \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: std_Size& - 
//
std_Size std_Size::operator+( const std_Size& size ) const
{
	return std_Size( width + size.width, height + size.height );
}
// End std_Size::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator-() //                        \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: std_Size& - 
//
std_Size std_Size::operator-( const std_Size& size ) const
{
	return std_Size( width - size.width, height - size.height );
}
// End std_Size::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator+() //                        \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Size& - 
//
std_Size std_Size::operator+( const std_Point& pt ) const
{
	return std_Size( width + pt.x, height + pt.y );
}
// End std_Size::operator+()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator-() //                        \author Logan Jones
///////////////////////////                            \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Size& - 
//
std_Size std_Size::operator-( const std_Point& pt ) const
{
	return std_Size( width - pt.x, height - pt.y );
}
// End std_Size::operator-()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator*() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Size - 
//
std_Size std_Size::operator*( const long s ) const
{
	return std_Size( width * s, height * s );
}
// End std_Size::operator*()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator/() //                        \author Logan Jones
///////////////////////////                            \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Size - 
//
std_Size std_Size::operator/( const long s ) const
{
	return std_Size( width / s, height / s );
}
// End std_Size::operator/()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator==() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator==( const std_Size& size ) const
{
	return ( (width==size.width) && (height==size.height) );
}
// End std_Size::operator==()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator!=() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator!=( const std_Size& size ) const
{
	return ( (width!=size.width) || (height!=size.height) );
}
// End std_Size::operator!=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator<() //                        \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator<( const std_Size& size ) const
{
	return ( (width<size.width) && (height<size.height) );
}
// End std_Size::operator<()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator<=() //                       \author Logan Jones
////////////////////////////                           \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator<=( const std_Size& size ) const
{
	return ( (width<=size.width) && (height<=size.height) );
}
// End std_Size::operator<=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator>() //                        \author Logan Jones
///////////////////////////                            \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator>( const std_Size& size ) const
{
	return ( (width>size.width) && (height>size.height) );
}
// End std_Size::operator>()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator>=() //                       \author Logan Jones
////////////////////////////                           \date 10/4/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: BOOL - 
//
BOOL std_Size::operator>=( const std_Size& size ) const
{
	return ( (width>=size.width) && (height>=size.height) );
}
// End std_Size::operator>=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator+=() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator+=( const std_Size& size )
{
	width += size.width;
	height+= size.height;
	return (*this);
}
// End std_Size::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator-=() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Size& size - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator-=( const std_Size& size )
{
	width -= size.width;
	height-= size.height;
	return (*this);
}
// End std_Size::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator+=() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator+=( const std_Point& pt )
{
	width += pt.x;
	height+= pt.y;
	return (*this);
}
// End std_Size::operator+=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator-=() //                       \author Logan Jones
////////////////////////////                           \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const std_Point& pt - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator-=( const std_Point& pt )
{
	width -= pt.x;
	height-= pt.y;
	return (*this);
}
// End std_Size::operator-=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator *=() //                      \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator *=( const long s )
{
	width *= s;
	height*= s;
	return (*this);
}
// End std_Size::operator *=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::operator /=() //                      \author Logan Jones
/////////////////////////////                          \date 10/3/2001
//               
//====================================================================
// Parameters:
//  const long s - 
//
// Return: std_Size& - 
//
std_Size& std_Size::operator /=( const long s )
{
	width /= s;
	height/= s;
	return (*this);
}
// End std_Size::operator /=()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::Set() //                              \author Logan Jones
/////////////////////                                  \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const long newWidth  - 
//  const long newHeight - 
//
void std_Size::Set( const long newWidth, const long newHeight )
{
	width = newWidth;
	height= newHeight;
}
// End std_Size::Set()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::Shift() //                            \author Logan Jones
///////////////////////                                \date 9/21/2001
//               
//====================================================================
// Parameters:
//  const long newWidth  - 
//  const long newHeight - 
//
void std_Size::Shift( const long newWidth, const long newHeight )
{
	width += newWidth;
	height+= newHeight;
}
// End std_Size::Shift()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_Size::ChangeSpace() //                      \author Logan Jones
////////////////////////////                           \date 5/24/2002
//               
//====================================================================
// Parameters:
//  const std_Size szFrom - 
//  const std_Size szTo   - 
//
void std_Size::ChangeSpace( const std_Size szFrom, const std_Size szTo )
{
	width = (width * szTo.width) / szFrom.width;
	height= (height* szTo.height)/ szFrom.height;
}
// End std_Size::ChangeSpace()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Size.inl //
///////////////////////
