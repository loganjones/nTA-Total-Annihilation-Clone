// util_TxtParse.cpp //                            \author Logan Jones
//////////////////////                                \date 2/26/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NOTE:
//    This is another class that, like fio_FileTree, was brought over
//    from previous projects, so please excuse the lack of structure.
//    Unlike fio_FileTree I cleaned it up a bit, a tiny bit.
//


#include "sys.h"
#include "util_TxtParse.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "util_TxtParse.inl"
#endif // defined( _DEBUG )

//////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
util_TxtParse::util_TxtParse()
{}
util_TxtParse::~util_TxtParse()
{}
//
//////////////////////////////////////////////////////////////////////


#define READ_WHILE( condition ) \
	for( ; m_Offset<m_TxtLength && (condition); ) {\
		assert( x<MAX_BUFFER_SIZE ); \
		if( m_Txt[m_Offset]=='/' && (m_Offset+1)<m_TxtLength ) \
			if( m_Txt[m_Offset+1]=='/' ) MoveToNextLine(); \
			else if( m_Txt[m_Offset+1]=='*' ) EatComment(); \
			else m_TextBank[x++] = m_Txt[m_Offset++]; \
		else m_TextBank[x++] = m_Txt[m_Offset++]; }\
	m_TextBank[x++] = '\0'; \
	++m_Offset


BOOL util_TxtParse::Continue( Parse_Info* Info )
{
	int		x = 0;

	while( MoveToNextValid() ) switch( m_Txt[m_Offset] )
	{
		case '[':
			++m_Offset;
			Info->Value = m_TextBank;
			READ_WHILE( m_Txt[m_Offset]!=']' );
			Info->Type = PARSE_Header;
            KillTrailers( Info->Value );
			return TRUE;

		case '{':
			++m_Locale;
			++m_Offset;
			Info->Type = PARSE_LevelUp;
			return TRUE;

		case '}':
			--m_Locale;
			++m_Offset;
			Info->Type = PARSE_LevelDown;
			return TRUE;

		default:
			Info->Variable = m_TextBank;
			READ_WHILE( m_Txt[m_Offset]!='=' );
			if( !MoveToNextValid() ) return FALSE;
			Info->Value = m_TextBank + x;
			READ_WHILE( m_Txt[m_Offset]!=';' );
			Info->Type = PARSE_VarVal;
			KillTrailers( Info->Variable );
			KillTrailers( Info->Value );
			return TRUE;

	} // end while( valid ) switch( char )

	return FALSE;
}
