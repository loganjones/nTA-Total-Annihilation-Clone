// std_FileIO_macOS.cpp //                         \author Logan Jones
/////////////////////////                             \date 9/13/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "std_FileIO.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#import <Foundation/Foundation.h>


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
void* fio_OpenFile( LPCTSTR strPath, enum fio_File_Flags flgFileAccess )
{
    // Assign the appropriate values for the mode flags.
    // NOTE: We assume that all files will be opened in "binary"
    //       mode. This should prevent the in+out bits from being mistranslated.
    const char *mode;
	switch( flgFileAccess )
	{
	case fio_READ:
		mode = "rb";
		break;
    case fio_WRITE:
        mode = "rb";
		break;
    case fio_READ_WRITE:
        mode = "w+b";
		break;
	}
    
    // Attempt to open/create the file
    FILE *file = fopen(strPath, mode);

    // Return the file's HANDLE
	return file;
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
    if (hFile) fclose((FILE *)hFile);
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
    if (!hFile) return 0;

    // Determine the opened file's size by seeking to the end of the file
    // and then checking the cursor's offset.
    // (Don't forget to restore the cursor to where it was before calling fio_GetFileSize())
    FILE *file = (FILE *)hFile;
    const long cursor = ftell(file);
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, cursor, SEEK_SET);
    
    return size;
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
    fseek((FILE *)hFile, Offset, SEEK_SET);
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
    // Read data from the file into the given buffer
    const size_t bytesRead = fread(Dest_Buffer, 1, BytesToRead, (FILE *)hFile);

    // Return the total amount of bytes read
	return bytesRead;
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
    // Write the data to the file
    const size_t bytesWritten = fwrite(Src_Buffer, 1, BytesToWrite, (FILE *)hFile);

    // Return the amount of bytes written
	return bytesWritten;
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
    struct stat sb;
    
    // Get the file status of strDir.
    if (stat(strDir, &sb) == -1) {
        return FALSE;
    }
    
    // Check the mode for the directory flag.
    return S_ISDIR(sb.st_mode);
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
//    char Drives[MAX_PATH];
//
//    // Get all the drives
//	if( GetLogicalDriveStrings(MAX_PATH,Drives)==0 )
//    {
//        strDrives[0] = '\0';
//        return 0;
//    }
//
//    DWORD   StrLength = 0;
//	LPTSTR	Drive = Drives;
    LPTSTR  CDDrive=strDrives;

//    // Check each one
//	for( UINT Type; Drive[0]!='\0'; Drive+=(strlen(Drive)+1))
//	{
//		Type = GetDriveType(Drive);
//
//        // Check for a CDROM
//		if( Type==DRIVE_CDROM )
//		{
//            // Copy it to the buffer
//            strcpy( CDDrive, Drive );
//            CDDrive += strlen(Drive)+1;
//            StrLength += strlen(Drive)+1;
//		}
//
//    } // End for( Drives )

    // Add a last NULL character
    CDDrive[0] = '\0';

    // Return the length of the buffer, could be 0 if no CDROM's were found
    return 0;
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
void* fio_FindFirstFile( LPTSTR FilePath, LPfio_FindFileData FileData )
{
    void* hFind = NULL;
    
    @autoreleasepool {
    
        NSFileManager *fm = [NSFileManager defaultManager];
        
        NSDirectoryEnumerator<NSURL *> *e = [fm enumeratorAtURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:FilePath]]
                                     includingPropertiesForKeys:@[NSURLNameKey, NSURLIsDirectoryKey, NSURLFileSizeKey]
                                                        options:NSDirectoryEnumerationSkipsHiddenFiles
                                                   errorHandler:^BOOL(NSURL * _Nonnull url, NSError * _Nonnull error) {
                                             if (error) { return NO; }
                                             return YES;
                                             }
                                             ];
        
        NSURL *first = e.nextObject;
        if (first) {
            NSString *fileName;
            [first getResourceValue:&fileName forKey:NSURLNameKey error:nil];
            NSNumber *isDirectory;
            [first getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:nil];
            NSNumber *fileSize;
            [first getResourceValue:&fileSize forKey:NSURLFileSizeKey error:nil];
            
            if (FileData) {
                if (fileName) { strcpy(FileData->Name, fileName.UTF8String); } else { FileData->Name[0] = '\0'; }
                FileData->Size = fileSize ? fileSize.integerValue : 0;
                FileData->Attributes = isDirectory.boolValue ? fio_FFA_DIRECTORY : fio_FFA_NORMAL;
            }
            
            hFind = (void *)CFBridgingRetain(e);
        }
        
    }
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
    if (!hFind) { return FALSE; }
    
    BOOL result = NO;
    @autoreleasepool {
        
        NSDirectoryEnumerator<NSURL *> *e = (__bridge NSDirectoryEnumerator<NSURL *> *)(hFind);
        
        NSURL *next = e.nextObject;
        if (next) {
            NSString *fileName;
            [next getResourceValue:&fileName forKey:NSURLNameKey error:nil];
            NSNumber *isDirectory;
            [next getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:nil];
            NSNumber *fileSize;
            [next getResourceValue:&fileSize forKey:NSURLFileSizeKey error:nil];
            
            if (FileData) {
                if (fileName) { strcpy(FileData->Name, fileName.UTF8String); } else { FileData->Name[0] = '\0'; }
                FileData->Size = fileSize ? fileSize.integerValue : 0;
                FileData->Attributes = isDirectory.boolValue ? fio_FFA_DIRECTORY : fio_FFA_NORMAL;
            }
            
            result = YES;
        }
    }
    return result;
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
    if (hFind) {
        CFBridgingRelease(hFind);
    }
}
// End fio_FindClose()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_FileIO_macOS.cpp //
///////////////////////////////