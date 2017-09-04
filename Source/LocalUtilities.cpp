/* 
 * Copyright (c) 2003, Timothy D. Kil
 * Please see the JanelLicense.txt file for the complete license.
 */

#define NOMINMAX
#include "Janel.h"
//#include <stdio.h>
#include <io.h>
#include "LocalUtilities.h"
#include "Debug.h"
#include "ErrHandler.h"
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>

using namespace std;

LocalUtilities::LocalUtilities()
{

}

LocalUtilities::~LocalUtilities()
{

}

tstring& LocalUtilities::trim(tstring& str)
{
	try
	{
		// trim leading whitespace
		tstring::size_type  indexNonWhiteSpace = str.find_first_not_of(_T(" \t\n"));
		if( indexNonWhiteSpace > 0 )
		{
			str.erase(0,indexNonWhiteSpace);
		}

		// trim trailing whitespace
		indexNonWhiteSpace = str.find_last_not_of(_T(" \t\n")); 
		if( indexNonWhiteSpace > 0 )
		{
			str.erase(indexNonWhiteSpace+1); 		
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in LocalUtilites.trim()") );
		throw;
	}
	return str;
}

tstring& LocalUtilities::swapChar(tstring& origString, 
									const TCHAR& origChar, 
									const TCHAR& newChar )
{
	try
	{
		tstring::size_type  foundIndex = 0;
		while( foundIndex != -1 )
		{
			foundIndex = origString.find_first_of(origChar,foundIndex);
			if( foundIndex != -1 )
			{
				origString[foundIndex] = newChar;
			}
		}
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in LocalUtilites.swapChar()") );
		throw;
	}
	return origString;
}

bool LocalUtilities::fileExists(const tstring& fileName)
{
	bool exists = false;
	try
	{
		_tfinddata_t *pfileDataFound = new _tfinddata_t;
		intptr_t hFileFound;
		hFileFound = _tfindfirst(fileName.c_str(), pfileDataFound);
		if(hFileFound == -1L)
		{
			exists = false;
		}
		else
		{
			exists = true;
		}

		delete pfileDataFound;
	}
	catch(...)
	{
		DEBUG_SHOW( _T("Exception in LocalUtilites.fileExists()") );
		throw;
	}
	return exists;
}

bool LocalUtilities::convertToBoolean(const tstring& text)
{
	tstring tmpText = text;
	trim(tmpText);
	TCHAR firstChar = tmpText[0];
	if( firstChar == _T('Y') ||
		firstChar == _T('y') ||
		firstChar == _T('t') ||
		firstChar == _T('T') )
	{
		return true;
	}

	return false;
}
 
double LocalUtilities::convertStringToPercent(const tstring& s)
{
	tistringstream i(s);
	double x;
	if (!(i >> x))
    {
        // It might make sense to throw an exception here, but we'll just
        // log it and move on for now.
        // FIXME - put a note in the log.
		x = -1.0;
    }
	return x;
 }

::std::string LocalUtilities::convertWideStringToUTF8(const tstring& s)
{
	const TCHAR* inBuff = s.c_str();
	size_t inLength = s.length();
	assert(inLength < ((size_t) std::numeric_limits<int>::max()));
	size_t outSize = WideCharToMultiByte(CP_UTF8, 0, inBuff, inLength, NULL, 0, NULL, NULL);
	char* buff = new char[outSize + 1];
	assert(outSize < ((size_t) std::numeric_limits<int>::max()));
	size_t actualSize = WideCharToMultiByte(CP_UTF8, 0, inBuff, inLength, buff, outSize, NULL, NULL);
	buff[actualSize] = 0;
	assert(actualSize == outSize);
	::std::string output(buff);
	delete [] buff;
	return output;
}