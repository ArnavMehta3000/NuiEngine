#include "StringConv.h"
#include "Core/Common/NuiWin.h"

namespace Nui
{
	WString StringConv::ToWide(const String& str) noexcept
	{
		int cch = MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), nullptr, 0);

		WString wideStr;
		wideStr.resize(cch);
		MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), wideStr.data(), cch);

		return wideStr;
	}

	String StringConv::ToNarrow(const WString& wstr) noexcept
	{
		int cch = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);

		String narrowStr;
		narrowStr.resize(cch);
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), &narrowStr[0], cch, nullptr, nullptr);

		return narrowStr;
	}

}