// std_Defs.h //                                   \author Logan Jones
///////////////                                        \date 9/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_DEFS_H_
#define _STD_DEFS_H_
/////////////////////////////////////////////////////////////////////

/*
// Define this to compile STLport in debug mode
#ifdef _DEBUG
 #define _STLP_DEBUG		1
#endif
*/

/////////////////////////////////////////////////////////////////////
// Fun with macros

// A generaly safe way to delete
#define SAFE_DELETE( POINTER_TO_SOMEWHERE ) \
	{ \
		if( POINTER_TO_SOMEWHERE!=NULL ) \
		{ \
			delete POINTER_TO_SOMEWHERE; \
			POINTER_TO_SOMEWHERE = NULL; \
		} \
	}

// ditto
#define SAFE_DELETE_ARRAY( POINTER_TO_ARRAY_SOMEWHERE ) \
	{ \
		if( POINTER_TO_ARRAY_SOMEWHERE!=NULL ) \
		{ \
			delete [] POINTER_TO_ARRAY_SOMEWHERE; \
			POINTER_TO_ARRAY_SOMEWHERE = NULL; \
		} \
	}

// For com objects, a generally safe release method
#define SAFE_RELEASE( POINTER_TO_COM_THINGY ) \
	{ \
		if( POINTER_TO_COM_THINGY!=NULL ) \
		{ \
			POINTER_TO_COM_THINGY->Release(); \
			POINTER_TO_COM_THINGY = NULL; \
		} \
	}

// For destroyable objects, a generally safe destory method
#define SAFE_DESTROY( POINTER_TO_THINGY ) \
	{ \
		if( POINTER_TO_THINGY!=NULL ) \
		{ \
			POINTER_TO_THINGY->Destroy(); \
			POINTER_TO_THINGY = NULL; \
		} \
	}

// For destroyable allocated objects, a generally safe destory and delete method
#define SAFE_DESTROY_AND_DELETE( POINTER_TO_THINGY ) \
	{ \
		if( POINTER_TO_THINGY!=NULL ) \
		{ \
			POINTER_TO_THINGY->Destroy(); \
			delete POINTER_TO_THINGY; \
			POINTER_TO_THINGY = NULL; \
		} \
	}

// Use this combined with inl files to reduce recompilation
#ifdef _DEBUG
#define INLINE
#else
#define INLINE		inline
#endif

// Boolean version of the FAILED macro
#define bFAILED( BOOLEAN_CONDITION )        ( FALSE==(BOOLEAN_CONDITION) )

// Use this as the last enumerator in an enum list to force the list to DWORD size
// The prefix provided must help ensure that the parameter anme is unique in its namespace
#define FORCE_DWORD( PREFIX )				PREFIX##_FORCE_DWORD = ( 0x7fffffff )

// A set of macros that provide switch() statement like ability for char*
#define STRING_SWITCH( LP_STRING ) \
	{ LPCTSTR strSwitchCompare = (LP_STRING); \
	if( FALSE ) {
#define CASE( STR_CASE )					} else if( stricmp(strSwitchCompare,(STR_CASE))==0 ) {
#define CASE_2( STR_1, STR_2 )				} else if( stricmp(strSwitchCompare,(STR_1))==0 || stricmp(strSwitchCompare,(STR_2))==0 ) {
#define CASE_3( STR_1, STR_2, STR_3 )		} else if( stricmp(strSwitchCompare,(STR_1))==0 || stricmp(strSwitchCompare,(STR_2))==0 || stricmp(strSwitchCompare,(STR_3))==0 ) {
#define CASE_4( STR_1, STR_2, STR_3, STR_4)	} else if( stricmp(strSwitchCompare,(STR_1))==0 || stricmp(strSwitchCompare,(STR_2))==0 || stricmp(strSwitchCompare,(STR_3))==0 || stricmp(strSwitchCompare,(STR_4))==0 ) {
#define CASE_EXACT( STR_CASE )				} else if( strcmp(strSwitchCompare,(STR_CASE))==0 ) {
#define CASE_PLUS( STR_CASE, FUNC, EXTRA )	} else if( FUNC(strSwitchCompare,(STR_CASE),EXTRA)==0 ) {
#define STRING_SWITCH_DEFAULT				} else {
#define END_STRING_SWITCH					} }

// A set of macros to provide code block functionality.
// Basicly th same as {} except you can break out and restart
#define BEGIN_CODE_BLOCK					{ do {
#define EXIT_CODE_BLOCK						break;
#define RESTART_CODE_BLOCK					continue;
#define END_CODE_BLOCK						} while(0); }


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_DEFS_H_)
