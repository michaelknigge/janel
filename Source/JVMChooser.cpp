/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "JVMChooser.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "WindowsRegistry.h"
#include <algorithm>
#include "LocalUtilities.h"

using namespace std;

JVMChooser::JVMChooser(Properties* pProperties)
{
	m_pProperties = pProperties;
}

JVMChooser::~JVMChooser(void)
{
}

bool isFirstJvmBetter(JVMInfo& firstJvmInfo, JVMInfo& secondJvmInfo)
{
	if( (firstJvmInfo.getComparableVersion()).compare( (secondJvmInfo.getComparableVersion()) ) < 0 )
	{
		return true;
	}

	return false;
}

JVMInfo* JVMChooser::getBestJVM()
{
	JVMInfo *pBestJvm = 0;
	try
	{
		pBestJvm = getJvmFromCustomJvmPath();
		if( pBestJvm != 0 )
		{
			return pBestJvm;
		}

		pBestJvm = getJvmFromCustomJavaHomePath();
		if( pBestJvm != 0 )
		{
			return pBestJvm;
		}

		vector<JVMInfo>* pVecJvmInfo = new vector<JVMInfo>;

		getAllJVMs(pVecJvmInfo);
		size_t count = pVecJvmInfo->size();
		
		breakoutBinJvmDirs(pVecJvmInfo);

		determineVersions(pVecJvmInfo);

		removeUnacceptableJVMs(pVecJvmInfo);

		if( pVecJvmInfo->size() == 0 )
		{
			throw string( "No acceptable JVMs were found." );
		}

		// get the JVMInfo with the best comparable version
		vector<JVMInfo>::iterator vecJVMInfoIter;
		vecJVMInfoIter = max_element(pVecJvmInfo->begin(), pVecJvmInfo->end(), isFirstJvmBetter );
		pBestJvm = &(*vecJVMInfoIter);
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( tstring(_T("Exception in JVMChooser.getBestJVM(): ")) + se );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getBestJVM()") );
		ErrHandler::severeError( _T("Error while locating the most appropriate JVM.") );
	}
	return pBestJvm;
}

void JVMChooser::getAllJVMs(vector<JVMInfo>* pVecJvmInfo)
{
	try
	{
		WindowsRegistry winsReg(m_pProperties);

		tstring regKey(_T("SOFTWARE\\JavaSoft\\Java Development Kit"));
		winsReg.addAllSdkJvms(pVecJvmInfo, regKey);
		regKey = _T("SOFTWARE\\JavaSoft\\Java Runtime Environment");
		winsReg.addAllJreJvms(pVecJvmInfo, regKey);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getAllJVMs()") );
		ErrHandler::severeError( _T("Error while locating JVMs.") );
	}
}

void JVMChooser::breakoutBinJvmDirs(vector<JVMInfo>* pVecJvmInfo)
{
	try
	{
		vector<JVMInfo>* pAdditionsVecJvmInfo = new vector<JVMInfo>;
		// loop through all the JVMs, if there is a server JVM add to additions
		size_t top = pVecJvmInfo->size();

		for( size_t j = 0; j < top; j++ )
		{
			size_t i = top - j - 1;
			DEBUG_SHOW( _T("Top of first loop through JVMInfos") );
			JVMInfo& jvmInfo = pVecJvmInfo->at(i);
			
			if( jvmInfo.existsJvmDLL(Properties::CLIENT_BIN_JVM_DIR) &&
					jvmInfo.existsJvmDLL(Properties::SERVER_BIN_JVM_DIR) )
			{
				JVMInfo* pNewJvmInfo = jvmInfo.partialClone();
				pNewJvmInfo->setBinJvmDir(Properties::SERVER_BIN_JVM_DIR);
				pNewJvmInfo->setJvmPath(pNewJvmInfo->getJvmDLLPath(Properties::SERVER_BIN_JVM_DIR));
				pAdditionsVecJvmInfo->push_back( *pNewJvmInfo );

				jvmInfo.setBinJvmDir(Properties::CLIENT_BIN_JVM_DIR);
				jvmInfo.setJvmPath(jvmInfo.getJvmDLLPath(Properties::CLIENT_BIN_JVM_DIR));
			}
			else if( jvmInfo.existsJvmDLL(Properties::CLIENT_BIN_JVM_DIR) )
			{
				jvmInfo.setBinJvmDir(Properties::CLIENT_BIN_JVM_DIR);
				jvmInfo.setJvmPath(jvmInfo.getJvmDLLPath(Properties::CLIENT_BIN_JVM_DIR));
			}
			else if( jvmInfo.existsJvmDLL(Properties::SERVER_BIN_JVM_DIR) )
			{
				jvmInfo.setBinJvmDir(Properties::SERVER_BIN_JVM_DIR);
				jvmInfo.setJvmPath(jvmInfo.getJvmDLLPath(Properties::SERVER_BIN_JVM_DIR));
			}
			else 
			{
				pVecJvmInfo->erase(pVecJvmInfo->begin() + i);
			}
		}

		// add the additions which are the server JVMs
		for( unsigned int i=0; i < pAdditionsVecJvmInfo->size(); i++ )
		{
			JVMInfo& jvmInfo = pAdditionsVecJvmInfo->at(i);
			
			pVecJvmInfo->push_back( jvmInfo );
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.breakoutBinJvmDirs()") );
		ErrHandler::severeError( _T("Error while separating out the different JVMs (client or server).") );
	}
}

void JVMChooser::determineVersions(vector<JVMInfo>* pVecJvmInfo)
{
	try
	{
		// loop through again but now client and server are two different JVMInfos
		for( unsigned int i=0; i < pVecJvmInfo->size(); i++ )
		{
			JVMInfo& jvmInfo = pVecJvmInfo->at(i);
			
			if( m_pProperties->isVersionGreaterThanEqualMin(jvmInfo.getVersion()) == false ||
					m_pProperties->isVersionLessThanEqualMax(jvmInfo.getVersion()) == false )
			{
				DEBUG_SHOW( tstring(_T("JVMChooser.determineVersions version is bad:")) + jvmInfo.getVersion() );
				jvmInfo.setAcceptable(false);
			}

			jvmInfo.setComparableVersion(jvmInfo.getComparableVersionUsingRegularVersion(jvmInfo.getVersion()));

			if( (m_pProperties->getCustomBinJvmDir().empty() && m_pProperties->getCustomJavaBundle().empty()) ||
				m_pProperties->isCustomJavaBundleRequired() ||
				(m_pProperties->isCustomJavaBundlePreferred() && !m_pProperties->isCustomBinJvmDirRequired()) )
			{
				// this is for if JavaBundle is important and as default
				jvmInfo.adjustComparableVersionForJavaBundle( m_pProperties->getCustomJavaBundle() );
				jvmInfo.adjustComparableVersionForBinJvmDir( m_pProperties->getCustomBinJvmDir() );
			}
			else
			{
				jvmInfo.adjustComparableVersionForBinJvmDir( m_pProperties->getCustomBinJvmDir() );
				jvmInfo.adjustComparableVersionForJavaBundle( m_pProperties->getCustomJavaBundle() );
			}
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.determineVersions()") );
		ErrHandler::severeError( _T("Error while determining versions of the JVMs.") );
	}
}

void JVMChooser::removeUnacceptableJVMs(vector<JVMInfo>* pVecJvmInfo)
{
	try
	{
		size_t top = pVecJvmInfo->size();

		for( size_t j = 0; j < top ; j++ )
		{
			size_t i = top - j - 1;
			JVMInfo& jvmInfo = pVecJvmInfo->at(i);
			
			if( !jvmInfo.isAcceptable() )
			{
				DEBUG_SHOW( _T("Removing JVM") ); 
				pVecJvmInfo->erase(pVecJvmInfo->begin()+i);
			}
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.removeUnacceptableJVMs()") );
		ErrHandler::severeError( _T("Error while removing unacceptable versions of the JVM.") );
	}
}

JVMInfo* JVMChooser::getJvmFromCustomJvmPath()
{
	JVMInfo* pJvmInfo = 0;
	try
	{
		if( !m_pProperties->getCustomJvmPath().empty() )
		{
			if( LocalUtilities::fileExists(m_pProperties->getCustomJvmPath()) )
			{
				pJvmInfo = new JVMInfo();
				pJvmInfo->setJvmPath(m_pProperties->getCustomJvmPath());
				return pJvmInfo;
			}
			else
			{
				throw tstring( _T("This JVM file could not be found: ") + m_pProperties->getCustomJvmPath() );
			}
		}
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getJvmFromCustomJvmPath(): ") + se );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getJvmFromCustomJvmPath()") );
		ErrHandler::severeError( _T("Error while getting the JVM from the custom jvm path setting.") );
	}
	return 0;
}

// Returns 0 if JVM is not set with custom path setting
JVMInfo* JVMChooser::getJvmFromCustomJavaHomePath()
{
	JVMInfo* pJvmInfo = 0;
	try
	{
		if( !m_pProperties->getCustomJavaHomePath().empty() )
		{
			if( m_pProperties->getCustomBinJvmDir().compare(Properties::REQUIRE_SERVER_BIN_JVM_DIR) == 0 )
			{
				pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::SERVER_BIN_JVM_DIR);
				if( pJvmInfo == 0 )
				{
					throw tstring( _T("The server JVM could not be found for Java home: ") + m_pProperties->getCustomJavaHomePath());
				}
			}
			else if( m_pProperties->getCustomBinJvmDir().compare(Properties::REQUIRE_CLIENT_BIN_JVM_DIR) == 0 )
			{
				pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::CLIENT_BIN_JVM_DIR);
				if( pJvmInfo == 0 )
				{
					throw tstring( _T("The client JVM could not be found for Java home: ") + m_pProperties->getCustomJavaHomePath());
				}
			}
			else if( m_pProperties->getCustomBinJvmDir().compare(Properties::PREFER_SERVER_BIN_JVM_DIR) == 0 )
			{
				pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::SERVER_BIN_JVM_DIR);
				if( pJvmInfo == 0 )
				{
					pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::CLIENT_BIN_JVM_DIR);
					if( pJvmInfo == 0 )
					{
						throw tstring( _T("A JVM could not be found for Java home: ") + m_pProperties->getCustomJavaHomePath());
					}
				}
			}
			else
			{
				pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::CLIENT_BIN_JVM_DIR);
				if( pJvmInfo == 0 )
				{
					pJvmInfo = getJvm(m_pProperties->getCustomJavaHomePath(),Properties::SERVER_BIN_JVM_DIR);
					if( pJvmInfo == 0 )
					{
						throw tstring( _T("A JVM could not be found for Java home: ") + m_pProperties->getCustomJavaHomePath());
					}
				}
			}
		}
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getJvmFromCustomJavaHomePath(): ") + se );
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getJvmFromCustomJavaHomePath()") );
		ErrHandler::severeError( _T("Error while getting the JVM from the custom Java path setting.") );
	}
	return pJvmInfo;
}

JVMInfo* JVMChooser::getJvm(const tstring& javaHomePath, const tstring& binJvmDir)
{
	JVMInfo* pJvmInfo = 0;
	try
	{			
		tstring pathToJvm = javaHomePath + tstring(_T("\\bin\\")) + binJvmDir + tstring(_T("\\jvm.dll"));
		if( LocalUtilities::fileExists(pathToJvm) )
		{
			pJvmInfo = new JVMInfo();
			pJvmInfo->setJvmPath(pathToJvm);
			pJvmInfo->setJavaHomePath(javaHomePath);
			return pJvmInfo;
		}
			
		pathToJvm = javaHomePath + tstring(_T("\\jre\\bin\\")) + binJvmDir + tstring(_T("\\jvm.dll"));
		if( LocalUtilities::fileExists(pathToJvm) )
		{
			pJvmInfo = new JVMInfo();
			pJvmInfo->setJvmPath(pathToJvm);
			pJvmInfo->setJavaHomePath(javaHomePath + tstring(_T("\\jre")));
			return pJvmInfo;
		}			
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMChooser.getJvm()") );
		ErrHandler::severeError( _T("Error while getting the JVM from the Java home path and JVM type.") );
	}
	return 0;
}

