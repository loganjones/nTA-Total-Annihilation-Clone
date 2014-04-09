// std_Time.inl //                                 \author Logan Jones
/////////////////                                     \date 12/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


// Windows functions
#ifdef _WIN32

//////////////////////////////////////////////////////////////////////
// std_PreciseTime() //                            \author Logan Jones
//////////////////////                                 \date 12/8/2001
//               
//====================================================================
// Return: std_PreciseTime_t - 
//
std_PreciseTime_t std_PreciseTime()
{
	LARGE_INTEGER	liCount;

	QueryPerformanceCounter( &liCount );

	return liCount.QuadPart;
}
// End std_PreciseTime()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_TimeDifferenceInSecs() //                   \author Logan Jones
///////////////////////////////                        \date 12/8/2001
//               
//====================================================================
// Parameters:
//  std_PreciseTime_t ptA - 
//  std_PreciseTime_t ptB - 
//
// Return: std_ExactTime - 
//
std_ExactTime std_TimeDifferenceInSecs( std_PreciseTime_t ptA, std_PreciseTime_t ptB ) 
{
	LARGE_INTEGER	liFreq;

	QueryPerformanceFrequency( &liFreq );

	return ( ptB - ptA ) / (std_ExactTime)liFreq.QuadPart;
}
// End std_TimeDifferenceInSecs()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// std_TimeDifferenceInmSecs() //                  \author Logan Jones
////////////////////////////////                      \date 12/29/2001
//               
//====================================================================
// Parameters:
//  std_PreciseTime_t ptA - 
//  std_PreciseTime_t ptB - 
//
// Return: std_ExactTime - 
//
std_ExactTime std_TimeDifferenceInmSecs( std_PreciseTime_t ptA, std_PreciseTime_t ptB )
{
	LARGE_INTEGER	liFreq;

	QueryPerformanceFrequency( &liFreq );

	return (( ptB - ptA ) * (std_ExactTime)1000.0) / liFreq.QuadPart;
}
// End std_TimeDifferenceInmSecs()
//////////////////////////////////////////////////////////////////////

#endif // defined( _WIN32 )


/////////////////////////////////////////////////////////////////////
// End - std_Time.inl //
///////////////////////
