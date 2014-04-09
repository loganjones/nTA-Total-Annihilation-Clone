// sys_Main.cpp //                         Author: Logan "Burn" Jones
/////////////////                                     Date: 9/30/2002
//
/////////////////////////////////////////////////////////////////////

#include "sys.h"
#include "game_NetMessages_Lexer.h"
#include "game_NetMessages_Parser.cpp.h"
#include <iostream>
#include "time.h"
int yyparse();

#define UNIT_INTERFACE_CLASS			"unit_Interface"
#define UNIT_MAIN_CLASS					"unit_Object"
#define UNIT_GROUP_CLASS				"unit_Group"
#define GAME_MAIN_CLASS					"game_Main"

vector<NetMessage_t>	Messages;

char					SourceDirectory[ MAX_PATH ];
char					FilePath[ MAX_PATH ];
const char				BorderString[] = "/////////////////////////////////////////////////////////////////////";
const char				MiddleString[] = "//\n//===================================================================\n//";
const char				AuthorString[] = "Author: AsmNetMsg.exe";
string					DateString;


#define BEGIN_CODE_INSERTION( THE_FILE_NAME ) { \
	char* pFileBuf;	DWORD FileSize;\
	if( GetFileBuffer(THE_FILE_NAME,SourceDirectory,&pFileBuf,FileSize) ) {\
	ofstream	fout( FilePath, ios::binary ); \
	char*		StartMark,*EndMark = pFileBuf

#define FIND_INSERTION_POINT( MARK_TEXT ) \
	StartMark = strstr( EndMark, "//{{AFX " MARK_TEXT ); \
	for( ; *StartMark!=0xA; ++StartMark); ++StartMark; \
	fout.write( EndMark, StartMark - EndMark ); \
	EndMark = strstr( StartMark, "//}}AFX " MARK_TEXT )

#define END_CODE_INSERTION() \
	fout.write( EndMark, (pFileBuf + FileSize) - EndMark ); \
	fout.close(); \
	delete [] pFileBuf; } }

#define BACKUP_FILE( _str_FILE_NAME_ )	CopyFile( _str_FILE_NAME_, "Backup/" _str_FILE_NAME_, FALSE )


void InsertNetCodeIntoSource();

void AddUnitInterfaceDefinitions( ofstream& fout );
void DoUnitMsgHandlerDefinitions( ofstream& fout, bool bVirtual=true );
void AddImplementations( ofstream& fout, int iHost, LPCTSTR strClassName );
void AddGameDefinitions( ofstream& fout );
void AssignHandlers( ofstream& fout, LPCTSTR strClassName );
void AddTranslators( ofstream& fout, LPCTSTR strClassName );
void MakeNetMsgSymbolFile( LPCTSTR strFileName, LPCTSTR strDir );

bool GetFileBuffer( LPCTSTR strFileName, LPCTSTR strDir, char** ppBuffer, DWORD& dwSize ); 
void MakeStrings();
void PrintFuncHeader( ofstream& fout, const string& strName );
void PrintFuncFooter( ofstream& fout, const string& strName );
void PrintFuncDecl( ofstream& fout, const string& strPreFix, const string& strName, const ParameterList_t& Params );
void PrintFuncCall( ofstream& fout, const string& strName, const ParameterList_t& Params );

void BackupFiles();


/////////////////////////////////////////////////////////////////////
// sys_Main() //                           Author: Logan "Burn" Jones
///////////////                                       Date: 9/30/2002
//
//===================================================================
//
void sys_Main()
{
	LPTSTR	File;
	LPTSTR	Start,End;
	bool	DoBackup = false;

	if( __argc<=1 )
		return ;

	// Get the input file path
	File = __argv[1];
	if( File[0]=='-' || File[0]=='+' || File[0]=='/' )
		if( stricmp(File+1,"Backup")==0 )
			DoBackup = true;
		else ;
	else if( File[0]=='\"' )
		if( End=strrchr(++File,'\"') )
			End[0] = '\0';

	// Get the output source directory
	if( __argc<=2 ) SourceDirectory[0] = '\0';
	else if( (__argv[2])[0]=='\"' )
	{
		Start = __argv[2] + 1;
		End = strrchr( Start, '\"' );
		strncpy( SourceDirectory, Start, End - Start );
		SourceDirectory[End - Start] = '\0';
	}
	else strcpy( SourceDirectory, __argv[2] );

	if( DoBackup )
		BackupFiles();

	// If the input file is valid, proceed
	else if( yyin = fopen( File, "rt") )
	{
		// Let the parser work
		yyparse();

		// Do something
		MakeStrings();
		InsertNetCodeIntoSource();

		// Close the file
		fclose( yyin );

	} // end // If the input file is valid
	else cout << "Could not open: " << File << endl;
}
// End sys_Main()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NewNetMessage() //                      Author: Logan "Burn" Jones
////////////////////                                  Date: 10/4/2002
//
//===================================================================
// Parameters:
//  int iType   - 
//  int iHost   - 
//  char* pName - 
//  char* pCode - 
//
// Return: NetMessage_t* - 
//
NetMessage_t* NewNetMessage( int iType, int iHost, char* pName, char* pCode, const ParameterList_t& MsgParams )
{
	Messages.push_back( NetMessage_t(iType,iHost,pName,pCode,MsgParams) );
	return &Messages.back();
}
// End NewNetMessage()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// InsertNetCodeIntoSource() //            Author: Logan "Burn" Jones
//////////////////////////////                       Date: 10/17/2002
//
//===================================================================
//
void InsertNetCodeIntoSource()
{
	// Splice code in the unit interface header file
	BEGIN_CODE_INSERTION( UNIT_INTERFACE_CLASS ".h" );
		FIND_INSERTION_POINT( "Net Messages" );
		AddUnitInterfaceDefinitions( fout );
	END_CODE_INSERTION();

	// Splice code in the unit interface implementation file
	BEGIN_CODE_INSERTION( UNIT_INTERFACE_CLASS ".cpp" );
		FIND_INSERTION_POINT( "Net Messages" );
		AddImplementations( fout, HOST_INTERFACE, UNIT_INTERFACE_CLASS );
	END_CODE_INSERTION();

	// Splice code in the unit and group header file
	BEGIN_CODE_INSERTION( UNIT_MAIN_CLASS ".h" );
		FIND_INSERTION_POINT( "Net Messages" );
		fout << "protected: // Net Message Handlers\n";
		DoUnitMsgHandlerDefinitions( fout, false );
	END_CODE_INSERTION();
	BEGIN_CODE_INSERTION( UNIT_GROUP_CLASS ".h" );
		FIND_INSERTION_POINT( "Net Messages" );
		fout << "protected: // Net Message Handlers\n";
		DoUnitMsgHandlerDefinitions( fout, false );
	END_CODE_INSERTION();

	// Splice code in the game header file
	BEGIN_CODE_INSERTION( GAME_MAIN_CLASS ".h" );
		FIND_INSERTION_POINT( "Net Messages" );
		AddGameDefinitions( fout );
	END_CODE_INSERTION();

	// Splice code in the game implementation file
	BEGIN_CODE_INSERTION( "game_NetMessages.cpp" );
		FIND_INSERTION_POINT( "Net Messages" );
		AddImplementations( fout, HOST_GAME, GAME_MAIN_CLASS );
		fout << "\n\n";
		AssignHandlers( fout, GAME_MAIN_CLASS );
		fout << "\n\n";
		AddTranslators( fout, GAME_MAIN_CLASS );
	END_CODE_INSERTION();

	MakeNetMsgSymbolFile( "game_NetMessages.h", SourceDirectory );
}
// End InsertNetCodeIntoSource()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// AddUnitInterfaceDefinitions() //        Author: Logan "Burn" Jones
//////////////////////////////////                   Date: 02-15-2003
//
//===================================================================
// Parameters:
//  ofstream& fout - 
//
void AddUnitInterfaceDefinitions( ofstream& fout )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	string							FullName;
	bool							bFirst;

	// Iterate through each message and make a declaration for each request
	for( msg=Messages.begin(),bFirst=true; msg!=msg_end; ++msg )
		if( (*msg).Type==START_REQUESTS && (*msg).Host==HOST_INTERFACE )
		{
			if(bFirst) fout << "public: // Requests made to the server\n", bFirst=false;
			FullName = "Request";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, "\tvoid", FullName, (*msg).Parameters );
			fout << ";\n";
		}

	// Iterate through each message and make a declaration for each notification
	for( msg=Messages.begin(),bFirst=true; msg!=msg_end; ++msg )
		if( (*msg).Type==START_NOTIFICATIONS && (*msg).Host==HOST_INTERFACE )
		{
			if(bFirst) fout << "protected: // Notifications sent to the client\n", bFirst=false;
			FullName = "Notify";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, "\tvoid", FullName, (*msg).Parameters );
			fout << ";\n";
		}

	fout << "protected: // Virtual handlers called by the translator and implemented in a derived class\n";
	DoUnitMsgHandlerDefinitions( fout );
}
// End AddUnitInterfaceDefinitions()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// DoUnitMsgHandlerDefinitions() //        Author: Logan "Burn" Jones
//////////////////////////////////                   Date: 02-15-2003
//
//===================================================================
// Parameters:
//  ofstream& fout     - 
//  bool bVirtual=true - 
//
void DoUnitMsgHandlerDefinitions( ofstream& fout, bool bVirtual )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	string							FullName;

	// Iterate through each message and make a implemenation for each message
	for( msg=Messages.begin(); msg!=msg_end; ++msg )
		if( (*msg).Host==HOST_INTERFACE )
		{
			if( (*msg).Type==START_REQUESTS ) FullName = "OnRequest";
			else FullName = "OnNotify";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, ((bVirtual) ? "\tvirtual void" : "\tvoid"), FullName, (*msg).Parameters );
			fout << ((bVirtual) ? " = 0;\n" : ";\n");
		}
}
// End DoUnitMsgHandlerDefinitions()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// AddImplementations() //                 Author: Logan "Burn" Jones
/////////////////////////                            Date: 02-15-2003
//
//===================================================================
// Parameters:
//  ofstream& fout - 
//
void AddImplementations( ofstream& fout, int iHost, LPCTSTR strClassName )
{
	vector<NetMessage_t>::iterator	msg = Messages.begin(),
									msg_end = Messages.end();
	string							FullName;

	// Iterate through each message and make an implemenation
	fout << "\n";
	for( bool first=true; msg!=msg_end; ++msg )
		if( (*msg).Host==iHost )
		{
			LPCTSTR strType = ((*msg).Type==START_REQUESTS) ? "Request" : "Notify";

			FullName = strClassName;
			FullName.append( "::" );
			FullName.append( strType );
			FullName.append( (*msg).Name );
			if( first ) first=false; else fout << "\n\n";

			PrintFuncHeader( fout, FullName );
			PrintFuncDecl( fout, "void", FullName, (*msg).Parameters );
			fout << "\n{\n";
			if( (*msg).Type==START_REQUESTS )
				fout << "\tif( theGame.IsGameServer ) On" << strType,
				PrintFuncCall( fout, (*msg).Name, (*msg).Parameters ),
				fout << ";\n\telse ";
			else fout << "\tif( theGame.IsGameServer ) On" << strType,
				PrintFuncCall( fout, (*msg).Name, (*msg).Parameters ),
				fout << ",\n\t\t";
			//fout << "theGame."<<strType<<"( "<<strType<<"_" << (*msg).Name << " )";
			fout << "net.Write().Write()\n\t\t<< (UINT8)"<<strType<<"_"<<(*msg).Name;
			if( (*msg).Host==HOST_INTERFACE ) fout << "\n\t\t<< m_UniqueID";

			ParameterList_t::const_iterator		it = (*msg).Parameters.begin(), end =(*msg).Parameters.end();
			for( ; it!=end; ++it)
				STRING_SWITCH((*it).Type)
					CASE("unit_Type")
						fout << "\n\t\t<< " << (*it).Name << (((*it).Flags&TYPE_POINTER) ? "->":".") << "Key";
					CASE("bool")
						fout << "\n\t\t<< (UINT8)" << (*it).Name;
					STRING_SWITCH_DEFAULT
						fout << "\n\t\t<< " << (*it).Name;
				END_STRING_SWITCH
			fout << ";\n}\n";
			PrintFuncFooter( fout, FullName );
		}
}
// End AddImplementations()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// AddGameDefinitions() //                 Author: Logan "Burn" Jones
/////////////////////////                            Date: 02-15-2003
//
//===================================================================
// Parameters:
//  ofstream& fout - 
//
void AddGameDefinitions( ofstream& fout )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	string							FullName;
	bool							bFirst;

	// Iterate through each message and make a declaration for each request
	for( msg=Messages.begin(),bFirst=true; msg!=msg_end; ++msg )
		if( (*msg).Type==START_REQUESTS && (*msg).Host==HOST_GAME )
		{
			if(bFirst) fout << "public: // Requests made to the server\n", bFirst=false;
			FullName = "Request";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, "\tvoid", FullName, (*msg).Parameters );
			fout << ";\n";
		}

	// Iterate through each message and make a declaration for each notification
	for( msg=Messages.begin(),bFirst=true; msg!=msg_end; ++msg )
		if( (*msg).Type==START_NOTIFICATIONS && (*msg).Host==HOST_GAME )
		{
			if(bFirst) fout << "public: // Notifications sent to the client\n", bFirst=false;
			FullName = "Notify";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, "\tvoid", FullName, (*msg).Parameters );
			fout << ";\n";
		}

	for( msg=Messages.begin(),bFirst=true; msg!=msg_end; ++msg )
		if( (*msg).Host==HOST_GAME )
		{
			if(bFirst) fout << "protected: // Net message Handlers\n", bFirst=false;
			if( (*msg).Type==START_REQUESTS ) FullName = "OnRequest";
			else FullName = "OnNotify";
			FullName.append( (*msg).Name );
			PrintFuncDecl( fout, "\tvoid", FullName, (*msg).Parameters );
			fout << ";\n";
		}
    
	fout << "private:\n";
	for( msg=Messages.begin(); msg!=msg_end; ++msg )
		fout << "\tstatic void CALLBACK "
			 << (((*msg).Type==START_REQUESTS) ? "CallRequest":"CallNotify")
			 << (*msg).Name << "( game_Main& Game );\n";
}
// End AddGameDefinitions()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// AssignHandlers() //                     Author: Logan "Burn" Jones
/////////////////////                                Date: 04-28-2003
//
//===================================================================
// Parameters:
//  ofstream& fout       - 
//  LPCTSTR strClassName - 
//
void AssignHandlers( ofstream& fout, LPCTSTR strClassName )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	string							FullName = strClassName;
	const char*						Type;
	bool							first = true;

	FullName.append( "::AssignNetMessageHandlers" );
	PrintFuncHeader( fout, FullName + "()" );
	fout << "void " << FullName << "()\n{\n";

	for( msg=Messages.begin(); msg!=msg_end; ++msg )
		Type = (((*msg).Type==START_REQUESTS) ? "Request":"Notify"),
		fout << "\tm_NetMessageHandlers["<<Type<<"_"<<(*msg).Name<<"] = &Call"<<Type<<(*msg).Name<<";\n";

	fout << "}\n";
	PrintFuncFooter( fout, FullName );
}
// End AssignHandlers()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// AddTranslators() //                     Author: Logan "Burn" Jones
/////////////////////                                Date: 02-15-2003
//
//===================================================================
// Parameters:
//  ofstream& fout       - 
//  LPCTSTR strClassName - 
//
void AddTranslators( ofstream& fout, LPCTSTR strClassName )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	string							FullName;
	bool							first = true;
	ParameterList_t::const_iterator	it, end;
	string							Offset;

	for( msg=Messages.begin(); msg!=msg_end; ++msg )
	{
		if( first ) first=false; else fout << "\n\n";

		// Set up function name
		FullName = strClassName;
		FullName.append( "::Call" );
		FullName.append( (((*msg).Type==START_REQUESTS) ? "Request":"Notify") );
		FullName.append( (*msg).Name );

		// Print header comments and code
		PrintFuncHeader( fout, FullName + "()" );
		fout << "void CALLBACK " << FullName << "( game_Main& Game )";

		fout << "\n{\n\t";
		if( (*msg).Host==HOST_GAME )
			fout << "Game.On",
			Offset = "0";
		else if( (*msg).Host==HOST_INTERFACE )
			fout << "Game.Units.GetUnitInterface(net.Read().UInt32At(0))->On",
			Offset = "sizeof(UINT32)";

		fout << (((*msg).Type==START_REQUESTS) ? "Request" : "Notify")
			 << (*msg).Name;
		if( (*msg).Parameters.empty() ) fout << "()";
		else {
			fout << "(\n\t\t";
			bool first = true;
			for( it=(*msg).Parameters.begin(),end=(*msg).Parameters.end(); it!=end; ++it)
			{
				if( first ) first=false; else fout << ",\n\t\t";
				STRING_SWITCH( (*it).Type )
					CASE( "unit_Type" )
						fout << (((*it).Flags&TYPE_REFERENCE) ? "*":"") << "Game.Units.GetUnitType(net.Read().UInt32At(" << Offset << "))",
						Offset.append( " + sizeof(UINT32)" );
					CASE( "INT8" )
						fout << "net.Read().Int8At("<<Offset<<")"; Offset.append( " + sizeof(INT8)" );
					CASE( "UINT8" )
						fout << "net.Read().UInt8At("<<Offset<<")"; Offset.append( " + sizeof(UINT8)" );
					CASE( "INT16" )
						fout << "net.Read().Int16At("<<Offset<<")"; Offset.append( " + sizeof(INT16)" );
					CASE( "UINT16" )
						fout << "net.Read().UInt16At("<<Offset<<")"; Offset.append( " + sizeof(UINT16)" );
					CASE( "INT32" )
						fout << "net.Read().Int32At("<<Offset<<")"; Offset.append( " + sizeof(INT32)" );
					CASE( "UINT32" )
						fout << "net.Read().UInt32At("<<Offset<<")"; Offset.append( " + sizeof(UINT32)" );
					CASE( "std_Point" )
						fout << "net.Read().PointAt("<<Offset<<")"; Offset.append( " + sizeof(INT32) + sizeof(INT32)" );
					CASE( "bool" )
						fout << "net.Read().UInt8At("<<Offset<<")!=0"; Offset.append( " + sizeof(UINT8)" );
					STRING_SWITCH_DEFAULT
						fout << "*(" << (*it).Type << "*)(pMessage + (" << Offset << "))",
						Offset.append( " + sizeof(" ),
						Offset.append( (*it).Type ),
						Offset.append( ")" );
				END_STRING_SWITCH
			}
			fout << " )";
		}
		fout << ";\n\tnet.Read().MoveCursor( "<<Offset<<" );\n}\n";

		// Print footer comments
		PrintFuncFooter( fout, FullName );
	}
}
// End AddTranslators()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// MakeNetMsgSymbolFile() //               Author: Logan "Burn" Jones
///////////////////////////                          Date: 02-15-2003
//
//===================================================================
// Parameters:
//  LPCTSTR strFileName - 
//
void MakeNetMsgSymbolFile( LPCTSTR strFileName, LPCTSTR strDir )
{
	vector<NetMessage_t>::iterator	msg,msg_end = Messages.end();
	ofstream						fout;
	const int						len = strlen( strFileName );

	string		NameUpper( len+2, '_' );
	for( int n=0; n<len; ++n) if(isalpha(strFileName[n])) NameUpper[n+1] = toupper(strFileName[n]);

	sprintf( FilePath, "%s%s", strDir, strFileName );
	fout.open( FilePath );
	fout << "// " << strFileName << " //"
		 << string( 70 - len - strlen(AuthorString) - 7, ' ' ) << AuthorString << endl
		 << string( len + 5, '/' )
		 << string( 70 - len - DateString.length() - 6, ' ' ) << DateString << endl
		 << MiddleString << endl << BorderString << endl
		 << "#ifndef " << NameUpper << endl
		 << "#define " << NameUpper << endl
		 << BorderString << endl << endl
		 << "enum game_NetMessages_t {\n";
	
	for( msg=Messages.begin(); msg!=msg_end; ++msg )
		fout << (((*msg).Type==START_REQUESTS) ? "\tRequest_" : "\tNotify_")
			 << (*msg).Name << ",\n";
	fout << "\tNET_MESSAGE_COUNT,\n"
		 << "}; // end enum game_NetMessages_t\n\n" 
		 << BorderString << endl << "#endif // !defined(" << NameUpper << ")";
	fout.close();
}
// End MakeNetMsgSymbolFile()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// PrintFuncHeader() //                    Author: Logan "Burn" Jones
//////////////////////                                Date: 10/5/2002
//
//===================================================================
// Parameters:
//  ofstream& fout        - 
//  const string& strName - 
//
void PrintFuncHeader( ofstream& fout, const string& strName )
{
	fout << BorderString << "\n// " << strName << " //"
		 << string( 70 - strName.length() - strlen(AuthorString) - 7, ' ' ) << AuthorString << endl
		 << string( strName.length() + 5, '/' )
		 << string( 70 - strName.length() - DateString.length() - 6, ' ' ) << DateString << endl
		 << MiddleString << endl;
}
// End PrintFuncHeader()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// PrintFuncFooter() //                    Author: Logan "Burn" Jones
//////////////////////                                Date: 10/5/2002
//
//===================================================================
// Parameters:
//  ofstream& fout        - 
//  const string& strName - 
//
void PrintFuncFooter( ofstream& fout, const string& strName )
{
	fout << "// End " << strName << "()\n" << BorderString << endl;
}
// End PrintFuncFooter()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// PrintFuncDecl() //                      Author: Logan "Burn" Jones
////////////////////                                  Date: 10/5/2002
//
//===================================================================
// Parameters:
//  ofstream& fout                - 
//  const string& strPreFix       - 
//  const string& strName         - 
//  const ParameterList_t& Params - 
//
void PrintFuncDecl( ofstream& fout, const string& strPreFix, const string& strName, const ParameterList_t& Params )
{
	ParameterList_t::const_iterator		it = Params.begin(), end =Params.end();

	fout << strPreFix << " " << strName;
	if( Params.empty() )
		fout << "()";
	else {
		fout << "( ";
		for( bool first=true ; it!=end; ++it)
		{
			if( first ) first = false;
			else fout << ", ";
			if( (*it).Flags&TYPE_CONSTANT ) fout << "const ";
			fout << (*it).Type;
			if( (*it).Flags&TYPE_REFERENCE ) fout << "& ";
			else if( (*it).Flags&TYPE_POINTER ) fout << "* ";
			else fout << " ";
			fout << (*it).Name;
		}
		fout << " )";
	}
}
// End PrintFuncDecl()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// PrintFuncCall() //                      Author: Logan "Burn" Jones
////////////////////                                 Date: 10/17/2002
//
//===================================================================
// Parameters:
//  ofstream& fout                - 
//  const string& strName         - 
//  const ParameterList_t& Params - 
//
void PrintFuncCall( ofstream& fout, const string& strName, const ParameterList_t& Params )
{
	ParameterList_t::const_iterator		it = Params.begin(), end =Params.end();
	
	fout << strName;	
	if( Params.empty() ) fout << "()";
	else {
		fout << "( ";
		for( bool first=true ; it!=end; ++it)
		{
			if( first ) first = false;
			else fout << ", ";
			fout << (*it).Name;
		}
		fout << " )";
	}
}
// End PrintFuncCall()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// GetFileBuffer() //                      Author: Logan "Burn" Jones
////////////////////                                  Date: 10/6/2002
//
//===================================================================
// Parameters:
//  LPCTSTR strFileName - 
//  LPCTSTR strDir      - 
//  char** ppBuffer     - 
//  DWORD& dwSize       - 
//
// Return: bool - 
//
bool GetFileBuffer( LPCTSTR strFileName, LPCTSTR strDir, char** ppBuffer, DWORD& dwSize )
{
	void*		hFile;
	ofstream	fout;

	// Try to open the file
	sprintf( FilePath, "%s%s", strDir, strFileName );
	hFile = fio_OpenFile( FilePath, fio_READ );
	if( hFile==NULL )
	{
		cout << strFileName << " not found, using backup... ";
		sprintf( FilePath, "%sBackup/%s", strDir, strFileName );
		hFile = fio_OpenFile( FilePath, fio_READ );
		if( hFile==NULL ){
			cout << "ERROR: Backup not found.\n";
			return false;
		} else {
			// Allocate a buffer
			dwSize = fio_GetFileSize(hFile);
			*ppBuffer = new char[dwSize];

			// Fill the buffer and close the file
			dwSize = fio_ReadFile( hFile, *ppBuffer, dwSize );
			fio_CloseFile(hFile);
			cout << "OK\n";

			// Make a new file from the backup
			sprintf( FilePath, "%s%s", strDir, strFileName );
			fout.open( FilePath, ios::binary );
			fout.write( *ppBuffer, dwSize ); fout.close();
			return true;
		}
	} else {
		// Allocate a buffer
		dwSize = fio_GetFileSize(hFile);
		*ppBuffer = new char[dwSize];

		// Fill the buffer and close the file
		dwSize = fio_ReadFile( hFile, *ppBuffer, dwSize );
		fio_CloseFile(hFile);
		return true;
	}
}
// End GetFileBuffer()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// MakeStrings() //                        Author: Logan "Burn" Jones
//////////////////                                    Date: 10/5/2002
//
//===================================================================
//
void MakeStrings()
{
	time_t	sysTime;
	tm*		LocalTime;
	char	str[32];

	time( &sysTime );
	LocalTime = localtime( &sysTime );

	sprintf( str, "Date: %d/%d/%d", LocalTime->tm_mon + 1, LocalTime->tm_mday, LocalTime->tm_year + 1900 );
	DateString = str;
}
// End MakeStrings()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// BackupFiles() //                        Author: Logan "Burn" Jones
//////////////////                                    Date: 1/17/2003
//
//===================================================================
//
void BackupFiles()
{
	GetCurrentDirectory( MAX_PATH, FilePath );
	SetCurrentDirectory( SourceDirectory );
	BACKUP_FILE( UNIT_INTERFACE_CLASS ".h" );
	BACKUP_FILE( UNIT_INTERFACE_CLASS ".cpp" );
	BACKUP_FILE( UNIT_MAIN_CLASS ".h" );
	BACKUP_FILE( UNIT_GROUP_CLASS ".h" );
	BACKUP_FILE( GAME_MAIN_CLASS ".h" );
	BACKUP_FILE( "game_NetMessages.cpp" );
	SetCurrentDirectory( FilePath );
}
// End BackupFiles()
/////////////////////////////////////////////////////////////////////


void yyerror(char *msg) { cout << "ERROR("<< line << ") " << msg << endl; }
extern "C" int yywrap(void) { return 1; }

#ifdef _WIN32
int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{ sys_Main(); return 0; }
#endif


/////////////////////////////////////////////////////////////////////
// End - sys_Main.cpp //
///////////////////////