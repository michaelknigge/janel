/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

// Janel.cpp : Defines the entry point for Janel

#include "Janel.h"
#include "Properties.h"
#include "JVMLauncher.h"
#include "Debug.h"
#include "ErrHandler.h"
#include <windows.h>
#include <direct.h>

using namespace std;

// console's main function
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{		
		Properties properties = getProperties(argc,argv);
		launchJavaMainMethod(&properties);

		return 0;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.main()") );
		ErrHandler::severeError();
	}
	return 1;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
#ifdef _UNICODE
	int argc;
	LPWSTR* tempArgv = NULL;
	LPWSTR* argv = NULL;
	tstring strLpCmdLine = lpCmdLine;
	if(strLpCmdLine.empty())
	{
		argc = 0;
		argv = new LPWSTR[1];
	}
	else
	{
		tempArgv = CommandLineToArgvW(lpCmdLine, &argc);
		argv = new LPWSTR[argc + 1];
	}
		
	WCHAR fullPathAndName[MAX_PATH+1];
	GetModuleFileName(NULL, fullPathAndName, MAX_PATH);
	argv[0] = fullPathAndName;

	for (int i = 0; i < argc; i++)
	{
		argv[i + 1] = tempArgv[i];
	}

	return _tmain(argc + 1, (_TCHAR**)argv);
#else
	return _tmain(__argc, __argv);
#endif
}

Properties& getProperties(int argc, TCHAR* argv[])
{
	Properties* pProperties = NULL;
	try
	{
		pProperties = new Properties;

 		TCHAR fullPathAndName[MAX_PATH+1];
 		GetModuleFileName(NULL, fullPathAndName, MAX_PATH);
 		pProperties->setFullPathAndNameOfExe(fullPathAndName);

 		TCHAR callerDir[MAX_PATH+1];
		_tgetcwd(callerDir,MAX_PATH);
 		pProperties->setCallerDir(callerDir);

		_tchdir(pProperties->getSelfHomePath().c_str());

		pProperties->setNumberOfInitialCommandLineArgs(argc-1);
		for( int i=1; i < argc; i++ )
		{
			pProperties->addCommandLineArgument(argv[i], i-1);
		}

		pProperties->loadProperties();
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.getProperties()") );
		ErrHandler::severeError();
	}
	return *pProperties;
}

void launchJavaMainMethod(Properties* pProperties)
{
	try
	{
		JVMLauncher jvmLauncher(pProperties);
		jvmLauncher.launch();
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.launchJavaMainMethod()") );
		ErrHandler::severeError();
	}
}



