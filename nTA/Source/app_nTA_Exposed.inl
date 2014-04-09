// app_nTA_Exposed.inl //                          \author Logan Jones
////////////////////////                              \date 9/25/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GfxLoadProc() //                       \author Logan Jones
////////////////////////////                           \date 9/25/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strComment - 
//  ...                - 
//
// Return: LRESULT - 
//
LRESULT CALLBACK app_nTA::GfxLoadProc( LPCTSTR strComment, ... )
{
	va_list 	vaArgs;

	// Access the argument list
	va_start( vaArgs, strComment );

	// Send the comment to the console
	theApp.Console.vComment( CT_LOAD, strComment, vaArgs );

	// Close the argument list
	va_end( vaArgs );

	return 1;
}
// End app_nTA::GfxLoadProc()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GfxErrorProc() //                      \author Logan Jones
/////////////////////////////                          \date 9/25/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strComment - 
//  ...                - 
//
// Return: LRESULT - 
//
LRESULT CALLBACK app_nTA::GfxErrorProc( LPCTSTR strComment, ... )
{
	va_list 	vaArgs;

	// Access the argument list
	va_start( vaArgs, strComment );

	// Send the comment to the console
	theApp.Console.vComment( CT_ERROR, strComment, vaArgs );

	// Close the argument list
	va_end( vaArgs );

	return 1;
}
// End app_nTA::GfxErrorProc()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::ft_AddScanDir() //                     \author Logan Jones
//////////////////////////////                        \date 11/11/2001
//               
//====================================================================
// Parameters:
//  LPVOID lpParams - 
//
// Return: LRESULT - 
//
LRESULT CALLBACK app_nTA::ft_AddScanDir( LPVOID lpParams )
{
	LPTSTR	strParams = static_cast<LPTSTR>(lpParams);
	char	Path[ MAX_PATH ];
	BOOL	bVerify = FALSE;

	if( !theApp.Console.RetrieveParam( strParams, app_Console::PT_STRING, (LPTSTR)Path ) )
	{
		theApp.Console.Comment( CT_ECHO, "Usage:	ft_AddScanDir <Directory> [Verify Flag]" );
		return 1;
	}

	if( Path[0]=='$' ) STRING_SWITCH( Path + 1 )
		CASE( "(TA_DIR)" )
			strcpy( Path, theApp.m_TotalAnnihilationDirectory );
		CASE( "(CD_DIRS)" )
			strcpy( Path, "?CD_DIRS" );
		CASE( "(MOD_DIR)" )
			strcpy( Path, theApp.m_GameType );
		CASE( "(THIS_DIR)" )
			strcpy( Path, theApp.Console.GetCurrentDirectory() );
	END_STRING_SWITCH

	theApp.Console.RetrieveParam( strParams, app_Console::PT_BOOL, &bVerify );

	theFileTree.AddDirectoryToScan( Path, bVerify );

	return 1;
}
// End app_nTA::ft_AddScanDir()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::ft_AddScanType() //                    \author Logan Jones
///////////////////////////////                       \date 11/11/2001
//               
//====================================================================
// Parameters:
//  LPVOID lpParams - 
//
// Return: LRESULT CALLBACK - 
//
LRESULT CALLBACK app_nTA::ft_AddScanType( LPVOID lpParams )
{
	LPTSTR	strParams = static_cast<LPTSTR>(lpParams);
	char	Ext[ 32 ];
	long	Priority;

	if( !theApp.Console.RetrieveParam( strParams, app_Console::PT_STRING, Ext ) ||
		!theApp.Console.RetrieveParam( strParams, app_Console::PT_INT, &Priority ) )
	{
		theApp.Console.Comment( CT_ECHO, "Usage:	ft_AddScanType <Extension> <Priority>" );
		return 1;
	}

	theFileTree.AddFileType( Ext, Priority );

	return 1;
}
// End app_nTA::ft_AddScanType()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::ft_AddSubs() //                        \author Logan Jones
///////////////////////////                           \date 11/11/2001
//               
//====================================================================
// Parameters:
//  LPVOID lpParams - 
//
// Return: LRESULT CALLBACK - 
//
LRESULT CALLBACK app_nTA::ft_AddSubs( LPVOID lpParams )
{
	LPTSTR	strParams = static_cast<LPTSTR>(lpParams);
	long	Priority;

	if( !theApp.Console.RetrieveParam( strParams, app_Console::PT_INT, &Priority ) )
	{
		theApp.Console.Comment( CT_ECHO, "Usage:	ft_AddSubs <Priority>" );
		return 1;
	}

	theFileTree.AddSubDirectoriesAsLibs( Priority );

	return 1;
}
// End app_nTA::ft_AddSubs()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::Exec() //                              \author Logan Jones
////////////////////                                   \date 8/26/2002
//               
//====================================================================
// Parameters:
//  LPVOID lpParams - 
//
// Return: LRESULT - 
//
LRESULT CALLBACK app_nTA::Exec( LPVOID lpParams )
{
	LPTSTR		strParams = static_cast<LPTSTR>(lpParams);
	char		Path[32];
	LPTSTR		Dir, Last;
	LPVOID		hFile;
	DWORD		FileSize;
	char*		pFileBuf;

	if( !theApp.Console.RetrieveParam( strParams, app_Console::PT_STRING, &Path ) )
	{
		theApp.Console.Comment( CT_ECHO, "Usage:	exec <Path>" );
		return 1;
	}

	hFile = fio_OpenFile( Path, fio_READ );
	if( hFile==NULL )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to open \'%s\'.", Path );
		return 1;
	}

	Dir = strrchr( Path, '/' );
	if( Dir ) Dir[0] = '\0';

	FileSize = fio_GetFileSize( hFile );
	pFileBuf = new char[ FileSize ];
	fio_ReadFile( hFile, pFileBuf, FileSize );
	Last = theApp.Console.GetCurrentDirectory();
	theApp.Console.SetCurrentDirectory( Dir );
	theApp.Console.Execute( pFileBuf, FileSize );
	theApp.Console.SetCurrentDirectory( Last );
	delete [] pFileBuf;

	fio_CloseFile( hFile );

	return 1;
}
// End app_nTA::Exec()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_nTA_Exposed.inl //
//////////////////////////////
