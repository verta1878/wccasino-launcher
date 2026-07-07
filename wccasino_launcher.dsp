# Microsoft Developer Studio Project File - Name="wccasino_launcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wccasino_launcher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "wccasino_launcher.mak".
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wccasino_launcher - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib /nologo /subsystem:windows /machine:I386 /out:"wccasino_launcher.exe"

!ELSEIF  "$(CFG)" == "wccasino_launcher - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib advapi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /out:"wccasino_launcher.exe"

!ENDIF

# Begin Target

# Name "wccasino_launcher - Win32 Release"
# Name "wccasino_launcher - Win32 Debug"
# Begin Source File

SOURCE=.\src\WCCASINO_LAUNCHER.CPP
# End Source File
# End Target
# End Project
