cd %~p0
set startdir=%cd%
cd ..
set rootdir=%cd%
cd %startdir%

echo rootdir  = %rootdir%
echo startdir = %startdir%
echo cwd      = %cd%

cd %rootdir%

@echo ON
TOOLS\doxygen\bin\doxygen %startdir%\tools\Doxyfile_module_spec
pause