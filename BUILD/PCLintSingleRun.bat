@echo off
cd %~p0
set startdir=%cd%
cd ..
set rootdir=%cd%
cd %startdir%

echo rootdir  = %rootdir%
echo startdir = %startdir%
echo cwd      = %cd%

cd %rootdir%

@echo on
python %startdir%\tools\GeneratePClintHelpFiles.py
python %startdir%\PclintSingleRun.py
