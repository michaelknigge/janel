@echo off
@setlocal

rem Change the INCLUDE environment variable to the appropriate directories on your machine
rem set INCLUDE=%INCLUDE%;C:\Java\j2sdk1.4.2_15\include\;C:\Java\j2sdk1.4.2_15\include\win32\

echo Making the console executable
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Console|Win32"
copy ".\Release Console\Janel.exe" ..\Executables\Win32\JanelConsole32.exe

"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Console|x64"
copy ".\x64\Release Console\Janel.exe" ..\Executables\x64\JanelConsole64.exe

@endlocal