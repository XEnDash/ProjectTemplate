IF [%1]==[] SET architecture=x64

IF [%1]==[x64] SET architecture=x64
IF [%1]==[x86] SET architecture=x86

SET src=..\src\*.cpp
SET obj=..\obj\
SET libs=user32.lib gdi32.lib winmm.lib opengl32.lib
SET optimization=/O2
SET debug=/Zi

IF %architecture%==x86 GOTO X86
IF %architecture%==x64 GOTO X64

:X86
SET exe=game_x86.exe
SET subsystem=/subsystem:windows,5.01
call vcvarsall.bat x86
goto COMPILE

:X64
SET exe=game_x64.exe
SET subsystem=/subsystem:windows,5.02
call vcvarsall.bat x64
goto COMPILE

:COMPILE
cd ..\bin\
cl.exe %src% /Fe%exe% /Fo%obj% %libs% %optimization% %debug% /link %subsystem%
cd ..\src\

:END