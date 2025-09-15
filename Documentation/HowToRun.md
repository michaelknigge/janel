# How to run Janel


## Short answer
The short answer to how-to run Janel is: Go to Janel's _Executables\\Win32_ directory. Change _JanelConsole32.exe_ to the name of your Java application that you want to launch, such as _MyApp.exe_. Rename a ```.lap``` file to _MyApp.lap_. In the .lap file set the property janel.main.class to the fully qualified name of the class that has your main method, and set the ```-Djava.class.path``` property to the desired classpath. Copy _MyApp.exe_ and _MyApp.lap_ to a directory, and you should be ready to launch your Java application with Janel.

Another example:

1. Go to Janel's _Executables_ directory.
2. Rename _JanelWindows32.exe_ to _MyExampleApp.exe_.
3. Rename _JanelWindows32.lap_ to _MyExampleApp.lap_.
4. Set the contents of MyExampleApp.lap to be:  
   ```
   janel.main.class=example.ClassThatHasMainMethod
   -Djava.class.path=lib\MyExample.jar
   ```
5. Put _MyExampleApp.exe_ and _MyExampleApp.lap_ in the same directory.
6. _MyExampleApp.exe_ is ready to launch.

## Files included
There are four binary Janel executables (listed below) included in the downloaded distribution file:

- _JanelWindows32.exe_ is for 32-bit (x86) Windows and does not create a console when launched and reports errors using pop-ups.
- _JanelWindows64.exe_ is for 64-bit (x64) Windows but otherwise behaves the same as JanelWindows32.exe.
- _JanelConsole32.exe_ is for 32-bit (x86) Windows and creates a console when launched and reports errors by writing to the console.
- _JanelConsole64.exe_ is for 64-bit (x64) Windows but otherwise behaves the same as JanelConsole32.exe.

There are example configuration files for the launcher, these have the extension of .lap.

## Windows service

The executable _JanelConsole32.exe_ and _JanelConsole64.exe_ can also run as a windows service. See the ```janel.service.*``` properties for the configuration (.lap) file.

## How the configuration (.LAP) file is found
The executable will only search the directory which it is in for a .lap file with the same name as itself. For example, _JanelWindows32.exe_ will look for _JanelWindows32.lap_ only within the directory in which _JanelWindows32.exe_ resides.

# Structure of a .LAP file
The .lap files are similar to Java properties files. Most of the properties have their names on the left separated by an equal sign, but for some properties an equals sign is not needed.

A `#` (hash) can be used for a comment but can only be used at the start of a line with no property definitions before it on the line.  
An example of a comment in a .lap file:  
`# this is a comment in a .lap file`

A `^` (caret) can be used to continue a line. It must be the last character in the line.  An example of a line continuation in a .lap file:  
```
-Djava.class.path=C:\path\to\some\where;^
..\..\Test\TestLaunch\dist\TestLaunch.jar
```

Currently there are two kinds of properties, custom properties that start with **janel** and Java system properties that start with **-**.

## Custom properties in a .LAP file
All custom properties are case-sensitive. The custom properties available are:

* ```janel.main.class``` : the main Java class to call, for example _mypackage.Start_ or _mypackage/Start_
* ```janel.main.argument``` : an argument to be passed to the Java main method. Multiple instances of this property may be used. The order of arguments corresponds to order in .lap file. These arguments follow any arguments passed at the actual command-line. For details see below.
* ```janel.allow.jvm.options``` : true/false, yes/no to allow setting of JVM options at the command line. Defaults to false. For details see below.
* ```janel.bin.jvm.dir``` : the type of JVM. Its current name reflects the fact that it's the directory name that holds the _jvm.dll_. Possible values are _prefer\_client_ (the default), _prefer\_server_, _require\_client_ (the same as just client), and _require\_server_ (the same as server). _prefer_ means that Janel will use the other type of JVM if necessary. All values are case-insensitive.
* ```janel.java.bundle``` : informs Janel whether to look for the JVM from a SDK or a JRE install. The possible values are _prefer\_jre_ (the default), _prefer\_sdk_, _require\_jre_ (the same as JRE), and _require\_sdk_ (the same as SDK). _prefer_ means that Janel will use the other Java installation if necessary. All values are case-insensitive.
* ```janel.jvm.path``` : the path to the _jvm.dll_ file, example _C:\\Java\\bin\\client\\jvm.dll_. If set but file does not exist, Janel will fall back to using another installed Java installation. If the the bitness of the JVM is different to the bitness of the runnung process, Janel will fail (default) or fall back to using another installed Java installation. See the Janel property ```janel.jvm.bitness.mismatch``` for details.
* ```janel.java.home.path``` : the Java home path, the root directory of the Java installation. For SDK installations the Java home path is the directory that holds directory _jre_, which holds the _bin_ directory. For JRE installations it is the directory that holds the _bin_ directory. If set but directory does not exist, Janel will fall back to using another installed Java installation. If set bit the bitness of the JVM is different to the bitness of the runnung process, Janel will fail (default) or fall back to using another installed Java installation. See the Janel property ```janel.jvm.bitness.mismatch``` for details.
* ```janel.jvm.bitness.mismatch``` : _fail_ or _skip_ specifies the desired behaviour of Janel when the bitness of the JVM (specified with ```janel.jvm.path``` or ```janel.java.home.path```) differs from the bitness of the running process (the Janel executable). The default behaviour is to fail (Janel will display a error message). If set to skip, Janel will ignore the specified JVM and fall back to using another installed Java installation.
* ```janel.min.java.version``` : the minimum version of the JVM, example 1.4.1
* ```janel.max.java.version``` : the maximum version of the JVM, example 1.7.0
* ```janel.trap.console.ctrl``` : _true_ or _false_ (_yes_ or _no_ works too) to trap console control commands and call an _initiateExit(int)_ static method in the main Java class. Defaults to false.
* ```janel.classpath.jars.dir``` : the path to one directory of jars that will be included in the classpath. Multiple instances of this property can be used to include multiple directories.
* ```janel.classpath.jars.dir.recursive``` : like ```janel.classpath.jars.dir```, but the specified directory is scanned recursively for jar files. Please note that the order of the jars on the classpath is undefined!
* ```janel.library.path.dir``` : the path to be added to Java system property ```java.library.path```. which is used to locate native DLLs. Multiple instances of this property may be used.
* ```janel.library.path.dir.recursive``` : like ```janel.library.path.dir```, but also all subdirectories of the specified directory are added to the library path. Please note that the order of the directories is undefined!
* ```janel.working.dir``` : just before the main class gets called, Janel will change the current working directory to the specified directory. Normally you should specify only one of the supported property value variables like _${CALLER\_DIR}_ or _${SELF\_HOME}_. Starting with Janel 3.0 the default is _${SELF\_HOME}_.
* ```janel.sysprop.process.id``` : _true_ or _false_ (_yes_ or _no_ works too) to send the Java system property process.id equal to the process id. Defaults to false.
* ```janel.error.default.text``` : overrides the default error text (_Error in Java launcher._) with a custom error message.
* ```janel.error.show.detail``` : _true_ or _false_ (_yes_ or _no_ works too) enables detailed error messages. If _true_, the default error message will be displayed followed by the details. Defaults to _true_.
* ```janel.environment.file``` : file (properties format) that contains additional environment variables to be set. No error will be thrown if the specified file can not be opened.
* ```janel.include.file``` : file (LAP format) that will be included at this point. This allows nesting and/or concatenating of LAP files. No error will be thrown if the specified file can not be opened. Furthermore no loop detection is performed. If you include file _foo.lap_ that includes _ba.lap_ that includes _foo.lap_ - you get an infinitive loop. You get what you've asked for.
* ```janel.debug.file``` : the file which will have debugging messages written to it.
* ```janel.memory.max.available.percent``` : sets the max memory via ```-Xmx``` as percent of available memory. Use only integers, no percent signs or decimals. 50 (not .50) is equal to 50 percent.
* ```janel.memory.init.available.percent``` : sets the initial memory via ```-Xms``` as percent of available memory. See above for percent explanation.
* ```janel.memory.max.total.percent``` : sets the max memory via ```-Xmx``` as percent of total physical memory. See above for percent explanation.
* ```janel.memory.init.total.percent``` : sets the initial memory via ```-Xms``` as percent of total physical memory. See above for percent explanation.
* ```janel.memory.max.upper.limit``` : sets in kilobytes the upper limit of the max memory as set by the other Janel memory custom properties.
* ```janel.memory.max.lower.limit``` : sets in kilobytes the lower limit of the max memory, similar to ```janel.memory.max.upper.limit```.
* ```janel.memory.init.upper.limit``` : sets in kilobytes the upper limit of the initial memory, similar to ```anel.memory.max.upper.limit```.
* ```janel.memory.init.lower.limit``` : sets in kilobytes the lower limit of the initial memory, similar to ```janel.memory.max.upper.limit```.
* ```janel.memory.check.limits``` : true/false, yes/no to limit the maximum heap space requested by the Janel memory settings to the largest available memory block. Defaults to false.
* ```janel.splash``` : relative path to an image file (GIF, PNG, JPEG) to be displayed as a splash screen upon start up. The path is relative to the executable.
* ```janel.service.name``` : name of the windows service to be used. If this property is specified, Janel will behave as a windows service.
* ```janel.service.display.name``` : The display name of the windows service. If this property is not specified, ```janel.service.name``` will be used.
* ```janel.service.description``` : A detailed description of the service. If this property is not specified, no description will be set. Janel will update the description on every start of the service. A recommended part of description would be release number.
* ```janel.service.option.install``` : If present, Janel will install the started executable as a windows service. The property ```janel.service.name must``` also be specified.
* ```janel.service.option.uninstall``` : If present, Janel will uninstall the windows service with the name specified in the property ```janel.service.name```.

Below is an example of a custom property in a .lap file. The Janel executable will call the main method in class _com.redskaper.Start_.
`janel.main.class=com.redskaper.Start`

## Determining the JVM with custom properties in a .LAP file
The exact path to the _jvm.dll_ that will be used to call the main method of the Java class is determined in the following order by setting Janel's custom properties. You do not have to think through any of these steps if you just want to use the JVM that is the latest version installed on the local PC. Just be sure to **NOT** set ```janel.jvm.path```, ```janel.java.home.path```, ```janel.java.bundle```, nor ```janel.bin.jvm.dir```. You will still need to set ```janel.main.class``` in all cases.

1. ```janel.jvm.path``` is set to the desired existing jvm.dll. The version of JVM will not be checked.
2. ```janel.java.home.path``` is set to an existing directory. The version of JVM will not be checked.
3. ```janel.java.bundle``` is set. ```janel.bin.jvm.dir``` will be assumed to be _prefer\_client_. The version of JVM will be checked.
4. ```janel.bin.jvm.dir``` is set. ```janel.java.bundle``` will be assumed to be _prefer\_jre_. The version of JVM will be checked.
5. Both ```janel.java.bundle``` and ```janel.bin.jvm.dir``` are set. The version of JVM will be checked.
6. None of the above custom properties are set. ```janel.bin.jvm.dir``` defaults to _prefer\_client_. ```janel.java.bundle``` defaults to _prefer\_jre_. The version of JVM will be checked.

## Java system properties in a .LAP file
Java system properties start with a `-` (dash) and are passed directly to the JVM. For example, `-Djava.class.path=C:\MyPrograms\program.jar`, `-verbose:gc`, and `-Xincgc` would be passed as is to the JVM.

For a list of the Java system properties a good place to look is the J2SE Javadocs for System.getProperties(). The standard options available are the system properties that are assigned with `-D` and the `-verbose[:class|gc|jni]` options. The nonstandard options are dependent upon the JVM and begin with `-X`. For a list of the available nonstandard options run `java.exe -X`.

Below is an example of a Java system property in a .lap file. The Java system property, this one corresponding to the classpath, will be set to the named jar.  
`-Djava.class.path=dist\TestLaunch.jar`

To send the process id of the executing process to the Java application as Java system property _process.id_ set the following in the .lap file.  
`janel.sysprop.process.id=true`  
This is the same as `-Dprocess.id=X`, where _X_ is the process id.

## Property value variables in a .LAP file
Properties may also contain variables that are resolved by the Janel executable. Their format is `${property value variable}`. For example, a property may be `-DJAVA_HOME=${FOUND_JAVA_HOME}`. Currently there are these property value variables available:

* ```FOUND_JAVA_HOME``` is the Java home path that is determined by the Janel executable
* ```FOUND_EXE_FOLDER``` is the path to the Janel executable. Same as _SELF\_HOME_ but does not remove the trailing _\\bin_.
* ```FOUND_EXE``` is the path and name to the Janel executable.
* ```SELF_HOME``` is the path to the Janel executable with a trailing _\\bin_ removed if it is present.
* ```SELF_NAME``` is name of the Janel executable (without the path) with a trailing _.exe_ removed.
* ```SELF_PARENT_NAME``` is name of the directory that contains the Janel executable (without the path). If Janel executable is in a directory named _bin_, than ```SELF_PARENT_NAME``` contains the name of the parent-parent directory. In example, ```SELF_PARENT_NAME``` will contain _MyApp_ when the Janel executable is in _C:\\Program Files\\MyApp_ or in _C:\\Program Files\\MyApp\\bin_.
* ```CALLER_DIR``` is the path to the directory in which the executable was called. Calling executable _C:\\Dev\\MyApp.exe_ from directory _C:\\Temp_ would place _C:\\Temp_ in ```$CALLER_DIR```.
* ```MAINCLASS_NAME``` is the name of the Java main class as specified in the property ```janel.main.class```. Note that the separator character is always the ```.``` (dot). So if _mypackage/Start_ was specified as the main class, this variable will contain _mypackage.Start_.
* ```MAINCLASS_SIMPLE_NAME``` is the simple name of the Java main class as specified in the property ```janel.main.class```. I. e. if _mypackage.Start_ was specified as the main class, this variable will contain _Start_.
* ```EXE_BITNESS``` is set to 32 or 64, depending on the bitness of the current running executable.
* ```OS_BITNESS``` is set to 32 or 64, depending on the bitness of operating system.

Below is an example of a property value variable in a .lap file. The Java system property called found.exe.folder.test will be set to the path to the Janel executable.  
`-Dfound.exe.folder.test=${FOUND_EXE_FOLDER}`
s
## Environment variables as property value variables in a .LAP file

Any environment variable may also be used as a property value variable by prefixing it with `env.`. For example, `${env.CATALINA_HOME}` would resolve to the value of the environment variable `CATALINA_HOME`. Note that a non-existing environment variable is substituted by an empty string. So `janel.include.file=${env.APPL_CONFIGDIR}\custom.lap` will resolve to `janel.include.file=\custom.lap` if the environment variable _APPL_CONFIGDIR_ is not set.

There may also a special operator be specified with the name of the environment variable.

* ```:#``` If the environment variable is not set or resolves to an empty value, the complete property is ignored. This might be useful if you include another file and make sure that no _wrong_ file gets included. For example `janel.include.file=${env.APPL_CONFIGDIR:#}\custom.lap` will be completely ignored if the environment variable _APPL_CONFIGDIR_ is not set or contains an empty value.
* ```:!``` If the environment variable is not set Janel will exit with an error message. For example if the environment variable _APPL_CONFIGDIR_ is not set, the property `janel.include.file=${env.APPL_CONFIGDIR:!}\custom.lap` will cause Janel to exit with the error message _Required environment variable APPL_CONFIGDIR is not set_.
* ```:-defaultValue``` If the environment variable is not set Janel will subsitute the environment variable with the specified default value. For example `janel.include.file=${env.APPL_CONFIGDIR:-.}\custom.lap` will be resolved to `janel.include.file=.\custom.lap` if the environmant variable _APPL_CONFIGDIR_ is not set.
* ```:=defaultValue``` Same as above, but the environment variable will additionally be set to the specified default value.

## Command line arguments
Command-line arguments passed to the Janel executable will be passed as arguments to the main method of the Java class. Command-line arguments can also be specified by using multiple instances of the `janel.main.argument` custom property in the .LAP file. Arguments passed to the Janel executable will precede arguments specified with `janel.main.argument`.

Below is an example of passing arguments to the Janel executable. MyApp.exe will pass the two arguments to the main method in the Java class (provided you have a MyApp.lap file correctly setup in MyApp.exe's directory).  
`MyApp.exe argument1 argument2`

## JVM options on the command line
Normally all options for the JVM have to be specified in the .LAP file. But for some special cases, you may need to use different JVM options for every started instance of your application (i. e. different settings for the -Xmx option). When the custom property `janel.allow.jvm.options` is set to yes/true, every argument on the command line that starts with a single hyphen is treated as a JVM option. These options are passed to the JVM on creation and are not passed to the Java main method.

Note that the options specified on the command line will follow the JVM options specified in the .LAP file. So, for example, if you specify `-Xmx1024M` in the .LAP file and `-Xmx2048M` on the command line, both options will be passed to the JVM, but the JVM will use the `-Xmx2048M` setting (this is an implementation detail of the JVM, but currently the Oracle JVM and the IBM JVM behave this way).

## Error messages
Janel has three possible parts to an error message:

1. The default text that precedes the detail of the error. This is by default _Error in Java launcher._ This can be set using the `janel.error.default.text` custom property.
2. The details of the error which follows the default text. Whether the detail is displayed_or not is set with the `janel.error.show.detail` custom property.
3. The outputted error message that occurs when an uncaught exception in the Java program causes it_to terminate. This is written to standard `error `before the other two error messages are displayed.

## Configuring and running a Windows service
Below is a sample .lap file for a Janel executable that operates as a service:

```
janel.main.class=myServiceMainClass
janel.min.java.version=1.8
janel.working.dir=${SELF_HOME}
janel.service.name=myService
janel.service.display.name=My Java-Service
janel.service.description=JANEL-powered Java-Service 1.0
janel.service.option.install=--install
janel.service.option.uninstall=--uninstall
```

* In this example running the Janel executable with the command line parameter `--install` will install a new service named _myService_ with the display name _My Java-Service_ and the specified description. Please note that all arguments that follow the `--install` will be passed to the service every time the service is started.
* The service can now be started. For example from the command line issue `sc start myService` to start the myService service. The service can be stopped in the same way. For example from the command line `sc stop myService` to stop the _myService_ service.
* When the service is requested to stop (either by the user or due to a system shutdown) the static method `initiateExit(int)` in the main class is invoked. This method is responsible for ending the service gracefully (return from the main method).
* When the JVM terminates the process, i. e. if `System.exit()` is invoked, the sercice will stop with an error message (error code 109, broken pipe).
* In this example running the Janel executable with the command line argument `--uninstall` will uninstall the service. Note that the service must not be started at the time of uninstalling.
* Error messages will be shown if the Janel executable is started as a console program.

## Executable's resources
Resources, in this case, are the resources compiled into the executable. Examples of such resources are the icon file and version information.

To edit the resources of the executable use either Visual Studio, or possibly one of the following Open Source resource editors will work for you: [ResourceExplorer](http://www.wilsonc.demon.co.uk/d10resourceeditor.htm) or [Resource Hacker](http://www.users.on.net/johnson/resourcehacker/).

## Custom icons
Custom icons for the executable can be set in one of two ways:

1. Use one of the resource editors mentioned above to replace the generic icons with your own custom icons.
2. Or replace the App.ico file with your own custom icon file, naming it App.ico. Then rebuild Janel following the instructions in [How to build](./HowToBuild.md).

## Versioning
To set the version of the executable, so that it can be viewed in the properties of the executable using Windows Explorer, you will need to add a Version resource to the executable. Use Visual Studio or, if one is available, a resource editor that allows you to add a Version resource. The following instructions are for Visual Studio.

1. Open the executable file with Visual Studio. You will see a view of the resources.
2. Right-click the name of the executable and select _Add Resource..._
3. Select _Version_.
4. Add your version information.
5. Save All.

## Frquently ased questions

### How to get janel.library.path.dir to include the same default paths as java.library.path normally contains?
The Java system property java.library.path normally includes the local directory ```.``` (dot) and whatever is in the _PATH_ environment variable. To include those directories when using ```janel.library.path.dir``` you will need to add them to your ```janel.library.path.dir``` specification in the .lap file. The following line would add the normal default paths and directory _src\\NativeTest\\dist_ to the directories that would be searched when trying to locate native DLLs.  
`janel.library.path.dir=src\NativeTest\dist;.;${env.PATH}`  
Also multiple ```janel.library.path.dir``` can be used in the .lap file.

### Which directory are relative paths in the .lap relative to?
Starting with Janel 3.0 relative paths within the .lap file are relative to the directory containing the executable, not relative to the directory from which the executable was launched. Please note that if you change to a working directory other than ```${SELF_HOME}``` (with the custom property janel.working.dir), then you should prefix all paths with ```${SELF_HOME}``` to make sure that all paths are resolved correctly, regardless of the current working directory.

### Which character set does Janel use?
Janel uses the Unicode character set so it is compatible with most languages.

### Why does my Windows Service not end?
Make sure you have implemented an _initiateExit(int)_ static method in your main Java class. This method gets called if the service is requested to stop.

### Why do I get a "broken pipe error" when I try to stop my Windows Service with the command line utility “sc”?
See the question above, first you have to implement the _initiateExit(int)_ static method in your main class. The responsibility of this method is to shut down your Windows Service (in other words, this method has to make sure that your main static method returns). If you do anything that simply terminates the process, i.e. calling _System.exit()_, Janel has no chance to inform Windows that the service has successfully ended (and furthermore, for the Service Control Manager the service has ended unexpectedly which in turn triggers the _broken pipe error_).

### When I try to stop my service (by using the GUI) it takes very long and finally ends in an error message "the service did not respond" - why?
See the question above, it is the same reason.











