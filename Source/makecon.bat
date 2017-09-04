@echo off
@setlocal

rem Change the JAVA_HOME environment variable to the directory of your JDK (*NOT* JRE).
rem set JAVA_HOME=C:\Program Files\Java\jdk1.8.0_101

echo Making the console executable
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Console|Win32"
copy ".\Release Console\Janel.exe" ..\Executables\Win32\JanelConsole32.exe

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Console|x64"
copy ".\x64\Release Console\Janel.exe" ..\Executables\x64\JanelConsole64.exe

@endlocal