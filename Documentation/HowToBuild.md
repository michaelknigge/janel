# How to build Janel with Visual Studio

These instructions were written for Visual Studio Community 2019.

## Console application (Windows character subsystem)

1. In the _Source_ directory edit the _makewin.bat_ file and change the path for _devenv.com_ and the Java Development Kit (JDK) to the correct path on your machine.
2. Execute _makecon.bat_.
3. _JanelConsole32.exe_ and _JanelConsole64.exe_ will be copied to the _Executables/Win32_ and _Executables/x64_ directories respectively.

## Non-Console application (Windows GUI subsystem)

1. In the _Source_ directory edit the _makewin.bat_ file and change the path for _devenv.com_ and the Java Development Kit (JDK) to the correct path on your machineto the correct path on your machine.
2. Execute _makewin.bat_.
3. _JanelWindows32.exe_ and _JanelWindows64.exe_ will be copied to the _Executables/Win32_ and _Executables/x64_ directories respectively.
  
## How to test

There are two sets of tests - tests that should succeed and tests that should fail. Both of them are Ant scripts executed via two different batch files. The tests' batch files are in the _Test_ directory. _runtests.bat_ runs the tests that should succeed, and _runfailtests.bat_ runs the tests that should fail.

Do the following before testing:

1.  Set environment variable `ANT_HOME` to the root directory of your Ant installation.
2.  Set environment variable `JAVA_HOME` to the root directory of your Java SDK (not JRE) installation.
3.  To execute the _runtests.bat_ and _runfailtests.bat_, open the command prompt with “Run as administrator”.


