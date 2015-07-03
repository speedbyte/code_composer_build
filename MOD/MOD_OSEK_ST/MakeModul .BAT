@echo off
set Old_TMS470_C_DIR=%TMS470_C_DIR%
set HELPDIR=%CD%
cd ..\..\ 
set TMS470_C_DIR=%CD%\tools\CCStudio_v3.3\tms470\cgtools\include;%CD%\tools\CCStudio_v3.3\tms470\cgtools\lib;%CD%\tools\CCStudio_v3.3\tms470\rtdx\include;%CD%\tools\CCStudio_v3.3\tms470\rtdx\lib;
cd %HELPDIR%
echo ******************************
echo * Build MODUL Example        *
echo ******************************
@echo on 
call .\..\..\tools\cygwin\bin\make -f makefileModul -s
@echo off
set TMS470_C_DIR=%TMS470_C_DIR%

pause