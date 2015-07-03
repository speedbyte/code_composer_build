@echo off
@echo ******************************
DATE /t
TIME /t
@echo ******************************
@echo * build started by:          *
@echo * %USERNAME%                 *
@echo ******************************
@echo * Recursive Build UHV NAR    *
@echo * Build in two parts         *
@echo ******************************
@echo *                            *
@echo ******************************
@echo **                          **
@echo ****Recursive Build Part 1****
@echo ******                  ******
@echo ********              ********
@echo ******************************
@echo * Generate Temp Build files  *
@echo ******************************
@rem @set 

setlocal
set old_startdir=%startdir%
set startdir=%cd%

cd %~p0
set scriptdir=%cd%
cd ..
set rootdir=%cd%
for /f "delims=" %%a in ('%rootdir%\tools\cygwin\bin\pwd') do @set altrootdir=%%a

cd %scriptdir%

rem @echo rootdir  = %rootdir%
rem @echo "altrootdir  = <%altrootdir%>"
@echo old_startdir = %old_startdir%
rem @echo startdir = %startdir%
rem @echo scriptdir = %scriptdir%
rem @echo cwd      = %cd%

set Old_TMS470_C_DIR=%TMS470_C_DIR%
set TMS470_C_DIR=%rootdir%\tools\CCStudio_v3.3\tms470\cgtools\include;%ROOTDIR%\tools\CCStudio_v3.3\tms470\cgtools\lib;%ROOTDIR%\tools\CCStudio_v3.3\tms470\rtdx\include;%ROOTDIR%\tools\CCStudio_v3.3\tms470\rtdx\lib;

rem @echo on

python tools/GeneratePClintHelpFiles.py
echo %errorlevel% > ..\build.result

cd %rootdir%
echo current working dir %cd%

python %rootdir%/TOOLS/Python_scripts/BuildScripts/GenerateBuildHelpFiles.py
echo %errorlevel% > build.result
if errorlevel 1 (
	cd %startdir%
	exit /b
)

python %rootdir%/TOOLS/Python_scripts/TraceScripts/GenerateFileID.py
echo %errorlevel% > build.result
if errorlevel 1 (
	cd %startdir%
	exit /b
)

rem @echo off
echo *                            *
echo ******************************
echo **                          **
echo ****Recursive Build Part 2****
echo ******                  ******
echo ********              ********
echo ******************************
echo *       Build UHV_NAR        *
echo ******************************
rem @echo on
call tools\cygwin\bin\make -s -f %altrootdir%/BUILD/SYS/makefile ROOTDIR=%altrootdir% %1 %2 %3 %4 %5 %6 %7 %8 %9
echo %errorlevel% > build.result
if errorlevel 1 (
	echo Fehler: %errorlevel%

	if not "%old_startdir%" == "" (
		echo cd %old_startdir% 
		cd %old_startdir% 
	) else ( 
		echo cd %startdir%
		cd %startdir%
	)
	echo Fehler: %errorlevel%
	exit /b %errorlevel%
)

if "%1" == "clean" (
	echo Skipping CRC calculation due to cleaning...
) 

@echo off
set TMS470_C_DIR=%Old_TMS470_C_DIR%

if not "%old_startdir%" == "" (
	cd %old_startdir% 
) else ( 
	cd %startdir%
)

