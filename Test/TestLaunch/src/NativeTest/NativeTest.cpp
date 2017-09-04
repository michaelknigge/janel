// NativeTest.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "NativeTest.h"
#include "TestLaunch015.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// This is the constructor of a class that has been exported.
// see NativeTest.h for the class definition
CNativeTest::CNativeTest()
{ 
	return; 
}

JNIEXPORT jstring JNICALL Java_testpackage_TestLaunch015_nativeTestMethod
  (JNIEnv *env, jobject pObj, jstring pParameter1)
{
	std::string strResult("FAILURE");

	const char *pszParameter1 = env->GetStringUTFChars(pParameter1,NULL);
	if( strcmp(pszParameter1,"TEST") == 0 )
	{
		strResult = "SUCCESS";
	}

	jstring pJstrResult = env->NewStringUTF(strResult.c_str());
	return pJstrResult;
}

