/* 
 * Copyright (c) 2006, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "PropertyFileEntry.h"
#include "Debug.h"
#include "ErrHandler.h"

using namespace std;

PropertyFileEntry::PropertyFileEntry()
{
}

PropertyFileEntry::~PropertyFileEntry()
{
}

tstring& PropertyFileEntry::getText()
{
	return m_text;
}

void PropertyFileEntry::setText(const tstring& text)
{
	m_text = text;
}

unsigned int PropertyFileEntry::getRowNumber()
{
	return m_rowNumber;
}

void PropertyFileEntry::setRowNumber(unsigned int rowNumber)
{
	m_rowNumber = rowNumber;
}

tstring& PropertyFileEntry::getName()
{
	return m_name;
}

void PropertyFileEntry::setName(const tstring& name)
{
	m_name = name;
}

tstring& PropertyFileEntry::getValue()
{
	return m_value;
}

void PropertyFileEntry::setValue(const tstring& value)
{
	m_value = value;
}
