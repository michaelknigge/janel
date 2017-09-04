
SETLOCAL
if "%ANT_HOME%"=="" set ANT_HOME=C:\JavaPrograms\apache-ant-1.6.2
if "%JAVA_HOME%"=="" set JAVA_HOME=C:\JavaPrograms\j2sdk1.4.2

call %ANT_HOME%\bin\ant.bat

ENDLOCAL