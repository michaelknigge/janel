@ECHO OFF

START /WAIT TestConsoleFail.exe
IF ERRORLEVEL 1 ( ECHO Error level is correctly returning a 1 ) ELSE ( ECHO Error level failed to return a 1 )

