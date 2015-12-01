@echo off

setlocal

set startdir=%cd%
cd %~p0
set scriptdir=%cd%
cd ..
set rootdir=%cd%
for /f "delims=" %%a in ('%rootdir%\tools\cygwin\bin\pwd') do @set altrootdir=%%a

cd %rootdir%

.\tools\cygwin\bin\make -s -f %altrootdir%/BUILD/SYS/makefile ROOTDIR=%altrootdir% clean 

cd %startdir%
IF NOT "%UHV_BUILD%" == "eclipse" pause