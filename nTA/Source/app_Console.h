// app_Console.h //                                \author Logan Jones
//////////////////                                    \date 9/19/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _APP_CONSOLE_H_
#define _APP_CONSOLE_H_
/////////////////////////////////////////////////////////////////////


#include <string>

//////////////////////////// app_Console ////////////////////////////
//
class app_Console
{

/////////////////////////////////////////////////
// Types
public:

	struct Command_t;
	typedef map< LPCTSTR, Command_t, pchar_less_nocase_t >			CommandMap_t;

	typedef LRESULT (CALLBACK* LPCommandProc_t)( LPVOID );
	typedef LRESULT (CALLBACK* LPCursorProc_t)(long,long);

	struct Comment_t;
	typedef list< Comment_t >								CommentList_t;

	typedef LRESULT (CALLBACK* LPCommentHandler_t)(LPTSTR,DWORD,LPVOID);


/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL ProcessInput();

	BOOL SetInputHandler( class in_Handler* pHandler );

	BOOL AddCommand( LPCTSTR strName, LPCommandProc_t lpCommandProc );
	INLINE BOOL AddVariable( LPCTSTR strName, LPCommandProc_t lpVarProc );
	INLINE BOOL AddToggle( LPCTSTR strName, LPCommandProc_t lpToggleProc, LPCommandProc_t lpUntoggleProc );

	BOOL SetCursorFunc( LPCursorProc_t lpCursorFunc );

	BOOL HandleInput(/* in_Devices inDevice, DWORD dwID, long lData */);

	void Execute( char* pExecutionCode, DWORD dwLength );
	INLINE void SetCurrentDirectory( LPTSTR strDir );
	INLINE LPTSTR GetCurrentDirectory() const;

	void Comment( DWORD dwType, LPCTSTR strComment, ... );
	void vComment( DWORD dwType, LPCTSTR strComment, va_list vaArguments );

	BOOL SetLogFile( LPCTSTR strLogName, DWORD dwWhatToLog, BOOL bErase=TRUE );

	INLINE void SetCommentHandler( LPCommentHandler_t pHandler, LPVOID pOwner=NULL );


/////////////////////////////////////////////////
// Utility
protected:

	INLINE void BankComment( LPTSTR strComment, DWORD dwType );

	
/////////////////////////////////////////////////
// Data members
public:

	CommandMap_t		m_Commands;

	DWORD				m_WhatToLog;
	char				m_LogFileName[32];

	in_Handler*			m_pHandler;

	LPTSTR				m_CurrentDirectory;

	LPCommentHandler_t	m_CommentHandler;
	LPVOID				m_CommentHandlerOwner;
	CommentList_t		m_CommentStack;


/////////////////////////////////////////////////
// CProcs
public:

	static LRESULT CALLBACK Bind();


/////////////////////////////////////////////////
// Internal types
public:	

	struct Command_t
	{
		LPCommandProc_t	lpCProc;
		DWORD			ParamCount;
	};

	struct CodeStream_t
	{
		char*	pStream;
		DWORD	Length;
	};

	enum ExecutionState_t
	{
		EXS_SEEKING_IDENTIFIER,
		EXS_SEEKING_PARAM_STRING,
		EXS_SEEK_NEXT_COMMAND,
	};

	enum ParamType_t
	{
		PT_INT,
		PT_STRING,
		PT_BOOL,
	};

	INLINE BOOL GetToken( LPTSTR strToken, CodeStream_t* pStream, ExecutionState_t exState );
	INLINE BOOL RetrieveParam( LPTSTR& strParams, ParamType_t ptType, LPVOID lpReturnedParam );

	struct Comment_t
	{
		string		Comment;
		DWORD		Type;
		std_Time_t	Time;

		Comment_t( LPTSTR strComment, DWORD dwType=0, std_Time_t tTime=0 );
		Comment_t( const Comment_t& Comment );
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	app_Console();
	virtual ~app_Console();
/////////////////////////////////////////////////

}; // End class - app_Console
/////////////////////////////////////////////////////////////////////


// Comment type flag used in Comment(). Can be combined
enum CommentTypes_t
{
	CT_NONE			=	( 0 ),
	CT_DEBUG		=	(1<<0),
	CT_LOAD			=	(1<<1),
	CT_EXTRA_LOAD	=	(1<<2),
	CT_NORMAL		=	(1<<3),
	CT_ERROR		=	(1<<4),
	CT_CHATTER		=	(1<<5),
	CT_ECHO			=	(1<<6),
	CT_ALL			=	(0xFFFFFFFF)
};


#ifndef _DEBUG
#include "app_Console.inl"
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_APP_CONSOLE_H_)
