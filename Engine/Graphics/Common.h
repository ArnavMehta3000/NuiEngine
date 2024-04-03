#pragma once
#include <Core/Common/NuiWin.h>
#include <Core/Common/Types.h>
#include <Core/Utils/Exceptions.h>
#include <Core/Math/Math.h>
#include <format>
// GFX Headers

#include <wincodec.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <wrl/client.h>


// Library links
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Nui
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr, StringView file, U32 line)
			: m_result(hr)
			, m_file(file)
			, m_line(line)
		{}

		const char* what() const noexcept override
		{
			static char s_str[256] = {};
			sprintf_s(s_str, "HRESULT Error in file %s(%u). %s",
				m_file.c_str(), m_line, GetWin32ErrorString(m_result).c_str());
			
			return s_str;
		}

	private:
		HRESULT m_result;
		String m_file;
		U32 m_line;
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr, __FILE__, __LINE__);
		}
	}

	template <class T>
	void SafeReleaseCOM(T** ptr)
	{
		if (*ptr)
		{
			(*ptr)->Release();
			*ptr = nullptr;
		}
	}
}
