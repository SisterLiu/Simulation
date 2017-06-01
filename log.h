#pragma once
#include <cstring>
#include <string>

typedef class WLog
{
	public:
	const wchar_t* getLine();
	void start();
	void addLog(const wchar_t*);
	void clear();
	WLog();
	private:
	int charCount;
	std::wstring str;
} WLOG, *PWLOG;
