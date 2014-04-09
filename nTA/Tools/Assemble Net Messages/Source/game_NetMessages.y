/* Parser for StringC */

%{

/* ------------------------------------------------------------------
   Initial code (copied verbatim to the output file)
   ------------------------------------------------------------------ */

// Includes

// Disable the unrecognized character escape sequence warning
#pragma warning( disable : 4129 )

#include "sys.h"
#include <malloc.h>
#include <string.h>
#include "game_NetMessages_Lexer.h"

#ifdef _DEBUG
 #define YYDEBUG 1
 #define YYERROR_VERBOSE
#endif

// Error-reporting function must be defined by the caller
void Error (char *format, ...);

// Forward references
void yyerror (char *msg);

ParameterList_t		gParameters;

%}

/* ------------------------------------------------------------------
   Yacc declarations
   ------------------------------------------------------------------ */

/* The structure for passing value between lexer and parser */
%union {
	long				l;
	char*				str;
	NetMessage_t*		pNM;
}

%token END_STMT OPEN_PAR CLOSE_PAR COMMA
%token CONSTANT REFERENCE POINTER
%token <l> START_REQUESTS
%token <l> START_NOTIFICATIONS
%token ERROR_TOKEN
%token <l> HOST_GAME HOST_INTERFACE
%token <str> CODE
%token <str> IDENTIFER
%token <l> INT_VALUE

%type <l> Host
%type <pNM> NetMessage

%%

/* ------------------------------------------------------------------
   Yacc grammar rules
   ------------------------------------------------------------------ */

Format
	: Requests Notifications				{  }
	;

Requests
	: START_REQUESTS						{ CurrentType = START_REQUESTS; }
	| START_REQUESTS						{ CurrentType = START_REQUESTS; }
	  NetMessages							{  }
	;

Notifications
	: START_NOTIFICATIONS					{ CurrentType = START_NOTIFICATIONS; }
	| START_NOTIFICATIONS					{ CurrentType = START_NOTIFICATIONS; }
	  NetMessages							{  }
	;

NetMessages
	: NetMessage							{  }
	| NetMessages NetMessage				{  }
	;

NetMessage
	: Host IDENTIFER ParameterList			{ $$ = NewNetMessage(CurrentType,$1,$2,NULL,gParameters); gParameters.clear() }
	;

Host
	: HOST_INTERFACE						{ $$ = HOST_INTERFACE; }
	| HOST_GAME								{ $$ = HOST_GAME; }
	;

ParameterList
	: OPEN_PAR CLOSE_PAR					{  }
	| OPEN_PAR Parameters CLOSE_PAR			{  }
	;

Parameters
	: Parameter								{  }
	| Parameters COMMA Parameter			{  }
	;

Parameter
	: IDENTIFER IDENTIFER					{ gParameters.push_back(Parameter_t($1,0,$2)); }
	| IDENTIFER REFERENCE IDENTIFER			{ gParameters.push_back(Parameter_t($1,TYPE_REFERENCE,$3)); }
	| IDENTIFER POINTER IDENTIFER			{ gParameters.push_back(Parameter_t($1,TYPE_POINTER,$3)); }
	| CONSTANT IDENTIFER IDENTIFER			{ gParameters.push_back(Parameter_t($2,TYPE_CONSTANT,$3)); }
	| CONSTANT IDENTIFER REFERENCE IDENTIFER{ gParameters.push_back(Parameter_t($2,TYPE_CONSTANT|TYPE_REFERENCE,$4)); }
	| CONSTANT IDENTIFER POINTER IDENTIFER	{ gParameters.push_back(Parameter_t($2,TYPE_CONSTANT|TYPE_POINTER,$4)); }
	;

%%
/* ------------------------------------------------------------------
   Additional code (again copied verbatim to the output file)
   ------------------------------------------------------------------ */

// Renable the unrecognized character escape sequence warning
#pragma warning( default : 4129 )
