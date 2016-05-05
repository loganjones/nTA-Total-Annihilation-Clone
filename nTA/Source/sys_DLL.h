// sys_DLL.h //                                    \author Logan Jones
//////////////                                        \date 9/20/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SYS_DLL_H_
#define _SYS_DLL_H_
/////////////////////////////////////////////////////////////////////


// Define a type for the DLL handle
#ifdef _WIN32
	typedef HINSTANCE HDLL;
	typedef FARPROC HDLLPROC;
#else
    typedef void * HDLL;
    typedef void * HDLLPROC;
#endif


// Loads a Dll given its path
inline HDLL sys_LoadDll( LPCTSTR strPath );


// Retrieves the address of an exported proc given its name
inline HDLLPROC sys_RetrieveProc( HDLL hDll, LPCTSTR strName );


// Retrieves the address of an exported proc given its ordinal
inline HDLLPROC sys_RetrieveProc( HDLL hDll, int iOrdinal );


// Closes a previously loaded Dll
inline void sys_UnloadDll( HDLL hDll );


#ifdef _WIN32
#  include "sys_DLL_Win32.inl"
#elif __APPLE__
#  include "sys_DLL_macOS.inl"
#else
#  error "Unknown Platform"
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SYS_DLL_H_)
