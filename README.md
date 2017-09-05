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

Thanks to the free Continuous Delivery service for Windows [AppVeyor](https://www.appveyor.com/) you can always
download a version of Janel that contains my changes. See the status badges at the top of the page.

Note that my enhancements and fixes will be available in the branch _enhanced_. The _master_ branch contains an unmodified version of the Janel source code available at [Sourceforge](https://sourceforge.net/projects/janel/)

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
