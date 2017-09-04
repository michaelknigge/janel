/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#include "Janel.h"
#include "PropertiesCustom.h"
#include "Debug.h"
#include "ErrHandler.h"

using namespace std;

const tstring PropertiesCustom::MAIN_CLASS = _T("janel.main.class");
const tstring PropertiesCustom::BIN_JVM_DIR = _T("janel.bin.jvm.dir");
const tstring PropertiesCustom::JAVA_BUNDLE = _T("janel.java.bundle");
const tstring PropertiesCustom::MIN_JAVA_VERSION = _T("janel.min.java.version");
const tstring PropertiesCustom::MAX_JAVA_VERSION = _T("janel.max.java.version");
const tstring PropertiesCustom::JVM_PATH = _T("janel.jvm.path");
const tstring PropertiesCustom::JAVA_HOME_PATH = _T("janel.java.home.path");
const tstring PropertiesCustom::TRAP_CONSOLE_CTRL = _T("janel.trap.console.ctrl");
const tstring PropertiesCustom::CLASSPATH_JARS_DIR = _T("janel.classpath.jars.dir");
const tstring PropertiesCustom::CLASSPATH_JARS_DIR_RECURSIVE = _T("janel.classpath.jars.dir.recursive");
const tstring PropertiesCustom::DEBUG_FILE = _T("janel.debug.file");
const tstring PropertiesCustom::LIBRARY_PATH_DIR = _T("janel.library.path.dir");
const tstring PropertiesCustom::LIBRARY_PATH_DIR_RECURSIVE = _T("janel.library.path.dir.recursive");
const tstring PropertiesCustom::WORKING_DIR = _T("janel.working.dir");
const tstring PropertiesCustom::SERVICE_NAME = _T("janel.service.name");
const tstring PropertiesCustom::SERVICE_DISPLAY_NAME = _T("janel.service.display.name");
const tstring PropertiesCustom::SERVICE_DESCRIPTION = _T("janel.service.description");
const tstring PropertiesCustom::SERVICE_OPTION_INSTALL = _T("janel.service.option.install");
const tstring PropertiesCustom::SERVICE_OPTION_UNINSTALL = _T("janel.service.option.uninstall");
const tstring PropertiesCustom::MAIN_ARGUMENT = _T("janel.main.argument");
const tstring PropertiesCustom::SYSPROP_PROCESS_ID = _T("janel.sysprop.process.id");
const tstring PropertiesCustom::SPLASH = _T("janel.splash");
const tstring PropertiesCustom::MEMORY_CHECK_LIMITS = _T("janel.memory.check.limits");
const tstring PropertiesCustom::MAX_MEMORY_AVAILABLE_PERCENT = _T("janel.memory.max.available.percent");
const tstring PropertiesCustom::MAX_MEMORY_TOTAL_PERCENT = _T("janel.memory.max.total.percent");
const tstring PropertiesCustom::MAX_MEMORY_UPPER_LIMIT = _T("janel.memory.max.upper.limit");
const tstring PropertiesCustom::MAX_MEMORY_LOWER_LIMIT = _T("janel.memory.max.lower.limit");
const tstring PropertiesCustom::INIT_MEMORY_AVAILABLE_PERCENT = _T("janel.memory.init.available.percent");
const tstring PropertiesCustom::INIT_MEMORY_TOTAL_PERCENT = _T("janel.memory.init.total.percent");
const tstring PropertiesCustom::INIT_MEMORY_UPPER_LIMIT = _T("janel.memory.init.upper.limit");
const tstring PropertiesCustom::INIT_MEMORY_LOWER_LIMIT = _T("janel.memory.init.lower.limit");
const tstring PropertiesCustom::CUSTOM_DEFAULT_ERROR_TEXT = _T("janel.error.default.text");
const tstring PropertiesCustom::SHOW_ERROR_DETAIL = _T("janel.error.show.detail");


PropertiesCustom::PropertiesCustom()
{
}

PropertiesCustom::~PropertiesCustom()
{
}

bool PropertiesCustom::isCustomProperty(const tstring& strProperty)
{
	bool isCustom = false;
	try
	{
		if( strProperty.empty() || strProperty.size() < 5 )
		{
			isCustom = false;
		}
		else if( strProperty.substr(0,5).compare(_T("janel")) == 0 )
		{
			DEBUG_SHOW( strProperty + _T(" is custom property") );
			isCustom = true;
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in PropertiesCustom::isCustomProperty") );
		ErrHandler::severeError( _T("Error checking if a property is custom.") );
	}
	return isCustom;
}
