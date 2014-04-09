
/*  A Bison parser, made from c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	END_STMT	258
#define	OPEN_PAR	259
#define	CLOSE_PAR	260
#define	COMMA	261
#define	CONSTANT	262
#define	REFERENCE	263
#define	POINTER	264
#define	START_REQUESTS	265
#define	START_NOTIFICATIONS	266
#define	ERROR_TOKEN	267
#define	HOST_GAME	268
#define	HOST_INTERFACE	269
#define	CODE	270
#define	IDENTIFER	271
#define	INT_VALUE	272

#line 3 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"


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


#line 39 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
typedef union {
	long				l;
	char*				str;
	NetMessage_t*		pNM;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		38
#define	YYFLAG		-32768
#define	YYNTBASE	18

#define YYTRANSLATE(x) ((unsigned)(x) <= 272 ? yytranslate[x] : 29)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     5,     6,    10,    12,    13,    17,    19,    22,
    26,    28,    30,    33,    37,    39,    43,    46,    50,    54,
    58,    63
};

static const short yyrhs[] = {    19,
    21,     0,    10,     0,     0,    10,    20,    23,     0,    11,
     0,     0,    11,    22,    23,     0,    24,     0,    23,    24,
     0,    25,    16,    26,     0,    14,     0,    13,     0,     4,
     5,     0,     4,    27,     5,     0,    28,     0,    27,     6,
    28,     0,    16,    16,     0,    16,     8,    16,     0,    16,
     9,    16,     0,     7,    16,    16,     0,     7,    16,     8,
    16,     0,     7,    16,     9,    16,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    65,    69,    70,    71,    75,    76,    77,    81,    82,    86,
    90,    91,    95,    96,   100,   101,   105,   106,   107,   108,
   109,   110
};

static const char * const yytname[] = {   "$","error","$undefined.","END_STMT",
"OPEN_PAR","CLOSE_PAR","COMMA","CONSTANT","REFERENCE","POINTER","START_REQUESTS",
"START_NOTIFICATIONS","ERROR_TOKEN","HOST_GAME","HOST_INTERFACE","CODE","IDENTIFER",
"INT_VALUE","Format","Requests","@1","Notifications","@2","NetMessages","NetMessage",
"Host","ParameterList","Parameters","Parameter",""
};
#endif

static const short yyr1[] = {     0,
    18,    19,    20,    19,    21,    22,    21,    23,    23,    24,
    25,    25,    26,    26,    27,    27,    28,    28,    28,    28,
    28,    28
};

static const short yyr2[] = {     0,
     2,     1,     0,     3,     1,     0,     3,     1,     2,     3,
     1,     1,     2,     3,     1,     3,     2,     3,     3,     3,
     4,     4
};

static const short yydefact[] = {     0,
     3,     0,     0,     6,     1,    12,    11,     4,     8,     0,
     0,     9,     0,     7,     0,    10,    13,     0,     0,     0,
    15,     0,     0,     0,    17,    14,     0,     0,     0,    20,
    18,    19,    16,    21,    22,     0,     0,     0
};

static const short yydefgoto[] = {    36,
     2,     3,     5,    11,     8,     9,    10,    16,    20,    21
};

static const short yypact[] = {    -7,
    -3,     2,     3,    20,-32768,-32768,-32768,     3,-32768,     5,
     3,-32768,    18,     3,    -5,-32768,-32768,     7,    -4,    13,
-32768,    -2,     8,     9,-32768,-32768,    -6,    10,    11,-32768,
-32768,-32768,-32768,-32768,-32768,    28,    29,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,    19,     1,-32768,-32768,-32768,     4
};


#define	YYLAST		31


static const short yytable[] = {    17,
    18,    18,     1,    23,    24,    28,    29,    -2,    12,    19,
    19,    25,     4,    30,    12,     6,     7,    26,    27,    -5,
    13,    15,    22,    31,    32,    34,    35,    37,    38,    14,
    33
};

static const short yycheck[] = {     5,
     7,     7,    10,     8,     9,     8,     9,    11,     8,    16,
    16,    16,    11,    16,    14,    13,    14,     5,     6,     0,
    16,     4,    16,    16,    16,    16,    16,     0,     0,    11,
    27
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

#line 3 "bison.simple"



/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.



   This program is free software; you can redistribute it and/or modify

   it under the terms of the GNU General Public License as published by

   the Free Software Foundation; either version 2, or (at your option)

   any later version.



   This program is distributed in the hope that it will be useful,

   but WITHOUT ANY WARRANTY; without even the implied warranty of

   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

   GNU General Public License for more details.



   You should have received a copy of the GNU General Public License

   along with this program; if not, write to the Free Software

   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */



/* As a special exception, when this file is copied by Bison into a

   Bison output file, you may use that output file without restriction.

   This special exception was added by the Free Software Foundation

   in version 1.24 of Bison.  */



#ifndef alloca

#ifdef __GNUC__

#define alloca __builtin_alloca

#else /* not GNU C.  */

#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)

#include <alloca.h>

#else /* not sparc */

#if defined (MSDOS) && !defined (__TURBOC__)

#include <malloc.h>

#else /* not MSDOS, or __TURBOC__ */

#if defined(_AIX)

#include <malloc.h>

 #pragma alloca

#else /* not MSDOS, __TURBOC__, or _AIX */

#ifdef __hpux

#ifdef __cplusplus

extern "C" {

void *alloca (unsigned int);

};

#else /* not __cplusplus */

void *alloca ();

#endif /* not __cplusplus */

#endif /* __hpux */

#endif /* not _AIX */

#endif /* not MSDOS, or __TURBOC__ */

#endif /* not sparc.  */

#endif /* not GNU C.  */

#endif /* alloca not defined.  */



/* This is the parser code that is written into each bison parser

  when the %semantic_parser declaration is not specified in the grammar.

  It was written by Richard Stallman by simplifying the hairy parser

  used when %semantic_parser is specified.  */



/* Note: there must be only one dollar sign in this file.

   It is replaced by the list of actions, each action

   as one case of the switch.  */



#define yyerrok		(yyerrstatus = 0)

#define yyclearin	(yychar = YYEMPTY)

#define YYEMPTY		-2

#define YYEOF		0

#define YYACCEPT	return(0)

#define YYABORT 	return(1)

#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.

   This remains here temporarily to ease the

   transition to the new meaning of YYERROR, for GCC.

   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)



#define YYTERROR	1

#define YYERRCODE	256



#ifndef YYPURE

#define YYLEX		yylex()

#endif



#ifdef YYPURE

#ifdef YYLSP_NEEDED

#ifdef YYLEX_PARAM

#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)

#else

#define YYLEX		yylex(&yylval, &yylloc)

#endif

#else /* not YYLSP_NEEDED */

#ifdef YYLEX_PARAM

#define YYLEX		yylex(&yylval, YYLEX_PARAM)

#else

#define YYLEX		yylex(&yylval)

#endif

#endif /* not YYLSP_NEEDED */

#endif



/* If nonreentrant, generate the variables here */



#ifndef YYPURE



int	yychar;			/*  the lookahead symbol		*/

YYSTYPE	yylval;			/*  the semantic value of the		*/

				/*  lookahead symbol			*/



#ifdef YYLSP_NEEDED

YYLTYPE yylloc;			/*  location data for the lookahead	*/

				/*  symbol				*/

#endif



int yynerrs;			/*  number of parse errors so far       */

#endif  /* not YYPURE */



#if YYDEBUG != 0

int yydebug;			/*  nonzero means print parse trace	*/

/* Since this is uninitialized, it does not stop multiple parsers

   from coexisting.  */

#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/



#ifndef	YYINITDEPTH

#define YYINITDEPTH 200

#endif



/*  YYMAXDEPTH is the maximum size the stacks can grow to

    (effective only if the built-in stack extension method is used).  */



#if YYMAXDEPTH == 0

#undef YYMAXDEPTH

#endif



#ifndef YYMAXDEPTH

#define YYMAXDEPTH 10000

#endif



/* Prevent warning if -Wstrict-prototypes.  */

#ifdef __GNUC__

int yyparse (void);

#endif



#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */

#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)

#else				/* not GNU C or C++ */

#ifndef __cplusplus



/* This is the most reliable way to avoid incompatibilities

   in available built-in functions on various systems.  */

static void

__yy_memcpy (from, to, count)

     char *from;

     char *to;

     int count;

{

  register char *f = from;

  register char *t = to;

  register int i = count;



  while (i-- > 0)

    *t++ = *f++;

}



#else /* __cplusplus */



/* This is the most reliable way to avoid incompatibilities

   in available built-in functions on various systems.  */

static void

__yy_memcpy (char *from, char *to, int count)

{

  register char *f = from;

  register char *t = to;

  register int i = count;



  while (i-- > 0)

    *t++ = *f++;

}



#endif

#endif



#line 192 "bison.simple"



/* The user can define YYPARSE_PARAM as the name of an argument to be passed

   into yyparse.  The argument should have type void *.

   It should actually point to an object.

   Grammar actions can access the variable by casting it

   to the proper pointer type.  */



#ifdef YYPARSE_PARAM

#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;

#else

#define YYPARSE_PARAM

#define YYPARSE_PARAM_DECL

#endif



int

yyparse(YYPARSE_PARAM)

     YYPARSE_PARAM_DECL

{

  register int yystate;

  register int yyn;

  register short *yyssp;

  register YYSTYPE *yyvsp;

  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */

  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */



  short	yyssa[YYINITDEPTH];	/*  the state stack			*/

  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/



  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */

  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */



#ifdef YYLSP_NEEDED

  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/

  YYLTYPE *yyls = yylsa;

  YYLTYPE *yylsp;



#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

#else

#define YYPOPSTACK   (yyvsp--, yyssp--)

#endif



  int yystacksize = YYINITDEPTH;



#ifdef YYPURE

  int yychar;

  YYSTYPE yylval;

  int yynerrs;

#ifdef YYLSP_NEEDED

  YYLTYPE yylloc;

#endif

#endif



  YYSTYPE yyval;		/*  the variable used to return		*/

				/*  semantic values from the action	*/

				/*  routines				*/



  int yylen;



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Starting parse\n");

#endif



  yystate = 0;

  yyerrstatus = 0;

  yynerrs = 0;

  yychar = YYEMPTY;		/* Cause a token to be read.  */



  /* Initialize stack pointers.

     Waste one element of value and location stack

     so that they stay on the same level as the state stack.

     The wasted elements are never initialized.  */



  yyssp = yyss - 1;

  yyvsp = yyvs;

#ifdef YYLSP_NEEDED

  yylsp = yyls;

#endif



/* Push a new state, which is found in  yystate  .  */

/* In all cases, when you get here, the value and location stacks

   have just been pushed. so pushing a state here evens the stacks.  */

yynewstate:



  *++yyssp = yystate;



  if (yyssp >= yyss + yystacksize - 1)

    {

      /* Give user a chance to reallocate the stack */

      /* Use copies of these so that the &'s don't force the real ones into memory. */

      YYSTYPE *yyvs1 = yyvs;

      short *yyss1 = yyss;

#ifdef YYLSP_NEEDED

      YYLTYPE *yyls1 = yyls;

#endif



      /* Get the current used size of the three stacks, in elements.  */

      int size = yyssp - yyss + 1;



#ifdef yyoverflow

      /* Each stack pointer address is followed by the size of

	 the data in use in that stack, in bytes.  */

#ifdef YYLSP_NEEDED

      /* This used to be a conditional around just the two extra args,

	 but that might be undefined if yyoverflow is a macro.  */

      yyoverflow("parser stack overflow",

		 &yyss1, size * sizeof (*yyssp),

		 &yyvs1, size * sizeof (*yyvsp),

		 &yyls1, size * sizeof (*yylsp),

		 &yystacksize);

#else

      yyoverflow("parser stack overflow",

		 &yyss1, size * sizeof (*yyssp),

		 &yyvs1, size * sizeof (*yyvsp),

		 &yystacksize);

#endif



      yyss = yyss1; yyvs = yyvs1;

#ifdef YYLSP_NEEDED

      yyls = yyls1;

#endif

#else /* no yyoverflow */

      /* Extend the stack our own way.  */

      if (yystacksize >= YYMAXDEPTH)

	{

	  yyerror("parser stack overflow");

	  return 2;

	}

      yystacksize *= 2;

      if (yystacksize > YYMAXDEPTH)

	yystacksize = YYMAXDEPTH;

      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));

      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));

      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));

      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));

#ifdef YYLSP_NEEDED

      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));

      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));

#endif

#endif /* no yyoverflow */



      yyssp = yyss + size - 1;

      yyvsp = yyvs + size - 1;

#ifdef YYLSP_NEEDED

      yylsp = yyls + size - 1;

#endif



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Stack size increased to %d\n", yystacksize);

#endif



      if (yyssp >= yyss + yystacksize - 1)

	YYABORT;

    }



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Entering state %d\n", yystate);

#endif



  goto yybackup;

 yybackup:



/* Do appropriate processing given the current state.  */

/* Read a lookahead token if we need one and don't already have one.  */

/* yyresume: */



  /* First try to decide what to do without reference to lookahead token.  */



  yyn = yypact[yystate];

  if (yyn == YYFLAG)

    goto yydefault;



  /* Not known => get a lookahead token if don't already have one.  */



  /* yychar is either YYEMPTY or YYEOF

     or a valid token in external form.  */



  if (yychar == YYEMPTY)

    {

#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Reading a token: ");

#endif

      yychar = YYLEX;

    }



  /* Convert token to internal form (in yychar1) for indexing tables with */



  if (yychar <= 0)		/* This means end of input. */

    {

      yychar1 = 0;

      yychar = YYEOF;		/* Don't call YYLEX any more */



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Now at end of input.\n");

#endif

    }

  else

    {

      yychar1 = YYTRANSLATE(yychar);



#if YYDEBUG != 0

      if (yydebug)

	{

	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);

	  /* Give the individual parser a way to print the precise meaning

	     of a token, for further debugging info.  */

#ifdef YYPRINT

	  YYPRINT (stderr, yychar, yylval);

#endif

	  fprintf (stderr, ")\n");

	}

#endif

    }



  yyn += yychar1;

  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)

    goto yydefault;



  yyn = yytable[yyn];



  /* yyn is what to do for this token type in this state.

     Negative => reduce, -yyn is rule number.

     Positive => shift, yyn is new state.

       New state is final state => don't bother to shift,

       just return success.

     0, or most negative number => error.  */



  if (yyn < 0)

    {

      if (yyn == YYFLAG)

	goto yyerrlab;

      yyn = -yyn;

      goto yyreduce;

    }

  else if (yyn == 0)

    goto yyerrlab;



  if (yyn == YYFINAL)

    YYACCEPT;



  /* Shift the lookahead token.  */



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);

#endif



  /* Discard the token being shifted unless it is eof.  */

  if (yychar != YYEOF)

    yychar = YYEMPTY;



  *++yyvsp = yylval;

#ifdef YYLSP_NEEDED

  *++yylsp = yylloc;

#endif



  /* count tokens shifted since error; after three, turn off error status.  */

  if (yyerrstatus) yyerrstatus--;



  yystate = yyn;

  goto yynewstate;



/* Do the default action for the current state.  */

yydefault:



  yyn = yydefact[yystate];

  if (yyn == 0)

    goto yyerrlab;



/* Do a reduction.  yyn is the number of a rule to reduce with.  */

yyreduce:

  yylen = yyr2[yyn];

  if (yylen > 0)

    yyval = yyvsp[1-yylen]; /* implement default value of the action */



#if YYDEBUG != 0

  if (yydebug)

    {

      int i;



      fprintf (stderr, "Reducing via rule %d (line %d), ",

	       yyn, yyrline[yyn]);



      /* Print the symbols being reduced, and their result.  */

      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)

	fprintf (stderr, "%s ", yytname[yyrhs[i]]);

      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);

    }

#endif




  switch (yyn) {

case 1:
#line 65 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 2:
#line 69 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ CurrentType = START_REQUESTS; ;
    break;}
case 3:
#line 70 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ CurrentType = START_REQUESTS; ;
    break;}
case 4:
#line 71 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 5:
#line 75 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ CurrentType = START_NOTIFICATIONS; ;
    break;}
case 6:
#line 76 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ CurrentType = START_NOTIFICATIONS; ;
    break;}
case 7:
#line 77 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 8:
#line 81 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 9:
#line 82 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 10:
#line 86 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ yyval.pNM = NewNetMessage(CurrentType,yyvsp[-2].l,yyvsp[-1].str,NULL,gParameters); gParameters.clear() ;
    break;}
case 11:
#line 90 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ yyval.l = HOST_INTERFACE; ;
    break;}
case 12:
#line 91 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ yyval.l = HOST_GAME; ;
    break;}
case 13:
#line 95 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 14:
#line 96 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 15:
#line 100 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 16:
#line 101 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{  ;
    break;}
case 17:
#line 105 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-1].str,0,yyvsp[0].str)); ;
    break;}
case 18:
#line 106 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-2].str,TYPE_REFERENCE,yyvsp[0].str)); ;
    break;}
case 19:
#line 107 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-2].str,TYPE_POINTER,yyvsp[0].str)); ;
    break;}
case 20:
#line 108 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-1].str,TYPE_CONSTANT,yyvsp[0].str)); ;
    break;}
case 21:
#line 109 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-2].str,TYPE_CONSTANT|TYPE_REFERENCE,yyvsp[0].str)); ;
    break;}
case 22:
#line 110 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"
{ gParameters.push_back(Parameter_t(yyvsp[-2].str,TYPE_CONSTANT|TYPE_POINTER,yyvsp[0].str)); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */

#line 487 "bison.simple"



  yyvsp -= yylen;

  yyssp -= yylen;

#ifdef YYLSP_NEEDED

  yylsp -= yylen;

#endif



#if YYDEBUG != 0

  if (yydebug)

    {

      short *ssp1 = yyss - 1;

      fprintf (stderr, "state stack now");

      while (ssp1 != yyssp)

	fprintf (stderr, " %d", *++ssp1);

      fprintf (stderr, "\n");

    }

#endif



  *++yyvsp = yyval;



#ifdef YYLSP_NEEDED

  yylsp++;

  if (yylen == 0)

    {

      yylsp->first_line = yylloc.first_line;

      yylsp->first_column = yylloc.first_column;

      yylsp->last_line = (yylsp-1)->last_line;

      yylsp->last_column = (yylsp-1)->last_column;

      yylsp->text = 0;

    }

  else

    {

      yylsp->last_line = (yylsp+yylen-1)->last_line;

      yylsp->last_column = (yylsp+yylen-1)->last_column;

    }

#endif



  /* Now "shift" the result of the reduction.

     Determine what state that goes to,

     based on the state we popped back to

     and the rule number reduced by.  */



  yyn = yyr1[yyn];



  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;

  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)

    yystate = yytable[yystate];

  else

    yystate = yydefgoto[yyn - YYNTBASE];



  goto yynewstate;



yyerrlab:   /* here on detecting error */



  if (! yyerrstatus)

    /* If not already recovering from an error, report this error.  */

    {

      ++yynerrs;



#ifdef YYERROR_VERBOSE

      yyn = yypact[yystate];



      if (yyn > YYFLAG && yyn < YYLAST)

	{

	  int size = 0;

	  char *msg;

	  int x, count;



	  count = 0;

	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */

	  for (x = (yyn < 0 ? -yyn : 0);

	       x < (sizeof(yytname) / sizeof(char *)); x++)

	    if (yycheck[x + yyn] == x)

	      size += strlen(yytname[x]) + 15, count++;

	  msg = (char *) malloc(size + 15);

	  if (msg != 0)

	    {

	      strcpy(msg, "parse error");



	      if (count < 5)

		{

		  count = 0;

		  for (x = (yyn < 0 ? -yyn : 0);

		       x < (sizeof(yytname) / sizeof(char *)); x++)

		    if (yycheck[x + yyn] == x)

		      {

			strcat(msg, count == 0 ? ", expecting `" : " or `");

			strcat(msg, yytname[x]);

			strcat(msg, "'");

			count++;

		      }

		}

	      yyerror(msg);

	      free(msg);

	    }

	  else

	    yyerror ("parse error; also virtual memory exceeded");

	}

      else

#endif /* YYERROR_VERBOSE */

	yyerror("parse error");

    }



  goto yyerrlab1;

yyerrlab1:   /* here on error raised explicitly by an action */



  if (yyerrstatus == 3)

    {

      /* if just tried and failed to reuse lookahead token after an error, discard it.  */



      /* return failure if at end of input */

      if (yychar == YYEOF)

	YYABORT;



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);

#endif



      yychar = YYEMPTY;

    }



  /* Else will try to reuse lookahead token

     after shifting the error token.  */



  yyerrstatus = 3;		/* Each real token shifted decrements this */



  goto yyerrhandle;



yyerrdefault:  /* current state does not do anything special for the error token. */



#if 0

  /* This is wrong; only states that explicitly want error tokens

     should shift them.  */

  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/

  if (yyn) goto yydefault;

#endif



yyerrpop:   /* pop the current state because it cannot handle the error token */



  if (yyssp == yyss) YYABORT;

  yyvsp--;

  yystate = *--yyssp;

#ifdef YYLSP_NEEDED

  yylsp--;

#endif



#if YYDEBUG != 0

  if (yydebug)

    {

      short *ssp1 = yyss - 1;

      fprintf (stderr, "Error: state stack now");

      while (ssp1 != yyssp)

	fprintf (stderr, " %d", *++ssp1);

      fprintf (stderr, "\n");

    }

#endif



yyerrhandle:



  yyn = yypact[yystate];

  if (yyn == YYFLAG)

    goto yyerrdefault;



  yyn += YYTERROR;

  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)

    goto yyerrdefault;



  yyn = yytable[yyn];

  if (yyn < 0)

    {

      if (yyn == YYFLAG)

	goto yyerrpop;

      yyn = -yyn;

      goto yyreduce;

    }

  else if (yyn == 0)

    goto yyerrpop;



  if (yyn == YYFINAL)

    YYACCEPT;



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Shifting error token, ");

#endif



  *++yyvsp = yylval;

#ifdef YYLSP_NEEDED

  *++yylsp = yylloc;

#endif



  yystate = yyn;

  goto yynewstate;

}

#line 113 "c:\development\myprojects\ta clone\copy of nta\tools\assemble net messages\source\game_netmessages.y"

/* ------------------------------------------------------------------
   Additional code (again copied verbatim to the output file)
   ------------------------------------------------------------------ */

// Renable the unrecognized character escape sequence warning
#pragma warning( default : 4129 )
