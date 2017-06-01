#include "log.h"

WLog::WLog()
{
	charCount = 0;
	str = L"";
}

const wchar_t* WLog::getLine()
{
	if(charCount >= str.length())
		return NULL;
	int lineStart, lineEnd;
	lineStart = lineEnd = charCount;
	while(str[lineEnd] != L'\n' && lineEnd < str.length())
	{
		lineEnd++;
	}
	charCount = lineEnd + 1;
	return str.substr(lineStart,lineEnd-lineStart).c_str();
}

void WLog::start()
{
	charCount = 0;
}

void WLog::addLog(const wchar_t* wc_str)
{
	str += wc_str;
	if(str.back() != L'\n')
		str += L'\n';
}

void WLog::clear()
{
	str = L"";
	charCount = 0;
}
