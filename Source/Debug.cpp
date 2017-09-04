/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "Debug.h"
#include <iostream>
#include <fstream>

using namespace std;

Debug::Debug()
{	
}

Debug::~Debug()
{	
}

void Debug::displayDevelopmentMessage(const tstring& msg)
{

#if defined(DEBUG_WRITE_TO_FILE)
    tofstream debugFile;
    
    debugFile.open( _T("JanelDebugOutput.txt"),
                    tofstream::out | tofstream::app );
    debugFile << msg.c_str();
    debugFile << endl;
    debugFile.close();
    
#elif defined(_CONSOLE)
	tcout << msg << endl;
#elif defined(_WINDOWS)
	    MessageBox( NULL,
                msg.c_str(),
                "",
                MB_OK | MB_ICONEXCLAMATION );
#endif
}

void Debug::displayMessage(const tstring& fileName, const tstring& msg)
{
    tofstream messageFile;

    messageFile.open( fileName.c_str(),
                      tofstream::out | tofstream::app );
	messageFile << msg.c_str();
    messageFile << endl;
	messageFile.close();    
}

