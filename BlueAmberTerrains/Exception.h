#pragma once


namespace BlueAmberTerrainsApp
{
	class Exception
	{
	public:
		// Exception constructor: constant string version.
		Exception(LPCTSTR msg, LPCSTR fileName, int lineNumber);

		// Exception constructor: formatted string version.
		Exception(LPCSTR fileName, int lineNumber, LPCTSTR fmtStr, ...);

		virtual ~Exception();

		// Get exception message.
		const wchar_t* message() const;

	private:
		std::string m_fileName;
		int m_lineNumber;
		std::wstring m_message;
	};

#define EXCEPTION(msg)		Exception(msg, __FILE__, __LINE__)

#define EXCEPTION_FMT(fmtStr, ...)		Exception(__FILE__, __LINE__, fmtStr, ##__VA_ARGS__)
}
