/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "Properties.h"
#include "PropertiesFile.h"
#include "WindowsRegistry.h"
#include "LocalUtilities.h"
#include "Debug.h"
#include "ErrHandler.h"
#include <io.h>
#include "JVMInfo.h"
#include <algorithm>
#include <Windows.h>
#include <iostream>

using namespace std;

const tstring Properties::JRE_JAVA_BUNDLE = _T("jre");
const tstring Properties::SDK_JAVA_BUNDLE = _T("sdk");
const tstring Properties::PREFER_JRE_JAVA_BUNDLE = _T("prefer_jre");
const tstring Properties::PREFER_SDK_JAVA_BUNDLE = _T("prefer_sdk");
const tstring Properties::REQUIRE_JRE_JAVA_BUNDLE = _T("require_jre");
const tstring Properties::REQUIRE_SDK_JAVA_BUNDLE = _T("require_sdk");

const tstring Properties::CLIENT_BIN_JVM_DIR = _T("client");
const tstring Properties::SERVER_BIN_JVM_DIR = _T("server");
const tstring Properties::PREFER_CLIENT_BIN_JVM_DIR = _T("prefer_client");
const tstring Properties::PREFER_SERVER_BIN_JVM_DIR = _T("prefer_server");
const tstring Properties::REQUIRE_CLIENT_BIN_JVM_DIR = _T("require_client");
const tstring Properties::REQUIRE_SERVER_BIN_JVM_DIR = _T("require_server");

#define BYTES_TO_KILOBYTES 1024


Properties::Properties()
{
	m_memoryCheckLimits = false;
	m_maxMemory = 0;
	m_maxMemoryUpperLimit = MAXINT;
	m_maxMemoryLowerLimit = 0;

	m_initMemory = 0;
	m_initMemoryUpperLimit = MAXINT;
	m_initMemoryLowerLimit = 0;
}

Properties::~Properties()
{

}

void Properties::loadProperties()
{
	try
	{
		PropertiesFile propsFile(this);
		propsFile.loadPropertiesFromFile();
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Properties.loadProperties()") );
		ErrHandler::severeError( _T("Error loading properties.") );
	}
}

SIZE_T GetLargestFreeMemRegion()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	VOID *p = 0;
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T largestSize = 0;
	while(p < systemInfo.lpMaximumApplicationAddress)
	{
		SIZE_T dwRet = VirtualQuery(p, &mbi, sizeof(mbi));
		if (dwRet > 0)
		{
			if (mbi.State == MEM_FREE)
			{
				if (largestSize < (mbi.RegionSize/1024))
				{
					largestSize = (mbi.RegionSize/1024);
				}
			}
			p = (void*) (((char*)p) + mbi.RegionSize);
		}
		else
		{
			p = (void*) (((char*)p) + systemInfo.dwPageSize);
		}
	}
	return largestSize;
}

void Properties::evaluateMemorySettings()
{
	SIZE_T estimatedLimit = MAXINT;
	if (m_memoryCheckLimits)
	{
		estimatedLimit = GetLargestFreeMemRegion() - 204800;
	}

	m_maxMemory = min(min(max(m_maxMemory, m_maxMemoryLowerLimit), m_maxMemoryUpperLimit), estimatedLimit);
	if (m_maxMemory > 0)
	{
		tstringstream maxMemoryProp;
		maxMemoryProp << _T("-Xmx") << m_maxMemory << _T("k");
		
		addJavaSystemProperty(maxMemoryProp.str());
	}

	m_initMemory = min(min(max(m_initMemory, m_initMemoryLowerLimit), m_initMemoryUpperLimit), m_maxMemory);
	if (m_initMemory > 0)
	{
		tstringstream initMemoryProp;
		initMemoryProp << _T("-Xms") << m_initMemory << _T("k");
		
		addJavaSystemProperty(initMemoryProp.str());
	}
}

void Properties::setFullPathAndNameOfExe(const tstring& fullPathAndNameOfExe)
{
	m_fullPathAndNameOfExe = fullPathAndNameOfExe;
	DEBUG_SHOW(_T("m_fullPathAndNameOfExe=") + m_fullPathAndNameOfExe);

	setSelfHomePath(fullPathAndNameOfExe);
}

/*
 * parameter fullPathOfExe is the full path, with any \bin directories
 */
void Properties::setSelfHomePath(const tstring& fullPathOfExe)
{
	tstring::size_type pathEnd = fullPathOfExe.find_last_of( _T('\\') );
	if ( pathEnd != tstring::npos )
	{
		m_selfHomePath = fullPathOfExe.substr( 0, pathEnd );
		tstring::size_type binStart = m_selfHomePath.find_last_of( _T('\\') );
		if ( binStart != string::npos && m_selfHomePath.substr(binStart).compare( _T("\\bin") ) == 0 )
		{
			m_selfHomePath = m_selfHomePath.substr( 0, binStart );
		}
	}
}

tstring& Properties::getFullPathAndNameOfExe()
{
	return m_fullPathAndNameOfExe;
}

tstring& Properties::getSelfHomePath()
{
	return m_selfHomePath;
}

void Properties::setMainClass(const tstring& mainClass)
{
	m_mainClass = mainClass;
}

tstring& Properties::getMainClass()
{
	m_mainClass = LocalUtilities::swapChar( m_mainClass, _T('.'), _T('/') ); 
	return m_mainClass;
}

void Properties::setCustomBinJvmDir(const tstring& binJvmDir)
{
	m_customBinJvmDir = binJvmDir;
	transform(m_customBinJvmDir.begin(), m_customBinJvmDir.end(), m_customBinJvmDir.begin(), (int(*)(int)) tolower);

	if( m_customBinJvmDir.compare(CLIENT_BIN_JVM_DIR) == 0 )
	{
		m_customBinJvmDir = REQUIRE_CLIENT_BIN_JVM_DIR;
	}
	else if( m_customBinJvmDir.compare(SERVER_BIN_JVM_DIR) == 0 )
	{
		m_customBinJvmDir = REQUIRE_SERVER_BIN_JVM_DIR;
	}
	DEBUG_SHOW( _T("setting m_customBinJvmDir=") + m_customBinJvmDir);
}

tstring& Properties::getCustomBinJvmDir()
{
	return m_customBinJvmDir;
}

void Properties::setCustomJavaBundle(const tstring& javaBundle)
{
	m_customJavaBundle = javaBundle;
	transform(m_customJavaBundle.begin(), m_customJavaBundle.end(), m_customJavaBundle.begin(), (int(*)(int)) tolower);

	if( m_customJavaBundle.compare(SDK_JAVA_BUNDLE) == 0 )
	{
		m_customJavaBundle = REQUIRE_SDK_JAVA_BUNDLE;
	}
	else if( m_customJavaBundle.compare(JRE_JAVA_BUNDLE) == 0 )
	{
		m_customJavaBundle = REQUIRE_JRE_JAVA_BUNDLE;
	}
	DEBUG_SHOW( _T("m_customJavaBundle=") + m_customJavaBundle);
}

tstring& Properties::getCustomJavaBundle()
{
	return m_customJavaBundle;
}

void Properties::setCustomJvmPath(const tstring& customJvmPath)
{
	if( !LocalUtilities::fileExists(customJvmPath) )
	{
		return;
	}

	m_customJvmPath = customJvmPath;
	transform(m_customJvmPath.begin(), m_customJvmPath.end(), m_customJvmPath.begin(), (int(*)(int)) tolower);
	DEBUG_SHOW( _T("m_customJvmPath=") + m_customJvmPath);
}

tstring& Properties::getCustomJvmPath()
{
	return m_customJvmPath;
}

void Properties::setCustomJavaHomePath(const tstring& customJavaHomePath)
{
	if( !LocalUtilities::fileExists(customJavaHomePath) )
	{
		return;
	}

	m_customJavaHomePath = customJavaHomePath;
}

tstring& Properties::getCustomJavaHomePath()
{
	return m_customJavaHomePath;
}

bool Properties::isJavaSystemProperty(const tstring& property)
{
	if( property[0] == _T('-') )
		return true;
	else
		return false;
}			

tstring& Properties::getCustomErrorDefaultText()
{
	return m_customErrorDefaultText;
}

void Properties::setCustomErrorDefaultText(const tstring& customErrorText)
{
	m_customErrorDefaultText = customErrorText;
	ErrHandler::setGenericErrorMessage(customErrorText);
}

bool Properties::getShowErrorDetail()
{
	return m_showErrorDetail;
}

void Properties::setShowErrorDetail(const bool showErrorDetails)
{
	m_showErrorDetail = showErrorDetails;
	ErrHandler::setShowErrorDetail(showErrorDetails);
}

void Properties::addJavaSystemProperty(const tstring& property)
{
	m_javaSystemProperties.push_back( property );
}

vector<tstring>& Properties::getJavaSystemProperties()
{
	return m_javaSystemProperties;
}

void Properties::addCommandLineArgument(const tstring& arg, unsigned int index)
{
	if(m_commandLineArguments.size() < index+1)
	{
		m_commandLineArguments.resize(index+1);
	}

	m_commandLineArguments[index] = arg;
}

void Properties::addCommandLineArgument(PropertyFileEntry& propFileEntry)
{
	int indexOfArg = propFileEntry.getRowNumber() + getNumberOfInitialCommandLineArgs();
	addCommandLineArgument(propFileEntry.getValue(), indexOfArg); 
}

vector<tstring>& Properties::getCommandLineArguments()
{
	vector<tstring>* pCommandLineArgs = new vector<tstring>;
	vector<tstring>::iterator iter;
	for(iter = m_commandLineArguments.begin(); iter != m_commandLineArguments.end(); iter++)
	{
		if(!iter->empty())
		{
			pCommandLineArgs->push_back(*iter);
		}
	}

	return *pCommandLineArgs;
}

void Properties::setMinJavaVersion(const tstring& minJavaVersion)
{
	m_minJavaVersion = minJavaVersion;
}

tstring& Properties::getMinJavaVersion()
{
	return m_minJavaVersion;
}

void Properties::setMaxJavaVersion(const tstring& maxJavaVersion)
{
	m_maxJavaVersion = maxJavaVersion;
}

tstring& Properties::getMaxJavaVersion()
{
	return m_maxJavaVersion;
}

bool Properties::isVersionLessThanEqualMax(const tstring& regularVersion)
{
	try
	{
		tstring& maxJavaVersion = getMaxJavaVersion();
		if( maxJavaVersion.empty() )
		{
			return true;
		}
		
		JVMInfo jvmInfo;
		tstring comparableMaxVersion = jvmInfo.getComparableVersionUsingRegularVersion(maxJavaVersion);
		tstring comparableVersion = jvmInfo.getComparableVersionUsingRegularVersion(regularVersion);
		if ( comparableVersion.compare(comparableMaxVersion) <= 0 )
		{
			return true;
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Properties.isVersionLessThanEqualMax()") );
		ErrHandler::severeError( _T("Error checking if version is less than the max version.") );
	}
	return false;
}

bool Properties::isVersionGreaterThanEqualMin(const tstring& regularVersion)
{
	try
	{
		tstring& minJavaVersion = getMinJavaVersion();
		if( minJavaVersion.empty() )
		{
			return true;
		}
		
		JVMInfo jvmInfo;
		tstring comparableMinVersion = jvmInfo.getComparableVersionUsingRegularVersion(minJavaVersion);
		tstring comparableVersion = jvmInfo.getComparableVersionUsingRegularVersion(regularVersion);
		if ( comparableVersion.compare(comparableMinVersion) >= 0 )
		{
			return true;
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Properties.isVersionGreaterThanEqualMin()") );
		ErrHandler::severeError( _T("Error checking if version is greater than the min version.") );
	}
	return false;
}

void Properties::setTrapConsoleCtrl(const tstring& trapConsoleCtrl)
{
	m_trapConsoleCtrl = trapConsoleCtrl;
}

bool Properties::getTrapConsoleCtrl()
{
	if( LocalUtilities::convertToBoolean(m_trapConsoleCtrl) )
	{
		return true;
	}

	return false;
}

void Properties::setSystemPropertyProcessId(const tstring& property)
{
	if( LocalUtilities::convertToBoolean(property) )
	{
		DWORD processId = GetCurrentProcessId();
		
		TCHAR systemProp[100];
		_stprintf_s( systemProp, _T("-Dprocess.id=%u"), processId);
		addJavaSystemProperty(systemProp);
	}
}

void Properties::setSplash(const tstring& property)
{
	DEBUG_SHOW(_T("setSplash"));
	m_splash = property;
}

tstring& Properties::getSplash()
{
	DEBUG_SHOW(_T("getSplash"));
	return m_splash;
}

void Properties::setWorkingDir(const tstring& property)
{
	DEBUG_SHOW(_T("setWorkingDir"));
	m_workingDir = property;
}

tstring& Properties::getWorkingDir()
{
	DEBUG_SHOW(_T("getWorkingDir"));
	return m_workingDir;
}

void Properties::setServiceName(const tstring& property)
{
	DEBUG_SHOW(_T("setServiceName"));
	m_serviceName = property;
}

tstring& Properties::getServiceName()
{
	DEBUG_SHOW(_T("getServiceName"));
	return m_serviceName;
}

void Properties::setServiceDisplayName(const tstring& property)
{
	DEBUG_SHOW(_T("setServiceDisplayName"));
	m_serviceDisplayName = property;
}

tstring& Properties::getServiceDisplayName()
{
	DEBUG_SHOW(_T("getServiceDisplayName"));
	return m_serviceDisplayName;
}

void Properties::setServiceDescription(const tstring& property)
{
	DEBUG_SHOW(_T("setServiceDescription"));
	m_serviceDescription = property;
}

tstring& Properties::getServiceDescription()
{
	DEBUG_SHOW(_T("getServiceDescription"));
	return m_serviceDescription;
}

void Properties::setServiceOptionInstall(const tstring& property)
{
	DEBUG_SHOW(_T("setServiceOptionInstall"));
	m_serviceOptionInstall = property;
}

tstring& Properties::getServiceOptionInstall()
{
	DEBUG_SHOW(_T("getServiceOptionInstall"));
	return m_serviceOptionInstall;
}

void Properties::setServiceOptionUninstall(const tstring& property)
{
	DEBUG_SHOW(_T("setServiceOptionUninstall"));
	m_serviceOptionUninstall = property;
}

tstring& Properties::getServiceOptionUninstall()
{
	DEBUG_SHOW(_T("getServiceOptionUninstall"));
	return m_serviceOptionUninstall;
}

double Properties::getAvailablePhysicalMemoryKilobytes()
{
    double physicalMemoryKb = 0.0;

    // Get physical memory amount.
#if _WIN32_WINNT>=0x0500
    DEBUG_SHOW(_T("using GlobalMemoryStatusEx"));
    // Windows 2000 and XP only.
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    physicalMemoryKb = statex.ullAvailPhys >> 10;
#else
    DEBUG_SHOW(_T("using GlobalMemoryStatus"));
    // Everything else, but only recognizes up to 4GB of memory!
    // See MS docs on GlobalMemoryStatus for more limitations.
    MEMORYSTATUS stat;
    GlobalMemoryStatus(&stat);
    physicalMemoryKb = stat.dwAvailPhys >> 10;
#endif

    return(physicalMemoryKb);
}

void Properties::setMemoryCheckLimits(const tstring& property)
{
	m_memoryCheckLimits = LocalUtilities::convertToBoolean(property);
}

bool Properties::isMemoryCheckLimits()
{
	return m_memoryCheckLimits;
}

void Properties::setMaxMemoryPercentOfAvailable(const tstring& property)
{
    double percent = LocalUtilities::convertStringToPercent( property );
    DEBUG_SHOW(_T("setMaxMemoryPercentOfAvailable"));
    
    if( percent < 0.0 ) {
		percent = 0.0;
    }
    
    if( percent > 100.0 ) {
		percent = 100.0;
    }
    
    double availablePhysicalMemory = getAvailablePhysicalMemoryKilobytes();

    int memoryToUse = (int)(availablePhysicalMemory * percent / 100.0);
	m_maxMemory = memoryToUse;
}

void Properties::setInitMemoryPercentOfAvailable(const tstring& property)
{
    double percent = LocalUtilities::convertStringToPercent( property );
    DEBUG_SHOW(_T("setInitMemoryPercentOfAvailable"));
    
    if( percent < 0.0 ) {
		percent = 0.0;
    }
    
    if( percent > 100.0 ) {
		percent = 100.0;
    }
    
    double availablePhysicalMemory = getAvailablePhysicalMemoryKilobytes();

    int memoryToUse = (int)(availablePhysicalMemory * percent / 100.0);
	m_initMemory = memoryToUse;
}

void Properties::setMaxMemoryUpperLimit(const tstring& property)
{
	int kb = _ttoi(property.c_str());
    DEBUG_SHOW(_T("setMaxMemoryUpperLimit"));

	m_maxMemoryUpperLimit = kb;
}

void Properties::setMaxMemoryLowerLimit(const tstring& property)
{
    int kb = _ttoi(property.c_str());
    DEBUG_SHOW(_T("setMaxMemoryLowerLimit"));

	m_maxMemoryLowerLimit = kb;
}


void Properties::setInitMemoryUpperLimit(const tstring& property)
{
    int kb = _ttoi(property.c_str());
    DEBUG_SHOW(_T("setInitMemoryUpperLimit"));

	m_initMemoryUpperLimit = kb;
}

void Properties::setInitMemoryLowerLimit(const tstring& property)
{
    int kb = _ttoi(property.c_str());
    DEBUG_SHOW(_T("setInitMemoryLowerLimit"));

	m_initMemoryLowerLimit = kb;
}

double Properties::getTotalPhysicalMemoryKilobytes()
{
    double physicalMemoryKb = 0.0;

    // Get physical memory amount.
#if _WIN32_WINNT>=0x0500
    // Windows 2000 and XP only.
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    physicalMemoryKb = (double) (statex.ullTotalPhys >> 10);
#else
    // Everything else, but only recognizes up to 4GB of memory!
    // See MS docs on GlobalMemoryStatus for more limitations.
    MEMORYSTATUS stat;
    GlobalMemoryStatus(&stat);
    physicalMemoryKb = stat.dwTotalPhys >> 10;
#endif

    return(physicalMemoryKb);
}

void Properties::setMaxMemoryPercentOfTotal(const tstring& property)
{
    double percent = LocalUtilities::convertStringToPercent( property );
    DEBUG_SHOW(_T("setMaxMemoryPercentOfTotal"));
    
    if( percent < 0.0 ) {
		percent = 0.0;
    }
    
    if( percent > 100.0 ) {
		percent = 100.0;
    }
    
    double availablePhysicalMemory = getTotalPhysicalMemoryKilobytes();

    int memoryToUse = (int)(availablePhysicalMemory * percent / 100.0);
	m_maxMemory = memoryToUse;
}

void Properties::setInitMemoryPercentOfTotal(const tstring& property)
{
    double percent = LocalUtilities::convertStringToPercent( property );
    DEBUG_SHOW(_T("setInitMemoryPercentOfTotal"));
    
    if( percent < 0.0 ) {
        percent = 0.0;
    }
    
    if( percent > 100.0 ) {
        percent = 100.0;
    }
    
    double availablePhysicalMemory = getTotalPhysicalMemoryKilobytes();

    int memoryToUse = (int)(availablePhysicalMemory * percent / 100.0);
	m_initMemory = memoryToUse;
}

void Properties::addJarsToClasspath(tstring& jarsDir, bool recursive)
{
	try
	{
		_tfinddata_t *pfileDataFound = new _tfinddata_t;
		intptr_t hFileFound;
		tstring fileSpec;
		bool isFirst = true;


		if(jarsDir[jarsDir.size()-1] != _T('\\'))
		{
			jarsDir = jarsDir + tstring(_T("\\"));
		}
		fileSpec = jarsDir + tstring(_T("*"));

		for(;;)
		{
			if( isFirst )
			{
				hFileFound = _tfindfirst(fileSpec.c_str(), pfileDataFound);
				isFirst = false;
				if(hFileFound == -1L)
				{
					break;
				}
			}
			else
			{
				if(_tfindnext( hFileFound, pfileDataFound) != 0)
				{
					break;
				}
			}

			tstring name = tstring(pfileDataFound->name);

			if( (pfileDataFound->attrib & _A_SUBDIR) != _A_SUBDIR )
			{
				if (name.size() > 4)
				{
					tstring suffix = name.substr(name.size() - 4, 4);
					transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
					if(suffix.compare(_T(".jar")) == 0)
					{
						m_jarsInClasspath = m_jarsInClasspath + tstring(_T(";")) + jarsDir + name;
					}
				}
			}
			else
			{
				if (recursive && name.compare(_T(".")) != 0 && name.compare(_T("..")) != 0)
				{
					addJarsToClasspath(jarsDir + name, true);
				}
			}
		}
		
		if( hFileFound != NULL )
		{
			_findclose(hFileFound);
		}

		delete pfileDataFound;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Properties::addJarsToClasspath()") );
		ErrHandler::severeError( _T("Error adding jars to classpath.") );
	}
}

tstring&  Properties::getJarsInClasspath()
{
	return m_jarsInClasspath;
}

bool Properties::isClasspathSystemProperty(const tstring& property)
{
	tstring cpSysProp(_T("-Djava.class.path"));
	if( property.compare(0,cpSysProp.length(),cpSysProp) == 0 )
		return true;
	else
		return false;
}			

bool Properties::isLibrarypathSystemProperty(const tstring& property)
{
	tstring libPathSysProp(_T("-Djava.library.path"));
	if( property.compare(0,libPathSysProp.length(),libPathSysProp) == 0 )
		return true;
	else
		return false;
}			

bool Properties::isCustomJavaBundlePreferred()
{
	DEBUG_SHOW( _T("Top of isCustomJavaBundlePreferred") );
	
	if(	m_customJavaBundle.compare(PREFER_JRE_JAVA_BUNDLE) == 0 ||
		m_customJavaBundle.compare(PREFER_SDK_JAVA_BUNDLE) == 0 
		)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool Properties::isCustomJavaBundleRequired()
{	
	if(	m_customJavaBundle.compare(REQUIRE_JRE_JAVA_BUNDLE) == 0 ||
		m_customJavaBundle.compare(REQUIRE_SDK_JAVA_BUNDLE) == 0 
		)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool Properties::isCustomBinJvmDirRequired()
{
	if(	m_customBinJvmDir.compare(REQUIRE_CLIENT_BIN_JVM_DIR) == 0 ||
		m_customBinJvmDir.compare(REQUIRE_SERVER_BIN_JVM_DIR) == 0 
		)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void Properties::setBestJvmInfo(JVMInfo* pJvmInfo)
{
	m_pBestJvmInfo = pJvmInfo;
}

JVMInfo* Properties::getBestJvmInfo()
{
	return m_pBestJvmInfo;
}

void Properties::setDebugFile(const tstring& debugFile)
{
	m_debugFile = debugFile;
}

tstring& Properties::getDebugFile()
{
	return m_debugFile;
}

void Properties::addToLibraryPath(tstring& libraryPathDir, bool recursive)
{
	_tfinddata_t *pfileDataFound = new _tfinddata_t;
	intptr_t hFileFound;
	tstring fileSpec;
	bool isFirst = true;

	try
	{
		m_libraryPath.append(_T(";")).append(libraryPathDir);
		if( recursive )
		{
			if(libraryPathDir[libraryPathDir.size()-1] != _T('\\'))
			{
				libraryPathDir = libraryPathDir + tstring(_T("\\"));
			}
			fileSpec = libraryPathDir + tstring(_T("*"));

			for(;;)
			{
				if( isFirst )
				{
					hFileFound = _tfindfirst(fileSpec.c_str(), pfileDataFound);
					isFirst = false;
					if(hFileFound == -1L)
					{
						break;
					}
				}
				else
				{
					if(_tfindnext( hFileFound, pfileDataFound) != 0)
					{
						break;
					}
				}

				if( (pfileDataFound->attrib & _A_SUBDIR) == _A_SUBDIR )
				{
					tstring name = tstring(pfileDataFound->name);
					if (recursive && name.compare(_T(".")) != 0 && name.compare(_T("..")) != 0)
					{
						addToLibraryPath(libraryPathDir + name, true);
					}
				}
			}

			if( hFileFound != NULL )
			{
				_findclose(hFileFound);
			}
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Properties::addToLibraryPath()") );
		ErrHandler::severeError( _T("Error adding directories to java.library.path.") );
	}
}

tstring& Properties::getLibraryPath()
{
	return m_libraryPath;
}

void Properties::setCallerDir(const tstring& callerDir)
{
	m_callerDir = callerDir;
	DEBUG_SHOW( _T("m_callerDir=") + m_callerDir);
}

tstring& Properties::getCallerDir()
{
	return m_callerDir;
}

void Properties::setNumberOfInitialCommandLineArgs(int numberOfInitialCommandLineArgs)
{
	m_numberOfInitialCommandLineArgs = numberOfInitialCommandLineArgs;
}

int Properties::getNumberOfInitialCommandLineArgs()
{
	return m_numberOfInitialCommandLineArgs;
}

tstring& Properties::toString()
{
	tstring* pPropsString = new tstring;
	pPropsString->append( _T("m_fullPathAndNameOfExe=") );
	pPropsString->append( m_fullPathAndNameOfExe );
	pPropsString->append( _T(",m_selfHomePath=") );
	pPropsString->append( m_selfHomePath );
	pPropsString->append( _T(",m_customJavaHomePath=") );
	pPropsString->append( m_customJavaHomePath );
	pPropsString->append( _T(",m_customBinJvmDir=") );
	pPropsString->append( m_customBinJvmDir );
	pPropsString->append( _T(",m_mainClass=") );
	pPropsString->append( m_mainClass );
	pPropsString->append( _T(",m_customJvmPath=") );
	pPropsString->append( m_customJvmPath );
	pPropsString->append( _T(",m_minJavaVersion=") );
	pPropsString->append( m_minJavaVersion );
	pPropsString->append( _T(",m_maxJavaVersion=") );
	pPropsString->append( m_maxJavaVersion );
	pPropsString->append( _T(",m_trapConsoleCtrl=") );
	pPropsString->append( m_trapConsoleCtrl );
	pPropsString->append( _T(",m_jarsInClasspath=") );
	pPropsString->append( m_jarsInClasspath );
	pPropsString->append( _T(",m_libraryPath=") );
	pPropsString->append( m_libraryPath );
	pPropsString->append( _T(",m_debugFile=") );
	pPropsString->append( m_debugFile );
	pPropsString->append( _T(",m_callerDir=") );
	pPropsString->append( m_callerDir );

	for(size_t i=0; i < m_javaSystemProperties.size(); i++)
	{
		size_t length = m_javaSystemProperties[i].length();

		tstringstream outputStr;
		outputStr << _T(",m_javaSystemProperties[") << i << _T("]=") << m_javaSystemProperties[i];

		DEBUG_SHOW(outputStr.str());
		pPropsString->append( outputStr.str() );
	}

	for(size_t i=0; i < m_commandLineArguments.size(); i++)
	{
		size_t length = m_commandLineArguments[i].length();
		tstringstream outputStr;

		outputStr << _T(",m_commandLineArguments[") << i << _T("]=") << m_commandLineArguments[i];

		pPropsString->append( outputStr.str() );
	}

	return *pPropsString;
}


