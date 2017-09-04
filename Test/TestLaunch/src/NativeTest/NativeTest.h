// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NATIVETEST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NATIVETEST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NATIVETEST_EXPORTS
#define NATIVETEST_API __declspec(dllexport)
#else
#define NATIVETEST_API __declspec(dllimport)
#endif

// This class is exported from the NativeTest.dll
class NATIVETEST_API CNativeTest {
public:
	CNativeTest(void);
	// TODO: add your methods here.
};

