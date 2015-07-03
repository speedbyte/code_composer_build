@echo off
REM to skip pause for auto build change to UHV build mode
set UHV_BUILD=eclipse

echo *********************************************
echo * call clean all with error and log output  *
echo *********************************************
call MakeClean.bat > clean.log 2> clean.err

echo *********************************************
echo * call make all with error and log output   *
echo *********************************************
call Make.bat > build.log 2> build.err

echo *********************************************
echo * build all finished                        *
echo *********************************************

