[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/yelrh6y19v32ktg6/branch/enhanced?svg=true)](https://ci.appveyor.com/project/michaelknigge/janel/branch/enhanced)
[![Download official version](https://img.shields.io/badge/Download-Official_version-orange.svg)](https://sourceforge.net/projects/janel/files/latest/download)
[![Download enhanced version](https://img.shields.io/badge/Download-Enhanced_version-orange.svg)](https://ci.appveyor.com/api/projects/michaelknigge/janel/artifacts/janel-enhanced.7z?branch=enhanced)

# Java Native Executable / Launcher
Janel is a free, Open Source Java native executable and launcher for Windows. Launch your Java application just like other
Windows executables. Within the configuration file set the location of the JVM, any of the Java system properties such as
classpath, and more. A choice of non-console and console applications is available. The process name is whatever you choose
for your .exe file. New for Janel 4.x is the ability to use the Janel executable as a Windows service.

The homepage of Janel is on [Sourceforge](https://sourceforge.net/projects/janel/). There you can download official releases.

# GitHub vs. Sourceforge
This project is a spin-off of the great [Janel](https://sourceforge.net/projects/janel/) by Timothy Kil.
Since version 3.4.0 of Janel I'm contributing code that enhances Janel and I always have passed my enhancements
to Timothy. Some of my enhancements and fixes of version 4.0.4 sadly got lost on the way to the official version.
So I decided to create this GitHub project that makes it a lot easier for me to bring my enhancements and fixes
to the public. 

I'll continue to push all my contributions to Timothy so he can integrate them into the official Janel distribution
(and I really hope that Timothy will integrate them because I'm no friend of fragmentation and I have no interest
in two competing Janel distributions).

Thanks to the free Continuous Delivery service [AppVeyor](https://www.appveyor.com/) you can always
download a version of Janel that contains my changes. See the status badges at the top of the page.

**Note that my enhancements and fixes will be available in the branch _enhanced_. The _master_ branch contains an unmodified version of the Janel source code available at [Sourceforge](https://sourceforge.net/projects/janel/)**

# Enhancements
These are my enhancements to the official Version of Janel 4.2.0
## Support of Java 9
Java 9 is fully supported (JDK and JRE). With version 9 the Windows registry keys that hold version information about installed Java versions changed. Furthermore, the JDK does no longer contain a subdirectory "jre" that contains the JRE. Due to these changes Janel had to be enhanced.
## Fix crash at startup if an environment variable is missing
Version 4.2.0 of Janel will crash at startup if you reference an environment variable in the LAP file that is not set. Imagine you want to add specific JARs to the CLASSPATH at runtime and specify

    janel.classpath.jars.dir.recursive=${env.USER_DEFINED_JARS_FOLDER}
    
in the LAP file. Now if the environment variable USER_DEFINED_JARS_FOLDER is not set, Janel will crash. My enhanced version fixes this bug.

## Fix crash at startup if the bitness of the JVM and Janel does not match
If you set the path of the JVM to be used, i. e. with

    janel.java.home.path=C:\Program Files\Java\jdk1.8.0_101
    
then Janel crashes if the bitness of the JVM does not match the bitness of the Janel executable. My enhanced version shows an error message that describes the problem to the user:

    Error in Java launcher. The DLL "C:\Program Files\Java\jdk1.8.0_101\jre\bin\server\jvm.dll"
    could not be loaded. The bitness of the DLL and the running
    process "JanelConsole32.exe" are different

## Specify the behaviour of bitness mismatch
Now you can specify how Janel should behave if the bitness of the JVM and the Janel executable does not match. Per default (see above) Janel will exit with an error message. But now you can also instruct Janel that it shall continue to look for a JVM that can be used. An example:

    janel.jvm.bitness.mismatch=skip
    janel.java.home.path=${env.JAVA_HOME}

Now Janel will try to use the JVM located in the directory specified in the environment variable JAVA_HOME - but if the bitness mismatches, Janel will continue to look for another suitable JVM.

## Specify JVM options on the command line
Janel uses the LAP file to pass options to the JVM. But for command line applications it is often desireable to specify (or override) JVM options at the command line. With the new Janel property

    janel.allow.jvm.options=true

exactly this is possible. If set to true, Janel will treat each command line parameter that starts with a single "-" as a Java option and pass this to the JVM. Note that these options are passed after the options specified in the LAP file. So for example if you specify the option -Xmx4G in the LAP file and -Xmx12G at the command line, the last one (12G) will win.

# License
MIT License

Copyright (c) 2017 Timothy Kil, Michael Knigge and others who had contributed code to Janel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
