// app_FileTree.h //                               \author Logan Jones
///////////////////                                   \date 9/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _APP_FILETREE_H_
#define _APP_FILETREE_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// app_FileTree ////////////////////////////
//
class app_FileTree
{

/////////////////////////////////////////////////
// Foward declare some of the internal structures used
protected:

	struct FileEntry_t;
	struct DirEntry_t;
    enum FileEntryType_t : short;

	struct ScanDir_t;
	struct ScanExt_t;

	struct ZIP_Central_Directory_Record;
	struct ZIP_Central_Directory_File_Header;
	struct ZIP_Local_File_Header;
	struct ZIP_Local_File_Data_Descriptor;
    enum ZIP_Compression_t : short;

	struct FindHandle_t;

	struct FileHandle_t;

	typedef list< ScanDir_t >	ScanDirList_t;
	typedef list< ScanExt_t >	ScanExtList_t;

	typedef int (CALLBACK* LPReadProc_t)(int,int,LPVOID);


/////////////////////////////////////////////////
// Publicly available structures
public:

	// File access handle type
	typedef FileHandle_t*	HFILE;

	// File and dir entry handle type
	typedef FileEntry_t*	HFILEENTRY;
	typedef DirEntry_t*		HDIRENTRY;

	// Find types
    enum FindType_t : short;
	struct FindData_t;
	typedef FindHandle_t*	HFIND;

	struct DefaultLoader_t{ bool Update(long lCurrent,long lGoal){return true;} };


/////////////////////////////////////////////////
// INTERFACE
public:

	// Adds a directory for Create() to scan
	DWORD AddDirectoryToScan( LPCTSTR strPath, BOOL bCertify=FALSE );

	// Adds a file type to search for in the given directory
	void AddFileType( LPCTSTR strExt, int iPriority, DWORD dwDir=0 );

	// Adds a specific branch of the file system to the file tree
	void AddDirectoryAsLib( int iPriority, BOOL bAddSubDirs=TRUE, DWORD dwDir=0 );

	// Adds the sub branches of a specific branch in the file system to the file tree
	void AddSubDirectoriesAsLibs( int iPriority, BOOL bAddSubDirs=TRUE, DWORD dwDir=0 );

	// Resets all scan directory settings
	void ClearScanDirectories();

	// Searches the given destinations for the given file types
	// and puts together the file tree
	BOOL Create();

	// Destroys the file tree structure
	void Destroy();

	DWORD DumpFileToBuffer( LPCTSTR strFilePath, BYTE** ppBuffer, LPReadProc_t lpReadProc=NULL, LPVOID lpProcVal=NULL );
	DWORD DumpFileToBuffer( HFILEENTRY pFile, BYTE** ppBuffer, LPReadProc_t lpReadProc=NULL, LPVOID lpProcVal=NULL );

	template <class _Progress_t>
	DWORD DumpFileToBuffer( LPCTSTR strFilePath, BYTE** ppBuffer, _Progress_t Progress=DefaultLoader_t() );

	HFILE OpenFile( LPTSTR strFilePath );
	DWORD GetFileSize( HFILE hFile );
	DWORD ReadFile( HFILE hFile, void* pDestBuffer, LPReadProc_t lpReadProc=NULL, LPVOID lpProcVal=NULL );
	DWORD ReadFile( HFILE hFile, void* pDestBuffer, DWORD dwSizeToRead );
	void CloseFile( HFILE hFile );

	DWORD GetEntryCountInDirectory( LPCTSTR strWhere );
	DWORD GetFileCountInDirectory( LPCTSTR strWhere );
	DWORD GetDirectoryCountInDirectory( LPCTSTR strWhere );

	// Starts the file tree entry search process
	HFIND Find( FindType_t What, LPCTSTR strWith, LPCTSTR strWhere, FindData_t* pAnswer );
	HFIND Find( FindType_t What, LPCTSTR strWith, HDIRENTRY hWhere, FindData_t* pAnswer );

	// Continuously find entries based on the previously given criteria
	BOOL ContinueFind( HFIND hFind, FindData_t* pAnswer );

	// Closes the handle provided by Find()
	void CloseFind( HFIND& hFind );


/////////////////////////////////////////////////
// Data
protected:

	// Directories to scan for file types to insert into the file tree
	ScanDirList_t			m_ScanDirectories;
	DWORD					m_NumScanDirectories;

	// The actual file tree
	DirEntry_t*				m_theTree;

	DWORD					m_CurrentLibraryPathIndex;
	char					m_LibraryPaths[ MAX_PATH * 256 ];


/////////////////////////////////////////////////
// Utility methods
protected:

	// Removes all scan directories marked for pruning
	void PruneScanDirectories();

	BOOL AddLibraryToTree( LPCTSTR strPath, int iPriority );

	BOOL AddTAHpiToTree( void* hFile, BYTE* pCDir, INT32 iKey, UINT32 uiOffset, int iPriority, DirEntry_t* pRootDir );
	BOOL AddTAKHpiToTree( void* hFile, BYTE* pCDir, char* pchNames, UINT32 uiOffset, int iPriority, DirEntry_t* pRootDir );
	BOOL AddZipToTree( void* hFile, int iPriority );
	void AddDirToTree( LPCTSTR strPath, BOOL bDoRoot, int iLayers, int iPriority, DirEntry_t* pRootDir );

// HPI methods
	inline UINT32 ReadAndDecrypt( void* hFile, INT32 iKey, UINT32 uiOffset, void* pBuffer, UINT32 uiSize );
	inline BOOL IsSQSH( BYTE* pBuffer );
	inline DWORD GetSQSHSize( BYTE* pBuffer );
	inline DWORD DeSQSH( BYTE* In, BYTE* Out );
	int ZLibDecompress( BYTE* in, BYTE* out, long CompressedSize, long DecompressedSize=(-1) );
	int LZ77Decompress( BYTE* in, BYTE* out );

// ZIP methods
	inline BOOL FindCentralDirectoryRecord( void* hFile, ZIP_Central_Directory_Record* pCDR );

// Tree methods
	inline DirEntry_t* AddDirectory( LPCTSTR strName, DirEntry_t* pRoot=NULL );
	inline FileEntry_t* AddFile( FileEntry_t& feData, DirEntry_t* pRoot );
	inline void CopyFileData( FileEntry_t& destFile, const FileEntry_t& srcFile );
	inline DirEntry_t* RetrieveDirectory( LPCTSTR strPath, DirEntry_t* pStartDir );
	inline FileEntry_t* RetrieveFile( LPCTSTR strPath, DirEntry_t* pStartDir );
	inline int GetDirSize( DirEntry_t* pDir );
	inline void DeleteBranch( DirEntry_t** ppBranch );
	inline void DumpBranchToFile( DirEntry_t* pBranch, LPTSTR strFileName, LPTSTR strPrefix=NULL );


/////////////////////////////////////////////////
// File tree structures
protected:

	struct DirEntry_t
	{
		char			Name[ 64 ];

		FileEntry_t*	SubFiles;
		DirEntry_t*		SubDirs;

		DirEntry_t*		Next;
	};

	struct FileEntry_t
	{
		char			Name[ 64 ];
		int				Priority;
		FileEntryType_t	Type;

		UINT32			Offset;
		UINT32			DecompressedSize;
		UINT32			CompressedSize;
		INT32			Key;
		DWORD			LibraryPathIndex;

		FileEntry_t*	Next;
	};

	enum FileEntryType_t : short
	{
		TA_FILE,
		TA_FILE_LZ77,
		TA_FILE_ZLIB,
		TAK_FILE,
		TAK_FILE_ZLIB,
		SYS_FILE,
		ZIP_FILE_UNCOMPRESSED,
		ZIP_FILE_DEFLATED,
	};


/////////////////////////////////////////////////
// Scan Directoy + File Type
protected:

	enum ScanDirFlags_t : short
	{
		SDF_NO_FLAG = 0,
		SDF_VERIFY					=	(1<<0),
		SDF_PRUNE_AFTER_CREATION	=	(1<<1),
	};

	struct ScanDir_t
	{
		char			strPath[ MAX_PATH ];
		DWORD			Flags;

		ScanExtList_t	Types;
	};

	struct ScanExt_t
	{
		char			strExtension[16];
		int				Priority;
	};


/////////////////////////////////////////////////
// File handle structures
protected:

	struct FileHandle_t
	{
		LPVOID			hSysFile;
		DWORD			LibraryIndex;
	};


/////////////////////////////////////////////////
// Find file structures
protected:

	struct FindHandle_t
	{
		FindType_t		What;
		char			With[ 32 ];
		DirEntry_t*		Where;

		DirEntry_t*		DirToSearch;
		FileEntry_t*	FileToSearch;
	};

public:

	enum FindType_t : short
	{
		EVERYTHING,
		FILES_WITH_EXTENSIONS,
		FILES_WITH_NAMES,
		FILES,
		DIRECTORIES,
	};

	struct FindData_t
	{
		BOOL			bIsFile;
		LPTSTR			Name;
		UINT32			Size;
		HDIRENTRY		hDir;
		HFILEENTRY		hFile;
	};


/////////////////////////////////////////////////
// Zip Types + definitions
protected:

#define ZIP_LOCAL_FILE_HEADER_SIGNATURE					( 0x04034b50 )
#define ZIP_CENTRAL_DIRECTORY_FILE_HEADER_SIGNATURE		( 0x02014b50 )
#define ZIP_CENTRAL_DIRECTORY_RECORD_SIGNATURE			( 0x06054b50 )

#define ZIP_ENCRYPTED				( 1<<0 )
#define ZIP_DECOMPRESSION_FLAG_1	( 1<<1 )
#define ZIP_DECOMPRESSION_FLAG_2	( 1<<2 )
#define ZIP_DATA_DESCRIPTOR_VALID	( 1<<3 )
#define ZIP_DEFLATION_FLAG			( 1<<4 )
#define ZIP_COMPRESSED_PATCHED_DATA	( 1<<5 )

	enum ZIP_Compression_t : short
	{
		ZIP_COMPRESSION_NONE = 0,
		ZIP_COMPRESSION_SHRUNK,
		ZIP_COMPRESSION_REDUCED_FACTOR_1,
		ZIP_COMPRESSION_REDUCED_FACTOR_2,
		ZIP_COMPRESSION_REDUCED_FACTOR_3,
		ZIP_COMPRESSION_REDUCED_FACTOR_4,
		ZIP_COMPRESSION_IMPLODED,
		ZIP_COMPRESSION_TOKENIZED,
		ZIP_COMPRESSION_DEFLATED,
		ZIP_COMPRESSION_DEFLATE_64,
		ZIP_COMPRESSION_DATE_LIBRAY_IMPLODING,
	};

#pragma pack(1)
	struct ZIP_Central_Directory_Record
	{
		DWORD			Signature;		// Should be 0x06054b50
		WORD			DiskNumber;
		WORD			CentralDirectoryDiskNumber;
		WORD			NumberOfEntries;
		WORD			NumberOfSpannedEntries;
		DWORD			SizeOfCentralDirectory;
		DWORD			OffsetToCentralDirectory;
		WORD			SizeOfZipComment;
	};

	struct ZIP_Central_Directory_File_Header
	{
		DWORD			Signature;		// Should be 0x02014b50 
		WORD			VersionMadeBy;
		WORD			VersionNeededToExtract;
		WORD			BitFlags;
		WORD			CompressionMethod;
		WORD			TimeLastModified;
		WORD			DateLastModified;
		DWORD			CRC32;
		DWORD			CompressedSize;
		DWORD			UncompressedSize;
		WORD			FilenameLength;
		WORD			ExtraFieldLength;
		WORD			FileCommentLength;
		WORD			DiskNumberStart;
		WORD			InternalFileAttributes;
		DWORD			ExternalFileAttributes;
		DWORD			OffsetToLocalHeader;
	};

	struct ZIP_Local_File_Header
	{
		DWORD			Signature;		// Should be 0x04034b50
		WORD			VersionNeededToExtract;
		WORD			BitFlags;
		WORD			CompressionMethod;
		WORD			TimeLastModified;
		WORD			DateLastModified;
		DWORD			CRC32;
		DWORD			CompressedSize;
		DWORD			UncompressedSize;
		WORD			FilenameLength;
		WORD			ExtraFieldLength;
	};

	struct ZIP_Local_File_Data_Descriptor
	{
		DWORD			CRC32;
		DWORD			CompressedSize;
		DWORD			UncompressedSize;
	};
#pragma pack()


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	app_FileTree();
	virtual ~app_FileTree();
/////////////////////////////////////////////////

}; // End class - app_FileTree
/////////////////////////////////////////////////////////////////////

// Include the definitions to the inline methods
#include "app_FileTree.inl"


/////////////////////////////////////////////////////////////////////
#endif // !defined(_APP_FILETREE_H_)
