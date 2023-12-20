/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "WindowsRegistry.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "Windows.h"

using namespace std;


WindowsRegistry::WindowsRegistry(Properties* pProperties)
{
	m_pProperties = pProperties;
}

WindowsRegistry::~WindowsRegistry()
{
}

void WindowsRegistry::addAllSdkJvms(vector<JVMInfo>* pVecJvmInfo, const tstring& regKey)
{
	addAllJvms(pVecJvmInfo, regKey, Properties::SDK_JAVA_BUNDLE);
}

void WindowsRegistry::addAllJreJvms(vector<JVMInfo>* pVecJvmInfo, const tstring& regKey)
{
	addAllJvms(pVecJvmInfo, regKey, Properties::JRE_JAVA_BUNDLE);
}

void WindowsRegistry::addAllJvms(vector<JVMInfo>* pVecJvmInfo, const tstring& regKey, const tstring& bundle)
{
	try
	{
		LONG result;
		HKEY hKey;

		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,regKey.c_str(),0,KEY_READ,&hKey);
		if (result != ERROR_SUCCESS)
		{
			// key may not exist so exit 
			return;
		}

		TCHAR lpName[MAX_PATH];

		for (DWORD index = 0, result = ERROR_SUCCESS; result == ERROR_SUCCESS; index++) 
		{ 
			DWORD lpcName = MAX_PATH;
			result = RegEnumKeyEx(hKey, 
					index, 
					lpName, 
					&lpcName, 
					NULL, 
					NULL, 
					NULL, 
					NULL); 

			if (result == ERROR_SUCCESS)
			{
				tstring dirName(lpName);
				if (dirName.empty())
				{
					continue;
				}

				tstring javaVersion = dirName;
				tstring oracleKeyPath = regKey + tstring(_T("\\")) + dirName;

				DEBUG_SHOW(_T("oracleKeyPath=") + oracleKeyPath);
				tstring& javaHome = getStringValue(oracleKeyPath, _T("JavaHome"));
				if (javaHome.empty())
				{
					// Seems to be no Oracle JDK/JRE... Let's check if it is an OpenJDK HotSpot
					tstring hotspotKeyPath = regKey + tstring(_T("\\")) + dirName + tstring(_T("\\hotspot\\MSI"));
					DEBUG_SHOW(_T("hotspotKeyPath=") + hotspotKeyPath);
					javaHome = getStringValue(hotspotKeyPath, _T("Path"));
					if (javaHome.empty())
					{
						// Next try.... Let's check if it is an OpenJDK OpenJ9
						tstring openJ9KeyPath = regKey + tstring(_T("\\")) + dirName + tstring(_T("\\openj9\\MSI"));
						DEBUG_SHOW(_T("openJ9KeyPath=") + openJ9KeyPath);
						javaHome = getStringValue(openJ9KeyPath, _T("Path"));
						if (javaHome.empty())
						{
							// Last try.... Let's check if it is an Azul Zulu JDK
							tstring zuluKeyPath = regKey + tstring(_T("\\")) + dirName;
							DEBUG_SHOW(_T("zuluKeyPath=") + zuluKeyPath);
							javaHome = getStringValue(zuluKeyPath, _T("InstallationPath"));
							javaVersion = getStringValue(zuluKeyPath, _T("CurrentVersion"));
							if (javaHome.empty() || javaVersion.empty())
							{
								continue;
							}
						}
					}
				}

				if (javaHome[javaHome.size() - 1] == _T('\\'))
				{
					javaHome.erase(javaHome.size() - 1);
				}

				JVMInfo *pJvmInfo = new JVMInfo;
				pJvmInfo->setJavaBundle(bundle);
				pJvmInfo->setJavaHomePath(javaHome);
				pJvmInfo->setVersion(javaVersion);

				DEBUG_SHOW( tstring(_T("Found JDK version ")) + javaVersion +  tstring(_T(" in directory  ")) + javaHome);

				pVecJvmInfo->push_back(*pJvmInfo);
			}
		} 

		RegCloseKey(hKey);
	}
	catch(tstring& se)
	{
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in WindowsRegistry.addAllSdkJvms()") );
		ErrHandler::severeError( _T("Error getting JVM paths from registry.") );
	}
}

tstring& WindowsRegistry::getStringValue(const tstring& regKeyPath, const tstring& regValueName)
{
	tstring* pStringValue = new tstring;
	try
	{
		LONG result;
		HKEY hKey;
		TCHAR regValueData[MAX_PATH];

		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,regKeyPath.c_str(),0,KEY_READ,&hKey);
		if (result != ERROR_SUCCESS)
		{
			// errors that should not cause a complete failure of launch
			_stprintf_s(regValueData,_T(""));
			DEBUG_SHOW( tstring(_T("Registry key ")) + regKeyPath + tstring(_T(" could not be opened.")));
		}
		else
		{
			DWORD regValueDataLength = MAX_PATH;
			DWORD type = REG_SZ;

			result = RegQueryValueEx(hKey,regValueName.c_str(),NULL,&type, (LPBYTE)regValueData,&regValueDataLength);
			if (result != ERROR_SUCCESS)
			{
				// errors that should not cause a complete failure of launch
				_stprintf_s(regValueData,_T(""));
				DEBUG_SHOW( tstring(_T("Registry key ")) + regKeyPath + tstring(_T(", value name ")) + regValueName + tstring(_T(" could not be opened.")));
			}
		}

		pStringValue = new tstring(regValueData);

		RegCloseKey(hKey);
	}
	catch(tstring& se)
	{
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in WindowsRegistry.addAllSdkJvms()") );
		ErrHandler::severeError( _T("Error getting JVM paths from registry.") );
	}

	return *pStringValue;
}
