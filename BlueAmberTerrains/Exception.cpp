#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Exception::Exception(LPCTSTR msg, LPCSTR fileName, int lineNumber)
	: m_fileName(fileName), m_lineNumber(lineNumber), m_message(msg)
{
}

Exception::Exception(LPCSTR fileName, int lineNumber, LPCTSTR fmtStr, ...)
	: m_fileName(fileName), m_lineNumber(lineNumber)
{
	va_list pArgs = nullptr;

	va_start(pArgs, fmtStr);

	int ccLen = ::_vscwprintf(fmtStr, pArgs);
	assert(ccLen >= 0);

	std::vector<wchar_t> buff(ccLen + 1);

	int ccWritten = ::vswprintf_s(&buff[0], buff.size(), fmtStr, pArgs);
	assert(ccWritten == ccLen);

	va_end(pArgs);

	if (ccWritten > 0)
	{
		m_message.assign(buff.cbegin(), buff.cend());
	}
}

Exception::~Exception()
{
}

const wchar_t* Exception::message() const
{
	return m_message.c_str();
}
