// std_FileIO.h //                                 \author Logan Jones
/////////////////                                     \date 9/13/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_FILEIO_H_
#define _STD_FILEIO_H_
/////////////////////////////////////////////////////////////////////


// Flags used by fio_OpenFile() for the desired access method
enum fio_File_Flags
{
	fio_READ,			// Open file as read only
	fio_WRITE,			// Open file as write only
	fio_READ_WRITE,		// Open file as both read and write
};

// Open a file
void* fio_OpenFile( LPCTSTR strPath, fio_File_Flags flgFileAccess=fio_READ );

// Close the file
void  fio_CloseFile( void* hFile );

// Get the file's size
DWORD fio_GetFileSize( void* hFile );

// Move the file pointer
void  fio_FileSeek( void* hFile, DWORD Offset );

// Retrieve bytes from the file
DWORD fio_ReadFile( void* hFile,
						   void* Dest_Buffer,
						   DWORD BytesToRead );

// Write bytes to the file
DWORD fio_WriteFile(void* hFile,
					void* Src_Buffer,
					DWORD BytesToWrite );

// Queries the file system on whether the given directory exists
BOOL fio_DirectoryExists( LPCTSTR strDir );

// Fills a buffer with strings that specify CD drives
DWORD fio_GetCDDriveStrings( LPTSTR strDrives );

// Structure used to get info on files found in the search
typedef struct fio_FindFileData
{
	char			Name[MAX_PATH];			// Name of the file
	DWORD			Size;					// Size of the file
	DWORD			Attributes;				// Attributes of the file
} *LPfio_FindFileData;

// Flags used by fio_FindFileData in the Attributes data member
enum fio_FindFileData_Flags
{
	fio_FFA_DIRECTORY=1,		// The file found is a directory
	fio_FFA_NORMAL=2,			// The file found is a normal file
};

// Begin the search and find the first file
void* fio_FindFirstFile( LPTSTR FilePath, LPfio_FindFileData FileData );

// Continue the search and find the next file
BOOL  fio_FindNextFile( void* hFind, LPfio_FindFileData FileData );

// End the search
void  fio_FindClose( void* hFind );


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_FILEIO_H_)
