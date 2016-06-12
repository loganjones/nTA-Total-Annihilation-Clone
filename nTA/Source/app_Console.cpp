// app_Console.cpp //                              \author Logan Jones
////////////////////                                  \date 9/19/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "app.h"
#include "input.h"
#include "app_Console.h"

#ifdef _DEBUG
#include "app_Console.inl"
#endif

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
app_Console::app_Console():
	m_pHandler( NULL ),
#ifdef DEBUG
    m_WhatToPrint( CT_ALL ),
#else
    m_WhatToPrint( CT_ERROR ),
#endif
	m_CommentHandler( NULL ),
	m_CommentHandlerOwner( NULL )
{}
app_Console::~app_Console()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::ProcessInput() //                  \author Logan Jones
////////////////////////////////                      \date 10/15/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_Console::ProcessInput()
{
	if( m_pHandler )
		input.DumpInputToHandler( m_pHandler );

	return TRUE;
}
// End app_Console::ProcessInput()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::SetInputHandler() //               \author Logan Jones
///////////////////////////////////                     \date 4/7/2002
//               
//====================================================================
// Parameters:
//  class in_Handler* pHandler - 
//
// Return: BOOL - 
//
BOOL app_Console::SetInputHandler( class in_Handler* pHandler )
{
	m_pHandler = pHandler;
	input.SetHandler( m_pHandler );
	return TRUE;
}
// End app_Console::SetInputHandler()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::AddCommand() //                    \author Logan Jones
//////////////////////////////                         \date 9/30/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strName                - 
//  LPCommandProc_t lpCommandProc - 
//
// Return: BOOL - 
//
BOOL app_Console::AddCommand( LPCTSTR strName, LPCommandProc_t lpCommandProc )
{
	Command_t		NewCommand;

	// Sanity check
	if( (strName==NULL) || (strName[0]=='\0') || (lpCommandProc==NULL) )
		return FALSE;

	// Make sure the command does not already exist
	if( m_Commands.find(strName)!=m_Commands.end() )
	{
		Comment( CT_DEBUG, "WARNING: Command \'%s\' already exists.", strName );
		return FALSE;
	}

	// Set the command data
	NewCommand.lpCProc = lpCommandProc;
	//NewCommand.ParamCount = dwParams;

	// Add the new command
	m_Commands[strName] = NewCommand;

	return TRUE;
}
// End app_Console::AddCommand()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::HandleInput() //                   \author Logan Jones
///////////////////////////////                        \date 9/29/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL app_Console::HandleInput(/* in_Devices inDevice, DWORD dwID, long lData */)
{
	return FALSE;
}
// End app_Console::HandleInput()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::Execute() //                       \author Logan Jones
///////////////////////////                            \date 9/19/2001
//               
//====================================================================
// Parameters:
//  char* pExecutionCode - 
//  DWORD dwLength       - 
//
void app_Console::Execute( char* pExecutionCode, DWORD dwLength )
{
	CodeStream_t				Stream;
	char						Token[ 256 ];
	CommandMap_t::iterator		itCommand;

	// Sanity check
	if( (pExecutionCode==NULL) || (dwLength==0) )
		return;

	// Setup the stream structure
	Stream.pStream = pExecutionCode;
	Stream.Length = dwLength;

	// While the stream is valid, execute the code
	while( Stream.Length )
	{
		// We are at the start of a stement, look for the
		// first identifier to tell us what to do
		if( !GetToken( Token, &Stream, EXS_SEEKING_IDENTIFIER ) )
			break;

		// Is this identifier valid
		itCommand = m_Commands.find( Token );
		if( itCommand==m_Commands.end() )
		{
			// Not valid, move on
			Comment( CT_NORMAL, "WARNING: Unknnown command \'%s\'", Token );
			if( GetToken( Token, &Stream, EXS_SEEK_NEXT_COMMAND ) )
				continue;
			else break;
		}

		// Retrieve the parameter string (if any)
	 	__unused Command_t* pCmd= &((*itCommand).second);
		if( !GetToken( Token, &Stream, EXS_SEEKING_PARAM_STRING ) )
			((*itCommand).second).lpCProc( NULL );
		else ((*itCommand).second).lpCProc( Token );

	} // end while( Stream.Length )

}
// End app_Console::Execute()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::Comment() //                       \author Logan Jones
///////////////////////////                            \date 9/25/2001
//               
//====================================================================
// Parameters:
//  DWORD dwType       - 
//  LPCTSTR strComment - 
//  ...                - 
//
void app_Console::Comment( DWORD dwType, LPCTSTR strComment, ... )
{
	static char	CommentString[ MAX_PATH ];
	va_list 	va_ArgList;
	FILE*		hFile;

	// Access the argument list
	va_start( va_ArgList, strComment );

	// Translate the comment
	vsprintf( CommentString, strComment, va_ArgList );

	// Close the argument list
	va_end( va_ArgList );

	// Add the comment to the history buffer
	BankComment( CommentString, dwType );

	if( m_CommentHandler ) m_CommentHandler( CommentString, dwType, m_CommentHandlerOwner );

	// Log this comment if desired
	if( m_WhatToLog & dwType )
	{
		// Open the log file
		if( !(hFile=fopen(m_LogFileName,"a+")) )
			return;

		// Append the comment to the file
        fwrite( CommentString, 1, strlen(CommentString), hFile );

		// Add a newline also
		putc( '\n', hFile );

		// Close the log file
		fclose( hFile );

    } // end if( log comment )
    
    // Print this comment if desired
    if( m_WhatToPrint & dwType )
    {
        printf("%s\n", CommentString);
        
    } // end if( print comment )
    
}
// End app_Console::Comment()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::vComment() //                      \author Logan Jones
////////////////////////////                           \date 9/25/2001
//               
//====================================================================
// Parameters:
//  DWORD dwType        - 
//  LPCTSTR strComment  - 
//  va_list vaArguments - 
//
void app_Console::vComment( DWORD dwType, LPCTSTR strComment, va_list vaArguments )
{
	static char	CommentString[ MAX_PATH ];
	FILE*		hFile;

	// Translate the comment
	vsprintf( CommentString, strComment, vaArguments );

	// Add the comment to the history buffer
	BankComment( CommentString, dwType );

	if( m_CommentHandler ) m_CommentHandler( CommentString, dwType, m_CommentHandlerOwner );

	// Log this comment if desired
	if( m_WhatToLog & dwType )
	{
		// Open the log file
		if( !(hFile=fopen(m_LogFileName,"a+")) )
			return;

		// Append the comment to the file
        fwrite( CommentString, 1, strlen(CommentString), hFile );

		// Add a newline also
		putc( '\n', hFile );

		// Close the log file
		fclose( hFile );

    } // end if( log comment )
    
    // Print this comment if desired
    if( m_WhatToPrint & dwType )
    {
        printf("%s\n", CommentString);
        
    } // end if( print comment )
}
// End app_Console::vComment()
//////////////////////////////////////////////////////////////////////


app_Console::Comment_t::Comment_t( LPTSTR strComment, DWORD dwType, std_Time_t tTime ):Comment(strComment),Type(dwType),Time(tTime){}
app_Console::Comment_t::Comment_t( const Comment_t& Comment ):Comment(Comment.Comment),Type(Comment.Type),Time(Comment.Time){}


//////////////////////////////////////////////////////////////////////
// app_Console::SetLogFile() //                    \author Logan Jones
//////////////////////////////                         \date 9/25/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strLogName - 
//  DWORD dwWhatToLog  - 
//  BOOL bErase        - 
//
// Return: BOOL - 
//
BOOL app_Console::SetLogFile( LPCTSTR strLogName, DWORD dwWhatToLog, BOOL bErase )
{
	FILE*		hFile;
	long		Pos;

	// Open the log file
	if( !(hFile=fopen(strLogName,"a+")) )
		return FALSE;

	// Check if the current log file needs to be cleared of if it is too large
	fprintf( hFile, "\n" );
	Pos = ftell( hFile );
	if( bErase || (Pos>8192) )
	{
		// Close the old log file
		fclose( hFile );

		// Open the file again but erase the contents
		hFile = fopen( strLogName, "w" );
	}
	else // Skip down a few lines
		fprintf( hFile, "\n" );

	// Close the log file
	fclose( hFile );

	// Set the log parameters
	strcpy( m_LogFileName, strLogName );
	m_WhatToLog = dwWhatToLog;

	return TRUE;
}
// End app_Console::SetLogFile()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_Console.cpp //
//////////////////////////
