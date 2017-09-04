/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */


#ifndef _JANEL_ERRHANDLER_H_INCLUDED_
#define _JANEL_ERRHANDLER_H_INCLUDED_


class ErrHandler  
{
private:
	static tstring s_genericErrorMsg;
	static bool s_showErrorDetail;

	static void display(const tstring& msg);
	static void showGenericErrorMessage();
	static void showErrorMessage(const tstring& msg);

	ErrHandler();
	virtual ~ErrHandler();

public:
	static void setGenericErrorMessage(const tstring& errorText);
	static void setShowErrorDetail(const bool showErrorDetails);
	static void severeError(const tstring& msg);
	static void severeError();
};

#endif // ifndef _JANEL_ERRHANDLER_H_INCLUDED_
