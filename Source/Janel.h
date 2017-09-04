/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_JANEL_H_INCLUDED_
#define _JANEL_JANEL_H_INCLUDED_
#include <sstream>
#include <TCHAR.h>

#ifdef _UNICODE
#define tstring std::wstring
#define tifstream std::wifstream
#define tofstream std::wofstream
#define tistringstream std::wistringstream
#define tstringstream ::std::wstringstream
#define tcout std::wcout
#else
#define tstring std::string
#define tifstream std::ifstream
#define tofstream std::ofstream
#define tistringstream std::istringstream
#define tstringstream ::std::stringstream
#endif

#include "Properties.h"

Properties& getProperties(int argc, TCHAR* argv[]);
void launchJavaMainMethod(Properties* pProperties);

#endif // ifndef _JANEL_JANEL_H_INCLUDED_
