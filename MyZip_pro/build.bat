@echo off

set BuildName=MyZipTool.exe
set BuildPath=.\OUTPUT

cd .
if exist %BuildPath%\%BuildName% del %BuildPath%\%BuildName%
g++ -g -o %BuildPath%\%BuildName% .\*.cpp .\MyZipTools\*.cpp .\SelectArgument\*.cpp .\BufferDecoder\*.cpp -static -Wall
if errorlevel 1 goto Error_Line
goto END_Line

:Error_Line
echo.
echo build has error!
pause
goto END_Line


:END_Line
exit
