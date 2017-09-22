/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_WINDOWSREGISTRY_H_INCLUDED_
#define _JANEL_WINDOWSREGISTRY_H_INCLUDED_

#include "Properties.h"
#include "JVMInfo.h"
#include <string>

class WindowsRegistry  
{
private:
	Properties* m_pProperties;

	void addAllJvms(std::vector<JVMInfo>* pVecJvmInfo, const tstring& regKey, const tstring& bundle);
	tstring& getStringValue(const tstring& regKeyPath, const tstring& regValueName);
public:
	WindowsRegistry(Properties* pProperties);
	virtual ~WindowsRegistry();
	
	void addAllSdkJvms(std::vector<JVMInfo>* pVecJvmInfo, const tstring& regKey);
	void addAllJreJvms(std::vector<JVMInfo>* pVecJvmInfo, const tstring& regKey);
};

#endif // ifndef _JANEL_WINDOWSREGISTRY_H_INCLUDED_
