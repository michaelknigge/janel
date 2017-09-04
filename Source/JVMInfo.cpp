/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "JVMInfo.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "Properties.h"
#include "LocalUtilities.h"

using namespace std;

JVMInfo::JVMInfo(void)
{
	m_acceptable = true;
}

JVMInfo::~JVMInfo(void)
{
}

void JVMInfo::setJavaBundle(const tstring& javaBundle)
{
	m_javaBundle = javaBundle;
}

tstring& JVMInfo::getJavaBundle()
{
	return m_javaBundle;
}

void JVMInfo::setBinJvmDir(const tstring& binJvmDir)
{
	m_binJvmDir = binJvmDir;
}

tstring& JVMInfo::getBinJvmDir()
{
	return m_binJvmDir;
}

void JVMInfo::setVersion(const tstring& version)
{
	m_version = version;
}

tstring& JVMInfo::getVersion()
{
	return m_version;
}

tstring& JVMInfo::getComparableVersionUsingRegularVersion(const tstring& regularVersion)
{
	tstring* pComparableVersion = new tstring();
	try
	{
		TCHAR copyVersion[100];
		const TCHAR sep[] = _T("._");
		TCHAR* context = NULL;
		TCHAR* token;
		_tcscpy_s(copyVersion, regularVersion.c_str());
		token = _tcstok_s(copyVersion, sep, &context);
		TCHAR prepdToken[50];
		
		// version number Type goes 1 through 4: 1=major, 2=minor, 3=micro, 4=build
		for(int versionNumberType=1; versionNumberType <= 4; versionNumberType++)
		{
			_stprintf_s(prepdToken,_T("%03s."),token);
			pComparableVersion->append(prepdToken);
			token = _tcstok_s(NULL, sep, &context);
			if( token == NULL && versionNumberType < 4 )
			{
				token = _T("0");
			}
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.getComparableVersionUsingRegularVersion()") );
		ErrHandler::severeError( _T("Error while getting comparable JVM version using regular version.") );
	}
	return *pComparableVersion;
}

void JVMInfo::setComparableVersion(const tstring& comparableVersion)
{
	m_comparableVersion = comparableVersion;
}

tstring& JVMInfo::getComparableVersion()
{
	return m_comparableVersion;
}

void JVMInfo::setJvmPath(const tstring& jvmPath)
{
	m_jvmPath = jvmPath;
}

tstring& JVMInfo::getJvmPath()
{
	return m_jvmPath;
}

void JVMInfo::setJavaHomePath(const tstring& javaHomePath)
{
	m_javaHomePath = javaHomePath;
}

tstring& JVMInfo::getJavaHomePath()
{
	return m_javaHomePath;
}

void JVMInfo::adjustComparableVersionForJavaBundle(tstring& javaBundleDesired)
{
	try
	{
		unsigned int rankingCode = 0; // 0 to 3 the greater the number the higher in the ranking

		if( getJavaBundle().compare(Properties::SDK_JAVA_BUNDLE) == 0 )
		{
			if( javaBundleDesired.compare(Properties::REQUIRE_SDK_JAVA_BUNDLE) == 0 )
			{
				rankingCode = 3;
			}
			else if( javaBundleDesired.compare(Properties::PREFER_SDK_JAVA_BUNDLE) == 0 )
			{
				rankingCode = 2;
			}
			else if( javaBundleDesired.compare(Properties::PREFER_JRE_JAVA_BUNDLE) == 0 ||
				javaBundleDesired.empty() )
			{
				rankingCode = 1;
			}
			else if( javaBundleDesired.compare(Properties::REQUIRE_JRE_JAVA_BUNDLE) == 0 )
			{
				setAcceptable(false);
				rankingCode = 0;
			}
			else
			{
				throw tstring( _T("Desired Java bundle settings are not correct.") );			
			}
		}
		else if( getJavaBundle().compare(Properties::JRE_JAVA_BUNDLE) == 0 )
		{
			if( javaBundleDesired.compare(Properties::REQUIRE_JRE_JAVA_BUNDLE) == 0 )
			{
				rankingCode = 3;
			}
			else if( javaBundleDesired.compare(Properties::PREFER_JRE_JAVA_BUNDLE) == 0 ||
				javaBundleDesired.empty() )
			{
				// PREFER_JRE_JAVA_BUNDLE is the default
				rankingCode = 2;
			}
			else if( javaBundleDesired.compare(Properties::PREFER_SDK_JAVA_BUNDLE) == 0 )
			{
				rankingCode = 1;
			}
			else if( javaBundleDesired.compare(Properties::REQUIRE_SDK_JAVA_BUNDLE) == 0 )
			{
				setAcceptable(false);
				rankingCode = 0;
			}
			else
			{
				throw tstring( _T("Desired Java bundle settings are not correct.") );
			}
		}
		else
		{
			throw tstring( _T("Java bundle must be set before adjusting comparable version") );
		}

		TCHAR addedToken[4];
		_stprintf_s(addedToken, _T("B%u."), rankingCode);
		setComparableVersion(getComparableVersion() + tstring(addedToken) );
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( se );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.adjustComparableVersionForJavaBundle()") );
		ErrHandler::severeError( _T("Error while adjusting the comparable Java version bundle.") );
	}
}

void JVMInfo::adjustComparableVersionForBinJvmDir(tstring& binJvmDirDesired)
{
	try
	{
		unsigned int rankingCode = 0; // 0 to 3 the greater the number the higher in the ranking

		if( getBinJvmDir().compare(Properties::CLIENT_BIN_JVM_DIR) == 0 )
		{
			if( binJvmDirDesired.compare(Properties::REQUIRE_CLIENT_BIN_JVM_DIR) == 0 )
			{
				rankingCode = 3;
			}
			else if( binJvmDirDesired.compare(Properties::PREFER_CLIENT_BIN_JVM_DIR) == 0 || 
				binJvmDirDesired.empty() )
			{
				// PREFER_CLIENT_BIN_JVM_DIR is the default
				rankingCode = 2;
			}
			else if( binJvmDirDesired.compare(Properties::PREFER_SERVER_BIN_JVM_DIR) == 0 )
			{
				rankingCode = 1;
			}
			else if( binJvmDirDesired.compare(Properties::REQUIRE_SERVER_BIN_JVM_DIR) == 0 )
			{
				setAcceptable(false);
				rankingCode = 0;
			}
			else
			{
				throw tstring( _T("Desired JVM settings are not correct.") );
			}
		}
		else if( getBinJvmDir().compare(Properties::SERVER_BIN_JVM_DIR) == 0 )
		{
			if( binJvmDirDesired.compare(Properties::REQUIRE_SERVER_BIN_JVM_DIR) == 0 )
			{
				rankingCode = 3;
			}
			else if( binJvmDirDesired.compare(Properties::PREFER_SERVER_BIN_JVM_DIR) == 0 )
			{
				rankingCode = 2;
			}
			else if( binJvmDirDesired.compare(Properties::PREFER_CLIENT_BIN_JVM_DIR) == 0 ||
				binJvmDirDesired.empty() )
			{
				rankingCode = 1;
			}
			else if( binJvmDirDesired.compare(Properties::REQUIRE_CLIENT_BIN_JVM_DIR) == 0 )
			{
				setAcceptable(false);
				rankingCode = 0;
			}
			else
			{
				throw tstring( _T("Desired JVM settings are not correct.") );
			}
		}
		else
		{
			throw tstring( _T("Bin Jvm Dir must be set before adjusting the comparable version.") );
		}

		TCHAR addedToken[4];
		_stprintf_s(addedToken, _T("D%u."), rankingCode);
		setComparableVersion(getComparableVersion() + tstring(addedToken) );
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( se );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.adjustComparableVersionForBinJvmDir()") );
		ErrHandler::severeError( _T("Error while adjusting the comparable Java JVM directories.") );
	}
}

JVMInfo* JVMInfo::partialClone()
{
	JVMInfo* pJvmInfo = NULL;
	try
	{
		pJvmInfo =  new JVMInfo();
		
		tstring* pJavaHomePath = new tstring(getJavaHomePath());
		pJvmInfo->setJavaHomePath(*pJavaHomePath);
		tstring* pJavaBundle = new tstring(getJavaBundle());
		pJvmInfo->setJavaBundle(*pJavaBundle);	
		tstring* pVersion = new tstring(getVersion());
		pJvmInfo->setVersion(*pVersion);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.clone()") );
		ErrHandler::severeError();
	}
	return pJvmInfo;
}

// JrePath is the path to the directory that holds
// the client or server directories.
tstring& JVMInfo::getJrePath()
{
	try
	{
		if( !m_jrePath.empty() )
		{
			return m_jrePath;
		}
		
		if( getJavaBundle().compare(Properties::JRE_JAVA_BUNDLE) == 0 )
		{
			m_jrePath = getJavaHomePath() + tstring(_T("\\bin\\"));
		}
		else if( getJavaBundle().compare(Properties::SDK_JAVA_BUNDLE) == 0 )
		{
			m_jrePath = getJavaHomePath() + tstring(_T("\\jre\\bin\\"));
		}
		else
		{
			throw tstring( _T("Java home path must be determined before the JRE path can be determined.") );
		}
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.getJrePath()") );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.getJrePath()") );
		ErrHandler::severeError( _T("Error while getting the JRE path.") );
	}
	return m_jrePath;
}

bool JVMInfo::existsJvmDLL(const tstring& binJvmDir)
{
	try
	{
		if( LocalUtilities::fileExists(getJvmDLLPath(binJvmDir)) )
		{
			return true;			
		}		
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.existsJvmDLL()") );
		ErrHandler::severeError( _T("Error while testing if JVM.DLL exists.") );
	}
	return false;
}

tstring& JVMInfo::getJvmDLLPath(const tstring& binJvmDir)
{
	tstring* pJvmDLLPath = new tstring();
	try
	{
		*pJvmDLLPath = getJrePath() + binJvmDir + tstring(_T("\\jvm.dll"));

		DEBUG_SHOW( tstring(_T("*pJvmDLLPath=")) + *pJvmDLLPath);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMInfo.getJvmDLLPath()") );
		ErrHandler::severeError( _T("Error while getting the jvm.dll path.") );
	}
	return *pJvmDLLPath;
}

void JVMInfo::setAcceptable(bool acceptable)
{
	m_acceptable = acceptable;
}

bool JVMInfo::isAcceptable()
{
	return m_acceptable;
}

tstring& JVMInfo::toString()
{
	tstring* pJvmInfoString = new tstring();

	pJvmInfoString->append( _T("m_jvmPath=") );
	pJvmInfoString->append( m_jvmPath );
	pJvmInfoString->append( _T(",m_javaBundle=") );
	pJvmInfoString->append( m_javaBundle );
	pJvmInfoString->append( _T(",m_binJvmDir=") );
	pJvmInfoString->append( m_binJvmDir );
	pJvmInfoString->append( _T(",m_comparableVersion=") );
	pJvmInfoString->append( m_comparableVersion );
	pJvmInfoString->append( _T(",m_javaHomePath=") );
	pJvmInfoString->append( m_javaHomePath );
	pJvmInfoString->append( _T(",m_version=") );
	pJvmInfoString->append( m_version );
	pJvmInfoString->append( _T(",m_jrePath=") );
	pJvmInfoString->append( m_jrePath );
	pJvmInfoString->append( _T(",m_acceptable=") );
	pJvmInfoString->append( m_acceptable == true ? _T("true") : _T("false") );

	return *pJvmInfoString;
}
