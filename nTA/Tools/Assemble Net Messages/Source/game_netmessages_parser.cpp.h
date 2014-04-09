typedef union {
	long				l;
	char*				str;
	NetMessage_t*		pNM;
} YYSTYPE;
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


extern YYSTYPE yylval;
