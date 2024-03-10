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
const tstring PropertyValueVariables::MAINCLASS_SIMPLENAME = _T("MAINCLASS_SIMPLENAME");

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
		    tstring tempString = strJustVariable.substr(4);
			tstring& strEnvVariable = tempString;
			size_t outputSize = 0;
			_tgetenv_s( &outputSize, NULL, 0, strEnvVariable.c_str() );
			if ( outputSize != 0)
			{
				TCHAR* envVariableValue = new TCHAR[outputSize];
				size_t vOutputSize = 0;
				_tgetenv_s( &vOutputSize, envVariableValue, outputSize, strEnvVariable.c_str() );
				assert(vOutputSize == outputSize);
				pValueFromVariable = new tstring(envVariableValue);
			}
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
			pValueFromVariable = new tstring(selfHome.substr(selfHome.find_last_of(_T('\\'))));
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
		else if (strJustVariable.compare(MAINCLASS_SIMPLENAME) == 0)
		{
			tstring& className = m_pProperties->getMainClass();
			pValueFromVariable = new tstring(className.substr(0, className.find_last_of(_T('/'))));
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

tstring& PropertyValueVariables::resolvePropertyVariables(tstring& strProperty)
{
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

			tstring prefixStrProperty = strProperty.substr(0,indexStartOfVariable);
			tstring postfixStrProperty = strProperty.substr(indexEndOfVariable+1);
			
			strProperty = prefixStrProperty + variableValue + postfixStrProperty;
			DEBUG_SHOW( _T("strProperty=") + strProperty);
			indexStartOfVariable = prefixStrProperty.size() + variableValue.size();
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
