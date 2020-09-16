@echo off
setlocal

rem Use dynamic shaders to build .inc files only
rem set dynamic_shaders=1
rem == Setup path to nmake.exe, from vc 2005 common tools directory ==

rem Use the compile tools appropriate to your Visual Studio version
rem Visual Studio 2010
rem call "%VS100COMNTOOLS%vsvars32.bat"
rem Visual Studio 2012
rem call "%VS110COMNTOOLS%vsvars32.bat"
rem Visual Studio 2013
call "%VS120COMNTOOLS%vsvars32.bat"

rem ================================
rem ==== MOD PATH CONFIGURATIONS ===

rem == Set the absolute or relative path to your mod's game directory here ==
rem == if ABBR_PATHS is 1 you should make this an absolute path
set GAMEDIR=..\..\..\game\template

rem == Set the relative path to steamapps\common\Alien Swarm\bin ==
rem == As above, this path originally did not support long directory names or spaces ==
rem == e.g. ..\..\..\..\..\PROGRA~2\Steam\steamapps\common\ALIENS~1\bin ==
set SDKBINDIR=..\..\..\game\bin

rem ==  Set the Path to your mods root source code ==
rem this should already be correct, accepts relative paths only!
set SOURCEDIR=..\..

rem == Originally GAMEDIR and SDKBINDIR did not support long file/directory names, so if you have ==
rem == issues, then set ABBR_PATHS here to 1 and update the paths below: ==
rem == instead of a path such as "C:\Program Files\Steam\steamapps\mymod" ==
rem == you may need to find the 8.3 abbreviation for the directory name using 'dir /x' ==
rem == and set the directory to something like C:\PROGRA~2\Steam\steamapps\sourcemods\mymod ==
set ABBR_PATHS=0

rem ==== MOD PATH CONFIGURATIONS END ===
rem ====================================

:: transforms GAMEDIR from relative to absolute path (if it is not already).
if %ABBR_PATHS% equ 1 goto skip_path_fix
set _temp=%CD%
cd %GAMEDIR%
set GAMEDIR=%CD%
cd %_temp%
:skip_path_fix

set TTEXE=..\..\devtools\bin\timeprecise.exe
if not exist %TTEXE% goto no_ttexe
goto no_ttexe_end

:no_ttexe
set TTEXE=time /t
:no_ttexe_end


rem echo.
rem echo ~~~~~~ buildsdkshaders %* ~~~~~~
%TTEXE% -cur-Q
set tt_all_start=%ERRORLEVEL%
set tt_all_chkpt=%tt_start%

set BUILD_SHADER=call buildshaders.bat
set ARG_EXTRA=

:: === Comment/Uncomment shader lists depending on what you want to build ===

:: %BUILD_SHADER% stdshader_dx9_20b		-game %GAMEDIR% -source %SOURCEDIR%
%BUILD_SHADER% stdshader_dx9_20b_new	-game %GAMEDIR% -source %SOURCEDIR% -dx9_30
:: %BUILD_SHADER% stdshader_dx9_30			-game %GAMEDIR% -source %SOURCEDIR% -dx9_30	-force30 
%BUILD_SHADER% stdshader_dx9_30_SDK		-game %GAMEDIR% -source %SOURCEDIR% -dx9_30	-force30 


rem echo.
if not "%dynamic_shaders%" == "1" (
  rem echo Finished full buildallshaders %*
) else (
  rem echo Finished dynamic buildallshaders %*
)

rem %TTEXE% -diff %tt_all_start% -cur
rem echo.
