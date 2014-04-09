# Microsoft Developer Studio Project File - Name="Direct3D8" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Direct3D8 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Direct3D8.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Direct3D8.mak" CFG="Direct3D8 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Direct3D8 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Direct3D8 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Direct3D8 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIRECT3D8_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIRECT3D8_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D3D8.lib D3DX8.lib /nologo /dll /machine:I386 /def:".\Source\Direct3D8.def"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Release\Direct3D8.dll
TargetName=Direct3D8
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL to gfx directory
PostBuild_Cmds=copy $(TargetPath) ..\..\Distribution\gfx\$(TargetName).dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Direct3D8 - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIRECT3D8_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIRECT3D8_EXPORTS" /YX /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D3D8.lib D3DX8.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
TargetPath=.\Debug\Direct3D8.dll
TargetName=Direct3D8
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL to gfx directory
PostBuild_Cmds=copy $(TargetPath) ..\..\Distribution\gfx_dbg\$(TargetName).dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Direct3D8 - Win32 Release"
# Name "Direct3D8 - Win32 Debug"
# Begin Group "Standard / Utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\std_FileIO_Win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\std_Types.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\util_RectFiller.cpp
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\sys_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_DLL_Win32.cpp
# End Source File
# End Group
# Begin Group "Subclasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\d3d_Cursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Cursor.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_DynamicImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_DynamicImage.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Font.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Model.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Model.inl
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Model_Dynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Model_Dynamic.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Palettes.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Surface.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Terrain.inl
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_Utility.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3d_VertexArray.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\Direct3D8.def

!IF  "$(CFG)" == "Direct3D8 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Direct3D8 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Direct3D8.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Direct3D8.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Direct3D8.inl
# End Source File
# End Target
# End Project
