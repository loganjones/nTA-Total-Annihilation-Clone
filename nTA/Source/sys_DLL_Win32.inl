// sys_DLL_Win32.inl //                            \author Logan Jones
//////////////////////                                \date 9/20/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_LoadDll() //                                \author Logan Jones
//////////////                                         \date 9/20/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath - 
//
// Return: HDLL - 
//
HDLL sys_LoadDll( LPCTSTR strPath )
{
	return LoadLibrary( strPath );
}
// End sys_LoadDll()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_RetrieveProc() //                           \author Logan Jones
///////////////////                                    \date 9/20/2001
//               
//====================================================================
// Parameters:
//  HDLL hDll       - 
//  LPCTSTR strName - 
//
// Return: HDLLPROC - 
//
HDLLPROC sys_RetrieveProc( HDLL hDll, LPCTSTR strName )
{
	return GetProcAddress( hDll, strName );
}
// End sys_RetrieveProc()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_RetrieveProc() //                           \author Logan Jones
///////////////////                                    \date 9/20/2001
//               
//====================================================================
// Parameters:
//  HDLL hDll    - 
//  int iOrdinal - 
//
// Return: HDLLPROC - 
//
HDLLPROC sys_RetrieveProc( HDLL hDll, int iOrdinal )
{
	return GetProcAddress( hDll, (LPCTSTR)iOrdinal );
}
// End sys_RetrieveProc()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_UnloadDll() //                              \author Logan Jones
////////////////                                       \date 9/20/2001
//               
//====================================================================
// Parameters:
//  HDLL hDll - 
//
void sys_UnloadDll( HDLL hDll )
{
	FreeLibrary( hDll );
}
// End sys_UnloadDll()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - sys_DLL_Win32.inl //
////////////////////////////
