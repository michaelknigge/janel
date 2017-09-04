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


int realMain(int argc, _TCHAR* argv[])
{
	try
	{
		Properties properties = getProperties(argc,argv);
		launchJavaMainMethod(&properties);
		return 0;
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in Janel.realMain()") );
		ErrHandler::severeError(se);
		return 1;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.realMain()") );
		ErrHandler::severeError();
		return 1;
	}
}


// console's or service's main function
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		Properties properties = getProperties(argc,argv);
		if(!properties.getServiceName().empty())
		{
			if(argc >= 2 && wcscmp(argv[1], properties.getServiceOptionInstall().c_str()) == 0)
			{
				// Everything after the first argument will be passed as command line arguments
				// to the Windows service...
				tstring serviceArgs;
				for(int ix = 2; ix < argc; ++ix)
				{
					serviceArgs.append(argv[ix]);
					if(ix < (argc - 1))
						serviceArgs.append(_T(" "));
				}
				DEBUG_SHOW( _T("Installing service....") );

				SC_HANDLE manager;
				if((manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
					throw tstring(_T("Error opening the Service Control Manager"));

		 		TCHAR fullPathAndName[MAX_PATH+1];
		 		GetModuleFileName(NULL, fullPathAndName, MAX_PATH);
				// The path to the executable has the be enclosed in "" if the path
				// contains spaces...
				tstring executableAndArguments;
				if(wcsstr(fullPathAndName, _T(" ")) == NULL)
				{
					executableAndArguments.append(fullPathAndName);
				}
				else
				{
					executableAndArguments.append(_T("\""));
					executableAndArguments.append(fullPathAndName);
					executableAndArguments.append(_T("\""));
				}

				if(serviceArgs.length() > 0)
				{
					executableAndArguments.append(_T(" "));
					executableAndArguments.append(serviceArgs);
				}

				SC_HANDLE service;
				service = CreateService(manager,
							properties.getServiceName().c_str(),
							properties.getServiceDisplayName().empty() ? properties.getServiceName().c_str() : properties.getServiceDisplayName().c_str(),
							SERVICE_ALL_ACCESS,
							SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
							SERVICE_AUTO_START,
							SERVICE_ERROR_NORMAL,
							executableAndArguments.c_str(),
							NULL,
							NULL,
							_T(""),
							NULL,
							NULL);

				if(service == NULL)
				{
					CloseServiceHandle(manager);
					throw tstring(_T("Error installing the service"));
				}

				if(!properties.getServiceDescription().empty())
				{
					SERVICE_DESCRIPTION desc;
					desc.lpDescription = (wchar_t *)properties.getServiceDescription().c_str();
					ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &desc);
				}

				CloseServiceHandle(service);
				CloseServiceHandle(manager);

				return 0;
			}

			if(argc == 2 && wcscmp(argv[1], properties.getServiceOptionUninstall().c_str()) == 0)
			{
				SC_HANDLE manager;
				if((manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
					throw tstring(_T("Error opening the Service Control Manager"));

				SC_HANDLE service;
				if((service = OpenService(manager, properties.getServiceName().c_str(), SERVICE_ALL_ACCESS)) == NULL)
				{
					CloseServiceHandle(manager);
					throw tstring(_T("Error opening the service"));
				}
   
				if (!DeleteService(service))
				{
					CloseServiceHandle(service);
					CloseServiceHandle(manager);
					throw tstring(_T("Error deleting the service"));
				}

				CloseServiceHandle(service);
				CloseServiceHandle(manager);
				
				return 0;
			}

			// Running as a service and no deinstallation or installation - in this case we try to
			// update the services's description (we ignore any errors here because they do not have any
			// impact on running the service....)
			if(!properties.getServiceDescription().empty())
			{
				SC_HANDLE manager;
				if((manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) != NULL)
				{
					SC_HANDLE service;
					if((service = OpenService(manager, properties.getServiceName().c_str(), SERVICE_ALL_ACCESS)) != NULL)
					{
						SERVICE_DESCRIPTION desc;
						desc.lpDescription = (wchar_t *)properties.getServiceDescription().c_str();
						
						ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &desc);
						CloseServiceHandle(service);
						CloseServiceHandle(manager);
					}
				}
			}
		}
		return realMain(argc, argv);
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in Janel.main()") );
		ErrHandler::severeError(se);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.main()") );
		ErrHandler::severeError();
	}
}

int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
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

	return realMain(argc + 1, (_TCHAR**)argv);
#else
	return realMain(__argc, __argv);
#endif
}

Properties& getProperties(int argc, TCHAR* argv[])
{
	// callerDir is static because getProperties() might be called multiple times
	// with different current working directories...
	static TCHAR callerDir[MAX_PATH+1] = { 0 };

	Properties* pProperties = NULL;
	try
	{
		pProperties = new Properties;

 		TCHAR fullPathAndName[MAX_PATH+1];
 		GetModuleFileName(NULL, fullPathAndName, MAX_PATH);
 		pProperties->setFullPathAndNameOfExe(fullPathAndName);

		if (callerDir[0] == 0x00)
		{
		   _tgetcwd(callerDir,MAX_PATH);
		}

		pProperties->setCallerDir(callerDir);

		// TODO: we should consider changing the working directory not here since
		// changing the directory has nothing to do with getting the properties...
        // The working directory should be changed just before we enter the
        // java world....
        // (separartion of concerns / single responsibility principle)
		_tchdir(pProperties->getSelfHomePath().c_str());

		pProperties->setNumberOfInitialCommandLineArgs(argc-1);
		for( int i=1; i < argc; i++ )
		{
			pProperties->addCommandLineArgument(argv[i], i-1);
		}

		pProperties->loadProperties();
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in Janel.getProperties()") );
		ErrHandler::severeError(se);
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
#ifdef _WINDOWS
		if(!pProperties->getServiceName().empty())
		{
			DEBUG_SHOW( _T("Use the JanelConsole32.exe or JanelConsole64.exe for Windows services.") );
			throw tstring(_T("Use the JanelConsole32.exe or JanelConsole64.exe for Windows services."));
		}
#endif
		JVMLauncher jvmLauncher(pProperties);
		if(pProperties->getServiceName().empty())
			jvmLauncher.launch();
		else
			jvmLauncher.launchService();
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in Janel.launchJavaMainMethod()") );
		ErrHandler::severeError(se);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in Janel.launchJavaMainMethod()") );
		ErrHandler::severeError();
	}
}



