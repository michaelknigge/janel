/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "PropertiesFile.h"
#include <iostream>
#include <fstream>
#include "LocalUtilities.h"
#include "PropertyValueVariables.h"
#include "PropertiesCustom.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "JVMChooser.h"
#include <algorithm>

using namespace std;

const char PropertiesFile::LINE_CONTINUATION_CARET = '^';

PropertiesFile::PropertiesFile(Properties* pProperties)
{
	m_pProperties = pProperties;
}

PropertiesFile::~PropertiesFile()
{

}

tstring& PropertiesFile::getPropertiesFileName()
{
	try
	{
		size_t indexOfExtension = m_pProperties->getFullPathAndNameOfExe().find_last_of( _T('.') );
		tstring extensionOfExecutable = _T("");
		
		if( indexOfExtension > 0 )
		{
			extensionOfExecutable = tstring(m_pProperties->getFullPathAndNameOfExe().substr( indexOfExtension ));
			::std::transform(extensionOfExecutable .begin(), extensionOfExecutable.end(), extensionOfExecutable.begin(), ::towupper);
		}
		
		if( extensionOfExecutable.compare( _T(".EXE") ) == 0 )
		{
			m_propertiesFileName = m_pProperties->getFullPathAndNameOfExe().substr( 0, indexOfExtension ) + _T(".lap");
		}
		else
		{
			m_propertiesFileName = (m_pProperties->getFullPathAndNameOfExe()) + _T(".lap");
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in PropertiesFile.getPropertiesFileName()") );
		ErrHandler::severeError( _T("Error getting configuration file name.") );
	}
	return m_propertiesFileName;
}

// remove comments and blanks from properties
// then process custom properties without variables
// then process custom properties with early variables, no late variables
// then process non-custom properties without variables
// then process non-custom properties with early variables, no late variables
// then get the best JVM
// then process custom properties with late variables
// then process non-custom properties with late variables
void PropertiesFile::loadPropertiesFromFile()
{
	try
	{
		vector<PropertyFileEntry*> propsFromFile;
        tstring& name = getPropertiesFileName();
		tifstream propsFile( name.c_str() );

		if( !propsFile )
		{
			throw tstring(_T("Could not open file ") + name);
		}

		// load propsFromFile leaving out comments and blank lines
		loadProps_NoCommentsBlanks(propsFile, propsFromFile);
		propsFile.close();

		PropertyValueVariables propertyValueVars( m_pProperties );
		PropertiesCustom customProps;
		
		// process custom properties without PropertyValueVariables
		processCustomProps_NoPropertyValueVars(propsFromFile, propertyValueVars);

		// then process custom properties with only early PropertyValueVariables
		processCustomProperties_WithOnlyEarlyPropertyValueVars(propsFromFile, propertyValueVars);

		// then process non-custom properties without PropertyValueVariables
		processNonCustomProps_NoPropertyValueVars(propsFromFile, propertyValueVars);

		// then process non-custom properties with only early PropertyValueVariables
		processNonCustomProps_WithOnlyEarlyPropertyValueVars(propsFromFile, propertyValueVars);

		// get the best JVM here
		JVMChooser chooser(m_pProperties);
		m_pProperties->setBestJvmInfo(chooser.getBestJVM());

		// then process custom properties with late PropertyValueVariables
		processCustomProps_WithLatePropertyValueVars(propsFromFile, propertyValueVars);

		// then process non-custom properties with late PropertyValueVariables
		processNonCustomProps_WithLatePropertyValueVars(propsFromFile, propertyValueVars);
	}
	catch(tstring& se)
	{
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in PropertiesFile.loadPropertiesFromFile()") );
		ErrHandler::severeError( _T("Error loading properties from configuration file.") );
	}
}

// load propsFromFile leaving out comments and blank lines
void PropertiesFile::loadProps_NoCommentsBlanks(tifstream& propsFile, vector<PropertyFileEntry*>& propsFromFile)
{
	tstring concatenatedPropertyLine;
	int propertyStartedAtRowNumber = 0;
	bool isPreviousLineTerminatorEscaped = false;

	int rowNumber = 0;
	while( propsFile.eof() != true )
	{
		TCHAR property[5000]; 
		propsFile.getline( property, 5000 );
		++rowNumber;
					
		tstring strProperty(property);
		tstring* pstrProperty = &strProperty;
		
		LocalUtilities::trim(strProperty);
		
		if( strProperty.empty() || isComment(strProperty) )
			continue;

		// should next line be concatenated to current line 
		bool isCurrentLineTerminatorEscaped = isLineTerminatorEscaped(strProperty);

		// if so, clean current string
		if (isCurrentLineTerminatorEscaped) 
		{
			strProperty.erase(strProperty.length() - 1);
			LocalUtilities::trim(strProperty);
		}

		// if this line does not continue last line, store row number
		if (!isPreviousLineTerminatorEscaped) 
			propertyStartedAtRowNumber = rowNumber;

		// join with last line (if there was any)
		concatenatedPropertyLine.append(strProperty);

		// if current property does not continue on next line -> store it
		if (!isCurrentLineTerminatorEscaped || propsFile.eof()) {					
			// copy concatenatedPropertyLine back to strProperty;
			strProperty.assign(concatenatedPropertyLine);

			PropertyFileEntry* pPropFileEntry = new PropertyFileEntry();
			pPropFileEntry->setRowNumber(propertyStartedAtRowNumber);
			pPropFileEntry->setText(*pstrProperty);
			propsFromFile.push_back( pPropFileEntry );

			// reset concatenation info
			concatenatedPropertyLine.clear();
			propertyStartedAtRowNumber = 0;
		}

		isPreviousLineTerminatorEscaped = isCurrentLineTerminatorEscaped;
	}
}

bool PropertiesFile::isLineTerminatorEscaped(const tstring& strProperty)
{
	bool terminatorEscaped = false;

	static const basic_string <char>::size_type npos = -1;
	size_t lineContinationCharIndex = strProperty.find_last_of(PropertiesFile::LINE_CONTINUATION_CARET);
	if(lineContinationCharIndex != npos)
	{
		size_t lastCharacterIndex = strProperty.length() - 1;
		if(lineContinationCharIndex == lastCharacterIndex)
		{
			terminatorEscaped  = true;
		}
	}

	return terminatorEscaped;
}


// process custom properties without PropertyValueVariables
void PropertiesFile::processCustomProps_NoPropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				!propertyValueVars.containsPropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( tstring(_T("custom property without vars ")) + propertyFileEntryText );
		}
	}
}

// process custom properties with only early PropertyValueVariables
void PropertiesFile::processCustomProperties_WithOnlyEarlyPropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				propertyValueVars.containsPropertyVariables(propertyFileEntryText) &&
				!propertyValueVars.containsLatePropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( _T("custom property with early vars ") + propertyFileEntryText );
		}
	}
}

// process non-custom properties without PropertyValueVariables
void PropertiesFile::processNonCustomProps_NoPropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( !PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				!propertyValueVars.containsPropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( _T("custom property without vars ") + propertyFileEntryText );
		}
	}
}

// process non-custom properties with only early PropertyValueVariables
void PropertiesFile::processNonCustomProps_WithOnlyEarlyPropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( !PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				propertyValueVars.containsPropertyVariables(propertyFileEntryText) &&
				!propertyValueVars.containsLatePropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( _T("custom property with early vars ") + propertyFileEntryText );
		}
	}		
}

// process custom properties with late PropertyValueVariables
void PropertiesFile::processCustomProps_WithLatePropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				propertyValueVars.containsLatePropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( _T("custom property with late vars ") + propertyFileEntryText );
		}
	}
}

// process non-custom properties with late PropertyValueVariables
void PropertiesFile::processNonCustomProps_WithLatePropertyValueVars(vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars)
{
	vector<PropertyFileEntry*>::iterator iter;
	for(iter = propsFromFile.begin(); iter != propsFromFile.end(); iter++)
	{
		tstring& propertyFileEntryText = (*iter)->getText();
		if( !PropertiesCustom::isCustomProperty(propertyFileEntryText) &&
				propertyValueVars.containsLatePropertyVariables(propertyFileEntryText) )
		{
			processProperty(*iter);
			DEBUG_SHOW( _T("non-custom property with late vars ") + propertyFileEntryText );
		}
	}		
}

tstring& PropertiesFile::parsePropertyName(const tstring& strProp)
{
	tstring* pPropertyName = 0;
	try
	{
		pPropertyName =  new tstring;
		tstring::size_type indexOfEqualSign = strProp.find_first_of( '=' );
		*pPropertyName = strProp.substr(0, indexOfEqualSign);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in parsePropertyName()") );
		ErrHandler::severeError( _T("Error parsing property name in file."));
	}
	return *pPropertyName;
}

tstring& PropertiesFile::parsePropertyValue(const tstring& strProp)
{
	tstring *pPropertyValue = 0;
	try
	{
		pPropertyValue = new tstring;
		tstring::size_type indexOfEqualSign = strProp.find_first_of( _T('=') );
		*pPropertyValue = strProp.substr(indexOfEqualSign + 1);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in parsePropertyValue()") );
		ErrHandler::severeError( _T("Error parsing property value in file.") );
	}
	return *pPropertyValue;
}

bool PropertiesFile::isComment(const tstring& strProperty)
{
	try
	{
		if( strProperty.empty() )
			return false;
		else if( strProperty[0] == _T('#') )
			return true;
		else
			return false;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in isComment") );
		ErrHandler::severeError( _T("Error determining if line is comment in file.") );
	}
	return false;
}

bool PropertiesFile::isAssignmentProperty(const tstring& strProperty)
{
	try
	{
		tstring::size_type indexOfEqualSign = strProperty.find_first_of( _T('=') );
		
		if( indexOfEqualSign >= 0 )
			return true;
		else
			return false;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in isAssignmentProperty") );
		ErrHandler::severeError( _T("Error determining if is an assignment property in file.") );
	}
	return false;
}

void PropertiesFile::processProperty(PropertyFileEntry* pPropFileEntry)
{
	try
	{
		tstring& strProperty = pPropFileEntry->getText();
		DEBUG_SHOW( tstring(_T("PropertiesFile::processProperty() strProperty=")) + tstring(strProperty) );
		PropertyValueVariables propertyValueVars( m_pProperties );
		propertyValueVars.resolvePropertyVariables(strProperty);

		// Currently not setting the name and value in PropertyFileEntry
		// unless specifically needed. If more situations arise that
		// they are needed then we should set them.
		tstring propertyName;
		tstring propertyValue;
		if( isAssignmentProperty(strProperty) )
		{
			propertyName = LocalUtilities::trim(
					parsePropertyName(strProperty) ); 
			propertyValue = LocalUtilities::trim(
					parsePropertyValue(strProperty) ); 
		}
		else
		{
			propertyName = LocalUtilities::trim( strProperty );
		}
		
		DEBUG_STMT( tstring debugReadProp = _T("strProperty=") + strProperty +
				_T(",propertyName=") + propertyName +
				_T(",propertyValue=") + propertyValue; )
		DEBUG_SHOW( debugReadProp );
		
		// isJavaSystemProperty
		if( m_pProperties->isJavaSystemProperty( strProperty ) )
		{
			m_pProperties->addJavaSystemProperty( strProperty );
		}
		// MAIN_CLASS
		else if( propertyName.compare(PropertiesCustom::MAIN_CLASS) == 0 )
		{
			m_pProperties->setMainClass(propertyValue);
		}
		// JAVA_BUNDLE
		else if( propertyName.compare(PropertiesCustom::JAVA_BUNDLE) == 0 )
		{
			m_pProperties->setCustomJavaBundle(propertyValue); 
		}
		// BIN_JVM_DIR
		else if( propertyName.compare(PropertiesCustom::BIN_JVM_DIR) == 0 )
		{
			m_pProperties->setCustomBinJvmDir(propertyValue);
		}
		// JVM_PATH
		else if( propertyName.compare(PropertiesCustom::JVM_PATH) == 0 )
		{
			m_pProperties->setCustomJvmPath(propertyValue);
		}
		// JAVA_HOME_PATH
		else if( propertyName.compare(PropertiesCustom::JAVA_HOME_PATH) == 0 )
		{
			m_pProperties->setCustomJavaHomePath(propertyValue);
		}
		// MIN_JAVA_VERSION
		else if( propertyName.compare(PropertiesCustom::MIN_JAVA_VERSION) == 0 )
		{
			m_pProperties->setMinJavaVersion(propertyValue);			
		}
		// MAX_JAVA_VERSION
		else if( propertyName.compare(PropertiesCustom::MAX_JAVA_VERSION) == 0 )
		{
			m_pProperties->setMaxJavaVersion(propertyValue);			
		}
		// TRAP_CONSOLE_CTRL
		else if ( propertyName.compare(PropertiesCustom::TRAP_CONSOLE_CTRL) == 0 )
		{
			m_pProperties->setTrapConsoleCtrl(propertyValue);
		}
		// CLASSPATH_JARS_DIR
		else if ( propertyName.compare(PropertiesCustom::CLASSPATH_JARS_DIR) == 0 )
		{
			m_pProperties->addJarsToClasspath(propertyValue, false);
		}
		// CLASSPATH_JARS_DIR_RECURSIVE
		else if ( propertyName.compare(PropertiesCustom::CLASSPATH_JARS_DIR_RECURSIVE) == 0 )
		{
			m_pProperties->addJarsToClasspath(propertyValue, true);
		}
		// DEBUG_FILE
		else if ( propertyName.compare(PropertiesCustom::DEBUG_FILE) == 0 )
		{
			m_pProperties->setDebugFile(propertyValue);
		}
		// LIBRARY_PATH_DIR
		else if ( propertyName.compare(PropertiesCustom::LIBRARY_PATH_DIR) == 0 )
		{
			m_pProperties->addToLibraryPath(propertyValue, false);
		}
		// LIBRARY_PATH_DIR_RECURSIVE
		else if ( propertyName.compare(PropertiesCustom::LIBRARY_PATH_DIR_RECURSIVE) == 0 )
		{
			m_pProperties->addToLibraryPath(propertyValue, true);
		}
		// WORKING_DIR
		else if ( propertyName.compare(PropertiesCustom::WORKING_DIR) == 0 )
		{
			m_pProperties->setWorkingDir(propertyValue);
		}
		// SERVICE_NAME
		else if ( propertyName.compare(PropertiesCustom::SERVICE_NAME) == 0 )
		{
			m_pProperties->setServiceName(propertyValue);
		}
		// SERVICE_DISPLAY_NAME
		else if ( propertyName.compare(PropertiesCustom::SERVICE_DISPLAY_NAME) == 0 )
		{
			m_pProperties->setServiceDisplayName(propertyValue);
		}
		// SERVICE_DESCRIPTION
		else if ( propertyName.compare(PropertiesCustom::SERVICE_DESCRIPTION) == 0 )
		{
			m_pProperties->setServiceDescription(propertyValue);
		}
		// SERVICE_OPTION_INSTALL
		else if ( propertyName.compare(PropertiesCustom::SERVICE_OPTION_INSTALL) == 0 )
		{
			m_pProperties->setServiceOptionInstall(propertyValue);
		}
		// SERVICE_OPTION_UNINSTALL
		else if ( propertyName.compare(PropertiesCustom::SERVICE_OPTION_UNINSTALL) == 0 )
		{
			m_pProperties->setServiceOptionUninstall(propertyValue);
		}
		// MAIN_ARGUMENT
		else if ( propertyName.compare(PropertiesCustom::MAIN_ARGUMENT) == 0 )
		{
			pPropFileEntry->setValue(propertyValue);
			m_pProperties->addCommandLineArgument(*pPropFileEntry);
		}
		// SYSPROP_PROCESS_ID
		else if ( propertyName.compare(PropertiesCustom::SYSPROP_PROCESS_ID) == 0 )
		{
			m_pProperties->setSystemPropertyProcessId(propertyValue);
		}
		// SPLASH
		else if ( propertyName.compare(PropertiesCustom::SPLASH) == 0 )
		{
			m_pProperties->setSplash(propertyValue);
		}
        // MEMORY_CHECK_LIMITS
        else if ( propertyName.compare(PropertiesCustom::MEMORY_CHECK_LIMITS) == 0 )
        {
            m_pProperties->setMemoryCheckLimits(propertyValue);
        }
        // MAX_MEMORY_AVAILABLE_PERCENT
        else if ( propertyName.compare(PropertiesCustom::MAX_MEMORY_AVAILABLE_PERCENT) == 0 )
        {
            m_pProperties->setMaxMemoryPercentOfAvailable(propertyValue);
        }
        // INIT_MEMORY_AVAILABLE_PERCENT
        else if ( propertyName.compare(PropertiesCustom::INIT_MEMORY_AVAILABLE_PERCENT) == 0 )
        {
            m_pProperties->setInitMemoryPercentOfAvailable(propertyValue);
        }
        // MAX_MEMORY_TOTAL_PERCENT
        else if ( propertyName.compare(PropertiesCustom::MAX_MEMORY_TOTAL_PERCENT) == 0 )
        {
            m_pProperties->setMaxMemoryPercentOfTotal(propertyValue);
        }
        // INIT_MEMORY_TOTAL_PERCENT
        else if ( propertyName.compare(PropertiesCustom::INIT_MEMORY_TOTAL_PERCENT) == 0 )
        {
            m_pProperties->setInitMemoryPercentOfTotal(propertyValue);
        }
        // MAX_MEMORY_UPPER_LIMIT
        else if ( propertyName.compare(PropertiesCustom::MAX_MEMORY_UPPER_LIMIT) == 0 )
        {
            m_pProperties->setMaxMemoryUpperLimit(propertyValue);
        }
        // INIT_MEMORY_UPPER_LIMIT
        else if ( propertyName.compare(PropertiesCustom::INIT_MEMORY_UPPER_LIMIT) == 0 )
        {
            m_pProperties->setInitMemoryUpperLimit(propertyValue);
        }
        // MAX_MEMORY_LOWER_LIMIT
        else if ( propertyName.compare(PropertiesCustom::MAX_MEMORY_LOWER_LIMIT) == 0 )
        {
            m_pProperties->setMaxMemoryLowerLimit(propertyValue);
        }
        // INIT_MEMORY_LOWER_LIMIT
        else if ( propertyName.compare(PropertiesCustom::INIT_MEMORY_LOWER_LIMIT) == 0 )
        {
            m_pProperties->setInitMemoryLowerLimit(propertyValue);
        }
		// CUSTOM_ERROR_TEXT
        else if ( propertyName.compare(PropertiesCustom::CUSTOM_DEFAULT_ERROR_TEXT) == 0 )
        {
            m_pProperties->setCustomErrorDefaultText(propertyValue);
        }
        // SHOW_ERROR_DETAIL
        else if ( propertyName.compare(PropertiesCustom::SHOW_ERROR_DETAIL) == 0 )
        {
			if ( LocalUtilities::convertToBoolean(propertyValue) ) 
			{
                m_pProperties->setShowErrorDetail(true);
            } 
			else 
			{
                m_pProperties->setShowErrorDetail(false);
            }
        }

		// put other property checks here
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in processProperty") );
		ErrHandler::severeError( _T("Error processing property.") );
	}
}

