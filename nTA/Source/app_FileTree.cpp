// app_FileTree.cpp //                             \author Logan Jones
/////////////////////                                 \date 9/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "sys.h"
#include "app_FileTree.h"
#include "ta_HPI.h"
#include "zlib.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
app_FileTree::app_FileTree():
	m_theTree( NULL ),
	m_NumScanDirectories( 0 ),
	m_CurrentLibraryPathIndex( 0 )
{}
app_FileTree::~app_FileTree()
{
	// Safety precautions
	ClearScanDirectories();
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::Create() //                       \author Logan Jones
////////////////////////////                           \date 9/14/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_FileTree::Create()
{
	ScanDirList_t::iterator			Scans;
	ScanExtList_t::iterator			Types;
	LPTSTR							pStr;
	char							FindPath[ MAX_PATH ];
	LPVOID							hFileFind;
	fio_FindFileData				FindData;
	BOOL							bFind;
	char							FilePath[ MAX_PATH ];

	// Reset the index to the library paths
	m_CurrentLibraryPathIndex = 0;

	// Create the root directory
	m_theTree = AddDirectory( "$$ROOT" );

	// Itererate through each scan directory
	for( Scans = m_ScanDirectories.begin(); Scans!=m_ScanDirectories.end(); ++Scans)
	{
		// Check for the char flag that indicates that this is not a regular directory
		// and that we are supposed to do something special
		if( (*Scans).strPath[0]=='?' )
		{
			// Get the option string
			pStr = (*Scans).strPath + 1;

			// What are we to do
			if( strnicmp(pStr,"CD_DIRS",7)==0 )
			{
				ScanDir_t		NewDir;

				// For this option we need to query the system for all
				// CD-Rom drives in the system, and then add those paths to m_ScanDirectories
				// for them to be scanned.

				// Clone the settings from this scan dir
				NewDir.Flags = (*Scans).Flags | SDF_PRUNE_AFTER_CREATION;
				NewDir.Types = (*Scans).Types;

				// Get a string buffer containing all the CD-Rom drives present
				fio_GetCDDriveStrings( FindPath );

				// Move up to the rest of the path (if any)
				pStr = strchr( (*Scans).strPath, '/' );

				// Iterate through each path given
				for( LPTSTR pPath=FindPath; pPath&&(pPath[0]!='\0'); pPath+=strlen(pPath)+1)
				{
					if( pPath[strlen(pPath)-1]=='\\' || pPath[strlen(pPath)-1]=='/' )
					{
						strncpy( NewDir.strPath, pPath, strlen(pPath) - 1 );
						NewDir.strPath[ strlen(pPath) - 1 ] = '\0';
					}
					else strcpy( NewDir.strPath, pPath );
					if( pStr ) strcat( NewDir.strPath, pStr );
					m_ScanDirectories.push_back( NewDir );
					m_NumScanDirectories++;
				} // end for( CD Drive Strings )

				// Move to the new scan dir
				continue;
			}

		} // end if( (*Scans).strPath[0]=='?' )

		// Check if this dir needs certification
		if( (*Scans).Flags & SDF_VERIFY )
		{
		} // end if( bCertify )

		// Iterate through each type
		for( Types = (*Scans).Types.begin(); Types!=(*Scans).Types.end(); ++Types)
		{
			// Check for a char flag that indicates that this is not a regular extension
			// and that we are supposed to do something special
			if( (*Types).strExtension[0]=='?' )
			{
				STRING_SWITCH( (*Types).strExtension + 1 )
					CASE( "ADD_BRANCH" )
						AddDirToTree( (*Scans).strPath, TRUE, -1, (*Types).Priority, m_theTree );
					CASE( "ADD_DIR" )
						AddDirToTree( (*Scans).strPath, TRUE, 0, (*Types).Priority, m_theTree );
					CASE( "ADD_SUB_BRANCH" )
						AddDirToTree( (*Scans).strPath, FALSE, -1, (*Types).Priority, m_theTree );
					CASE( "ADD_SUB_DIR" )
						AddDirToTree( (*Scans).strPath, FALSE, 1, (*Types).Priority, m_theTree );
				END_STRING_SWITCH
			}
			else // Just a normal extension
			{
				// Configure the find string
				sprintf( FindPath, "%s/*.%s", (*Scans).strPath, (*Types).strExtension );

				// Begin the search
				hFileFind = fio_FindFirstFile( FindPath, &FindData );
				bFind = (hFileFind)? TRUE:FALSE;
				while( bFind )
				{
					// Configure the file string
					sprintf( FilePath, "%s/%s", (*Scans).strPath, FindData.Name );

					// Try to add it to the tree
					if( AddLibraryToTree(FilePath,(*Types).Priority) )
					{
						strcpy( m_LibraryPaths + m_CurrentLibraryPathIndex, FilePath );
						m_CurrentLibraryPathIndex += strlen( FilePath ) + 1;
					}

					// Find the next entry
					bFind = fio_FindNextFile( hFileFind, &FindData );

				} // end while( bFind )
				fio_FindClose( hFileFind );

			} // end else // Just a normal extension

		} // end for( Types )

	} // end for( Scans )

	// Erase any scan directories that were marked during creation
	PruneScanDirectories();

	return TRUE;
}
// End app_FileTree::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::Destroy() //                      \author Logan Jones
/////////////////////////////                          \date 9/14/2001
//               
//====================================================================
//
void app_FileTree::Destroy()
{
	DeleteBranch( &m_theTree );
}
// End app_FileTree::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddDirectoryToScan() //           \author Logan Jones
////////////////////////////////////////               \date 9/14/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath - 
//  BOOL bCertify   - 
//
// Return: DWORD - 
//
DWORD app_FileTree::AddDirectoryToScan( LPCTSTR strPath, BOOL bCertify )
{
	// Check for error
	if( strPath==NULL ) return FALSE;

	// Fill out the new entry
	ScanDir_t		NewDir;
	strcpy( NewDir.strPath, strPath );
	NewDir.Flags = (bCertify) ? SDF_VERIFY : 0;
	NewDir.Types.clear();

	// Make sure that the last char of the Path is not '\'
	int Last = strlen( NewDir.strPath ) - 1;
	while( NewDir.strPath[ Last ]=='/' ) NewDir.strPath[ Last-- ] = '\0';

	// Add this entry to the list
	m_ScanDirectories.push_back( NewDir );
	m_NumScanDirectories++;

	// Return the current number of scan directories
	return m_NumScanDirectories;
}
// End app_FileTree::AddDirectoryToScan()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddFileType() //                  \author Logan Jones
/////////////////////////////////                      \date 9/14/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strExt - 
//  int iPriority  - 
//  DWORD dwDir    - 
//
void app_FileTree::AddFileType( LPCTSTR strExt, int iPriority, DWORD dwDir )
{
	ScanDir_t*		pDir;
	ScanExt_t		NewExt;

	// Check for error
	if( (strExt==NULL)||(m_ScanDirectories.empty()) ) return;

	// If desired, get the scan directory at the given index
	if( dwDir!=0 )
	{
		// Iterate to the proper index
		ScanDirList_t::iterator		itor = m_ScanDirectories.begin();
		for( DWORD i=1; (itor!=m_ScanDirectories.end())&&(i<dwDir); i++,++itor);
		if( itor!=m_ScanDirectories.end() )
			pDir = &(*itor);
		else pDir = &m_ScanDirectories.back();
	}
	else // Just get the last entered directory by default
		pDir = &m_ScanDirectories.back();

	// Fill out the new entry
	strcpy( NewExt.strExtension, strExt );
	NewExt.Priority = iPriority;

	// Add this entry to the list
	pDir->Types.push_back( NewExt );
}
// End app_FileTree::AddFileType()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddDirectoryAsLib() //            \author Logan Jones
///////////////////////////////////////                \date 9/14/2001
//               
//====================================================================
// Parameters:
//  int iPriority    - 
//  BOOL bAddSubDirs - 
//  DWORD dwDir      - 
//
void app_FileTree::AddDirectoryAsLib( int iPriority, BOOL bAddSubDirs, DWORD dwDir )
{
	ScanDir_t*		pDir;
	ScanExt_t		NewExt;

	// Sanity check, there has to be something to add this to
	if( m_ScanDirectories.empty() )
		return;

	// If desired, get the scan directory at the given index
	if( dwDir!=0 )
	{
		// Iterate to the proper index
		ScanDirList_t::iterator		itor = m_ScanDirectories.begin();
		for( DWORD i=1; (itor!=m_ScanDirectories.end())&&(i<dwDir); i++,++itor);
		if( itor!=m_ScanDirectories.end() )
			pDir = &(*itor);
		else pDir = &m_ScanDirectories.back();
	}
	else // Just get the last entered directory by default
		pDir = &m_ScanDirectories.back();

	// Fill out the new ext entry
	if( bAddSubDirs ) strcpy( NewExt.strExtension, "?ADD_BRANCH" );
	else strcpy( NewExt.strExtension, "?ADD_DIR" );
	NewExt.Priority = iPriority;

	// Add this entry to the list
	pDir->Types.push_back( NewExt );
}
// End app_FileTree::AddDirectoryAsLib()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddSubDirectoriesAsLibs() //      \author Logan Jones
/////////////////////////////////////////////          \date 9/14/2001
//               
//====================================================================
// Parameters:
//  int iPriority    - 
//  BOOL bAddSubDirs - 
//  DWORD dwDir      - 
//
void app_FileTree::AddSubDirectoriesAsLibs( int iPriority, BOOL bAddSubDirs, DWORD dwDir )
{
	ScanDir_t*		pDir;
	ScanExt_t		NewExt;

	// Sanity check, there has to be something to add this to
	if( m_ScanDirectories.empty() )
		return;

	// If desired, get the scan directory at the given index
	if( dwDir!=0 )
	{
		// Iterate to the proper index
		ScanDirList_t::iterator		itor = m_ScanDirectories.begin();
		for( DWORD i=1; (itor!=m_ScanDirectories.end())&&(i<dwDir); i++,++itor);
		if( itor!=m_ScanDirectories.end() )
			pDir = &(*itor);
		else pDir = &m_ScanDirectories.back();
	}
	else // Just get the last entered directory by default
		pDir = &m_ScanDirectories.back();

	// Fill out the new ext entry
	if( bAddSubDirs ) strcpy( NewExt.strExtension, "?ADD_SUB_BRANCH" );
	else strcpy( NewExt.strExtension, "?ADD_SUB_DIR" );
	NewExt.Priority = iPriority;

	// Add this entry to the list
	pDir->Types.push_back( NewExt );
}
// End app_FileTree::AddSubDirectoriesAsLibs()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::ClearScanDirectories() //         \author Logan Jones
//////////////////////////////////////////             \date 9/14/2001
//               
//====================================================================
//
void app_FileTree::ClearScanDirectories()
{
	m_ScanDirectories.clear();
}
// End app_FileTree::ClearScanDirectories()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::PruneScanDirectories() //         \author Logan Jones
//////////////////////////////////////////             \date 9/19/2001
//               
//====================================================================
//
void app_FileTree::PruneScanDirectories()
{
	ScanDirList_t::iterator			Scans;

	for( Scans = m_ScanDirectories.begin(); Scans!=m_ScanDirectories.end();)
	{
		if( (*Scans).Flags & SDF_PRUNE_AFTER_CREATION )
		{
			Scans = m_ScanDirectories.erase( Scans );
			m_NumScanDirectories--;
		}
		else ++Scans;
	}
}
// End app_FileTree::PruneScanDirectories()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::DumpFileToBuffer() //             \author Logan Jones
//////////////////////////////////////                 \date 9/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strFilePath     - 
//  BYTE** ppBuffer         - 
//  LPReadProc_t lpReadProc - 
//  LPVOID lpProcVal        - 
//
// Return: DWORD - 
//
DWORD app_FileTree::DumpFileToBuffer( LPCTSTR strFilePath, BYTE** ppBuffer, LPReadProc_t lpReadProc, LPVOID lpProcVal )
{
	return DumpFileToBuffer( RetrieveFile( strFilePath, m_theTree ), ppBuffer, lpReadProc, lpProcVal );
}
// End app_FileTree::DumpFileToBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::DumpFileToBuffer() //             \author Logan Jones
//////////////////////////////////////                 \date 2/13/2002
//               
//====================================================================
// Parameters:
//  HFILEENTRY pFile        - 
//  BYTE** ppBuffer         - 
//  LPReadProc_t lpReadProc - 
//  LPVOID lpProcVal        - 
//
// Return: DWORD - 
//
DWORD app_FileTree::DumpFileToBuffer( HFILEENTRY pFile, BYTE** ppBuffer, LPReadProc_t lpReadProc, LPVOID lpProcVal )
{
	char				Path[ MAX_PATH ];
	void*				hHostFile;
	DWORD				TotalBytes = 0;
#define ReadProc( A, B )		{ if( lpReadProc ) lpReadProc( (A), (B), lpProcVal ); }
	ReadProc( 1, 0 );

	// Sanity check
	if( (pFile==NULL)||(ppBuffer==NULL) )
	{
		return 0;
	}

	// Configure the path
	if( pFile->Type==SYS_FILE )
		sprintf( Path, "%s/%s", m_LibraryPaths + pFile->LibraryPathIndex, pFile->Name );
	else strcpy( Path, m_LibraryPaths + pFile->LibraryPathIndex );

	// Open the host file
	hHostFile = fio_OpenFile( Path );
	if( hHostFile==NULL )
	{
		return 0;
	}

	// Allocate the new buffer
	(*ppBuffer) = new BYTE[ pFile->DecompressedSize ];
	if( (*ppBuffer)==NULL )
	{
		fio_CloseFile( hHostFile );
		return 0;
	}

	// What kind of file is the data stored in
	switch( pFile->Type )
	{
		case TA_FILE: // Uncompressed TA file
			ReadAndDecrypt( hHostFile,
							pFile->Key,
							pFile->Offset,
							(*ppBuffer),
							pFile->DecompressedSize );
			TotalBytes = pFile->DecompressedSize;
			break;
		// end case TA_FILE

		case TA_FILE_LZ77: // Compressed TA file
		case TA_FILE_ZLIB: // Compressed TA file
		{
			#define SafeChunkBufferSize		( TA_HPI_CHUNK_DEFAULT_SIZE + 1024 )
			DWORD	Offset;
			DWORD*	ChunkSizes;
			BYTE*	AChunk;
			DWORD	NumChunks,x;
			
			//long Key = pFile->FileDate;
			// Get the offset and calculate the number of chunks used foe the file
			Offset = pFile->Offset;
			NumChunks = (pFile->DecompressedSize / TA_HPI_CHUNK_DEFAULT_SIZE) + 
						( (pFile->DecompressedSize % TA_HPI_CHUNK_DEFAULT_SIZE) ? 1:0 );

			// Allocate a buffer and read in the chunk sizes + advance the offset
			ChunkSizes = new DWORD[ NumChunks ];
			ReadAndDecrypt( hHostFile, pFile->Key, Offset, ChunkSizes, NumChunks*sizeof(DWORD) );
			Offset += NumChunks*sizeof(DWORD);

			// Make sure all the chunks will fit in the buffer
			#ifdef _DEBUG
				for( x=0; x<NumChunks; x++ )
					assertEx( SafeChunkBufferSize>ChunkSizes[x], "Chunk to large to fit in default buffer" );
			#endif

			// Allocat a buffer to read the compressed chunks into
			AChunk = new BYTE[ SafeChunkBufferSize ];

			// Iterate through each chunk
			for( x=0; x<NumChunks; x++ )
			{
				// Read in the chunk and advance the offset
				ReadAndDecrypt( hHostFile, pFile->Key, Offset, AChunk, ChunkSizes[x] );
				Offset += ChunkSizes[x];

				// Decompress
				TotalBytes += DeSQSH( AChunk, (*ppBuffer) + TotalBytes );

				// Update the progress
				ReadProc( NumChunks, x );

			} // end for( NumChunks )

			// Delete the buffers and move on
			delete [] AChunk;
			delete [] ChunkSizes;
			#undef SafeChunkBufferSize
			break;
		} // end case TA_FILE_LZ77, TA_FILE_ZLIB

		case TAK_FILE: // Uncompressed TAK file
			fio_FileSeek( hHostFile, pFile->Offset );
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case TAK_FILE

		case TAK_FILE_ZLIB: // Compressed TAK file
		{
			BYTE*	pChunk = new BYTE[ pFile->CompressedSize ];
			fio_FileSeek( hHostFile, pFile->Offset );
			fio_ReadFile( hHostFile, pChunk, pFile->CompressedSize );
			TotalBytes = DeSQSH( pChunk, (*ppBuffer) );
			delete [] pChunk;
			break;
		} // end case TAK_FILE_ZLIB

		case SYS_FILE: // Normal file system file
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case SYS_FILE

		case ZIP_FILE_UNCOMPRESSED: // Uncompressed zip file
			fio_FileSeek( hHostFile, pFile->Offset );
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case ZIP_FILE_UNCOMPRESSED

		case ZIP_FILE_DEFLATED:
		{
			z_stream	zlibStream;
			BYTE*		Buffer;
			DWORD		Offset;
			DWORD		CompressedSize;

			zlibStream.next_out = (*ppBuffer);
			zlibStream.avail_out = pFile->DecompressedSize;

			zlibStream.avail_in = 0;
			zlibStream.total_in = 0;

			zlibStream.zalloc = Z_NULL;
			zlibStream.zfree = Z_NULL;
			zlibStream.opaque = Z_NULL;

			if( inflateInit2( &zlibStream, -MAX_WBITS )!=Z_OK )
				break;

			Buffer = new BYTE[16384];
			if( Buffer==NULL )
				break;

			Offset = pFile->Offset;
			CompressedSize = pFile->CompressedSize;
			while( zlibStream.avail_out>0 )
			{
				if( zlibStream.avail_in==0 )
				{
					UINT uiBufferSize = (CompressedSize<16384) ? CompressedSize : 16384;
					fio_FileSeek( hHostFile, Offset );
					uiBufferSize = fio_ReadFile( hHostFile, Buffer, uiBufferSize );

					Offset += uiBufferSize;
					CompressedSize -= uiBufferSize;

					zlibStream.next_in = Buffer;
					zlibStream.avail_in = uiBufferSize;
				}
				
				if( inflate( &zlibStream, Z_SYNC_FLUSH )!=Z_OK )
					break;

				ReadProc( pFile->DecompressedSize, pFile->DecompressedSize - zlibStream.avail_out );
			}
			TotalBytes = pFile->DecompressedSize - zlibStream.avail_out;

			inflateEnd( &zlibStream );
			delete [] Buffer;
			break;
		} // end case ZIP_FILE_DEFLATED

	} // end switch( pFile->Type )

	fio_CloseFile( hHostFile );
	ReadProc( 1, 1 );
	return TotalBytes;
#undef ReadProc
}
// End app_FileTree::DumpFileToBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::DumpFileToBuffer() //             \author Logan Jones
//////////////////////////////////////                 \date 2/13/2002
//               
//====================================================================
// Parameters:
//  HFILEENTRY pFile        - 
//  BYTE** ppBuffer         - 
//  LPReadProc_t lpReadProc - 
//  LPVOID lpProcVal        - 
//
// Return: DWORD - 
//
template <class _Progress_t>
DWORD app_FileTree::DumpFileToBuffer( LPCTSTR strFilePath, BYTE** ppBuffer, _Progress_t Progress )
{
	char				Path[ MAX_PATH ];
	void*				hHostFile;
	DWORD				TotalBytes = 0;

	Progress.Update( 0, 1 );
    HFILEENTRY pFile = RetrieveFile( strFilePath, m_theTree );

	// Sanity check
	if( (pFile==NULL)||(ppBuffer==NULL) )
	{
		return 0;
	}

	// Configure the path
	if( pFile->Type==SYS_FILE )
		sprintf( Path, "%s/%s", m_LibraryPaths + pFile->LibraryPathIndex, pFile->Name );
	else strcpy( Path, m_LibraryPaths + pFile->LibraryPathIndex );

	// Open the host file
	hHostFile = fio_OpenFile( Path );
	if( hHostFile==NULL )
	{
		return 0;
	}

	// Allocate the new buffer
	(*ppBuffer) = new BYTE[ pFile->DecompressedSize ];
	if( (*ppBuffer)==NULL )
	{
		fio_CloseFile( hHostFile );
		return 0;
	}

	// What kind of file is the data stored in
	switch( pFile->Type )
	{
		case TA_FILE: // Uncompressed TA file
			ReadAndDecrypt( hHostFile,
							pFile->Key,
							pFile->Offset,
							(*ppBuffer),
							pFile->DecompressedSize );
			TotalBytes = pFile->DecompressedSize;
			break;
		// end case TA_FILE

		case TA_FILE_LZ77: // Compressed TA file
		case TA_FILE_ZLIB: // Compressed TA file
		{
			#define SafeChunkBufferSize		( TA_HPI_CHUNK_DEFAULT_SIZE + 1024 )
			DWORD	Offset;
			DWORD*	ChunkSizes;
			BYTE*	AChunk;
			DWORD	NumChunks,x;
			
			//long Key = pFile->FileDate;
			// Get the offset and calculate the number of chunks used foe the file
			Offset = pFile->Offset;
			NumChunks = (pFile->DecompressedSize / TA_HPI_CHUNK_DEFAULT_SIZE) + 
						( (pFile->DecompressedSize % TA_HPI_CHUNK_DEFAULT_SIZE) ? 1:0 );

			// Allocate a buffer and read in the chunk sizes + advance the offset
			ChunkSizes = new DWORD[ NumChunks ];
			ReadAndDecrypt( hHostFile, pFile->Key, Offset, ChunkSizes, NumChunks*sizeof(DWORD) );
			Offset += NumChunks*sizeof(DWORD);

			// Make sure all the chunks will fit in the buffer
			#ifdef _DEBUG
				for( x=0; x<NumChunks; x++ )
					assertEx( SafeChunkBufferSize>ChunkSizes[x], "Chunk to large to fit in default buffer" );
			#endif

			// Allocat a buffer to read the compressed chunks into
			AChunk = new BYTE[ SafeChunkBufferSize ];

			// Iterate through each chunk
			for( x=0; x<NumChunks; x++ )
			{
				// Read in the chunk and advance the offset
				ReadAndDecrypt( hHostFile, pFile->Key, Offset, AChunk, ChunkSizes[x] );
				Offset += ChunkSizes[x];

				// Decompress
				TotalBytes += DeSQSH( AChunk, (*ppBuffer) + TotalBytes );

				// Update the progress
				Progress.Update( x, NumChunks );

			} // end for( NumChunks )

			// Delete the buffers and move on
			delete [] AChunk;
			delete [] ChunkSizes;
			#undef SafeChunkBufferSize
			break;
		} // end case TA_FILE_LZ77, TA_FILE_ZLIB

		case TAK_FILE: // Uncompressed TAK file
			fio_FileSeek( hHostFile, pFile->Offset );
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case TAK_FILE

		case TAK_FILE_ZLIB: // Compressed TAK file
		{
			BYTE*	pChunk = new BYTE[ pFile->CompressedSize ];
			fio_FileSeek( hHostFile, pFile->Offset );
			fio_ReadFile( hHostFile, pChunk, pFile->CompressedSize );
			TotalBytes = DeSQSH( pChunk, (*ppBuffer) );
			delete [] pChunk;
			break;
		} // end case TAK_FILE_ZLIB

		case SYS_FILE: // Normal file system file
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case SYS_FILE

		case ZIP_FILE_UNCOMPRESSED: // Uncompressed zip file
			fio_FileSeek( hHostFile, pFile->Offset );
			TotalBytes = fio_ReadFile( hHostFile, (*ppBuffer), pFile->DecompressedSize );
			break;
		// end case ZIP_FILE_UNCOMPRESSED

		case ZIP_FILE_DEFLATED:
		{
			z_stream	zlibStream;
			BYTE*		Buffer;
			DWORD		Offset;
			DWORD		CompressedSize;

			zlibStream.next_out = (*ppBuffer);
			zlibStream.avail_out = pFile->DecompressedSize;

			zlibStream.avail_in = 0;
			zlibStream.total_in = 0;

			zlibStream.zalloc = Z_NULL;
			zlibStream.zfree = Z_NULL;
			zlibStream.opaque = Z_NULL;

			if( inflateInit2( &zlibStream, -MAX_WBITS )!=Z_OK )
				break;

			Buffer = new BYTE[16384];
			if( Buffer==NULL )
				break;

			Offset = pFile->Offset;
			CompressedSize = pFile->CompressedSize;
			while( zlibStream.avail_out>0 )
			{
				if( zlibStream.avail_in==0 )
				{
					UINT uiBufferSize = (CompressedSize<16384) ? CompressedSize : 16384;
					fio_FileSeek( hHostFile, Offset );
					uiBufferSize = fio_ReadFile( hHostFile, Buffer, uiBufferSize );

					Offset += uiBufferSize;
					CompressedSize -= uiBufferSize;

					zlibStream.next_in = Buffer;
					zlibStream.avail_in = uiBufferSize;
				}
				
				if( inflate( &zlibStream, Z_SYNC_FLUSH )!=Z_OK )
					break;

                Progress.Update( pFile->DecompressedSize, pFile->DecompressedSize - zlibStream.avail_out );
			}
			TotalBytes = pFile->DecompressedSize - zlibStream.avail_out;

			inflateEnd( &zlibStream );
			delete [] Buffer;
			break;
		} // end case ZIP_FILE_DEFLATED

	} // end switch( pFile->Type )

	fio_CloseFile( hHostFile );
	Progress.Update( 1, 1 );
	return TotalBytes;
}
// End app_FileTree::DumpFileToBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::OpenFile() //                     \author Logan Jones
//////////////////////////////                         \date 9/15/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strFilePath - 
//
// Return: HFILE - 
//
app_FileTree::HFILE app_FileTree::OpenFile( LPTSTR strFilePath )
{
	// Error check
	if( strFilePath==NULL ) return NULL;

	// Return the result of RetrieveFile()
	return (HFILE)RetrieveFile( strFilePath, m_theTree );
}
// End app_FileTree::OpenFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::GetFileSize() //                  \author Logan Jones
/////////////////////////////////                      \date 9/15/2001
//               
//====================================================================
// Parameters:
//  HFILE hFile - 
//
// Return: DWORD - 
//
DWORD app_FileTree::GetFileSize( HFILE hFile )
{
	// Return the decompressed size if the handle is valid
	//if( hFile ) return hFile->DecompressedSize;
	//else
	return 0;
}
// End app_FileTree::GetFileSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::ReadFile() //                     \author Logan Jones
//////////////////////////////                         \date 9/15/2001
//               
//====================================================================
// Parameters:
//  HFILE hFile             - 
//  void* pDestBuffer       - 
//  LPReadProc_t lpReadProc - 
//  LPVOID lpProcVal        - 
//
// Return: DWORD - 
//
DWORD app_FileTree::ReadFile( HFILE hFile, void* pDestBuffer, LPReadProc_t lpReadProc, LPVOID lpProcVal )
{
	return 0;
}
// End app_FileTree::ReadFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::ReadFile() //                     \author Logan Jones
//////////////////////////////                         \date 9/15/2001
//               
//====================================================================
// Parameters:
//  HFILE hFile        - 
//  void* pDestBuffer  - 
//  DWORD dwSizeToRead - 
//
// Return: DWORD - 
//
DWORD app_FileTree::ReadFile( HFILE hFile, void* pDestBuffer, DWORD dwSizeToRead )
{
	return 0;
}
// End app_FileTree::ReadFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::CloseFile() //                    \author Logan Jones
///////////////////////////////                        \date 9/15/2001
//               
//====================================================================
// Parameters:
//  HFILE hFile - 
//
void app_FileTree::CloseFile( HFILE hFile )
{
}
// End app_FileTree::CloseFile()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::GetEntryCountInDirectory() //     \author Logan Jones
//////////////////////////////////////////////          \date 4/3/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strWhere - 
//
// Return: DWORD - 
//
DWORD app_FileTree::GetEntryCountInDirectory( LPTSTR strWhere )
{
	DirEntry_t*		pWhere;
	DirEntry_t*		pDir;
	FileEntry_t*	pFile;
	DWORD			count;

	// Retrieve the directory with which to search
	if( (strWhere==NULL)||(strWhere[0]=='\0') ) pWhere = m_theTree;
	else pWhere = RetrieveDirectory( strWhere, m_theTree );

	// Count the directories
	for( count=0,pDir=pWhere->SubDirs; pDir; ++count,pDir=pDir->Next);

	// Count the files
	for( pFile=pWhere->SubFiles; pFile; ++count,pFile=pFile->Next);

	// Return the count
	return count;
}
// End app_FileTree::GetEntryCountInDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::GetFileCountInDirectory() //      \author Logan Jones
/////////////////////////////////////////////           \date 4/3/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strWhere - 
//
// Return: DWORD - 
//
DWORD app_FileTree::GetFileCountInDirectory( LPTSTR strWhere )
{
	DirEntry_t*		pWhere;
	FileEntry_t*	pFile;
	DWORD			count;

	// Retrieve the directory with which to search
	if( (strWhere==NULL)||(strWhere[0]=='\0') ) pWhere = m_theTree;
	else pWhere = RetrieveDirectory( strWhere, m_theTree );

	// Count the files
	for( count=0,pFile=pWhere->SubFiles; pFile; ++count,pFile=pFile->Next);

	// Return the count
	return count;
}
// End app_FileTree::GetFileCountInDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::GetDirectoryCountInDirectory() // \author Logan Jones
//////////////////////////////////////////////////      \date 4/3/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strWhere - 
//
// Return: DWORD - 
//
DWORD app_FileTree::GetDirectoryCountInDirectory( LPTSTR strWhere )
{
	DirEntry_t*		pWhere;
	DirEntry_t*		pDir;
	DWORD			count;

	// Retrieve the directory with which to search
	if( (strWhere==NULL)||(strWhere[0]=='\0') ) pWhere = m_theTree;
	else pWhere = RetrieveDirectory( strWhere, m_theTree );

	// Count the directories
	for( count=0,pDir=pWhere->SubDirs; pDir; ++count,pDir=pDir->Next);

	// Return the count
	return count;
}
// End app_FileTree::GetDirectoryCountInDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::Find() //                         \author Logan Jones
//////////////////////////                             \date 9/16/2001
//               
//====================================================================
// Parameters:
//  FindType_t What     - 
//  LPCTSTR strWith     - 
//  LPCTSTR strWhere    - 
//  FindData_t* pAnswer - 
//
// Return: HFIND - 
//
app_FileTree::HFIND app_FileTree::Find( FindType_t What, LPCTSTR strWith, LPCTSTR strWhere, FindData_t* pAnswer )
{
	DirEntry_t*		pDir;

	// Retrieve the directory with which to search
	if( (strWhere==NULL)||(strWhere[0]=='\0') ) pDir = m_theTree;
	else pDir = RetrieveDirectory( strWhere, m_theTree );

	// Return a pointer to the structure
	return Find( What, strWith, pDir, pAnswer );
}
// End app_FileTree::Find()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::Find() //                         \author Logan Jones
//////////////////////////                             \date 9/16/2001
//               
//====================================================================
// Parameters:
//  FindType_t What     - 
//  LPCTSTR strWith     - 
//  HDIRENTRY hWhere    - 
//  FindData_t* pAnswer - 
//
// Return: HFIND - 
//
app_FileTree::HFIND app_FileTree::Find( FindType_t What, LPCTSTR strWith, HDIRENTRY hWhere, FindData_t* pAnswer )
{
	DirEntry_t*		pDir = static_cast<DirEntry_t*>(hWhere);
	FindHandle_t*	hFind;

	// Make sure its valid
	if( (pDir==NULL) || ((pDir->SubDirs==NULL)&&(pDir->SubFiles==NULL)) )
		return NULL;

	// Edit out some impossible situations
	if( (What==FILES_WITH_NAMES)&&(strWith==NULL) )
		What = FILES;

	// Allocate and fill out the structure
	hFind = new FindHandle_t;
	hFind->What = What;
	if( strWith ) strcpy( hFind->With, strWith ); else hFind->With[0]='\0';
	hFind->Where = pDir;

	// Set the search pointers
	switch( What )
	{
		case EVERYTHING:
			hFind->DirToSearch = pDir->SubDirs;
			hFind->FileToSearch = pDir->SubFiles;
			break;
		case FILES_WITH_EXTENSIONS:
		case FILES_WITH_NAMES:
		case FILES:
			hFind->DirToSearch = NULL;
			hFind->FileToSearch = pDir->SubFiles;
			break;
		case DIRECTORIES:
			hFind->DirToSearch = pDir->SubDirs;
			hFind->FileToSearch = NULL;
			break;
		default:
			CloseFind( hFind );
			return NULL;
	}

	// Start the search
	if( ContinueFind( hFind, pAnswer )==FALSE )
	{
		// Nothing found, shut down the search
		CloseFind( hFind );
		return NULL;
	}

	// Return a pointer to the structure
	return hFind;
}
// End app_FileTree::Find()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::ContinueFind() //                 \author Logan Jones
//////////////////////////////////                     \date 9/16/2001
//               
//====================================================================
// Parameters:
//  HFIND hFind         - 
//  FindData_t* pAnswer - 
//
// Return: BOOL - 
//
BOOL app_FileTree::ContinueFind( HFIND hFind, FindData_t* pAnswer )
{
	// Sanity check
	if( (hFind==NULL)||(pAnswer==NULL) )
		return NULL;

	// Check the search dir
	if( hFind->DirToSearch )
	{
		DirEntry_t* pDir = static_cast<DirEntry_t*>(hFind->DirToSearch);

		// Fill out the answer
		pAnswer->bIsFile = FALSE;
		pAnswer->Name = pDir->Name;
		pAnswer->Size = GetDirSize( pDir );
		pAnswer->hDir = pDir;
		pAnswer->hFile = NULL;

		// Set the next search target
		hFind->DirToSearch = pDir->Next;

		// Return the handle
		return TRUE;
	}

	// Check the search file
	FileEntry_t* pFile = static_cast<FileEntry_t*>(hFind->FileToSearch);
	for( ; pFile; pFile=pFile->Next )
	{
		LPTSTR strPtr = strrchr( pFile->Name, '.' );
		if( hFind->What==FILES_WITH_EXTENSIONS )
		{
			if( hFind->With[0]=='\0' )
			{
				if( strPtr!=NULL )
					continue;
			}
			else
			{
				if( strPtr==NULL )
					continue;
				strPtr++;
				if( stricmp(strPtr,hFind->With)!=0 )
					continue;
			}
		}
		else if( hFind->What==FILES_WITH_NAMES )
		{
			char SubStr[64];
			if( strPtr==NULL )
				strcpy( SubStr, pFile->Name );
			else
			{
				strncpy( SubStr, pFile->Name, strPtr - pFile->Name );
				SubStr[ strPtr - pFile->Name ] = '\0';
			}
			if( stricmp(SubStr,hFind->With)!=0 )
				continue;
		}

		// Fill out the answer
		pAnswer->bIsFile = TRUE;
		pAnswer->Name = pFile->Name;
		pAnswer->Size = pFile->DecompressedSize;
		pAnswer->hDir = NULL;
		pAnswer->hFile = pFile;

		// Set the next search target
		hFind->FileToSearch = pFile->Next;

		// Return the handle
		return TRUE;

	} // end while( pFile )

	// Nothing left
	return FALSE;
}
// End app_FileTree::ContinueFind()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::CloseFind() //                    \author Logan Jones
///////////////////////////////                        \date 9/16/2001
//               
//====================================================================
// Parameters:
//  HFIND& hFind - 
//
void app_FileTree::CloseFind( HFIND& hFind )
{
	if( hFind )
	{
		delete hFind;
		hFind = NULL;
	}
}
// End app_FileTree::CloseFind()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddLibraryToTree() //             \author Logan Jones
//////////////////////////////////////                 \date 9/15/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath - 
//  int iPriority   - 
//
// Return: BOOL - 
//
BOOL app_FileTree::AddLibraryToTree( LPCTSTR strPath, int iPriority )
{
	void*						hFile;
	TA_HPI_HEADER				Header;
	BOOL						bResult;

	// Open the file
	hFile = fio_OpenFile( strPath, fio_READ );

	// Make sure its valid
	if( hFile==NULL )
		return FALSE;

	// Read the first part into a TA_HPI_HEADER structure
	fio_ReadFile( hFile, &Header, sizeof(TA_HPI_HEADER) );

	// Check if this an HPI or not
	if( Header.HPIMarker!=TA_HPI_MARKER )
	{
		// This still might be a zip file so check that
		if( Header.HPIMarker==ZIP_LOCAL_FILE_HEADER_SIGNATURE )
		{
			// Seek back to the beginning of the file
			fio_FileSeek( hFile, 0 );

			// Add this zip to the tree
			bResult = AddZipToTree( hFile, iPriority );
		}
		else // not a zip
		{
			bResult = FALSE;
		}

	}
	// This is an HPI
	else switch( Header.Version )
	{
		case TA_HPI_TOTAL_ANNIHILATION: // TA style HPI

			TA_HPI_EXT_HEADER	TAHeader;
			BYTE*				Dir;

			// Read in the extended header
			fio_ReadFile( hFile, &TAHeader, sizeof(TA_HPI_EXT_HEADER) );

			// Calculate the key
			if( TAHeader.HeaderKey!=0 )
				TAHeader.HeaderKey = ~( (TAHeader.HeaderKey * 4) | (TAHeader.HeaderKey >> 6) );

			// Allocate some memory for the directory
			Dir = new BYTE[ TAHeader.DirectorySize ];
			if( Dir==NULL )
			{
				assertEx( 0, "Not enough memory to allocate TA_HPI central directory" );
				bResult = FALSE;
				break;
			}

			// Read in the directory
			if( !ReadAndDecrypt(
					hFile,
					TAHeader.HeaderKey,
					TAHeader.OffsetToDirectory,
					Dir + TAHeader.OffsetToDirectory,
					TAHeader.DirectorySize - TAHeader.OffsetToDirectory) )
			{
				assertEx( 0, "Error reading and decrypting TA_HPI central directory" );
				delete [] Dir;
				bResult = FALSE;
				break;
			}

			// Add the TA_HPI library to the tree
			bResult = AddTAHpiToTree(
				hFile,
				Dir,
				TAHeader.HeaderKey,
				TAHeader.OffsetToDirectory,
				iPriority,
				m_theTree );

			delete [] Dir;
			break;

		case TA_HPI_KINGDOMS: // TA:K style HPI
			//assertEx( 0, "TA:K style HPIs are not yet supported" );
			//bResult = FALSE;
			TAK_HPI_EXT_HEADER	TAKHeader;
			BYTE*				pTemp;
			BYTE*				pDir;
			char*				pNames;

			// Read in the extended header
			fio_ReadFile( hFile, &TAKHeader, sizeof(TAK_HPI_EXT_HEADER) );

			// Allocate some memory for the directory
			pDir = new BYTE[ TAKHeader.DirectorySize ];
			if( pDir==NULL )
			{
				assertEx( 0, "Not enough memory to allocate TAK_HPI central directory" );
				bResult = FALSE;
				break;
			}

			// Read in the directory
			fio_FileSeek( hFile, TAKHeader.OffsetToDirectory );
			fio_ReadFile( hFile, pDir, TAKHeader.DirectorySize );

			// Check if it is "SQSHed" and decompress if neccessary
			if( IsSQSH(pDir) )
			{
				pTemp = pDir;
				pDir = new BYTE[ GetSQSHSize(pTemp) ];
				DeSQSH( pTemp, pDir );
				delete [] pTemp;
			}

			// Allocate some memory for the names
			pNames = new char[ TAKHeader.FileNameSize ];
			if( pNames==NULL )
			{
				assertEx( 0, "Not enough memory to allocate TAK_HPI names buffer" );
				bResult = FALSE;
				break;
			}

			// Read in the directory
			fio_FileSeek( hFile, TAKHeader.OffsetToFileNames );
			fio_ReadFile( hFile, pNames, TAKHeader.FileNameSize );

			// Check if it is "SQSHed" and decompress if neccessary
			if( IsSQSH((BYTE*)pNames) )
			{
				pTemp =(BYTE*)pNames;
				pNames = new char[ GetSQSHSize(pTemp) ];
				DeSQSH( pTemp, (BYTE*)pNames );
				delete [] pTemp;
			}

			// Add the TAK_HPI library to the tree
			bResult = AddTAKHpiToTree( hFile, pDir, pNames, 0, iPriority, m_theTree );

			delete [] pDir;
			delete [] pNames;
			break;

		default: // Version not recognized
			bResult = FALSE;
			break;

	} // end else switch( Header.Version )

	// Close the file
	fio_CloseFile( hFile );

	// Return the result
	return bResult;
}
// End app_FileTree::AddLibraryToTree()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddTAHpiToTree() //               \author Logan Jones
////////////////////////////////////                   \date 9/15/2001
//               
//====================================================================
// Parameters:
//  void* hFile          - 
//  BYTE* pCDir          - 
//  INT32 iKey           - 
//  UINT32 uiOffset      - 
//  int iPriority        - 
//  DirEntry_t* pRootDir - 
//
// Return: BOOL - 
//
BOOL app_FileTree::AddTAHpiToTree( void* hFile, BYTE* pCDir, INT32 iKey, UINT32 uiOffset, int iPriority, DirEntry_t* pRootDir )
{
	TA_HPI_DIR_HEADER*		pDirEntry;
	TA_HPI_ENTRY*			pEntry;
	UINT32					NumEntries;
	LPTSTR					pEntryName;
	BOOL					bResult = FALSE;

	// Get the pointers to the starting entries
	pDirEntry = (TA_HPI_DIR_HEADER*)(pCDir + uiOffset);
	pEntry = (TA_HPI_ENTRY*)(pCDir + pDirEntry->OffsetToEntryArray);

	// Iterate through each entry
	for( NumEntries=pDirEntry->NumberOfEntries; NumEntries>0; --NumEntries)
	{
		// Grab the entry name
		pEntryName = (LPTSTR)(pCDir + pEntry->OffsetToName);

		// What kind of entry is this
		switch( pEntry->EntryFlag )
		{
			case TA_HPI_ENTRY_DIRECTORY: // This is a directory

				DirEntry_t*		pDir;

				// Add this new dir to the tree
				pDir = AddDirectory( pEntryName, pRootDir );

				if( AddTAHpiToTree(
						hFile,
						pCDir,
						iKey,
						pEntry->OffsetToEntryData,
						iPriority,
						pDir ) )
					bResult = TRUE;
				break;

			case TA_HPI_ENTRY_FILE: // This is a file

				TA_HPI_FILE_ENTRY*	pFileEntry;
				FileEntry_t			FileData;

				// Get the file entry
				pFileEntry = (TA_HPI_FILE_ENTRY*)(pCDir + pEntry->OffsetToEntryData);

				// Fill out the file data
				strcpy( FileData.Name, pEntryName );
				FileData.Priority = iPriority;
				FileData.LibraryPathIndex = m_CurrentLibraryPathIndex;
				FileData.Offset = pFileEntry->OffsetToFileData;
				FileData.DecompressedSize = pFileEntry->FileSize;
				FileData.CompressedSize = 0;
				FileData.Key = iKey;

				// Set the type
				switch( pFileEntry->CompressionType )
				{
					case TA_HPI_FILE_NOT_COMPRESSED:
						FileData.Type = TA_FILE;
						break;

					case TA_HPI_FILE_LZ77:
						FileData.Type = TA_FILE_LZ77;
						break;

					default:
					case TA_HPI_FILE_ZLIB:
						FileData.Type = TA_FILE_ZLIB;
						break;
				}

				// Add the file to the tree
				if( AddFile(FileData,pRootDir)->Priority==FileData.Priority )
					bResult = TRUE;
				break;

		} // end switch( Entry->EntryFlag )

		++pEntry;

	} // end for( NumEntries )

	// Return the result
	return bResult;
}
// End app_FileTree::AddTAHpiToTree()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddTAKHpiToTree() //              \author Logan Jones
/////////////////////////////////////                  \date 9/16/2001
//               
//====================================================================
// Parameters:
//  void* hFile          - 
//  BYTE* pCDir          - 
//  char* pchNames       - 
//  UINT32 uiOffset      - 
//  int iPriority        - 
//  DirEntry_t* pRootDir - 
//
// Return: BOOL - 
//
BOOL app_FileTree::AddTAKHpiToTree( void* hFile, BYTE* pCDir, char* pchNames, UINT32 uiOffset, int iPriority, DirEntry_t* pRootDir )
{
	TAK_HPI_DIR_ENTRY*	RootEntry;
	TAK_HPI_FILE_ENTRY*	FileEntry;
	TAK_HPI_DIR_ENTRY*	DirEntry;
	DirEntry_t*			pDir;
	FileEntry_t			FileData;
	LPTSTR				EntryName;
	DWORD				EntryOffset;
	UINT32				NumEntries;
	BOOL				bUsedIndex = FALSE;

	// Grab the root entry structure
	RootEntry = (TAK_HPI_DIR_ENTRY*)(pCDir + uiOffset);

	// Save the offset and grab the entry structure
	EntryOffset = RootEntry->OffsetToSubDirectoryArray;
	DirEntry = (TAK_HPI_DIR_ENTRY*)(pCDir + EntryOffset);

	// Iterate through each sub dir
	for( NumEntries=RootEntry->NumberOfSubDirectories; NumEntries>0; --NumEntries,++DirEntry)
	{
		// Get the name of this dir
		EntryName = (LPTSTR)(pchNames + DirEntry->OffsetToDirectoryName);

		// Add it to the tree
		pDir = AddDirectory( EntryName, pRootDir );

		// Recurse this sub dir
		if( AddTAKHpiToTree( hFile, pCDir, pchNames, EntryOffset, iPriority, pDir ) )
			bUsedIndex = TRUE;

		// Advance the offset
		EntryOffset += sizeof(TAK_HPI_DIR_ENTRY);

	} // end for( NumberOfSubDirectories )

	// Grab the file entry structure
	FileEntry = (TAK_HPI_FILE_ENTRY*)(pCDir + RootEntry->OffsetToFileEntryArray);

	// Iterate through each file
	for( NumEntries=RootEntry->NumberOfFileEntries; NumEntries>0; --NumEntries,++FileEntry)
	{
		// Get the name of this dir
		EntryName = (LPTSTR)(pchNames + FileEntry->OffsetToFileName);

		// Fill out the file data
		strcpy( FileData.Name, EntryName );
		FileData.Priority = iPriority;
		FileData.LibraryPathIndex = m_CurrentLibraryPathIndex;
		FileData.Offset = FileEntry->OffsetToFileData;
		FileData.DecompressedSize = FileEntry->DecompressedSize;
		FileData.CompressedSize = FileEntry->CompressedSize;
		FileData.Key = FileEntry->Checksum;
		FileData.Type = (FileEntry->CompressedSize==0) ? TAK_FILE : TAK_FILE_ZLIB;

		// Add the file to the tree
		if( AddFile(FileData,pRootDir)->Priority==FileData.Priority )
			bUsedIndex = TRUE;
	}

	return bUsedIndex;
}
// End app_FileTree::AddTAKHpiToTree()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddZipToTree() //                 \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
//               
//====================================================================
// Parameters:
//  void* hFile   - 
//  int iPriority - 
//
// Return: BOOL - 
//
BOOL app_FileTree::AddZipToTree( void* hFile, int iPriority )
{
	BOOL								UsedIndex = FALSE;
	DWORD								FileSize;
	ZIP_Central_Directory_Record		CDR;
	BYTE*								CDBuffer;
	BYTE*								pCD;
	ZIP_Central_Directory_File_Header*	CDFileHeader;
	char								FilePath[MAX_PATH];
	FileEntry_t							FileData;
	DirEntry_t*							pDir;
	LPTSTR								pPathSting;
	LPTSTR								pSearchSting;
	int									length;
	char								DirString[128];

	// Assume there is no zip comment at the end of the file
	FileSize = fio_GetFileSize( hFile );
	fio_FileSeek( hFile, FileSize - sizeof(ZIP_Central_Directory_Record) );
	fio_ReadFile( hFile, &CDR, sizeof(ZIP_Central_Directory_Record) );
	if( (CDR.Signature!=ZIP_CENTRAL_DIRECTORY_RECORD_SIGNATURE) &&
		(FindCentralDirectoryRecord(hFile,&CDR)==FALSE) )
	{
		return FALSE;
	}

	// Disk spanning is not supported
	if( (CDR.DiskNumber!=0) ||
		(CDR.CentralDirectoryDiskNumber!=0) ||
		(CDR.NumberOfSpannedEntries!=CDR.NumberOfEntries) )
	{
		return FALSE;
	}

	// Move to the start of the central directory
	fio_FileSeek( hFile, CDR.OffsetToCentralDirectory );

	// Allocate a buffer for the central directory and read it in
	pCD = CDBuffer = new BYTE[ CDR.SizeOfCentralDirectory ];
	if( CDBuffer==NULL ) return FALSE;
	fio_ReadFile( hFile, CDBuffer, CDR.SizeOfCentralDirectory );

	// Iterate through each entry
	for( DWORD EntryNum=0; EntryNum<CDR.NumberOfEntries; EntryNum++)
	{
		// Grab the file header and move up the Central directory pointer
		CDFileHeader = (ZIP_Central_Directory_File_Header*)pCD;
		pCD += sizeof(ZIP_Central_Directory_File_Header);

		// Copy the file path
		strncpy( FilePath, (LPSTR)pCD, CDFileHeader->FilenameLength );
		FilePath[ CDFileHeader->FilenameLength ] = '\0';

		// Move the Central directory pointer to the next entry
		pCD += CDFileHeader->FilenameLength + CDFileHeader->ExtraFieldLength + CDFileHeader->FileCommentLength;

		// Check if this entry is a folder
		if( (LOBYTE(CDFileHeader->ExternalFileAttributes))==16 )
			continue;

		FileData.LibraryPathIndex = m_CurrentLibraryPathIndex;
		FileData.Offset = CDFileHeader->OffsetToLocalHeader + sizeof(ZIP_Local_File_Header) + CDFileHeader->FilenameLength;
		FileData.DecompressedSize = CDFileHeader->UncompressedSize;
		FileData.CompressedSize = CDFileHeader->CompressedSize;
		FileData.Key = CDFileHeader->CRC32;
		FileData.Priority = iPriority;

		switch( CDFileHeader->CompressionMethod )
		{
			case ZIP_COMPRESSION_NONE:
				FileData.Type = ZIP_FILE_UNCOMPRESSED;
				break;

			case ZIP_COMPRESSION_DEFLATED:
				FileData.Type = ZIP_FILE_DEFLATED;
				break;

			default: continue;
		}

		pDir = m_theTree;
		pPathSting = FilePath;
		pSearchSting = strchr( pPathSting, '/' );
		if( pSearchSting==NULL )
		{
			// Set the file's name
			strcpy( FileData.Name, FilePath);
		}
		else
		{
			// Iterate through the path until the file name is reached
			while( pSearchSting )
			{
				// Retrieve the directory name from the path
				length = pSearchSting - pPathSting;
				strncpy( DirString, pPathSting, length );
				DirString[ length ] = '\0';

				// Add the directory to the tree
				pDir = AddDirectory( DirString, pDir );

				// Find the next directory name (if any)
				pPathSting = pSearchSting + 1;
				pSearchSting = strchr( pPathSting, '/' );

			} // end while( pSearchSting )

			// Set the file's name
			strcpy( FileData.Name, pPathSting);
		}
	
		// Add the file and set the UsedIndex flag if necessary
		if( AddFile(FileData,pDir)->Priority==FileData.Priority )
			UsedIndex = TRUE;

	} // end for( NumberOfEntries )

	return UsedIndex;
}
// End app_FileTree::AddZipToTree()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::AddDirToTree() //                 \author Logan Jones
//////////////////////////////////                     \date 9/15/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPath      - 
//  BOOL bDoRoot         - 
//  int iLayers          - 
//  int iPriority        - 
//  DirEntry_t* pRootDir - 
//
void app_FileTree::AddDirToTree( LPCTSTR strPath, BOOL bDoRoot, int iLayers, int iPriority, DirEntry_t* pRootDir )
{
	LPVOID							hFileFind;
	fio_FindFileData				FindData;
	BOOL							bFind;
	char							FilePath[ MAX_PATH ];
	char							FindPath[ MAX_PATH ];
	DirEntry_t*						pDir;
	FileEntry_t						FileData;
	BOOL							bUsedIndex = FALSE;
	DWORD							theIndex;

	// Setup the find path
	sprintf( FindPath, "%s/*", strPath );

	// Begin the search
	hFileFind = fio_FindFirstFile( FindPath, &FindData );
	bFind = (hFileFind)? TRUE:FALSE;
	while( bFind )
	{
		// Is this a directory
		if( (FindData.Attributes==fio_FFA_DIRECTORY)&&(iLayers!=0)&&(FindData.Name[0]!='.') )
		{
			sprintf( FilePath, "%s/%s", strPath, FindData.Name );

			pDir = AddDirectory( FindData.Name, pRootDir );

			AddDirToTree( FilePath, TRUE, iLayers-1, iPriority, pDir );
		}
		// Is this a file
		else if( (FindData.Attributes==fio_FFA_NORMAL)&&bDoRoot )
		{
			if( bUsedIndex==FALSE )
			{
				theIndex = m_CurrentLibraryPathIndex;
				strcpy( m_LibraryPaths + m_CurrentLibraryPathIndex, strPath );
				m_CurrentLibraryPathIndex += strlen( strPath ) + 1;
				bUsedIndex = TRUE;
			}
			
			strcpy( FileData.Name, FindData.Name );
			FileData.Priority = iPriority;
			FileData.LibraryPathIndex = theIndex;
			FileData.DecompressedSize = FindData.Size;
			FileData.Type = SYS_FILE;

			AddFile( FileData, pRootDir );
		}

		// Find the next entry
		bFind = fio_FindNextFile( hFileFind, &FindData );

	} // end while( bFind )
	fio_FindClose( hFileFind );
}
// End app_FileTree::AddDirToTree()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::IsSQSH() //                       \author Logan Jones
////////////////////////////                           \date 9/16/2001
//               
//====================================================================
// Parameters:
//  BYTE* pBuffer - 
//
// Return: BOOL - 
//
BOOL app_FileTree::IsSQSH( BYTE* pBuffer )
{
	return ( ((LPTA_HPI_CHUNK)pBuffer)->Marker==TA_HPI_CHUNK_MARKER );
}
// End app_FileTree::IsSQSH()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::GetSQSHSize() //                  \author Logan Jones
/////////////////////////////////                      \date 9/16/2001
//               
//====================================================================
// Parameters:
//  BYTE* pBuffer - 
//
// Return: DWORD - 
//
DWORD app_FileTree::GetSQSHSize( BYTE* pBuffer )
{
	return ((LPTA_HPI_CHUNK)pBuffer)->DecompressedSize;
}
// End app_FileTree::GetSQSHSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::DeSQSH() //                       \author Logan Jones
////////////////////////////                           \date 9/16/2001
//               
//====================================================================
// Parameters:
//  BYTE* In  - 
//  BYTE* Out - 
//
// Return: DWORD - 
//
DWORD app_FileTree::DeSQSH( BYTE* In, BYTE* Out )
{
	TA_HPI_CHUNK*		pChunk;

	// Grab a pointer to the chunk and advance the original pointer
	pChunk = (TA_HPI_CHUNK*)In;
	In += sizeof(TA_HPI_CHUNK);

	// Check the chunk marker for 'SQSH'
	if( pChunk->Marker!=TA_HPI_CHUNK_MARKER )
	{
		return 0;
	}

	// Check for encryption and decrypt if necessary
	if( pChunk->EncryptionFlag )
	{
		for( DWORD x=0; x<pChunk->CompressedSize; x++ )
			In[x] = (In[x] - ((BYTE)(x))) ^ ((BYTE)(x));
	}

	// Decompress the chunk
	switch( pChunk->CompressionType )
	{

		case TA_HPI_CHUNK_NOT_COMPRESSED: // Data is just sitting there
			memcpy( Out, In, pChunk->DecompressedSize );
			break;

		case TA_HPI_CHUNK_LZ77: // Sliding window
			LZ77Decompress( In, Out );
			break;

		case TA_HPI_CHUNK_ZLIB: // Good ole zlib
			ZLibDecompress( In, Out, pChunk->CompressedSize );
			break;

	} // end switch( CompressionType )

	// Return the size of the new decompresed data
	return pChunk->DecompressedSize;
}
// End app_FileTree::DeSQSH()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::ZLibDecompress() //                      \author JoeD
////////////////////////////////////                   \date 5/16/1998
//               
//====================================================================
// Parameters:
//  BYTE* in              - 
//  BYTE* out             - 
//  long CompressedSize   - 
//  long DecompressedSize - 
//
// Return: int - 
//
int app_FileTree::ZLibDecompress( BYTE* in, BYTE* out, long CompressedSize, long DecompressedSize )
{
  z_stream zs;
  int result;
  if( DecompressedSize<0 ) DecompressedSize = TA_HPI_CHUNK_DEFAULT_SIZE;

  zs.next_in = in;
  zs.avail_in = CompressedSize;
  zs.total_in = 0;

  zs.next_out = out;
  zs.avail_out = DecompressedSize;
  zs.total_out = 0;

  zs.msg = NULL;
  zs.state = NULL;
  zs.zalloc = Z_NULL;
  zs.zfree = Z_NULL;
  zs.opaque = NULL;

  zs.data_type = Z_BINARY;
  zs.adler = 0;
  zs.reserved = 0;

  result = inflateInit(&zs);
  if (result != Z_OK) {
    return 0;
  }

  result = inflate(&zs, Z_FINISH);
  if (result != Z_STREAM_END) {
    zs.total_out = 0;
  }

  result = inflateEnd(&zs);
  if (result != Z_OK) {
    return 0;
  }

	return zs.total_out;
}
// End app_FileTree::ZLibDecompress()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_FileTree::LZ77Decompress() //                      \author JoeD
////////////////////////////////////                   \date 5/16/1998
//               
//====================================================================
// Parameters:
//  BYTE* in  - 
//  BYTE* out - 
//
// Return: int - 
//
int app_FileTree::LZ77Decompress( BYTE* in, BYTE* out )
{
	int x;
	int work1;
	int work2;
	int work3;
	int inptr;
	int outptr;
	int count;
	int done;
	char DBuff[4096];
	int DPtr;

	done = false;

  inptr = 0;
	outptr = 0;
	work1 = 1;
	work2 = 1;
	work3 = in[inptr++];
	
	while (!done) {
	  if ((work2 & work3) == 0) {
  		out[outptr++] = in[inptr];
		  DBuff[work1] = in[inptr];
		  work1 = (work1 + 1) & 0xFFF;
		  inptr++;
		}
	  else {
  		count = *((unsigned short *) (in+inptr));
			inptr += 2;
			DPtr = count >> 4;
			if (DPtr == 0) {
				return outptr;
			}
			else {
				count = (count & 0x0f) + 2;
				if (count >= 0) {
					for (x = 0; x < count; x++) {
						out[outptr++] = DBuff[DPtr];
						DBuff[work1] = DBuff[DPtr];
						DPtr = (DPtr + 1) & 0xFFF;
		        work1 = (work1 + 1) & 0xFFF;
					}

				}
			}
		}
		work2 *= 2;
		if (work2 & 0x0100) {
			work2 = 1;
			work3 = in[inptr++];
		}
	}

	return outptr;
}
// End app_FileTree::LZ77Decompress()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_FileTree.cpp //
////////////////////////////
