@ECHO OFF

:: Confirm SDKDIR variable is set
call "%~d0%~p0set_vproject"

:: Set the -game for the tools. This should be where your game or sourcemod is.
:: Eg C:\Program Files (x86)\Steam\steamapps\common\Half-Life 2\hl2 or
:: C:\Program Files (x86)\Steam\steamapps\sourcemods\my_mod
:: VBSP, VVIS, And VRAD require them. 

set gamedir="C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2010\template"

:: Load the dropped vmf.
ECHO "Loading %~1"

:: Now take the precached information and use it with the tool exes.
:: Feel Free to edit the prams if needed. This simply compiles the map like hammer would at
:: Basic, HDR compile. (Stock).
 
title Compiling Map: %1

"%SDKDIR%vbsp.exe" -game %gamedir% "%~1"
"%SDKDIR%vvis.exe" -game %gamedir% "%~1"
"%SDKDIR%vrad.exe" -both -game %gamedir% "%~1"

echo Copying BSP to mod dir is still being developed. Please manually copy.

::echo Copying BSP to mod dir.
::COPY "%~d0%~p0%~1.bsp" %gamedir%\maps\ /Y

:: We are done! Pause the process so when the mapper comes back from his coffee break, 
:: they will find that the compiling process is done!
pause