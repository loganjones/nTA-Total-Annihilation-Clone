// std_Time.h //                                   \author Logan Jones
///////////////                                       \date 12/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_TIME_H_
#define _STD_TIME_H_
/////////////////////////////////////////////////////////////////////


#ifdef _WIN32


// Time types
typedef DWORD	std_Time_t;
typedef __int64	std_PreciseTime_t;
typedef double	std_ExactTime;

// Just make a macro to wrap timeGetTime()
#define std_Time()			timeGetTime()

// Use the performance counter to get the precise time
inline std_PreciseTime_t std_PreciseTime();

// Finds the difference between two precise times in seconds
inline std_ExactTime std_TimeDifferenceInSecs( std_PreciseTime_t ptA, std_PreciseTime_t ptB );

// Finds the difference between two precise times in miliseconds
inline std_ExactTime std_TimeDifferenceInmSecs( std_PreciseTime_t ptA, std_PreciseTime_t ptB );


#endif // defined( _WIN32 )


#include "std_Time.inl"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_TIME_H_)
