:: Confirm SDKDIR variable is set
call "%~d0%~p0set_vproject"

title Compiling QC: %1

"%SDKDIR%studiomdl.exe" -game "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2010\template" %*

pause