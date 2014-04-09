# Microsoft Developer Studio Project File - Name="Assemble Net Messages" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Assemble Net Messages - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Assemble Net Messages.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Assemble Net Messages.mak" CFG="Assemble Net Messages - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Assemble Net Messages - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Assemble Net Messages - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Assemble Net Messages - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\Source\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /out:"Release/AsmNetMsg.exe"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Release\AsmNetMsg.exe
TargetName=AsmNetMsg
SOURCE="$(InputPath)"
PostBuild_Desc=Copying Net Messages Assembler to bin
PostBuild_Cmds=copy $(TargetPath) ..\..\bin\$(TargetName).exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Assemble Net Messages - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Source\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt" /out:"Debug/AsmNetMsg.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Assemble Net Messages - Win32 Release"
# Name "Assemble Net Messages - Win32 Debug"
# Begin Source File

SOURCE=.\Source\game_NetMessages.l

!IF  "$(CFG)" == "Assemble Net Messages - Win32 Release"

# Begin Custom Build - Generating lexer form $(InputPath)
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.l
InputName=game_NetMessages

"$(InputDir)\$(InputName)_Lexer.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\bin\flex.exe" -o$(InputDir)\$(InputName)_Lexer.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "Assemble Net Messages - Win32 Debug"

# Begin Custom Build - Generating lexer form $(InputPath)
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.l
InputName=game_NetMessages

"$(InputDir)\$(InputName)_Lexer.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\bin\flex.exe" -o$(InputDir)\$(InputName)_Lexer.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Source\game_NetMessages.y

!IF  "$(CFG)" == "Assemble Net Messages - Win32 Release"

# Begin Custom Build - Generating parser from $(InputPath)
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.y
InputName=game_NetMessages

"$(InputDir)\$(InputName)_Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\bin\bison.exe" --defines -o $(InputDir)\$(InputName)_Parser.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "Assemble Net Messages - Win32 Debug"

# Begin Custom Build - Generating parser from $(InputPath)
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.y
InputName=game_NetMessages

"$(InputDir)\$(InputName)_Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\bin\bison.exe" --defines --verbose -o $(InputDir)\$(InputName)_Parser.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Source\game_NetMessages_Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_NetMessages_Lexer.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_netmessages_parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_netmessages_parser.cpp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\std_FileIO_Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\sys.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_Main.cpp
# End Source File
# End Target
# End Project
