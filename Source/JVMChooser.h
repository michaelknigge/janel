/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_JVMCHOOSER_H_INCLUDED_
#define _JANEL_JVMCHOOSER_H_INCLUDED_

#include "Properties.h"
#include "JVMInfo.h"

class JVMChooser
{
	Properties *m_pProperties;
	void breakoutBinJvmDirs(std::vector<JVMInfo>* pVecJvmInfo);
	void determineVersions(std::vector<JVMInfo>* pVecJvmInfo);
	void removeUnacceptableJVMs(std::vector<JVMInfo>* pVecJvmInfo);
	JVMInfo* getJvmFromCustomJvmPath();
	JVMInfo* getJvmFromCustomJavaHomePath();
	JVMInfo* getJvm(const tstring& javaHomePath, const tstring& binJvmDir);

public:
	JVMInfo* getBestJVM();
	void getAllJVMs(std::vector<JVMInfo>*);

	JVMChooser(Properties *pProperties);
	~JVMChooser(void);
};

#endif // ifndef _JANEL_JVMCHOOSER_H_INCLUDED_
