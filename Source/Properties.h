/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_PROPERTIES_H_INCLUDED_
#define _JANEL_PROPERTIES_H_INCLUDED_

#include "JVMInfo.h"
#include <string>
#include <vector>
#include <Windows.h>
#include "PropertyFileEntry.h"

class Properties  
{
private:
	tstring m_fullPathAndNameOfExe; // full path and name of this executable
	tstring m_selfHomePath; // root of this executable, with "bin\executable.exe" stripped
	tstring m_customJavaHomePath; // set from janel.java.home.path
	tstring m_mainClass; // fully qualified name of main class
	tstring m_customJvmPath;	// set from janel.jvm.path
	tstring m_customBinJvmDir;	// set from janel.bin.jvm.dir = client, server, prefer-client, prefer-server
	tstring m_customJavaBundle; // set from janel.java.bundle = JRE, SDK, PREFER-SDK, PREFER-JRE
	tstring m_customErrorDefaultText; // set from janel.error.default.text
	bool m_showErrorDetail;		// set from janel.error.show.detail  (Defaults to true)
	std::vector<tstring> m_javaSystemProperties; 
	std::vector<tstring> m_commandLineArguments;
	tstring m_minJavaVersion; //set the minimum java version allowed
	tstring m_maxJavaVersion; //set the maximum java version allowed
	tstring m_trapConsoleCtrl; // set the console ctrl trap option
	tstring m_jarsInClasspath; // added to with janel.classpath.jars.dir and/or janel.classpath.jars.dir.recursive
	JVMInfo* m_pBestJvmInfo;	// the best JVM found
	tstring m_debugFile;	// debug file from janel.debug.file
	tstring m_libraryPath; // added to with janel.library.path.dir and/or janel.library.jars.dir.recursive
	tstring m_workingDir; // Working directoty to be set
	tstring m_callerDir; // the directory from which the executable is called
	tstring m_serviceName; // name of the Windows Service (used for execution and installation)
	tstring m_serviceDisplayName; // display name of the Windows Service (used for installation)
	tstring m_serviceDescription; // description of the Windows Service (used for installation)
	tstring m_serviceOptionInstall; // Option-String (i. e. "--install") for performing Windows Service Installation
	tstring m_serviceOptionUninstall; // Option-String (i. e. "--remove") for performing Windows Service Uninstallation
	int m_numberOfInitialCommandLineArgs; // number of commandline arguments NOT passed with janel.main.argument

	bool m_memoryCheckLimits; // Add check for largest available memory region
	SIZE_T m_maxMemory; // Kilobytes used as the maximum heap size (-Xmx)
	SIZE_T m_maxMemoryUpperLimit; // Upper limit in kb for the maximum heap size
	SIZE_T m_maxMemoryLowerLimit; // Lower limit in kb for the maximum heap size

	SIZE_T m_initMemory; // Kilobytes used as the initial heap size (-Xms)
	SIZE_T m_initMemoryUpperLimit; // Upper limit in kb for the initial heap size
	SIZE_T m_initMemoryLowerLimit; // Lower limit in kb for the initial heap size

	tstring m_splash; // Relative path to splash screen image

	void loadPropsFile();
	double getAvailablePhysicalMemoryKilobytes();
	double getTotalPhysicalMemoryKilobytes();
public:
	static const tstring JRE_JAVA_BUNDLE;
	static const tstring SDK_JAVA_BUNDLE;
	static const tstring PREFER_JRE_JAVA_BUNDLE;
	static const tstring PREFER_SDK_JAVA_BUNDLE;
	static const tstring REQUIRE_JRE_JAVA_BUNDLE;
	static const tstring REQUIRE_SDK_JAVA_BUNDLE;

	static const tstring CLIENT_BIN_JVM_DIR;
	static const tstring SERVER_BIN_JVM_DIR;
	static const tstring PREFER_CLIENT_BIN_JVM_DIR;
	static const tstring PREFER_SERVER_BIN_JVM_DIR;
	static const tstring REQUIRE_CLIENT_BIN_JVM_DIR;
	static const tstring REQUIRE_SERVER_BIN_JVM_DIR;

	Properties();
	virtual ~Properties();

	void loadProperties();
	void evaluateMemorySettings();

	void setFullPathAndNameOfExe(const tstring& fullPathAndNameOfExe);
	tstring& getFullPathAndNameOfExe();

	void setSelfHomePath(const tstring& fullPathOfExe);
	tstring& getSelfHomePath();

	void setMainClass(const tstring& mainClass);
	tstring& getMainClass();

	void setCustomBinJvmDir(const tstring& binJvmDir);
	tstring& getCustomBinJvmDir();

	void setCustomJavaBundle(const tstring& javaBundle);
	tstring& getCustomJavaBundle();

	void setCustomJvmPath(const tstring& customJvmPath);
	tstring& getCustomJvmPath();

	void setCustomJavaHomePath(const tstring& customJavaHomePath);
	tstring& getCustomJavaHomePath();

	void setCustomErrorDefaultText(const tstring& customErrorText);
	tstring& getCustomErrorDefaultText();

	void setShowErrorDetail(const bool showDetails);
	bool getShowErrorDetail();

	bool isJavaSystemProperty(const tstring& property);
	void addJavaSystemProperty(const tstring& property);
	std::vector<tstring>& getJavaSystemProperties();

	void addCommandLineArgument(const tstring& arg, unsigned int index);
	void addCommandLineArgument(PropertyFileEntry& propFileEntry);
	std::vector<tstring>& getCommandLineArguments();

	void setMinJavaVersion(const tstring& minJavaVersion);
	tstring& getMinJavaVersion();

	void setMaxJavaVersion(const tstring& maxJavaVersion);
	tstring& getMaxJavaVersion();

	bool isVersionLessThanEqualMax(const tstring& regularVersion);
	bool isVersionGreaterThanEqualMin(const tstring& regularVersion);

	void setTrapConsoleCtrl(const tstring& trapConsoleCtrl);
	bool getTrapConsoleCtrl();

	void addJarsToClasspath(tstring& jarsDir, bool recursive);
	tstring& getJarsInClasspath();

	bool isClasspathSystemProperty(const tstring& property);

	void setBestJvmInfo(JVMInfo* pJvmInfo);
	JVMInfo* getBestJvmInfo();

	bool isCustomJavaBundleRequired();
	bool isCustomJavaBundlePreferred();
	bool isCustomBinJvmDirRequired();

	void setDebugFile(const tstring& debugFile);
	tstring& getDebugFile();

	void addToLibraryPath(tstring& libraryPathDir, bool recursive);
	tstring& getLibraryPath();
	bool isLibrarypathSystemProperty(const tstring& property);

	void setSystemPropertyProcessId(const tstring& property);
	
	void setSplash(const tstring& property);
	tstring& getSplash();

	void setWorkingDir(const tstring& property);
	tstring& getWorkingDir();

	void setMemoryCheckLimits(const tstring& property);
	bool isMemoryCheckLimits();
	void setMaxMemoryPercentOfAvailable(const tstring& property);
	void setInitMemoryPercentOfAvailable(const tstring& property);
	void setMaxMemoryPercentOfTotal(const tstring& property);
	void setInitMemoryPercentOfTotal(const tstring& property);
	void setMaxMemoryUpperLimit(const tstring& property);
	void setInitMemoryUpperLimit(const tstring& property);
	void setMaxMemoryLowerLimit(const tstring& property);
	void setInitMemoryLowerLimit(const tstring& property);

	void setCallerDir(const tstring& callerDir);
	tstring& getCallerDir();

	void setServiceName(const tstring& serviceName);
	tstring& getServiceName();

	void setServiceDisplayName(const tstring& serviceDisplayName);
	tstring& getServiceDisplayName();

	void setServiceDescription(const tstring& serviceDescription);
	tstring& getServiceDescription();

	void setServiceOptionInstall(const tstring& serviceOptionInstall);
	tstring& getServiceOptionInstall();

	void setServiceOptionUninstall(const tstring& serviceOptionUninstall);
	tstring& getServiceOptionUninstall();

	void setNumberOfInitialCommandLineArgs(int numberOfInitialCommandLineArgs);
	int getNumberOfInitialCommandLineArgs();

	tstring& toString();
};

#endif // ifndef _JANEL_PROPERTIES_H_INCLUDED_
