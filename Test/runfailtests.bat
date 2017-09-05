@echo off

SETLOCAL

if "%ANT_HOME%"=="" set ANT_HOME=C:\Programs\apache-ant-1.6.1
if "%JAVA_HOME%"=="" set JAVA_HOME=C:\Java\j2sdk1.4.2_05

REM Set "headless=true" to disable all GUI tests
call %ANT_HOME%\bin\ant.bat -f AntTestFail.xml -Dheadless=false

PAUSE

ENDLOCAL