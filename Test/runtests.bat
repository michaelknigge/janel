@echo off

SETLOCAL

if "%ANT_HOME%"=="" set ANT_HOME=C:\Programs\apache-ant-1.6.1
if "%JAVA_HOME%"=="" set JAVA_HOME=C:\Java\j2sdk1.4.2_05

REM Set "headless" to "true" to skip all GUI tests
REM Set "noadmin" to "true" to skip all tests that require elevated rights
call %ANT_HOME%\bin\ant.bat -f AntTest.xml -Dheadless=false -Dnoadmin=false

PAUSE

ENDLOCAL