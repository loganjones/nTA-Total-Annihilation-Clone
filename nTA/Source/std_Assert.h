// std_Assert.h //                                 \author Logan Jones
/////////////////                                      \date 9/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_ASSERT_H_
#define _STD_ASSERT_H_
/////////////////////////////////////////////////////////////////////


// Make sure the asserts are not defined
#undef assert
#undef assertEx


// Only define and prototype assert if in debug mode
#ifdef _DEBUG

	// Macro used to break to the debugger
	#define BREAK_INTO_DEBUGGER			{_asm { int 3 }}

	// External function called by the debug assert macros
	extern BOOL std_Assert( LPTSTR strAssertion, LPTSTR strExtraInfo, LPTSTR strFileName, int iLine );

	// Standard assert macro
	#define assert( ASSERTION ) \
		if( !(ASSERTION) ) \
		{ \
			if( std_Assert( #ASSERTION, NULL, __FILE__, __LINE__ ) ) \
				BREAK_INTO_DEBUGGER \
		}

	// Extended assert macro that provides an extra parameter other information
	#define assertEx( ASSERTION, EXTRA_INFO ) \
		if( !(ASSERTION) ) \
		{ \
			if( std_Assert( #ASSERTION, EXTRA_INFO, __FILE__, __LINE__ ) ) \
				BREAK_INTO_DEBUGGER \
		}


#else // Not _DEBUG, just make some empty macros

	#define assert( ASSERTION )
	#define assertEx( ASSERTION, EXTRA_INFO )

#endif // !defined(_DEBUG) 


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_ASSERT_H_)
