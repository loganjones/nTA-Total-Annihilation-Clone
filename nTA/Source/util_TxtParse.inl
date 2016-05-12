// util_TxtParse.inl //                            \author Logan Jones
//////////////////////                                \date 5/21/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::Begin() //                       \author Logan Jones
///////////////////////////                            \date 5/21/2002
//               
//====================================================================
// Parameters:
//  BYTE* Txt        - 
//  DWORD TxtLength  - 
//  Parse_Info* Info - 
//
// Return: BOOL - 
//
BOOL util_TxtParse::Begin( BYTE* Txt, DWORD TxtLength, Parse_Info* Info )
{
	assert( Txt!=NULL && TxtLength>0 );

	m_Txt = Txt;
	m_TxtLength = TxtLength;
	m_Offset = 0;
	m_Locale = 0;

	return Continue(Info);
}
// End util_TxtParse::Begin()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::SkipUntil() //                   \author Logan Jones
///////////////////////////////                        \date 8/18/2002
//               
//====================================================================
// Parameters:
//  int iLocale - 
//
// Return: BOOL - 
//
BOOL util_TxtParse::SkipUntil( int iLocale )
{
	for( ; m_Locale!=iLocale; ++m_Offset )
		if( m_Offset==m_TxtLength ) return FALSE;
		else if( m_Txt[m_Offset]=='{' ) ++m_Locale;
		else if( m_Txt[m_Offset]=='}' ) --m_Locale;

	return m_Offset<m_TxtLength;
}
// End util_TxtParse::SkipUntil()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::Locale() //                      \author Logan Jones
////////////////////////////                           \date 5/21/2002
//               
//====================================================================
// Return: const int - 
//
const int util_TxtParse::Locale() const
{
	return m_Locale;
}
// End util_TxtParse::Locale()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::KillTrailers() //                \author Logan Jones
//////////////////////////////////                     \date 5/21/2002
//               
//====================================================================
// Parameters:
//  LPTSTR str - 
//
void util_TxtParse::KillTrailers( LPTSTR str )
{
	assert( str );

	ssize_t len = strlen(str)-1;
	for( ; len>=0 && iswspace(str[len]); --len)
		str[len] = '\0';
}
// End util_TxtParse::KillTrailers()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::MoveToNextLine() //              \author Logan Jones
////////////////////////////////////                   \date 5/21/2002
//               
//====================================================================
//
void util_TxtParse::MoveToNextLine()
{
	while( m_Offset<m_TxtLength )
		if( m_Txt[m_Offset++]==0x0A )
			break;
}
// End util_TxtParse::MoveToNextLine()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::EatComment() //                  \author Logan Jones
////////////////////////////////                       \date 5/21/2002
//               
//====================================================================
//
void util_TxtParse::EatComment()
{
	while( m_Offset<m_TxtLength )
		if( m_Txt[m_Offset]=='*' && (m_Offset+1)<m_TxtLength && m_Txt[m_Offset+1]=='/' )
		{
			m_Offset += 2;
			break;
		}
		else ++m_Offset;
}
// End util_TxtParse::EatComment()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_TxtParse::MoveToNextValid() //             \author Logan Jones
/////////////////////////////////////                  \date 5/21/2002
//               
//====================================================================
// Return: const bool - 
//
const bool util_TxtParse::MoveToNextValid()
{
	while( m_Offset<m_TxtLength )
		if( iswspace(m_Txt[m_Offset]) )
			++m_Offset;
		else if( m_Txt[m_Offset]=='/' && (m_Offset+1)<m_TxtLength )
			if( m_Txt[m_Offset+1]=='/' ) MoveToNextLine();
			else if( m_Txt[m_Offset+1]=='*' ) EatComment();
			else return true;
        else return true;
	return false;
}
// End util_TxtParse::MoveToNextValid()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - util_TxtParse.inl //
////////////////////////////
