// app_Console.inl //                              \author Logan Jones
////////////////////                                  \date 9/19/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::AddVariable() //                   \author Logan Jones
///////////////////////////////                        \date 9/30/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strName            - 
//  LPCommandProc_t lpVarProc - 
//
// Return: BOOL - 
//
BOOL app_Console::AddVariable( LPCTSTR strName, LPCommandProc_t lpVarProc )
{
	return AddCommand( strName, lpVarProc );
}
// End app_Console::AddVariable()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::AddToggle() //                     \author Logan Jones
/////////////////////////////                          \date 9/30/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strName                 - 
//  LPCommandProc_t lpToggleProc   - 
//  LPCommandProc_t lpUntoggleProc - 
//
// Return: BOOL - 
//
BOOL app_Console::AddToggle( LPCTSTR strName, LPCommandProc_t lpToggleProc, LPCommandProc_t lpUntoggleProc )
{
	char NewName[32] = "+\0";
	strcat( NewName, strName );

	if( bFAILED(AddCommand(NewName,lpToggleProc)) )
		return FALSE;
	
	NewName[0] = '-';
	return AddCommand( NewName, lpUntoggleProc );
}
// End app_Console::AddToggle()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::SetCurrentDirectory() //           \author Logan Jones
///////////////////////////////////////                \date 8/26/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strDir - 
//
void app_Console::SetCurrentDirectory( LPTSTR strDir )
{
	m_CurrentDirectory = strDir;
}
// End app_Console::SetCurrentDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::GetCurrentDirectory() //           \author Logan Jones
///////////////////////////////////////                \date 8/26/2002
//               
//====================================================================
// Return: LPTSTR - 
//
LPTSTR app_Console::GetCurrentDirectory() const
{
	return m_CurrentDirectory;
}
// End app_Console::GetCurrentDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::GetToken() //                      \author Logan Jones
////////////////////////////                           \date 9/19/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strToken          - 
//  CodeStream_t* pStream    - 
//  ExecutionState_t exState - 
//
// Return: BOOL - 
//
BOOL app_Console::GetToken( LPTSTR strToken, CodeStream_t* pStream, ExecutionState_t exState )
{
// WARNING! WARNING! ugly macros ahead. I apologize.
#define StreamValid()			( pStream->Length>0 )
#define StreamUp()				++pStream->pStream, --pStream->Length
#define isLineComment()			( (pStream->Length>1)&&(*pStream->pStream=='/')&&(*(pStream->pStream+1)=='/') )
#define isBlockComment()		( (pStream->Length>1)&&(*pStream->pStream=='/')&&(*(pStream->pStream+1)=='*') )
#define SkipLineComment()		{ for( ;StreamValid()&&(*pStream->pStream!=0x0A); StreamUp()); StreamUp();}
#define SkipBlockComment() \
	{ for( ;StreamValid(); StreamUp()) { \
		if( (pStream->Length>1)&&(*pStream->pStream=='*')&&(*(pStream->pStream+1)=='/') ) \
		{ StreamUp(); StreamUp(); break;}  } }
#define SeekToChar()			{ for( ;StreamValid(); ) { \
	if( isLineComment() ) { SkipLineComment(); continue; }\
	if( isBlockComment() ) { StreamUp(); StreamUp(); SkipBlockComment(); continue; }\
	if( !isspace(*pStream->pStream) ) break; StreamUp(); } }
#define SeekToCharOrEnd()		{ for( ;StreamValid(); ) { \
	if( isLineComment() ) { SkipLineComment(); continue; }\
	if( isBlockComment() ) { StreamUp(); StreamUp(); SkipBlockComment(); continue; }\
	if( (*pStream->pStream==0x0A)||(*pStream->pStream==0x0D)||(!isspace(*pStream->pStream)) ) break; StreamUp(); } }
#define SeekToEnd()		{ for( ;StreamValid(); ) { \
	if( isLineComment() ) { SkipLineComment(); continue; }\
	if( isBlockComment() ) { StreamUp(); StreamUp(); SkipBlockComment(); continue; }\
	if( (*pStream->pStream==0x0A)||(*pStream->pStream==0x0D)||(*pStream->pStream==';') ) break; StreamUp(); } }
#define isident( CHAR )			( isalnum(CHAR) || ((CHAR)=='_') )
#define FillToken( CONDITION )	{ for( x=0; StreamValid()&&(!(isLineComment()||isBlockComment()))&&(CONDITION); StreamUp(),x++) { strToken[x] = *pStream->pStream; } strToken[x] = '\0';}
#define FillParamString()		

	//char		LeadingChar;
	int			x;

	// If the stream is done, return
	if( pStream->Length==0 )
		return FALSE;

	// What state are we in
	switch( exState )
	{
		case EXS_SEEKING_IDENTIFIER:
			BEGIN_CODE_BLOCK

			// Find the next non-space
			SeekToChar();

			// Make sure the stream hasn't run out
			if( StreamValid() )
			{
				// Get the identifier and return
				FillToken( !isspace(*pStream->pStream)&&(*pStream->pStream!=';') );
				if( strToken[0]=='\0' )
				{
					if( StreamValid() )
					{
						StreamUp();
						RESTART_CODE_BLOCK
					}
					else return FALSE;
				}
				else return TRUE;
			}
			else return FALSE;
			END_CODE_BLOCK
		// end case EXS_SEEKING_IDENTIFIER

		case EXS_SEEKING_PARAM_STRING:

			// Find the next non-space or end deliminator
			SeekToCharOrEnd();

			// Make sure the stream hasn't run out
			if( StreamValid() )
			{
				// Fill the token until an end char is reached
				BOOL bInString = FALSE;
				for( x=0; StreamValid()&&(!(isLineComment()||isBlockComment())); StreamUp(),x++)
				{
					if( *pStream->pStream=='\"' ) bInString = !bInString;
					if( ((*pStream->pStream==';')&&(!bInString))||(*pStream->pStream==0x0A)||(*pStream->pStream==0x0D) )
						break;
					strToken[x] = *pStream->pStream;
				}
				strToken[x] = '\0';
				//FillToken( (*pStream->pStream!=';')&&(*pStream->pStream!=0x0A)&&(*pStream->pStream!=0x0D) );
				if( StreamValid() ) { StreamUp(); }
				if( strToken[0]=='\0' ) return FALSE;
				else return TRUE;
			}
			else return FALSE;
		// end case EXS_SEEKING_PARAM_STRING

		case EXS_SEEK_NEXT_COMMAND:

			// Find the next end deliminator
			SeekToEnd();

			// Move up one
			if( StreamValid() ) { StreamUp(); }

			// Return whether the stream is done
			return StreamValid();
		// end case EXS_SEEK_NEXT_COMMAND

	} // end switch( exState )

	// Shouldn't get to here, but anyway;
	// return FALSE, nothing was found
	return FALSE;
}
// End app_Console::GetToken()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::RetrieveParam() //                 \author Logan Jones
/////////////////////////////////                     \date 11/11/2001
//               
//====================================================================
// Parameters:
//  LPTSTR& strParams      - 
//  ParamType_t ptType     - 
//  LPVOID lpReturnedParam - 
//
// Return: BOOL - 
//
BOOL app_Console::RetrieveParam( LPTSTR& strParams, ParamType_t ptType, LPVOID lpReturnedParam )
{
	int x;

	if( (strParams==NULL) || strlen(strParams)==0 ) return FALSE;

	switch( ptType )
	{
		case PT_INT:
			*((long*)lpReturnedParam) = atol( strParams );
			for( ; (*strParams!='\0')&&(!isspace(*strParams)); ++strParams);
			for( ; (*strParams!='\0')&&(isspace(*strParams)); ++strParams);
			return TRUE;

		case PT_STRING:
		{
			LPTSTR str = (LPTSTR)lpReturnedParam;
			if( *strParams=='\"' )
			{
				++strParams;
				for( x=0; (*strParams!='\0')&&(*strParams!='\"'); ++strParams,++x) str[x] = *strParams;
				str[x] = '\0';
				if( *strParams!='\0' ) ++strParams;
				for( ; (*strParams!='\0')&&(isspace(*strParams)); ++strParams);
			}
			else
			{
				strcpy( str, strParams );
				strParams = NULL;
			}
			return TRUE;
		}

		case PT_BOOL:
		{
			char	str[ 32 ];

			for( x=0; (*strParams!='\0')&&(!isspace(*strParams)); ++strParams,++x) str[x] = *strParams;
			str[x] = '\0';

			if( isdigit(str[0]) )
				*((int*)lpReturnedParam) = atoi(str);
			else if( stricmp(str,"TRUE")==0 )
				*((BOOL*)lpReturnedParam) = TRUE;
			else if( stricmp(str,"FALSE")==0 )
				*((BOOL*)lpReturnedParam) = FALSE;
			else return FALSE;

			for( ; (*strParams!='\0')&&(isspace(*strParams)); ++strParams);

			return TRUE;
		}

	} // end switch( ptType )

	// Shouldn't get to here, but anyway;
	// return FALSE, nothing was found
	return FALSE;
}
// End app_Console::RetrieveParam()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// app_Console::AddCommentToBuffer() //            \author Logan Jones
//////////////////////////////////////                \date 11/18/2001
//               
//====================================================================
// Parameters:
//  LPTSTR strComment - 
//  DWORD dwType      - 
//
void app_Console::AddCommentToBuffer( LPTSTR strComment, DWORD dwType )
{
	const DWORD				Size = strlen(strComment) + 1;
	Comment_t				CommentInfo;

	if( (m_CurrentOffset + Size) >= COMMENT_BUFFER_SIZE ) m_CurrentOffset = 0;

	while( !m_CommentStack.empty() && (m_CurrentOffset<=m_StartOffset) && (m_CurrentOffset+Size>=m_StartOffset) )
	{
		m_CommentStack.pop_front();
		m_StartOffset = m_CommentStack.front().dwOffset;
	}

	memcpy( m_CommentBuffer + m_CurrentOffset, strComment, Size );
	CommentInfo.dwOffset = m_CurrentOffset;
	m_CurrentOffset += Size;
	CommentInfo.dwType = dwType;
	CommentInfo.dwTime = std_Time();
	m_CommentStack.push_back( CommentInfo );
}
// End app_Console::AddCommentToBuffer()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// app_Console::BankComment() //                   \author Logan Jones
///////////////////////////////                        \date 4/12/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strComment - 
//  DWORD dwType      - 
//
void app_Console::BankComment( LPTSTR strComment, DWORD dwType )
{
	if( m_CommentStack.size()>=64 )
		m_CommentStack.pop_front();
	m_CommentStack.push_back( Comment_t(strComment,dwType,std_Time()) );
}
// End app_Console::BankComment()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_Console::SetCommentHandler() //             \author Logan Jones
/////////////////////////////////////                  \date 4/12/2002
//               
//====================================================================
// Parameters:
//  LPCommentHandler_t pHandler - 
//  LPVOID pOwner               - 
//
void app_Console::SetCommentHandler( LPCommentHandler_t pHandler, LPVOID pOwner )
{
	m_CommentHandler = pHandler;
	m_CommentHandlerOwner = NULL;
}
// End app_Console::SetCommentHandler()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_Console.inl //
//////////////////////////
