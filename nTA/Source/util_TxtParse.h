// util_TxtParse.h //                              \author Logan Jones
////////////////////                                  \date 2/26/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UTIL_TXT_PARSE_H_
#define _UTIL_TXT_PARSE_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NOTE:
//    This is another class that, like fio_FileTree, was brought over
//    from previous projects, so please excuse the lack of structure.
//


#define MAX_BUFFER_SIZE				( 2048 )


/////////////////////////////////////////////////////////////////////
// Information gathered from the last parse update

// Possible values of the Type member
enum Parse_Info_Types
{
	PARSE_Header,           // An header was encountered ie [HEADER NAME]
                            // The header name is in the Value Member
	PARSE_VarVal,
	PARSE_LevelUp,
	PARSE_LevelDown,
	FORCE_DWORD( PARSE ),
};

typedef struct Parse_Info
{
	Parse_Info_Types	Type;
	LPTSTR				Variable;
	LPTSTR				Value;
} *LPParse_Info;


/////////////////////////////////////////////////////////////////////
//
class util_TxtParse  
{

/////////////////////////////////////////////////
// INTERFACE
public:

	INLINE BOOL Begin( BYTE* Txt, DWORD TxtLength, Parse_Info* Info );
	BOOL Continue( Parse_Info* Info );
	INLINE BOOL SkipUntil( int iLocale );

	INLINE const int Locale() const;

	INLINE static void KillTrailers( LPTSTR str );


/////////////////////////////////////////////////
// Utility methods
protected:

	INLINE void MoveToNextLine();
	INLINE void EatComment();
	INLINE const bool MoveToNextValid();


/////////////////////////////////////////////////
// Data members
protected:

	BYTE*				m_Txt;
	DWORD				m_TxtLength;
	DWORD				m_Offset;
	int					m_Locale;
	char				m_TextBank[ MAX_BUFFER_SIZE ];


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	util_TxtParse();
	virtual ~util_TxtParse();
/////////////////////////////////////////////////

}; // End class - util_TxtParse
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "util_TxtParse.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UTIL_TXT_PARSE_H_)
