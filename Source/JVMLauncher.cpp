/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "JVMLauncher.h"
#include "Debug.h"
#include "ErrHandler.h"
#include "JVMChooser.h"
#include "Windows.h"
#include "ATLConv.h"

using namespace std;

JVMLauncher::JVMLauncher(Properties* pProperties)
{
	m_pProperties = pProperties;
}

JVMLauncher::~JVMLauncher()
{

}

static JVMLauncher* jvmLauncher;

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

	result = m_pVM->AttachCurrentThread((void**)&pJniEnvironment, NULL);
	if (result >= 0)
	{
		// I can't figure out how to get java.lang.System, exit method, so I require that main
		// class have an "initiateExit" static method that we can call which will in turn call exit.
#ifdef _UNICODE
		USES_CONVERSION;
		javaClass = pJniEnvironment->FindClass( W2A(m_pProperties->getMainClass().c_str()) );
#else
		javaClass = pJniEnvironment->FindClass( m_pProperties->getMainClass().c_str() );
#endif
		if (javaClass == 0)
		{
			if (pJniEnvironment->ExceptionOccurred())
				pJniEnvironment->ExceptionDescribe();
		}
		else
		{
			javaMethodId = pJniEnvironment->GetStaticMethodID(javaClass, "initiateExit", "(I)V");
			if (javaMethodId == 0)
			{
				if (pJniEnvironment->ExceptionOccurred())
					pJniEnvironment->ExceptionDescribe();
			}
			else
			{
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
#ifdef _UNICODE
		javaClass = pJniEnvironment->FindClass( W2A(m_pProperties->getMainClass().c_str()) );
#else
		javaClass = pJniEnvironment->FindClass( m_pProperties->getMainClass().c_str() );
#endif
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

		argsJObjArray = (jobjectArray)pJniEnvironment->NewObjectArray( argsVector.size(),
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
			pJniEnvironment->SetObjectArrayElement(argsJObjArray, i, 
				pJniEnvironment->NewString((const jchar*)argsVector.at(i).c_str(), argsVector.at(i).length()));
		}
		if(pJniEnvironment->ExceptionOccurred() != 0)
		{
			pJniEnvironment->ExceptionDescribe();
			throw tstring(_T("Error occurred while setting command line arguments for the Java class ") + m_pProperties->getMainClass());
		}

		// if trapping console
		if (m_pProperties->getTrapConsoleCtrl())
		{
			jvmLauncher = this;
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

		int numberOfSystemProperties = m_pProperties->getJavaSystemProperties().size();
		
		// if -Djava.class.path and janel.classpath.jars.dir / janel.classpath.jars.dir.recursive are set
		// if -Djava.library.path and janel.library.path.dir / janel.library.path.dir.recursive are set
		// deal with it here
		bool isJavaClasspathSysPropIncluded = false;
		bool isJavaLibrarypathSysPropIncluded = false;
		for( int j=0; j < numberOfSystemProperties; j++ )
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
		for( int j=0; j < numberOfSystemProperties; j++ )
		{
			tstring& javaSysProp(m_pProperties->getJavaSystemProperties().at(j));
			
#ifdef _UNICODE
			USES_CONVERSION;
			pJvmOptions[j].optionString = strdup(W2A(javaSysProp.c_str()));
#else
			pJvmOptions[j].optionString = (char*)javaSysProp.c_str();
#endif
			DEBUG_SHOW( tstring(_T("setupJavaVMInitArgs javaSysProp=")) + javaSysProp );
		}

		jvmInitArgs.nOptions = numberOfSystemProperties;
		jvmInitArgs.options  = pJvmOptions;
		jvmInitArgs.ignoreUnrecognized = JNI_FALSE;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in JVMLauncher.setupJavaVMInitArgs()") );
		ErrHandler::severeError( _T("Error while setting up JVM init arguments.") );
	}
}
