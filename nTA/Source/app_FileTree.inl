//  app_FileTree.inl //                            \author Logan Jones
//////////////////////                                \date 9/15/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::ReadAndDecrypt() //              \author Logan Jones
////////////////////////////////////                   \date 9/15/2001
//               
//====================================================================
// Parameters:
//  void* hFile     - 
//  INT32 iKey      - 
//  UINT32 uiOffset - 
//  void* pBuffer   - 
//  UINT32 uiSize   - 
//
// Return: UINT32 - 
//
UINT32  app_FileTree::ReadAndDecrypt( void* hFile, INT32 iKey, UINT32 uiOffset, void* pBuffer, UINT32 uiSize )
{
	UINT32		uiResult;
	BYTE*		pBuf;

	// Seek to the offset
	fio_FileSeek( hFile, uiOffset );

	// Read into the buffer
	uiResult = fio_ReadFile( hFile, pBuffer, uiSize );

	// Cast the pointer to a BYTE type
	pBuf = static_cast<BYTE*>(pBuffer);

	// Decrypt the buffer
	if( iKey ) for (INT32 tkey,count = 0; count<static_cast<INT32>(uiResult); count++) 
	{	
		tkey = (uiOffset + count) ^ iKey;
		pBuf[count] = static_cast<BYTE>(tkey ^ ~pBuf[count]);
	}

	// Return the actual amount of bytes read from the file
	return uiResult;
}
// End  app_FileTree::ReadAndDecrypt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::FindCentralDirectoryRecord() //  \author Logan Jones
////////////////////////////////////////////////       \date 9/15/2001
//               
//====================================================================
// Parameters:
//  void* hFile                        - 
//  ZIP_Central_Directory_Record* pCDR - 
//
// Return: BOOL - 
//
BOOL  app_FileTree::FindCentralDirectoryRecord( void* hFile, ZIP_Central_Directory_Record* pCDR )
{
	// TODO: Implement FindCentralDirectoryRecord()
	assertEx( 0, "FindCentralDirectoryRecord() not supported yet\ndue to laziness" );
	return FALSE;
}
// End  app_FileTree::FindCentralDirectoryRecord()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::AddDirectory() //                \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
// Description:  Attaches a new directory to the file tree
//               
//====================================================================
// Parameters:
//  LPCTSTR strName   - Desired name of the new directory
//  DirEntry_t* pRoot - Desired parent directory for the new entry
//
// Return: DirEntry_t* - If successful, returns a pointer to the new directory, NULL otherwise
//
 app_FileTree::DirEntry_t*  app_FileTree::AddDirectory( LPCTSTR strName, DirEntry_t* pRoot )
{
	DirEntry_t* pNewDir;
	DirEntry_t** ppNewDir;

	// Sanity Check
	if( strName==NULL )
		return NULL;

	// Attach this entry to its parent if desired
	if( pRoot )
	{
		if( pRoot->SubDirs==NULL )
			ppNewDir = &pRoot->SubDirs;
		else // Make sure it doesn't already exist
		{
            DirEntry_t* pPrev, *pDir;
			for( pPrev=NULL,pDir=pRoot->SubDirs; pDir; pDir=pDir->Next)
			{
				pPrev = pDir;
				if( stricmp(pDir->Name,strName)==0 ) return pDir;
			}
			ppNewDir = &pPrev->Next;
		}
	}
	else ppNewDir = &pNewDir;

	// Allocate the new dir
	(*ppNewDir) = new DirEntry_t;
	assertEx( (*ppNewDir), "Unable to allocate memory for the new directory" );
	if( (*ppNewDir)==NULL ) return NULL;

	// Fill out the new entry
	ZeroMemory( (*ppNewDir), sizeof(DirEntry_t) );
	strcpy( (*ppNewDir)->Name, strName );

	// Return a pointer to the new entry
	return (*ppNewDir);
}
// End  app_FileTree::AddDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::AddFile() //                     \author Logan Jones
/////////////////////////////                          \date 9/15/2001
// Description:  Attaches a new file to the file tree
//               
//====================================================================
// Parameters:
//  FileEntry_t& feData - FileEntry_t structure containing the description of the new file
//  DirEntry_t* pRoot   - Desired parent directory for the new entry
//
// Return: FileEntry_t* - If successful, returns a pointer to the new file, NULL otherwise
//
 app_FileTree::FileEntry_t*  app_FileTree::AddFile( FileEntry_t& feData, DirEntry_t* pRoot )
{
	FileEntry_t** ppNewFile;

	// Sanity Check
	if( pRoot==NULL )
		return NULL;

	// Make sure the name doesn't already exists
	if( pRoot->SubFiles==NULL )
		ppNewFile = &pRoot->SubFiles;
	else // Find the last sub file
	{
        FileEntry_t* pPrev, *pFile;
		for( pPrev=NULL,pFile=pRoot->SubFiles; pFile; pFile=pFile->Next)
		{
			pPrev = pFile;
			if( stricmp(pFile->Name,feData.Name)==0 )
			{
				if( feData.Priority>pFile->Priority )
					CopyFileData( *pFile, feData );
				return pFile;
			}
		}
		ppNewFile = &pPrev->Next;
	}

	// Allocate the new file
	(*ppNewFile) = new FileEntry_t;
	assertEx( (*ppNewFile), "Unable to allocate memory for the new file" );
	if( (*ppNewFile)==NULL ) return NULL;

	// Fill out the new entry
	ZeroMemory( (*ppNewFile), sizeof(FileEntry_t) );
	CopyFileData( *(*ppNewFile), feData );

	// Return a pointer to the new entry
	return (*ppNewFile);
}
// End  app_FileTree::AddFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::CopyFileData() //                \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
//               
//====================================================================
// Parameters:
//  FileEntry_t& destFile      - 
//  const FileEntry_t& srcFile - 
//
void  app_FileTree::CopyFileData( FileEntry_t& destFile, const FileEntry_t& srcFile )
{
	strcpy( destFile.Name, srcFile.Name );

	destFile.Priority = srcFile.Priority;
	destFile.Type = srcFile.Type;

	destFile.Offset = srcFile.Offset;
	destFile.DecompressedSize = srcFile.DecompressedSize;
	destFile.CompressedSize = srcFile.CompressedSize;
	destFile.Key = srcFile.Key;
	destFile.LibraryPathIndex = srcFile.LibraryPathIndex;
}
// End  app_FileTree::CopyFileData()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::RetrieveDirectory() //           \author Logan Jones
///////////////////////////////////////                \date 9/15/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath       - 
//  DirEntry_t* pStartDir - 
//
// Return: DirEntry_t* - 
//
 app_FileTree::DirEntry_t*  app_FileTree::RetrieveDirectory( LPCTSTR strPath, DirEntry_t* pStartDir )
{
	BOOL		ThisIsTheTarget;
	char		Target[ 64 ];
	LPTSTR		Next;
	DirEntry_t*	pDir;

	// Find the next occurance of the '/' (if any)
	Next = strchr( strPath, '/' );

	// Check if '/' was found
	if( Next==NULL )
	{
		ThisIsTheTarget = TRUE;
		strcpy( Target, strPath);
	}
	else // '/' was found
	{
		ThisIsTheTarget = FALSE;
		strncpy( Target, strPath, Next-strPath );
		Target[ Next-strPath ] = '\0';
		Next++;
	}

	// Iterate through each entry looking for Target
	for( pDir = pStartDir->SubDirs; pDir; pDir=pDir->Next)
	{
		if( stricmp(pDir->Name,Target)==0 )
			if( ThisIsTheTarget ) return pDir;
			else return RetrieveDirectory( Next, pDir );

	} // end for( SubDirs )

	// Not found
	return NULL;
}
// End  app_FileTree::RetrieveDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::RetrieveFile() //                \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath       - 
//  DirEntry_t* pStartDir - 
//
// Return: FileEntry_t* - 
//
 app_FileTree::FileEntry_t*  app_FileTree::RetrieveFile( LPCTSTR strPath, DirEntry_t* pStartDir )
{
	DirEntry_t*		pDir;
	char			Path[ MAX_PATH ];
	LPTSTR			FileName;
	FileEntry_t*	pFile;

	// Get the position of the last '/' (if any)
	FileName = strrchr( strPath, '/' );

	// Check if 
	if( FileName==NULL )
	{
		pDir = pStartDir;
	}
	else // '/' was found
	{
		strncpy( Path, strPath, FileName-strPath );
		Path[ FileName-strPath ] = '\0';
		strPath = FileName+1;
		pDir = RetrieveDirectory( Path, pStartDir );
	}

	// Make sure we have a parent dir
	if( pDir==NULL )
		return NULL;

	// Iterate through each file until the target is found
	for( pFile=pDir->SubFiles; pFile; pFile=pFile->Next)
		if( stricmp(pFile->Name,strPath)==0 ) return pFile;

	return NULL;
}
// End  app_FileTree::RetrieveFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::GetDirSize() //                  \author Logan Jones
////////////////////////////////                       \date 9/16/2001
//               
//====================================================================
// Parameters:
//  DirEntry_t* pDir - 
//
// Return: int - 
//
int  app_FileTree::GetDirSize( DirEntry_t* pDir )
{
	int count = 0;
	for( DirEntry_t* pSubDir=pDir->SubDirs; pSubDir; pSubDir=pSubDir->Next,count++);
	for( FileEntry_t* pSubFile=pDir->SubFiles; pSubFile; pSubFile=pSubFile->Next,count++);
	return count;
}
// End  app_FileTree::GetDirSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::DeleteBranch() //                \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
//               
//====================================================================
// Parameters:
//  DirEntry_t** ppBranch - 
//
void  app_FileTree::DeleteBranch( DirEntry_t** ppBranch )
{
	// Sanity check
	if( (ppBranch==NULL)||(*ppBranch==NULL) )
		return;

	// Delete all subfiles if there are any
	if( (*ppBranch)->SubFiles )
	{
		FileEntry_t* pDel,*pFile = (*ppBranch)->SubFiles;
		while( pFile )
		{
			pDel = pFile;
			pFile = pFile->Next;
			delete pDel;
		}
	}

	// Delete all subdirss if there are any
	if( (*ppBranch)->SubDirs )
	{
		DirEntry_t* pDel,*pDir = (*ppBranch)->SubDirs;
		while( pDir )
		{
			pDel = pDir;
			pDir = pDir->Next;
			DeleteBranch( &pDel );
		}
	}

	// Delete the root and set the pointer to NULL
	delete (*ppBranch);
	(*ppBranch) = NULL;
}
// End  app_FileTree::DeleteBranch()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  app_FileTree::DumpBranchToFile() //            \author Logan Jones
//////////////////////////////////////                 \date 9/15/2001
//               
//====================================================================
// Parameters:
//  DirEntry_t* pBranch - 
//  LPTSTR strFileName  - 
//  LPTSTR strPrefix    - 
//
void  app_FileTree::DumpBranchToFile( DirEntry_t* pBranch, LPTSTR strFileName, LPTSTR strPrefix )
{
	FILE*	hFile;
	char	NewPrefix[ MAX_PATH ];

	// Check if the file needs openning
	if( strPrefix==NULL )
	{
		hFile =fopen( strFileName, "wt" );
		if( hFile==NULL ) return;
	}
	else hFile = (FILE*)strFileName;

	for( DirEntry_t* pDir=pBranch->SubDirs; pDir; pDir=pDir->Next)
	{
		if( strPrefix )
		{
			sprintf( NewPrefix, "%s/%s", strPrefix, pDir->Name );
			DumpBranchToFile( pDir, (LPTSTR)hFile, NewPrefix );
		}
		else DumpBranchToFile( pDir, (LPTSTR)hFile, pDir->Name );
	}
	
	for( FileEntry_t* pFile=pBranch->SubFiles; pFile; pFile=pFile->Next)
	{
		if( strPrefix ) fprintf( hFile, "%s/%s\n", strPrefix, pFile->Name );
		else fprintf( hFile, "%s\n", pFile->Name );
	}

	if( strPrefix==NULL ) fclose( hFile );
}
// End  app_FileTree::DumpBranchToFile()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End -  app_FileTree.inl //
////////////////////////////
