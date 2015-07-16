@echo off
echo Configuring Source SDK environment variables.

:: Set local environment variable that will only persist in processes
:: created by the current command processor shell.  We use this CD
:: trick so that we can set the variable to the canonical, absolute
:: name, without any ..'s in it.
PUSHD .

CD "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2010\template"
SET VPROJECT=%CD%

:: Here we set the path of the SDK tools. Here we find Hammer, Faceposer, etc...
set SDKDIR=C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2010\bin\

POPD

echo VPROJECT=%VPROJECT%