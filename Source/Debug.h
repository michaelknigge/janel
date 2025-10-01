/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_DEBUG_H_INCLUDED_
#define _JANEL_DEBUG_H_INCLUDED_

// define DEBUG_APP to see debug messages
//#define DEBUG_APP
#undef DEBUG_APP

// define DEBUG_APP and DEBUG_WRITE_TO_FILE to write to file JanelDebugOutput.txt
#define DEBUG_WRITE_TO_FILE
//#undef DEBUG_WRITE_TO_FILE


#ifdef DEBUG_APP
	#define DEBUG_SHOW(x) Debug::displayDevelopmentMessage(x);
	#define DEBUG_STMT(x) x;
#else
	#define DEBUG_SHOW(x) 
	#define DEBUG_STMT(x)
#endif

#include <iostream>

class Debug  
{
public:
	Debug();
	virtual ~Debug();

	static void displayDevelopmentMessage(const tstring& msg);
	static void displayMessage(const tstring& fileName, const tstring& msg);
};

#endif // ifndef _JANEL_DEBUG_H_INCLUDED_
