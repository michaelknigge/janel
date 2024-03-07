@echo off
mkdir Executables
mkdir Executables\Win32
mkdir Executables\x64

cd Source

msbuild salamand.sln /t:build "/p:Configuration=Release Console" /p:Platform=Win32
msbuild salamand.sln /t:build "/p:Configuration=Release Console" /p:Platform=x64
msbuild salamand.sln /t:build "/p:Configuration=Release Windows" /p:Platform=Win32
msbuild salamand.sln /t:build "/p:Configuration=Release Windows" /p:Platform=x64

copy "Release Console\Janel.exe" ..\Executables\Win32\JanelConsole32.exe
copy "Release Windows\Janel.exe" ..\Executables\Win32\JanelWindows32.exe
copy "x64\Release Console\Janel.exe" ..\Executables\x64\JanelConsole64.exe
copy "x64\Release Windows\Janel.exe" ..\Executables\x64\JanelWindows64.exe

cd ..

set DISTRIBUTION=Distribution
mkdir %DISTRIBUTION%


copy Documentation\HowToRun.html %DISTRIBUTION%
copy "Source\Release Windows\Janel.exe" %DISTRIBUTION%\JanelWindows32.exe
copy "Source\Release Console\Janel.exe" %DISTRIBUTION%\JanelConsole32.exe
copy "Source\x64\Release Windows\Janel.exe" %DISTRIBUTION%\JanelWindows64.exe
copy "Source\x64\Release Console\Janel.exe" %DISTRIBUTION%\JanelConsole64.exe
copy JanelLicense.txt %DISTRIBUTION%

git log > %DISTRIBUTION%\Changelog.txt

"C:\Program Files\7-Zip\7z.exe" a -r janel-%GITHUB_SHA%.7z %DISTRIBUTION%
