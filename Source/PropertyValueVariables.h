/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_PROPERTYVALUEVARIABLES_H_INCLUDED_
#define _JANEL_PROPERTYVALUEVARIABLES_H_INCLUDED_

#include "Properties.h"
#include <string>

class PropertyValueVariables  
{
private:
	Properties* m_pProperties;
public:
	static const tstring FOUND_JAVA_HOME;
	static const tstring SELF_HOME;
	static const tstring FOUND_EXE_FOLDER;
	static const tstring CALLER_DIR;

	PropertyValueVariables(Properties* pProperties);
	virtual ~PropertyValueVariables();

	tstring& resolvePropertyVariables(tstring& strProperty);
	tstring& getValueFromVariable(const tstring& strVariable);
	bool containsPropertyVariables(const tstring& strProperty);
	bool containsLatePropertyVariables(const tstring& strProperty);
};

#endif // ifndef _JANEL_PROPERTYVALUEVARIABLES_H_INCLUDED_
