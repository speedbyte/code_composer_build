@echo off

setlocal
set ORG_UHV_BUILD=%UHV_BUILD%

set startdir=%cd%
cd %~p0
set scriptdir=%cd%
cd ..
set rootdir=%cd%
rem cd %scriptdir%

rem echo rootdir   = %rootdir%
rem echo startdir  = %startdir%
rem echo scriptdir = %scriptdir%
rem echo cwd       = %cd%

call %scriptdir%\BuildMake.bat
if errorlevel 1 (
	echo BuildMake: returns error %errorlevel%
	cd %startdir%
	IF NOT "%UHV_BUILD%" == "eclipse" pause
	exit /b
)

cd %startdir%
IF NOT "%UHV_BUILD%" == "eclipse" pause
