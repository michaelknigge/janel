/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#ifndef _JANEL_PROPERTIESCUSTOM_H_INCLUDED_
#define _JANEL_PROPERTIESCUSTOM_H_INCLUDED_

#include <string>

class PropertiesCustom  
{
public:
	static const tstring MAIN_CLASS;
	static const tstring BIN_JVM_DIR;
	static const tstring JAVA_BUNDLE;
	static const tstring MIN_JAVA_VERSION;
	static const tstring MAX_JAVA_VERSION;
	static const tstring JVM_PATH;
	static const tstring JAVA_HOME_PATH;
	static const tstring TRAP_CONSOLE_CTRL;
	static const tstring CLASSPATH_JARS_DIR;
	static const tstring CLASSPATH_JARS_DIR_RECURSIVE;
	static const tstring DEBUG_FILE;
	static const tstring LIBRARY_PATH_DIR;
	static const tstring LIBRARY_PATH_DIR_RECURSIVE;
	static const tstring WORKING_DIR;
	static const tstring SERVICE_NAME;
	static const tstring SERVICE_DISPLAY_NAME;
	static const tstring SERVICE_DESCRIPTION;
	static const tstring SERVICE_OPTION_INSTALL;
	static const tstring SERVICE_OPTION_UNINSTALL;
	static const tstring MAIN_ARGUMENT;
	static const tstring SYSPROP_PROCESS_ID;
	static const tstring SPLASH;
	static const tstring MEMORY_CHECK_LIMITS;
	static const tstring MAX_MEMORY_AVAILABLE_PERCENT;
	static const tstring MAX_MEMORY_TOTAL_PERCENT;
	static const tstring MAX_MEMORY_UPPER_LIMIT;
	static const tstring MAX_MEMORY_LOWER_LIMIT;
	static const tstring INIT_MEMORY_AVAILABLE_PERCENT;
	static const tstring INIT_MEMORY_TOTAL_PERCENT;
	static const tstring INIT_MEMORY_UPPER_LIMIT;
	static const tstring INIT_MEMORY_LOWER_LIMIT;
	static const tstring CUSTOM_DEFAULT_ERROR_TEXT;
	static const tstring SHOW_ERROR_DETAIL;


	PropertiesCustom();
	virtual ~PropertiesCustom();

	static bool isCustomProperty(const tstring& strProperty);
};

#endif // ifndef _JANEL_PROPERTIESCUSTOM_H_INCLUDED_
