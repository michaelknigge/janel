@echo off
@setlocal

rem Change the JAVA_HOME environment variable to the directory of your JDK (*NOT* JRE).
rem set JAVA_HOME=C:\Program Files\Java\jdk1.8.0_101

echo Making the non-console executable
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Windows|Win32"
copy ".\Release Windows\Janel.exe" ..\Executables\Win32\JanelWindows32.exe

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Windows|x64"
copy ".\x64\Release Windows\Janel.exe" ..\Executables\x64\JanelWindows64.exe

@endlocal