/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_JVMLAUNCHER_H_INCLUDED_
#define _JANEL_JVMLAUNCHER_H_INCLUDED_


#include "Properties.h"
#include <jni.h>

class JVMLauncher  
{
private:
	Properties* m_pProperties;
	JavaVM* m_pVM;
	void setupJavaVMInitArgs(JavaVMInitArgs& jvmInitArgs);
public:
	JVMLauncher(Properties* pProperties);
	virtual ~JVMLauncher();
	void launch();
	void launchService();
	void callExit();
	Properties* getProperties();
	void teardownJavaVMInitArgs( JavaVMInitArgs& jvmInitArgs );
};

#endif // ifndef _JANEL_JVMLAUNCHER_H_INCLUDED_
