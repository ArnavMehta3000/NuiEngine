#pragma once
#include <Core/Common/NuiWin.h>
#include <Core/Common/Types.h>
#include <Core/Utils/Exceptions.h>
#include <Core/Math/Math.h>
#include <wrl/client.h>

#include <d3d11sdklayers.h>
#include <wincodec.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DirectXColors.h>


// Library links
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")


namespace Nui::Graphics
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr, StringView expression, StringView file, U32 line)
			: m_result(hr)
			, m_file(file)
			, m_line(line)
		{}

		const char* what() const noexcept override
		{
			static char s_str[512] = {};
			sprintf_s(s_str, "DXCall caught an exception\nMessage: %s\nExpression: %s\nFile: %s(%u)",
				GetWin32ErrorString(m_result).c_str(), m_expression.c_str(), m_file.c_str(), m_line);
			
			return s_str;
		}

	private:
		HRESULT m_result;
		String m_file;
		String m_expression;
		U32 m_line;
	};

	inline void ThrowIfFailed(HRESULT hr, StringView expression, StringView file, U32 line)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr, expression, file, line);
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

	template <typename T>
	HRESULT SetDebugName(T deviceChild, const String& debugName)
	{
#if defined(NUI_DEBUG)
		return deviceChild->SetPrivateData(WKPDID_D3DDebugObjectName, (U32)debugName.size(), debugName.data());
#else
		return S_OK;
#endif // defined(NUI_DEBUG)

	}

#define DXCall(expr) ThrowIfFailed(expr, #expr, __FILE__, __LINE__)
// Align to float4 (16 bytes)
#define Float4Align __declspec(align(16))
}
