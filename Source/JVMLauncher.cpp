/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#define NOMINMAX
#include <Windows.h>
#include <ATLConv.h>
#include "Janel.h"
#include "JVMLauncher.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "JVMChooser.h"
#include <cassert>
#include <limits>
#include "LocalUtilities.h"

using namespace std;

// TODO: Due to these global variables the class JVMLauncher should/can be
// implemented as a singleton - with a factory method "create()" and
// a getter "getInstance()" for the instance....
static JVMLauncher* jvmLauncher;
static SERVICE_STATUS_HANDLE statusHandle = NULL;
static SERVICE_STATUS serviceStatus;

JVMLauncher::JVMLauncher(Properties* pProperties)
{
	m_pProperties = pProperties;
	jvmLauncher = this;
}

JVMLauncher::~JVMLauncher()
{

}

Properties* JVMLauncher::getProperties()
{
	return m_pProperties;
}

void JVMLauncher::teardownJavaVMInitArgs( JavaVMInitArgs& jvmInitArgs )
{
#ifdef _UNICODE
	jsize count = jvmInitArgs.nOptions;
	for (jsize index = 0; index < count; index++) {
		delete [] jvmInitArgs.options[index].optionString;
	}
#endif

}

// Ctrl handler trap is to give service semantics
BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		jvmLauncher->callExit();
		break;
	case CTRL_LOGOFF_EVENT:
		// ignore, we're a service
		break;
	default:
		return FALSE;  // what is this one we don't understand?
	}
	return TRUE;  // we process all events we know about
}

void JVMLauncher::callExit()
{
	jclass javaClass;
	jmethodID javaMethodId;
	JNIEnv* pJniEnvironment;
	jint result;

	DEBUG_SHOW( _T("callExit invoked!"));

	result = m_pVM->AttachCurrentThread((void**)&pJniEnvironment, NULL);
	if (result >= 0)
	{
		DEBUG_SHOW(_T("Attached to current thread"));
		// I can't figure out how to get java.lang.System, exit method, so I require that main
		// class have an "initiateExit" static method that we can call which will in turn call exit.
#ifdef _UNICODE
		DEBUG_SHOW( _T("Getting Java class to call initiateExit on. UNICODE") );
		tstring mainClassString = m_pProperties->getMainClass();
		std::string output = LocalUtilities::convertWideStringToUTF8(mainClassString);
		size_t outSize = output.size();
		size_t buffLength = outSize + 1;
		char* buff = new char[buffLength];
		buff[outSize] = 0;
		::strncpy_s(buff, buffLength, output.c_str(), outSize);
		javaClass = pJniEnvironment->FindClass( buff );
		delete[] buff;
#else
		DEBUG_SHOW(_T("Getting Java class to call initiateExit on. NON-UNICODE"));
		javaClass = pJniEnvironment->FindClass( m_pProperties->getMainClass().c_str() );
#endif
		if (javaClass == 0)
		{
			DEBUG_SHOW(_T("Java class not found for call to initiateExit."));
			if (pJniEnvironment->ExceptionOccurred())
				pJniEnvironment->ExceptionDescribe();
		}
		else
		{
			javaMethodId = pJniEnvironment->GetStaticMethodID(javaClass, "initiateExit", "(I)V");
			if (javaMethodId == 0)
			{
				DEBUG_SHOW(_T("initiateExit method not found."));
				// TODO: We should call System.exit() here to shut down - in case of a windows service
				// if there is no initiateExit() Method -> the service would never end....
				if (pJniEnvironment->ExceptionOccurred())
					pJniEnvironment->ExceptionDescribe();
			}
			else
			{
				DEBUG_SHOW(_T("Calling initiateExit from JVMLauncher"));
				pJniEnvironment->CallStaticVoidMethod(javaClass, javaMethodId, 0);
			}
		}
		m_pVM->DetachCurrentThread();
	}
}

typedef int (*SplashLoadMemory_t)(void* pdata, int size);
typedef int (*SplashLoadFile_t)(const char* filename);
typedef void (*SplashInit_t)(void);
typedef void (*SplashClose_t)(void);
typedef void (*SplashSetFileJarName_t)(const char* fileName, 
                                       const char* jarName);

VOID WINAPI serviceControlHandler(DWORD controlCode)
{
	DEBUG_SHOW( _T("Entered serviceControlHandler()") );
	static DWORD dwCheckPoint = 1;

	if(statusHandle == NULL) // should never-ever happen...
		return;

	if(controlCode == SERVICE_CONTROL_SHUTDOWN || controlCode == SERVICE_CONTROL_STOP)
	{
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		serviceStatus.dwControlsAccepted  = 0;
	}
	else
	{
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	}
  
	if (serviceStatus.dwCurrentState == SERVICE_RUNNING || serviceStatus.dwCurrentState == SERVICE_STOPPED)
		serviceStatus.dwCheckPoint = 0;
	else
		serviceStatus.dwCheckPoint = dwCheckPoint++;

	SetServiceStatus(statusHandle, &serviceStatus);

	if(serviceStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		DEBUG_SHOW( _T("Stopping service...") );
		jvmLauncher->callExit();
	}

	return;
}

VOID WINAPI serviceMain(unsigned long argc, _TCHAR *argv[])
{
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = NO_ERROR;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 60 * 1000;

	Properties* pProperties = jvmLauncher->getProperties();
	if((statusHandle = RegisterServiceCtrlHandler(pProperties->getServiceName().c_str(), serviceControlHandler)) == NULL)
	{
		DEBUG_SHOW( _T("RegisterServiceCtrlHandler() failed") );
		return;
	}
	
	SetServiceStatus(statusHandle, &serviceStatus);

	jvmLauncher->launch();
  
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(statusHandle, &serviceStatus);
}


void JVMLauncher::launchService()
{
	// when running as a service we don't want to "see" any annoying error message box...
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	SERVICE_TABLE_ENTRY entry[2];

	entry[0].lpServiceName = (wchar_t *) m_pProperties->getServiceName().c_str();
	entry[0].lpServiceProc = serviceMain;

	entry[1].lpServiceName = NULL;
	entry[1].lpServiceProc = NULL;

	DEBUG_SHOW( _T("Start Service Control Dispatcher..."));
	if(StartServiceCtrlDispatcher(entry) == 0)
	{
		if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
			throw tstring(_T("This program is a Windows service and can not be run as a regular application."));
		else
		throw tstring(_T("Error starting the service control dispatcher."));
	}
}

void JVMLauncher::launch()
{
	try
	{
		// change to the desired diretory
		tstring workingDir = m_pProperties->getWorkingDir();
		if (!workingDir.empty() && _tchdir(workingDir.c_str()) != 0)
		{
			throw tstring(_T("Could not change directory to ") + workingDir);
		}

		JVMInfo *pBestJvmInfo = m_pProperties->getBestJvmInfo();

		if( m_pProperties->getDebugFile().size() > 0 )
		{
			Debug::displayMessage(m_pProperties->getDebugFile(), tstring(_T("The chosen JVM: ")) + pBestJvmInfo->toString()); 
			Debug::displayMessage(m_pProperties->getDebugFile(), tstring(_T("The properties chosen: ")) + m_pProperties->toString()); 
		}

		DEBUG_SHOW( tstring(_T("pBestJvmInfo->toString()=")) + pBestJvmInfo->toString() );

		JNIEnv* pJniEnvironment;
		JavaVM* pJvm;
		jint result;
		jclass javaClass;
		jmethodID javaMethodId;
		jobjectArray argsJObjArray;

		DEBUG_SHOW( tstring(_T("m_pProperties->toString()=")) + m_pProperties->toString() );

#ifdef _UNICODE
		USES_CONVERSION;
#endif

		// If the check for memory limits flag is set, a dummy window is created 
		// to trigger loading of windowing DLLs.
		// The dummy window is closed right before the java main method gets called.
		// Closing it here prevents starting of the splash screen.
		HWND dummyWnd = NULL;
		if (m_pProperties->isMemoryCheckLimits())
		{
			dummyWnd = CreateWindowEx(WS_EX_TOOLWINDOW, _T("EDIT"), _T("JanelDummy"), WS_DISABLED| WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
		}

		// TODO: pre-loading the CRT shoud be a little bit more intelligent. We should iterate over all existing
		// msvcr*.dll files and load the one with the highest number.... Well... there should be only one
		// present because it makes no sense to use/ship different versions of the CRT....

		// Trying to pre-load the CRT from the desired JAVA-Directory...
		tstring crtpath = m_pProperties->getBestJvmInfo()->getJavaHomePath() + tstring(_T("\\bin\\msvcr71.dll"));
		DEBUG_SHOW(tstring(_T("Trying to load msvcr71.dll (for Java 1.6)...")));
		HMODULE crt = LoadLibrary(crtpath.c_str());
		if (crt)
		{
			DEBUG_SHOW(tstring(_T("msvcr71.dll loaded")));
		}
		else
		{
			crtpath = m_pProperties->getBestJvmInfo()->getJavaHomePath() + tstring(_T("\\bin\\msvcr100.dll"));
			DEBUG_SHOW(tstring(_T("Trying to load msvcr100.dll (for Java 1.7)...")));
			crt = LoadLibrary(crtpath.c_str());
			if(crt)
			{
				DEBUG_SHOW(tstring(_T("msvcr100.dll loaded")));
			}
		}

		// Show the splash screen
		tstring splash = m_pProperties->getSplash();
		if (!splash.empty())
		{
			DEBUG_SHOW(tstring(_T("Showing splash screen ")) + splash);
			tstring sspath = m_pProperties->getBestJvmInfo()->getJavaHomePath() + tstring(_T("\\bin\\splashscreen.dll"));

			DEBUG_SHOW(tstring(_T("Loading splashscreen.dll from ")) + sspath);
			HMODULE hSplashLibrary = LoadLibrary(sspath.c_str());

			if (hSplashLibrary)
			{
				SplashInit_t splashInit = (SplashInit_t)GetProcAddress(hSplashLibrary, "SplashInit");
				splashInit();

				SplashLoadFile_t splashLoadFile = (SplashLoadFile_t)GetProcAddress(hSplashLibrary, "SplashLoadFile");
				splashLoadFile(W2A(splash.c_str()));

				SplashSetFileJarName_t splashSetFileJarName = (SplashSetFileJarName_t)GetProcAddress(hSplashLibrary, "SplashSetFileJarName");
				splashSetFileJarName(W2A(splash.c_str()), NULL);
			}
			else
				DEBUG_SHOW(tstring(_T("Failed to load splashscreen.dll")));
		}

		// create JVM
		HINSTANCE hJvm = NULL;
		const TCHAR* pJvmPath = pBestJvmInfo->getJvmPath().c_str();
		DEBUG_SHOW( tstring(_T("pJvmPath()=")) + tstring(pJvmPath) );
		hJvm = LoadLibrary(pJvmPath);

		typedef jint (JNICALL CreateJavaVM_t)( JavaVM**, void**, void* );
		CreateJavaVM_t* CreateJavaVM;
		CreateJavaVM = (CreateJavaVM_t*)GetProcAddress( hJvm, "JNI_CreateJavaVM" );
		DEBUG_SHOW(_T("Just about to CreateJavaVM"));

		// Evaluation of memory settings considers the current state of the stack.
		// It finds the largest free memory region and limits the size of the object
		// heap to this size plus some overhead for the VM itsself. In cases when
		// the process loads additional DLLs prior to starting the VM, e.g. on machines with
		// IME (Input Method Editor - japanese...) this leads to a not enough heap space error.
		// Therefore we need to evaluate memory settings as late as possible.
		m_pProperties->evaluateMemorySettings();

		// setup options
		JavaVMInitArgs jvmInitArgs;
		setupJavaVMInitArgs(jvmInitArgs);

		result = CreateJavaVM(&pJvm,(void**)&pJniEnvironment,&jvmInitArgs);

		teardownJavaVMInitArgs(jvmInitArgs);

		if (result < 0)
		{
			throw tstring(_T("Cannot create Java VM."));
		}
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occurred while creating Java VM."));
		}
		m_pVM = pJvm;		
		
		// find main class
        tstring& mainClass = m_pProperties->getMainClass();
        
#ifdef _UNICODE
        ::std::string utf8MainClass = LocalUtilities::convertWideStringToUTF8(mainClass);
#else
        ::std::string utf8MainClass = m_pProperties->getMainClass();
#endif
        javaClass = pJniEnvironment->FindClass( utf8MainClass.c_str() );
        
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occurred while attempting to find Java class ") + m_pProperties->getMainClass());
		}
		if (javaClass == 0)
		{
			throw tstring( _T("Cannot find main Java class ") + (m_pProperties->getMainClass()) );
		}

		javaMethodId = pJniEnvironment->GetStaticMethodID(javaClass, "main", "([Ljava/lang/String;)V");
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occurred while getting main method in Java class ") + m_pProperties->getMainClass());
		}
		if (javaMethodId == 0)
		{
			throw tstring(_T("Cannot find main method in Java class."));
		}

		// set command-line arguments to pass to main method
		vector<tstring>& argsVector = m_pProperties->getCommandLineArguments();

		size_t vecSize = argsVector.size();
		assert(vecSize < ((size_t) std::numeric_limits<jsize>::max()));
		argsJObjArray = (jobjectArray)pJniEnvironment->NewObjectArray( (jsize) vecSize,
							pJniEnvironment->FindClass("java/lang/String"),
							pJniEnvironment->NewStringUTF(""));
		if (argsJObjArray == 0)
		{
			throw tstring(_T("Error creating new object array to hold command line arguments."));
		}

		for(unsigned int i=0; i < argsVector.size(); i++)
		{
			DEBUG_STMT( TCHAR debugstr[5000]; );
			DEBUG_STMT( _stprintf(debugstr, _T("command line arg[%u]=%s"), i, argsVector.at(i).c_str()); );
			DEBUG_SHOW( debugstr );

			size_t argSize = argsVector.at(i).length();
			assert(argSize < ((size_t) std::numeric_limits<jsize>::max()));
			pJniEnvironment->SetObjectArrayElement(argsJObjArray, i, 
				pJniEnvironment->NewString((const jchar*)argsVector.at(i).c_str(), (jsize) argSize));
		}
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occurred while setting command line arguments for the Java class ") + m_pProperties->getMainClass());
		}

		// if trapping console
		if (m_pProperties->getTrapConsoleCtrl())
		{
			if (!::SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE))
			{
				throw tstring(_T("Unable to catch console ctrl events."));
			}
		}

		// Delete dummy window again. This must be done this late, since otherwise the splashscreen
		// somehow reacts on the window event and closes.
		if (dummyWnd != NULL)
		{
			DestroyWindow(dummyWnd);
		}

		// call main method in Java class passing arguments
		pJniEnvironment->CallStaticVoidMethod(javaClass, javaMethodId, argsJObjArray);
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occured while calling main method in Java class ") + m_pProperties->getMainClass());
		}

		pJvm->DestroyJavaVM();
		::FreeLibrary(hJvm);
	
		::SetConsoleCtrlHandler(ConsoleCtrlHandler, FALSE);
	}
	catch(tstring& se)
	{
		ErrHandler::severeError( se );
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMLauncher.launch()") );
		ErrHandler::severeError( _T("Error while launching program with JVM.") );
	}
}

void JVMLauncher::setupJavaVMInitArgs(JavaVMInitArgs& jvmInitArgs)
{
	try
	{
		memset(&jvmInitArgs, 0, sizeof(jvmInitArgs));
		jvmInitArgs.version  = JNI_VERSION_1_2;

		size_t numberOfSystemProperties = m_pProperties->getJavaSystemProperties().size();
		
		// if -Djava.class.path and janel.classpath.jars.dir / janel.classpath.jars.dir.recursive are set
		// if -Djava.library.path and janel.library.path.dir / janel.library.path.dir.recursive are set
		// deal with it here
		bool isJavaClasspathSysPropIncluded = false;
		bool isJavaLibrarypathSysPropIncluded = false;
		for( size_t j=0; j < numberOfSystemProperties; j++ )
		{
			tstring& javaSysProp(m_pProperties->getJavaSystemProperties().at(j));
			if( m_pProperties->isClasspathSystemProperty(javaSysProp) == true )
			{
				isJavaClasspathSysPropIncluded = true;
				m_pProperties->getJavaSystemProperties().at(j) = javaSysProp + (m_pProperties->getJarsInClasspath());
			}
			else if( m_pProperties->isLibrarypathSystemProperty(javaSysProp) == true )
			{
				isJavaLibrarypathSysPropIncluded = true;
				m_pProperties->getJavaSystemProperties().at(j) = javaSysProp + (m_pProperties->getLibraryPath());
			}
		}

		if( (isJavaClasspathSysPropIncluded == false) && !(m_pProperties->getJarsInClasspath().empty()) )
		{
			numberOfSystemProperties++;
			tstring tempString = tstring(_T("-Djava.class.path=")) + (m_pProperties->getJarsInClasspath());
			tstring& javaSysProp = tempString;
			m_pProperties->addJavaSystemProperty(javaSysProp);
		}

		if( (isJavaLibrarypathSysPropIncluded == false) && !(m_pProperties->getLibraryPath().empty()) )
		{
			numberOfSystemProperties++;
			tstring tempString = tstring(_T("-Djava.library.path=")) + (m_pProperties->getLibraryPath());
			tstring& javaSysProp = tempString;
			m_pProperties->addJavaSystemProperty(javaSysProp);
		}

		JavaVMOption* pJvmOptions = new JavaVMOption[numberOfSystemProperties];
		for( size_t j=0; j < numberOfSystemProperties; j++ )
		{
			tstring& javaSysProp(m_pProperties->getJavaSystemProperties().at(j));
			
#ifdef _UNICODE
			const TCHAR* inBuff = javaSysProp.c_str();
			
			std::string output = LocalUtilities::convertWideStringToUTF8(javaSysProp);
			size_t outStrLen = output.size();
            size_t buffLength = outStrLen + 1;
			char* buff = new char[buffLength];
			buff[outStrLen] = 0;

            ::strncpy_s(buff, buffLength, output.c_str(), outStrLen);
			
			pJvmOptions[j].optionString = buff;
#else
			pJvmOptions[j].optionString = (char*)javaSysProp.c_str();
#endif
            pJvmOptions[j].extraInfo = nullptr;
			DEBUG_SHOW( tstring(_T("setupJavaVMInitArgs javaSysProp=")) + javaSysProp );
		}

		assert(numberOfSystemProperties < ((size_t) std::numeric_limits<jsize>::max()));
		jvmInitArgs.nOptions = (jsize) numberOfSystemProperties;
		jvmInitArgs.options  = pJvmOptions;
		jvmInitArgs.ignoreUnrecognized = JNI_FALSE;
	}
	catch(tstring& se)
	{
		DEBUG_SHOW( _T("Exception in JVMLauncher.setupJavaVMInitArgs()") );
		ErrHandler::severeError(se);
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMLauncher.setupJavaVMInitArgs()") );
		ErrHandler::severeError( _T("Error while setting up JVM init arguments.") );
	}
}
