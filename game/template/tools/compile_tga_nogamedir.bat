@echo off
:: Batch file to launch vtex with proper environment

title Compiling TGA/PSD: %1

:: Confirm SDKDIR variable is set
call "%~d0%~p0set_vproject"

:: Convert textures without recompiling them.

echo Executing: "vtex.exe" %*
"%SDKDIR%vtex.exe" -dontusegamedir %*
