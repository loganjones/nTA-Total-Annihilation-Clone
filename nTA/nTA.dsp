# Microsoft Developer Studio Project File - Name="nTA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=nTA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nTA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nTA.mak" CFG="nTA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nTA - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "nTA - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nTA - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib zlib.lib dxguid.lib dinput8.lib dsound.lib ws2_32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
WkspDir=.
TargetPath=.\Release\nTA.exe
TargetName=nTA
SOURCE="$(InputPath)"
PostBuild_Desc=Copying exe to distribution directory
PostBuild_Cmds=copy $(TargetPath) .\Distribution\$(TargetName).exe	"$(WkspDir)/bin/AsmNetMsg.exe" -backup "$(WkspDir)/source/"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "nTA - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib zlib.lib dxguid.lib dinput8.lib dsound.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBC" /out:"Debug/nTA_dbg.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
WkspDir=.
TargetPath=.\Debug\nTA_dbg.exe
TargetName=nTA_dbg
SOURCE="$(InputPath)"
PostBuild_Desc=Copying exe to distribution directory
PostBuild_Cmds=copy $(TargetPath) .\Distribution\$(TargetName).exe	"$(WkspDir)/bin/AsmNetMsg.exe" -backup "$(WkspDir)/source/"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "nTA - Win32 Release"
# Name "nTA - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter "h;cpp;c;cxx;inl;def"
# Begin Group "Standard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\std.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Assert.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_FileIO.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_FileIO_Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\std_Math.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Math.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_OS_Includes.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Platform.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Point.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Point.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Rect.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Rect.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_RectF.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_RectF.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Size.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Size.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_SizeF.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_SizeF.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Time.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Time.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\std_Types.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Types.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Vector.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Vector2.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Vector2.inl
# End Source File
# Begin Source File

SOURCE=.\Source\std_Vector3.h
# End Source File
# Begin Source File

SOURCE=.\Source\std_Vector3.inl
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\sys.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_App.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\sys_App.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_App.inl
# End Source File
# Begin Source File

SOURCE=.\Source\sys_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_DLL.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_DLL_Win32.inl
# End Source File
# Begin Source File

SOURCE=.\Source\sys_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\sys_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\sys_Win32.cpp
# End Source File
# End Group
# Begin Group "Subsystems"

# PROP Default_Filter ""
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\gfx.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Cursor.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_DynamicImage.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Font.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Interface.inl
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Model.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Surface.h
# End Source File
# Begin Source File

SOURCE=.\Source\gfx_Terrain.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\in_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\in_DInput8.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\in_DInput8.h
# End Source File
# Begin Source File

SOURCE=.\Source\input.h
# End Source File
# Begin Source File

SOURCE=.\Source\input_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\input_Externals.h
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\snd.h
# End Source File
# Begin Source File

SOURCE=.\Source\snd_DSound8.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\snd_DSound8.h
# End Source File
# Begin Source File

SOURCE=.\Source\snd_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\snd_Externals.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\net.h
# End Source File
# Begin Source File

SOURCE=.\Source\net_DirectPlay8.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\net_DirectPlay8.h
# End Source File
# Begin Source File

SOURCE=.\Source\net_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\net_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\net_Sockets.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\net_Sockets.h
# End Source File
# Begin Source File

SOURCE=.\Source\net_Sockets.inl
# End Source File
# End Group
# End Group
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Group "nTA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\app_nTA.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\app_nTA.h
# End Source File
# Begin Source File

SOURCE=.\Source\app_nTA.inl
# End Source File
# Begin Source File

SOURCE=.\Source\app_nTA_Exposed.inl
# End Source File
# Begin Source File

SOURCE=.\Source\app_nTA_Win32.cpp
# End Source File
# End Group
# Begin Group "File Tree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\app_FileTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\app_FileTree.h
# End Source File
# Begin Source File

SOURCE=.\Source\app_FileTree.inl
# End Source File
# Begin Source File

SOURCE=.\Source\zconf.h
# End Source File
# Begin Source File

SOURCE=.\Source\zlib.h
# End Source File
# End Group
# Begin Group "Console"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\app_Console.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\app_Console.h
# End Source File
# Begin Source File

SOURCE=.\Source\app_Console.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\app.h
# End Source File
# Begin Source File

SOURCE=.\Source\app_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\app_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\app_Externals.h
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\util_RectFiller.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\util_RectFiller.h
# End Source File
# Begin Source File

SOURCE=.\Source\util_TxtParse.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\util_TxtParse.h
# End Source File
# Begin Source File

SOURCE=.\Source\util_TxtParse.inl
# End Source File
# End Group
# Begin Group "TA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\Cavedog\exptype.h
# End Source File
# Begin Source File

SOURCE=.\Source\Cavedog\sfxtype.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_3DO.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_COB.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_COB_Instructions.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_GAF.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_GUI.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_GUI.inl
# End Source File
# Begin Source File

SOURCE=.\Source\ta_HPI.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_OTA.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_OTA.inl
# End Source File
# Begin Source File

SOURCE=.\Source\ta_TDF.h
# End Source File
# Begin Source File

SOURCE=.\Source\ta_TDF.inl
# End Source File
# Begin Source File

SOURCE=.\Source\ta_TNT.h
# End Source File
# End Group
# Begin Group "Windowing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\wnd.h
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Manager.inl
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_TextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_TextBox.h
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Window.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Window.h
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Window.inl
# End Source File
# Begin Source File

SOURCE=.\Source\wnd_Window_Events.cpp
# End Source File
# End Group
# Begin Group "Gui"

# PROP Default_Filter ""
# Begin Group "Gadgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\gadget_Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_Button.h
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_ListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_ListBox.h
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_ListBox.inl
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_Slider.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_Slider.h
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_Slider.inl
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_String.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gadget_String.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Gadget.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Gadget.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Gadget.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\gui.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Messages.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Resources.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Resources.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Window.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Window.h
# End Source File
# Begin Source File

SOURCE=.\Source\gui_Window.inl
# End Source File
# End Group
# Begin Group "Front End"

# PROP Default_Filter ""
# Begin Group "Menus"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\fe_MainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_MainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_MultiplayerSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_MultiplayerSetup.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_NewMultiplayerGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_NewMultiplayerGame.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Options.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectConnection.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectGame.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_SelectMap.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Single.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Single.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Skirmish.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Skirmish.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Skirmish_Win32.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\fe.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_GuiManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_GuiManager.h
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fe_Wnd.h
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Base Objects"

# PROP Default_Filter ""
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\scene_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\scene_Object.h
# End Source File
# Begin Source File

SOURCE=.\Source\scene_Object.inl
# End Source File
# Begin Source File

SOURCE=.\Source\scene_System.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\scene_System.h
# End Source File
# Begin Source File

SOURCE=.\Source\scene_System.inl
# End Source File
# End Group
# Begin Group "Physics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\physics_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\physics_Object.h
# End Source File
# Begin Source File

SOURCE=.\Source\physics_Object.inl
# End Source File
# Begin Source File

SOURCE=.\Source\physics_System.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\physics_System.h
# End Source File
# Begin Source File

SOURCE=.\Source\physics_System.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\object.h
# End Source File
# Begin Source File

SOURCE=.\Source\object_Defs.h
# End Source File
# End Group
# Begin Group "Units"

# PROP Default_Filter ""
# Begin Group "Unit Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\unit_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Object.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Object.inl
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Object_Commands.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Object_Events.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Object_Handlers.cpp
# End Source File
# End Group
# Begin Group "Unit Factory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\unit_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Factory.inl
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Factory_Load.cpp
# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\script.h
# End Source File
# Begin Source File

SOURCE=.\Source\script_Code.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\script_Code.h
# End Source File
# Begin Source File

SOURCE=.\Source\script_Code.inl
# End Source File
# Begin Source File

SOURCE=.\Source\script_Instructions.h
# End Source File
# Begin Source File

SOURCE=.\Source\script_Machine.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\script_Machine.h
# End Source File
# Begin Source File

SOURCE=.\Source\script_Machine.inl
# End Source File
# Begin Source File

SOURCE=.\Source\script_Proccess.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\script_Proccess.h
# End Source File
# Begin Source File

SOURCE=.\Source\script_Proccess.inl
# End Source File
# Begin Source File

SOURCE=.\Source\script_Proccess_Handlers.cpp
# End Source File
# End Group
# Begin Group "Base Behaviours"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\unit_Aircraft.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Aircraft.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Ground.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Ground.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Hovercraft.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Hovercraft.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Seacraft.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Seacraft.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Structure.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Structure.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\unit.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Group.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Group.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_MoveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\unit_MoveInfo.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_MoveInfo.inl
# End Source File
# Begin Source File

SOURCE=.\Source\unit_SoundCategory.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Task.h
# End Source File
# Begin Source File

SOURCE=.\Source\unit_Type.h
# End Source File
# End Group
# Begin Group "Features"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\feature.h
# End Source File
# Begin Source File

SOURCE=.\Source\feature_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\feature_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Source\feature_Factory.inl
# End Source File
# Begin Source File

SOURCE=.\Source\feature_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\feature_Object.h
# End Source File
# End Group
# Begin Group "Weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\game_Weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_Weapon.h
# End Source File
# Begin Source File

SOURCE=.\Source\weapon.h
# End Source File
# Begin Source File

SOURCE=.\Source\weapon_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\weapon_Factory.h
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\game_Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_Player.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Player.inl
# End Source File
# Begin Source File

SOURCE=.\Source\player.h
# End Source File
# Begin Source File

SOURCE=.\Source\player_AI.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\player_AI.h
# End Source File
# Begin Source File

SOURCE=.\Source\player_Local.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\player_Local.h
# End Source File
# Begin Source File

SOURCE=.\Source\player_Network.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\player_Network.h
# End Source File
# End Group
# Begin Group "User Interface"

# PROP Default_Filter ""
# Begin Group "IGUI Derivations"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\igui_BuildButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_BuildButton.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_Menu.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_OrderButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_OrderButton.h
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\igui_ControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_ControlBar.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_ControlBar.inl
# End Source File
# Begin Source File

SOURCE=.\Source\igui_InfoBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_InfoBar.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_MiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_MiniMap.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_ResourceBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_ResourceBar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\game_UserInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_UserInterface.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_UserInterface.inl
# End Source File
# Begin Source File

SOURCE=.\Source\game_UserInterface_Load.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_Console.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_Console.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\igui_GameWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\igui_GameWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\game.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Defs.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Externals.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_Externals.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Load.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_LoadFacilitator.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_Main.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Main.inl
# End Source File
# Begin Source File

SOURCE=.\Source\game_NetMessages.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_NetMessages.nm

!IF  "$(CFG)" == "nTA - Win32 Release"

# Begin Custom Build
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.nm

BuildCmds= \
	"$(WkspDir)/bin/AsmNetMsg.exe" $(InputPath) "$(WkspDir)/source/"

"$(InputDir)/unit_Interface.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/unit_Interface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/game_Main.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/game_NetMessages.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "nTA - Win32 Debug"

# Begin Custom Build
InputDir=.\Source
WkspDir=.
InputPath=.\Source\game_NetMessages.nm

BuildCmds= \
	"$(WkspDir)/bin/AsmNetMsg.exe" $(InputPath) "$(WkspDir)/source/"

"$(InputDir)/unit_Interface.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/unit_Interface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/game_Main.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/game_NetMessages.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Source\game_Settings.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_StreamMessages.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_StreamMessages.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game_Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Source\game_Terrain.inl
# End Source File
# End Group
# End Group
# Begin Group "Resources"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\nTA.ico
# End Source File
# Begin Source File

SOURCE=.\res\nTA.rc
# End Source File
# Begin Source File

SOURCE=.\res\resource.h
# End Source File
# Begin Source File

SOURCE=.\res\TA.ico
# End Source File
# End Group
# Begin Group "Configurations"

# PROP Default_Filter "cfg"
# Begin Group "root"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Distribution\root\Startup.cfg
# End Source File
# End Group
# Begin Group "OTA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Distribution\OTA\Startup.cfg
# End Source File
# End Group
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Distribution\Log.txt
# End Source File
# End Group
# End Target
# End Project
