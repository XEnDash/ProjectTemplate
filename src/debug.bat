IF [%1]==[] SET architecture=x64

IF [%1]==[x64] SET architecture=x64
IF [%1]==[x86] SET architecture=x86

IF %architecture%==x86 GOTO X86
IF %architecture%==x64 GOTO X64

:X86
SET exe=game_x86.exe
call vcvarsall.bat x86
goto DEBUG

:X64
SET exe=game_x64.exe
call vcvarsall.bat x64
goto DEBUG

:DEBUG
cd ..\bin\
devenv.exe %exe%
cd ..\src\

:END