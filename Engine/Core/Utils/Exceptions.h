#pragma once
#include "Core/Common/Types.h"
#include "Core/Common/NuiWin.h"

namespace Nui
{
	inline String GetWin32ErrorString(DWORD errorCode)
	{
		char errorString[MAX_PATH];
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			errorString,
			MAX_PATH,
			NULL);

		std::string message = "Win32 Error: ";
		message += errorString;
		return message;
	}

}