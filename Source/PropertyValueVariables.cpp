/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "PropertyValueVariables.h"
#include "LocalUtilities.h"
#include "Debug.h"
#include "ErrHandler.h"
#include <cassert>

using namespace std;

const tstring PropertyValueVariables::FOUND_JAVA_HOME = _T("FOUND_JAVA_HOME");
const tstring PropertyValueVariables::FOUND_EXE_FOLDER = _T("FOUND_EXE_FOLDER");
const tstring PropertyValueVariables::FOUND_EXE = _T("FOUND_EXE");
const tstring PropertyValueVariables::SELF_HOME = _T("SELF_HOME");
const tstring PropertyValueVariables::SELF_NAME = _T("SELF_NAME");
const tstring PropertyValueVariables::SELF_PARENT_NAME = _T("SELF_PARENT_NAME");
const tstring PropertyValueVariables::CALLER_DIR = _T("CALLER_DIR");
const tstring PropertyValueVariables::EXE_BITNESS = _T("EXE_BITNESS");
const tstring PropertyValueVariables::OS_BITNESS = _T("OS_BITNESS");
const tstring PropertyValueVariables::MAINCLASS_NAME = _T("MAINCLASS_NAME");
const tstring PropertyValueVariables::MAINCLASS_SIMPLE_NAME = _T("MAINCLASS_SIMPLE_NAME");

PropertyValueVariables::PropertyValueVariables(Properties* pProperties)
{
	m_pProperties = pProperties;
}

PropertyValueVariables::~PropertyValueVariables()
{

}

tstring& PropertyValueVariables::getValueFromVariable(const tstring& strVariable)
{
	tstring* pValueFromVariable = new tstring;
	try
	{
		tstring strJustVariable = strVariable.substr(2,strVariable.size()-1);
		
		DEBUG_SHOW(_T("strJustVariable=") + strJustVariable);

		if( strJustVariable.compare( FOUND_JAVA_HOME ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getBestJvmInfo()->getJavaHomePath();
		}
		else if ( strJustVariable.find( _T("env.") ) == 0)
		{
			pValueFromVariable = new tstring(getValueFromEnvironmentVariable(strJustVariable.substr(4)));
		}
		else if ( strJustVariable.compare( SELF_HOME ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getSelfHomePath();
		}
		else if ( strJustVariable.compare( SELF_NAME ) == 0 )
		{
			tstring& nameOfExecutable = m_pProperties->getFullPathAndNameOfExe();
			
			tstring tempString = nameOfExecutable.substr(0, nameOfExecutable.find_last_of(_T('.')));
			pValueFromVariable = new tstring(tempString.substr(nameOfExecutable.find_last_of(_T('\\')) + 1));
		}
		else if (strJustVariable.compare(SELF_PARENT_NAME) == 0)
		{
			tstring& selfHome = m_pProperties->getSelfHomePath();
			pValueFromVariable = new tstring(selfHome.substr(selfHome.find_last_of(_T('\\')) + 1));
		}
		else if (strJustVariable.compare( FOUND_EXE ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getFullPathAndNameOfExe();
		}
		else if (strJustVariable.compare( FOUND_EXE_FOLDER ) == 0 )
		{
			tstring& nameOfExecutable = m_pProperties->getFullPathAndNameOfExe();
			pValueFromVariable = new tstring(nameOfExecutable.substr(0, nameOfExecutable.find_last_of(_T('\\')) ) );
		}
		else if (strJustVariable.compare( CALLER_DIR ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getCallerDir();
		}
		else if (strJustVariable.compare( EXE_BITNESS ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getExeBitness();
		}
		else if (strJustVariable.compare( OS_BITNESS ) == 0 )
		{
			pValueFromVariable = &m_pProperties->getOperatingSystemBitness();
		}
		else if (strJustVariable.compare(MAINCLASS_NAME) == 0)
		{
			pValueFromVariable = &LocalUtilities::swapChar(m_pProperties->getMainClass(), _T('/'), _T('.'));
		}
		else if (strJustVariable.compare(MAINCLASS_SIMPLE_NAME) == 0)
		{
			tstring& className = m_pProperties->getMainClass();
			pValueFromVariable = new tstring(className.substr(className.find_last_of(_T('/')) + 1));
		}
		// check for other property value variables
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in getValueFromVariable") );
		ErrHandler::severeError( _T("Error getting value from property variable.") );
	}
	return *pValueFromVariable;
}

tstring PropertyValueVariables::getValueFromEnvironmentVariable(const tstring& strEnvVariable)
{
	tstring strEnvVariableName;
	tstring strEnvVariableDefault;

	size_t indexStartOfColon = strEnvVariable.find_first_of( _T(":") );
	if (indexStartOfColon != -1)
	{
		strEnvVariableName = strEnvVariable.substr(0, indexStartOfColon);
		strEnvVariableDefault = strEnvVariable.substr(indexStartOfColon + 1);
	}
	else
	{
		strEnvVariableName = strEnvVariable;
		strEnvVariableDefault = _T("");
	}

	DEBUG_SHOW(_T("env variable name=") + strEnvVariableName);
	DEBUG_SHOW(_T("env variable default=") + strEnvVariableName);

	DWORD outputSize = GetEnvironmentVariable(strEnvVariableName.c_str(), NULL, 0);

	if (outputSize == 0 && strEnvVariableDefault.size() > 0)
	{
		if (strEnvVariableDefault[0] == _T('!'))
		{
			ErrHandler::severeError(_T("Required environment variable ") + strEnvVariableName + _T(" is not set."));
		}

		if (strEnvVariableDefault[0] == _T('#'))
		{
			DEBUG_SHOW(_T("returning empty string for non-existing environment variable"));
			tstring* pValueFromVariable = new tstring;
			return *pValueFromVariable;
		}

		if (strEnvVariableDefault.size() > 1 && strEnvVariableDefault[0] == _T('-'))
		{
			DEBUG_SHOW(_T("returning = ") + strEnvVariableDefault.substr(1));
			return strEnvVariableDefault.substr(1);
		}

		if (strEnvVariableDefault.size() > 1 && strEnvVariableDefault[0] == _T('='))
		{
			DEBUG_SHOW(_T("returning and setting = ") + strEnvVariableDefault.substr(1));
			SetEnvironmentVariable(strEnvVariableName.c_str(), strEnvVariableDefault.substr(1).c_str());
			return strEnvVariableDefault.substr(1);
		}
	}
	
	if (outputSize != 0)
	{
		TCHAR* envVariableValue = new TCHAR[outputSize];
		GetEnvironmentVariable(strEnvVariableName.c_str(), envVariableValue, outputSize);

		tstring* pValueFromVariable = new tstring(envVariableValue);
		DEBUG_SHOW(_T("returning env var value = ") + *pValueFromVariable);
		return *pValueFromVariable;
	}
	else
	{
		DEBUG_SHOW(_T("returning empty string"));
		tstring* pValueFromVariable = new tstring;
		return *pValueFromVariable;
	}
}

tstring& PropertyValueVariables::resolvePropertyVariables(tstring& strProperty)
{
	DEBUG_SHOW( _T("start resolving:") + strProperty);
	try
	{
		tstring::size_type indexStartOfVariable = 0;
		while( indexStartOfVariable != -1 )
		{
			indexStartOfVariable = strProperty.find_first_of( _T("${"), indexStartOfVariable );
			if( indexStartOfVariable == -1 )
				break;

			tstring::size_type indexEndOfVariable = strProperty.find_first_of( _T("}"), indexStartOfVariable );

			tstring variable = strProperty.substr(indexStartOfVariable, indexEndOfVariable - indexStartOfVariable);
			LocalUtilities::trim(variable);
			DEBUG_SHOW( _T("variable=") + variable);
			tstring variableValue = getValueFromVariable(variable);
			DEBUG_SHOW( _T("variableValue=") + variableValue);

			// If the variable value is empty *AND* the property ends with ":+#", then we will ignore the
			// whole property.... in example, if you have "janel.include.file=${env.FOOBA:#}\custom.lap" in
			// the LAP file and the environment variable "FOOBA" is not set (or empty!), then the whole line
			// gets ignored...
			if (variableValue.size() == 0)
			{
				size_t len = variable.size();
				size_t pos = variable.find_first_of(_T(":#"));

				if (pos == len - 2)
				{
					strProperty[0] = _T('#'); // Ugly hack... Comment out the line...
				}
			}

			tstring prefixStrProperty = strProperty.substr(0,indexStartOfVariable);
			tstring postfixStrProperty = strProperty.substr(indexEndOfVariable+1);
			
			strProperty = prefixStrProperty + variableValue + postfixStrProperty;
			DEBUG_SHOW( _T("strProperty=") + strProperty);
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in resolvePropertyVariables") );
		ErrHandler::severeError( _T("Error resolving property variables."));
	}
	return strProperty;
}

bool PropertyValueVariables::containsPropertyVariables(const tstring& strProperty)
{
	try
	{
		size_t indexStartOfVariable = strProperty.find_first_of( _T("${") );
		if( indexStartOfVariable == -1 )
		{
			return false;
		}

		size_t indexEndOfVariable = strProperty.find_first_of( _T("}"), indexStartOfVariable );
		if( indexEndOfVariable == -1 )
		{
			return false;
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in containsEarlyPropertyVariables") );
		ErrHandler::severeError( _T("Error while determining if property contains early variables.") );
	}
	return true;
}

bool PropertyValueVariables::containsLatePropertyVariables(const tstring& strProperty)
{
	bool latePropertyVariableExist = false;
	try
	{
		size_t indexStartOfVariable = 0;
		for( ;; )
		{
			indexStartOfVariable = strProperty.find_first_of( _T("${"), indexStartOfVariable);
			if( indexStartOfVariable == -1 )
			{
				return latePropertyVariableExist;
			}

			size_t indexEndOfVariable = strProperty.find_first_of( _T("}"), indexStartOfVariable );
			if( indexEndOfVariable == -1 )
			{
				return latePropertyVariableExist;
			}

			tstring variable = strProperty.substr(indexStartOfVariable+2, indexEndOfVariable - indexStartOfVariable - 2);
			LocalUtilities::trim(variable);
			DEBUG_SHOW( _T("containsLatePropertyVariables variable=") + variable);
			
			// compare late property value variables here
			// future - put late property value variables in a vector
			if( variable.compare( FOUND_JAVA_HOME ) == 0 )
			{
				return true;
			}

			indexStartOfVariable = indexEndOfVariable;
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in containsLatePropertyVariables") );
		ErrHandler::severeError( _T("Error while determining if property contains late variables.") );
	}
	return latePropertyVariableExist;
}
