@echo off
@setlocal

rem Change the INCLUDE environment variable to the appropriate directories on your machine
rem set INCLUDE=%INCLUDE%;C:\Java\j2sdk1.4.2_15\include\;C:\Java\j2sdk1.4.2_15\include\win32\

echo Making the non-console executable
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Windows|Win32"
copy ".\Release Windows\Janel.exe" ..\Executables\Win32\JanelWindows32.exe

"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" Janel.sln /rebuild "Release Windows|x64"
copy ".\x64\Release Windows\Janel.exe" ..\Executables\x64\JanelWindows64.exe

@endlocal