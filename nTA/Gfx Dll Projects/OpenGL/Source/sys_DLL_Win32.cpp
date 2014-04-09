// sys_DLL_Win32.cpp //                    Author: Logan "Burn" Jones
//////////////////////                                Date: 9/11/2001
//
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "ogl.h"


// Instansiate the external gfx pointers
gfx_Interface*		pGfxSystem = NULL;
gfx_Interface**		ppGfx = NULL;


//////////////////////////////////////////////////////////////////////
// DllMain() //                             Author: Logan "Burn" Jones
//////////////                                         Date: 9/11/2001
//               
//====================================================================
// Parameters:
//  HINSTANCE hinstDLL - handle to DLL module
//  DWORD fdwReason    - reason for calling function
//  LPVOID lpvReserved - reserved
//
// Return: BOOL WINAPI - Return TRUE if successful, FALSE otherwise
//
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	// Why are we here
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			// Initialize the pointers to NULL
			ppGfx = NULL;
			pGfxSystem = NULL;

			break;
		// end case DLL_PROCESS_ATTACH

		case DLL_PROCESS_DETACH:
			// Delete the interface
			delete pGfxSystem;
			pGfxSystem = NULL;

			// Set the pointer to NULL if neccessary
			if( ppGfx!=NULL )
				(*ppGfx) = NULL;

			break;
		// end case DLL_PROCESS_DETACH

	} // end switch( fdwReason )

	return TRUE;
}
// End DllMain()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_Assert() //                          Author: Logan "Burn" Jones
/////////////////                                      Date: 9/10/2001
// Description:  Temporary home of the assert function. Just calls
//               the Win32 message box with the Abort Retry Fail option
//====================================================================
// Parameters:
//  LPTSTR strAssertion - The condition code that failed the assertion (in string form of course)
//  LPTSTR strExtraInfo - A string containing any extra info given by the coder, or NULL for nothing
//  LPTSTR strFileName  - String bufer containing the file name in which the assertion occured
//  int iLine           - Line number in the file where the assertion occured
//
// Return: BOOL - Returns TRUE to break into the debugger, FALSE to continue
//
BOOL std_Assert( LPTSTR strAssertion, LPTSTR strExtraInfo, LPTSTR strFileName, int iLine )
{
	static BOOL stdAssert_bIgnore = FALSE;
	int			iResult;
	char		strAssertTitle[ 256 ];
	char		strAssertBox[ 1024 ];

	// Just continue if the ignore flag has been set
	if( stdAssert_bIgnore==TRUE ) return FALSE;

	// Come up with a nice message foe the user ;)
	sprintf( strAssertBox,
			"Program: %s\n"
			"File: %s\n"
			"Line: %d\n"
			"\n"
			"assert( %s ); Failed\n",
			__argv[0], strFileName, iLine,
			strAssertion );
	if( strExtraInfo ) sprintf( strAssertBox, "%s\n%s\n", strAssertBox, strExtraInfo );
	strcat( strAssertBox, 
			"\n"
			"\n"
			"* Press Abort to break into the debugger.\n"
			"* Press Retry to continue program execution.\n"
			"* Press Ignore to disable any further assertion failures." );

	// Generic title
	sprintf( strAssertTitle, "Assertion Failure" );

	// Call the message box
	iResult = MessageBox(
		NULL,
		strAssertBox,
		strAssertTitle,
		MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST );

	// Check the result
	switch( iResult )
	{
	case IDABORT: return TRUE;
	case IDRETRY: return FALSE;
	case IDIGNORE:
		stdAssert_bIgnore = TRUE;
		break;
	}

	// Continue execution
	return FALSE;
}
// End sys_Assert()
//////////////////////////////////////////////////////////////////////


#endif // defined( _WIN32 )
/////////////////////////////////////////////////////////////////////
// End - sys_DLL_Win32.cpp //
////////////////////////////
