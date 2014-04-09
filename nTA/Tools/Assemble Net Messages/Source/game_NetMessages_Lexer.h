// game_NetMessages_Lexer.h //             Author: Logan "Burn" Jones
/////////////////////////////                         Date: 10/1/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_NETMESSAGES_LEXER_H_
#define _GAME_NETMESSAGES_LEXER_H_
/////////////////////////////////////////////////////////////////////


#include "sys.h"

extern int				line;
extern char				TextBuffer[1024 * 4];
extern DWORD			TextOffset;
extern int				CurrentType;

#define TYPE_REFERENCE	1
#define TYPE_CONSTANT	2
#define TYPE_POINTER	4

#ifdef _GAME_NETMESSAGES_LEXER_CPP_
	int					line;
	char				TextBuffer[1024 * 4];
	DWORD				TextOffset;
	int					CurrentType;
#else
   // Import some variables
   extern FILE *yyin;  // the input stream

   // Function prototype
   int yylex ();
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_NETMESSAGES_LEXER_H_)
