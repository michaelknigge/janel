/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_LOCALUTILITIES_H_INCLUDED_
#define _JANEL_LOCALUTILITIES_H_INCLUDED_

#include <string>

class LocalUtilities  
{
public:
	LocalUtilities();
	virtual ~LocalUtilities();

	static tstring& trim(tstring& str);
	static tstring& swapChar(tstring& origString, 
					const TCHAR& origChar, const TCHAR& newChar );
	static bool fileExists(const tstring& fileName);
	static bool convertToBoolean(const tstring& text);
	static double convertStringToPercent(const tstring& s);
	static ::std::string convertWideStringToUTF8(const tstring& s);
};

#endif // ifndef _JANEL_LOCALUTILITIES_H_INCLUDED_
