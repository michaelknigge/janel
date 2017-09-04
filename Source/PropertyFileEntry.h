/* 
 * Copyright (c) 2006, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_PROPERTYFILEENTRY_H_INCLUDED_
#define _JANEL_PROPERTYFILEENTRY_H_INCLUDED_

#include <string>

class PropertyFileEntry  
{
private:
	unsigned int m_rowNumber;
	tstring m_text;
	tstring m_name;
	tstring m_value;

public:
	PropertyFileEntry();
	virtual ~PropertyFileEntry();

	void setRowNumber(unsigned int rowNumber);
	unsigned int getRowNumber();	
	void setText(const tstring& text);
	tstring& getText();	
	void setName(const tstring& name);
	tstring& getName();	
	void setValue(const tstring& value);
	tstring& getValue();	

};

#endif // ifndef _JANEL_PROPERTYFILEENTRY_H_INCLUDED_
