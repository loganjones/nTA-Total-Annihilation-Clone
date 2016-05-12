// std_FileIO_Win32.cpp //                         \author Logan Jones
/////////////////////////                             \date 9/13/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "std.h"
#include "std_FileIO.h"


//////////////////////////////////////////////////////////////////////
// fio_OpenFile() //                               \author Logan Jones
///////////////////                                    \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			LPCTSTR strPath - Path to the file to open
//			flgFileAccess   - Flag determining the access to the opened file
//
// Return :			void*	- Returs a handle to the open file, NULL otherwise
//
void* fio_OpenFile( LPCTSTR strPath, fio_File_Flags flgFileAccess )
{
    // Assign the appropriate values for access and creation flags
    DWORD Access = GENERIC_READ;
	DWORD Create = OPEN_EXISTING;
	switch( flgFileAccess )
	{
	case fio_READ:
		Access = GENERIC_READ;
		Create = OPEN_EXISTING;
		break;
	case fio_WRITE:
		Access = GENERIC_WRITE;
		Create = CREATE_ALWAYS;
		break;
	case fio_READ_WRITE:
		Access = GENERIC_READ|GENERIC_WRITE;
		Create = CREATE_ALWAYS;
		break;
	}

    // Attempt to create the file
	HANDLE File = CreateFile( strPath,
							  Access,
							  FILE_SHARE_READ,
							  NULL,
							  Create,
							  (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS),
							  NULL );

    // If creation did not succeed, return NULL
	if( File==INVALID_HANDLE_VALUE ) return NULL;

    // Return the file's HANDLE
	return File;
}
// End fio_OpenFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_CloseFile() //                              \author Logan Jones
////////////////////                                   \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFile	- Handle returned by fio_OpenFile()
//
void  fio_CloseFile( void* hFile )
{
    CloseHandle(hFile);
}
// End fio_CloseFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_GetFileSize() //                            \author Logan Jones
//////////////////////                                 \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFile	- Handle returned by fio_OpenFile()
//
// Return :		DWORD	- Size of the file in bytes
//
DWORD fio_GetFileSize( void* hFile )
{
    // Return the size of the file with the given handle
    return GetFileSize(hFile,NULL);
}
// End fio_GetFileSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_FileSeek() //                               \author Logan Jones
///////////////////                                    \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFile	    - Handle returned by fio_OpenFile()
//			DWORD Offset	- Offset in the file with which to set
//                            the file pointer
//
void  fio_FileSeek( void* hFile, DWORD Offset )
{
    // Set the file pointer to the given offset starting from the
    // beginning of the file
    SetFilePointer( hFile, Offset, NULL, FILE_BEGIN ); 
}
// End fio_FileSeek()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_ReadFile() //                               \author Logan Jones
///////////////////                                    \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFile	        - Handle returned by fio_OpenFile()
//			void* Dest_Buffer	- The destination buffer for the data to be read
//			DWORD BytesToRead	- The amount of bytes in the file to read
//
// Return :			DWORD	- Returns the total amount of bytes read
//
DWORD fio_ReadFile( void* hFile, void* Dest_Buffer, DWORD BytesToRead )
{
	DWORD BytesRead=0;

    // Read data from the file into the given buffer
	ReadFile( hFile, Dest_Buffer, BytesToRead, &BytesRead, NULL );

    // Return the total amount of bytes read
	return BytesRead;
}
// End fio_ReadFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_WriteFile() //                              \author Logan Jones
////////////////////                                   \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFile     	- Handle returned by fio_OpenFile()
//			void* Src_Buffer	- The source buffer for the data to write
//			DWORD BytesToWrite	- The amount of bytes in the buffer to write
//
// Return :			DWORD	- Returns the amount of bytes written
//
DWORD fio_WriteFile( void* hFile, void* Src_Buffer, DWORD BytesToWrite )
{
    DWORD BytesWritten=0;

    // Write the data to the file
	WriteFile( hFile, Src_Buffer, BytesToWrite, &BytesWritten, NULL );

    // Return the amount of bytes written
	return BytesWritten;
}
// End fio_WriteFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_DirectoryExists() //                        \author Logan Jones
//////////////////////////                             \date 2/19/2001
// Description:  Queries whether a given directory exists. The Win32
//               implementation atempts to create a file in the directory
//====================================================================
// Parameters :
//			LPCTSTR strDir	-	The directory to query
//
// Return :			BOOL	-	Returns TRUE if it exists, FALSE otherwise
//
BOOL fio_DirectoryExists( LPCTSTR strDir )
{
    char                tmpFile[MAX_PATH];
    HANDLE              hFile;
    BOOL                bExists = FALSE;

    // Manipulate tmpFile into the form "strDir/DirectoryQuery.tmp"
    strcpy( tmpFile, strDir );
    if( tmpFile[strlen(strDir)-1]!='/' )
        strcat( tmpFile, "/" );
    strcat( tmpFile, "DirectoryQuery.tmp" );

    // Attempt to create the file
    hFile = CreateFile( tmpFile,
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                        NULL );
    if( hFile!=INVALID_HANDLE_VALUE )
    {
        // The file was created successfully, clean up and return TRUE
        CloseHandle( hFile );
        DeleteFile( tmpFile );
        bExists = TRUE;
    }
    // else - The directory does not exist

    // Return the result
    return bExists;
}
// End fio_DirectoryExists()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_GetCDDriveStrings() //                      \author Logan Jones
////////////////////////////                           \date 2/27/2001
// Description:  Aquires all the drives in the system and checks their type.
//               If a drive is a CDROM, its path is added to strDrives.
//               
//====================================================================
// Parameters:
//  LPTSTR strDrives - The buffer to fill with drive strings.
//
// Return: DWORD - Returns the length of strDrives, or 0 if failed
//
DWORD fio_GetCDDriveStrings( LPTSTR strDrives )
{
    char Drives[MAX_PATH];

    // Get all the drives
	if( GetLogicalDriveStrings(MAX_PATH,Drives)==0 )
    {
        strDrives[0] = '\0';
        return 0;
    }

    DWORD   StrLength = 0;
	LPTSTR	Drive = Drives;
    LPTSTR  CDDrive=strDrives;

    // Check each one
	for( UINT Type; Drive[0]!='\0'; Drive+=(strlen(Drive)+1))
	{
		Type = GetDriveType(Drive);

        // Check for a CDROM
		if( Type==DRIVE_CDROM )
		{
            // Copy it to the buffer
            strcpy( CDDrive, Drive );
            CDDrive += strlen(Drive)+1;
            StrLength += strlen(Drive)+1;
		}

    } // End for( Drives )

    // Add a last NULL character
    CDDrive[0] = '\0';

    // Return the length of the buffer, could be 0 if no CDROM's were found
    return StrLength;
}
// End fio_GetCDDriveStrings()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_FindFirstFile() //                          \author Logan Jones
////////////////////////                               \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			LPTSTR FilePath	- The desired search path
//			FileData	    - A pointer to the structure to be filled
//                            with the search results, if any
//
void* fio_FindFirstFile( LPCTSTR FilePath, LPfio_FindFileData FileData )
{
    HANDLE              hFind;
	WIN32_FIND_DATA     W32_FileData;

    // Start the search
	hFind = FindFirstFile( FilePath, &W32_FileData );

    // Check if the search has failed
	if( hFind==INVALID_HANDLE_VALUE )
	{
		return NULL;
	}

    // Fill the FileData parameter with the results of the search
	strcpy( FileData->Name, W32_FileData.cFileName );
	FileData->Size = W32_FileData.nFileSizeLow;
	if( W32_FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
		FileData->Attributes = fio_FFA_DIRECTORY;
	else FileData->Attributes = fio_FFA_NORMAL;

    // Return the HANDLE to the search
	return hFind;
}
// End fio_FindFirstFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_FindNextFile() //                           \author Logan Jones
///////////////////////                                \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFind	- Handle returned by fio_FindFirstFile()
//			FileData	- A pointer to the structure to be filled
//                        with the search results, if any
//
// Return :		BOOL	- Returns TRUE if something was found, FALSE otherwise
//
BOOL  fio_FindNextFile( void* hFind, LPfio_FindFileData FileData )
{
	WIN32_FIND_DATA     W32_FileData;

    // Continue the search
	if( FindNextFile(hFind,&W32_FileData)==FALSE )
    {
        // Nothing left to list, return FALSE
        return FALSE;
    }

    // Fill the FileData parameter with the results of the search
	strcpy( FileData->Name, W32_FileData.cFileName );
	FileData->Size = W32_FileData.nFileSizeLow;
	if( W32_FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
		FileData->Attributes = fio_FFA_DIRECTORY;
	else FileData->Attributes = fio_FFA_NORMAL;

    // A file was found, return TRUE
	return TRUE;
}
// End fio_FindNextFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fio_FindClose() //                              \author Logan Jones
////////////////////                                   \date 2/19/2001
// Description:  
//               
//====================================================================
// Parameters :
//			void* hFind	- Handle returned by fio_FindFirstFile()
//
void  fio_FindClose( void* hFind )
{
    // Close the search
    if ( hFind ) FindClose( hFind );
}
// End fio_FindClose()
//////////////////////////////////////////////////////////////////////


#endif // !defined(_WIN32)
/////////////////////////////////////////////////////////////////////
// End - std_FileIO_Win32.cpp //
///////////////////////////////