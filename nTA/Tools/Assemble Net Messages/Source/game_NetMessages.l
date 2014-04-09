/* game_NetMessages.l //                   Author: Logan "Burn" Jones
///////////////////////                               Date: 9/30/2002
//
///////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////////////////////////////////
// PreCode */
%{

// Include the base code
#include "sys.h"

#define  _GAME_NETMESSAGES_LEXER_CPP_
#include "game_NetMessages_Lexer.h"
#include "game_NetMessages_Parser.cpp.h"

// For some reason isatty is called _isatty in VC..
#ifdef _WIN32
 #include <io.h>
 #define  isatty _isatty
#endif

// the yywrap function is declared by the caller
extern "C" int yywrap ();

#define YY_USER_INIT		line = 1;
#define NEW_LINE			++line;
#define PUT_TEXT_CHAR(CH)	TextBuffer[ TextOffset++ ] = CH

int comment_caller,code_level;
DWORD code_start;

// Disable the unrecognized character escape sequence warning
#pragma warning( disable : 4129 )

%}
/* End PreCode
///////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////
// Definitions */

LETTER		[a-zA-Z_]
DIGIT		[0-9]
NUMBER		{DIGIT}+
IDENT		{LETTER}({LETTER}|{DIGIT})*
WSPACE		[ \t]+


/*///////////////////////////////////////////////
// Exclusive start conditions */
%x c_comment cpp_comment
%x code_segment


/*///////////////////////////////////////////////////////////////////
// Rules */
%%

"[Client Requests]"				{ return START_REQUESTS; }
"[Server Notifications]"		{ return START_NOTIFICATIONS; }

"%INTERFACE"					{ return HOST_INTERFACE; }
"%GAME"							{ return HOST_GAME; }

"const"							return CONSTANT;
"("								return OPEN_PAR;
")"								return CLOSE_PAR;
","								return COMMA;
"&"								return REFERENCE;
"*"								return POINTER;

"//"							{ comment_caller = INITIAL; BEGIN(cpp_comment); }
"/*"							{ comment_caller = INITIAL; BEGIN(c_comment); }

"{"								{ comment_caller = code_segment; code_level = 1; code_start = TextOffset; BEGIN(code_segment); }

{IDENT}							{ yylval.str = TextBuffer + TextOffset; strcpy( TextBuffer + TextOffset, yytext ); TextOffset += yyleng + 1; return IDENTIFER; }
{NUMBER}						{ yylval.l = (long)strtoul(yytext,NULL,0); return INT_VALUE; }
\n								{ NEW_LINE }
{WSPACE}						{ /*noting*/ }
.								{ return ERROR_TOKEN; }

<<EOF>>							{ yyterminate(); }

<c_comment>[^*\n]*				/* eat anything that's not a '*' */
<c_comment>"*"+[^*/\n]*			/* eat up '*'s not followed by '/'s */
<c_comment>\n					NEW_LINE
<c_comment>"*"+"/"				BEGIN(comment_caller);

<cpp_comment>[^\n]*				/* eat anything that's not a '\n' */
<cpp_comment>\n					{ NEW_LINE BEGIN(comment_caller); }

<code_segment>[^{}\n]*			{ strcpy( TextBuffer + TextOffset, yytext ); TextOffset += yyleng; }
<code_segment>\n				{ NEW_LINE PUT_TEXT_CHAR('\n'); }
<code_segment>"{"				{ ++code_level; PUT_TEXT_CHAR('{'); }
<code_segment>"}"				{ --code_level;
								  if(code_level) PUT_TEXT_CHAR('}');
								  else {
									yylval.str = TextBuffer + code_start;
									PUT_TEXT_CHAR('\0');
									BEGIN(INITIAL);
									return CODE; } }

%% /* End Rules
///////////////////////////////////////////////////////////////////*/

/////////////////////////////////////////////////////////////////////
// PostCode

// Renable the unrecognized character escape sequence warning
#pragma warning( default : 4129 )

// End PostCode
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// End - game_NetMessages.l //
/////////////////////////////