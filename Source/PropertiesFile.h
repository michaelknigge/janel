/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_PROPERTIESFILE_H_INCLUDED_
#define _JANEL_PROPERTIESFILE_H_INCLUDED_


#include "Properties.h"
#include <string>
#include <vector>
#include <fstream>
#include "PropertyValueVariables.h"
#include "PropertyFileEntry.h"

class PropertiesFile  
{
private:
	tstring m_propertiesFileName;
	Properties* m_pProperties;

	void processProperty(PropertyFileEntry* pPropFileEntry);
	tstring& parsePropertyName(const tstring& strProp);
	tstring& parsePropertyValue(const tstring& strProp);
	bool isComment(const tstring& strProperty);
	bool isAssignmentProperty(const tstring& strProperty);
	void loadProps_NoCommentsBlanks(tifstream& propsFile, std::vector<PropertyFileEntry*>& propsFromFile);
	void processCustomProps_NoPropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	void processCustomProperties_WithOnlyEarlyPropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	void processNonCustomProps_NoPropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	void processNonCustomProps_WithOnlyEarlyPropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	void processCustomProps_WithLatePropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	void processNonCustomProps_WithLatePropertyValueVars(std::vector<PropertyFileEntry*>& propsFromFile, PropertyValueVariables& propertyValueVars);
	bool isLineTerminatorEscaped(const tstring& strProperty);

public:
	PropertiesFile(Properties* pProperties);
	virtual ~PropertiesFile();

	tstring& getPropertiesFileName();	
	void loadPropertiesFromFile();

	static const char LINE_CONTINUATION_CARET;

};

#endif // ifndef _JANEL_PROPERTIESFILE_H_INCLUDED_
