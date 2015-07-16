@echo off
:: Batch file to launch vtex with proper environment

title Compiling TGA/PSD: %1

:: Confirm SDKDIR variable is set
call "%~d0%~p0set_vproject"

:: Convert textures without recompiling them.

echo Executing: "vtex.exe" %*
"%SDKDIR%vtex.exe" -game "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2010\template" %*
