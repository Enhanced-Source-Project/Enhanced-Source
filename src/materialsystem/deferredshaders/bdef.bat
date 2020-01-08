@echo off
setlocal

rem Use dynamic shaders to build .inc files only
rem set dynamic_shaders=0
rem == Setup path to nmake.exe, from vc 2005 common tools directory ==
call "%VS140COMNTOOLS%vsvars32.bat"

rem ================================
rem ==== MOD PATH CONFIGURATIONS ===

rem == Set the absolute path to your mod's game directory here ==
rem == Note that this path needs does not support long file/directory names ==
rem == So instead of a path such as "C:\Program Files\Steam\steamapps\mymod" ==
rem == you need to find the 8.3 abbreviation for the directory name using 'dir /x' ==
rem == and set the directory to something like C:\PROGRA~2\Steam\steamapps\sourcemods\mymod ==
set GAMEDIR=D:\Code\Enhanced-Source\game\deferred

rem == Set the relative path to SourceSDK\bin\orangebox\bin ==
rem == As above, this path does not support long directory names or spaces ==
rem == e.g. ..\..\..\..\..\PROGRA~2\Steam\steamapps\<USER NAME>\sourcesdk\bin\orangebox\bin ==
set SDKBINDIR=..\..\..\game\bin

rem ==  Set the Path to your mods root source code ==
rem this should already be correct, accepts relative paths only!
set SOURCEDIR=..\..

rem ==== MOD PATH CONFIGURATIONS END ===
rem ====================================





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

%BUILD_SHADER% deferred_shaders_short -game %GAMEDIR% -source %SOURCEDIR% -dx9_30 -force30
:: -force30


echo.
if not "%dynamic_shaders%" == "1" (
  echo Finished full buildallshaders %*
) else (
  echo Finished dynamic buildallshaders %*
)

%TTEXE% -diff %tt_all_start% -cur
echo.
