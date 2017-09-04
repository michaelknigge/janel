/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include <iostream>
#include "ErrHandler.h"
#include "Windows.h"

using namespace std;

extern SERVICE_STATUS_HANDLE statusHandle;

tstring ErrHandler::s_genericErrorMsg = _T("Error in Java launcher.");
bool ErrHandler::s_showErrorDetail = true;

ErrHandler::ErrHandler()
{

}

ErrHandler::~ErrHandler()
{

}

void ErrHandler::setGenericErrorMessage(const tstring& errorText)
{
	s_genericErrorMsg = errorText;
}

void ErrHandler::setShowErrorDetail(const bool showErrorDetails)
{
	s_showErrorDetail = showErrorDetails;
}

void ErrHandler::display(const tstring& msg)
{
#ifdef _CONSOLE
	// TODO: When running as a service all error messages currently disappear because a service
	// is not allowed to interact with the desktop and/or has no console.... :-( So we should
	// have an error log file (specified by a janel property) or we should log to the
	// microsoft windows event log....
	tcout << msg.c_str() << endl;
#endif

#ifdef _WINDOWS
	TCHAR name[MAX_PATH+1];
	GetModuleFileName(NULL, name, MAX_PATH);
    MessageBox( NULL,
		msg.c_str(),
			name, 
			MB_OK | MB_ICONEXCLAMATION );
    
#endif
}

void ErrHandler::showGenericErrorMessage()
{
	display( s_genericErrorMsg );
}

void ErrHandler::showErrorMessage(const tstring& msg)
{
	display( msg );
}

void ErrHandler::severeError(const tstring& msg)
{
	if (s_showErrorDetail) {
		tstring errString;
		errString.append(s_genericErrorMsg);
		errString.append(_T(" "));
		errString.append(msg);
		showErrorMessage( errString );
	} else {
		showGenericErrorMessage();
	}
	exit(1);
}

void ErrHandler::severeError()
{
	showGenericErrorMessage();
	exit(1);
}

