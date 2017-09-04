/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_JVMINFO_H_INCLUDED_
#define _JANEL_JVMINFO_H_INCLUDED_

#include <string>

class JVMInfo
{
	tstring m_javaBundle;		// SDK or JRE
	tstring m_binJvmDir;		// client or server
	tstring m_version;			// 1.4.1_03
	tstring m_comparableVersion; // 001.004.001.003.0.0. see the adjustComparableVersion... functions for explanation of last two zeros
	tstring m_jvmPath;			// path to jvm.dll
	tstring m_javaHomePath;		// path to root of Java installation
	tstring m_jrePath;		// path to the directory that holds the client or server directory
	bool m_acceptable;			// whether this jvm should be used

public:
	void setJavaBundle(const tstring& javaBundle);
	tstring& getJavaBundle();
	void setBinJvmDir(const tstring& binJvmDir);
	tstring& getBinJvmDir();
	void setVersion(const tstring& version);
	tstring& getVersion();
	void setComparableVersion(const tstring& comparableVersion);
	tstring& getComparableVersionUsingRegularVersion(const tstring& regularVersion);
	tstring& getComparableVersion();
	void setJvmPath(const tstring& jvmPath);
	tstring& getJvmPath();
	void setJavaHomePath(const tstring& javaHomePath);
	tstring& getJavaHomePath();
	void adjustComparableVersionForJavaBundle(tstring& javaBundleDesired);
	void adjustComparableVersionForBinJvmDir(tstring& binJvmDirDesired);
	JVMInfo* partialClone();
	tstring& getJrePath();
	tstring& getJvmDLLPath(const tstring& binJvmDir);
	bool existsJvmDLL(const tstring& binJvmDir);
	void setAcceptable(bool acceptable);
	bool isAcceptable();
	tstring& toString();


	JVMInfo(void);
	~JVMInfo(void);
};

#endif // ifndef _JANEL_JVMINFO_H_INCLUDED_
