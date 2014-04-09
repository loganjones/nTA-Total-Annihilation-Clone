// sys.h //                                Author: Logan "Burn" Jones
//////////                                            Date: 9/30/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _SYS_H_
#define _SYS_H_
/////////////////////////////////////////////////////////////////////


#include "../../../Source/std.h"
#include <fstream>

struct NetMessage_t;
struct Parameter_t;
typedef vector<Parameter_t>	ParameterList_t;

NetMessage_t* NewNetMessage( int iType, int iHost, char* pName, char* pCode, const ParameterList_t& MsgParams );

struct NetMessage_t {
	int			Type;
	int			Host;
	char*		Name;
	char*		Code;
	ParameterList_t	Parameters;

	NetMessage_t( int iType, int iHost, char* pName, char* pCode, const ParameterList_t& MsgParams ):
		Type(iType),Host(iHost),Name(pName),Code(pCode),Parameters(MsgParams){}
};

struct Parameter_t {
	char*		Type;
	DWORD		Flags;
	char*		Name;

	Parameter_t( char* strType=NULL, DWORD dwFlags=0, char* strName=NULL ):
		Type(strType),Flags(dwFlags),Name(strName){}
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SYS_H_)
