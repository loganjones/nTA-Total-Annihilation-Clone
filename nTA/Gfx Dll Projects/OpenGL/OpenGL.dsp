# Microsoft Developer Studio Project File - Name="OpenGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OpenGL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenGL.mak" CFG="OpenGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenGL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenGL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENGL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENGL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OpenGL32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\OpenGL.dll
TargetName=OpenGL
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL to gfx directory
PostBuild_Cmds=copy $(TargetPath) ..\..\Distribution\gfx\$(TargetName).dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENGL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENGL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OpenGL32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
TargetPath=.\Debug\OpenGL.dll
TargetName=OpenGL
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL to gfx directory
PostBuild_Cmds=copy $(TargetPath) ..\..\Distribution\gfx_dbg\$(TargetName).dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "OpenGL - Win32 Release"
# Name "OpenGL - Win32 Debug"
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

SOURCE=.\Source\ogl_Cursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Cursor.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_DynamicImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_DynamicImage.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Font.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Model.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Palettes.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Surface.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Source\ogl_Terrain.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\gfx_OpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_OpenGL.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_OpenGL_Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\OpenGL.def
# End Source File
# End Target
# End Project
