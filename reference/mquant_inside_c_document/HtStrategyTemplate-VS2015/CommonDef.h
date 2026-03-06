#pragma once
#include <string>
//#include <tchar.h>
#include <windows.h>

static std::string AnsiToUtf8(const char *srcCode)//²âÊÔ
{
	WCHAR*   strA;
	int   i = MultiByteToWideChar(CP_ACP, 0, (char*)srcCode, -1, NULL, 0);
	strA = new   WCHAR[i];
	MultiByteToWideChar(CP_ACP, 0, (char   *)srcCode, -1, strA, i);

	i = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
	char   *strB = new   char[i];
	WideCharToMultiByte(CP_UTF8, 0, strA, -1, strB, i, NULL, NULL);
	//strB¼´ÎªËùÇó
	std::string strUtf8 = strB;
	delete[]strA;
	delete[]strB;
	return   strUtf8;
}

#define UTF8STR(str) AnsiToUtf8(str).c_str()