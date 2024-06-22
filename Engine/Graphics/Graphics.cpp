#include "Graphics.h"
#include <Core/Utils/Singleton.h>

namespace Nui::Graphics
{
	static unsigned char g_shaderByteCode[] = {
		#include "ShaderPS.hlsl.h"
	};

	void Init(HWND outputWindow)
	{
		Singleton<D3DManager>::Get().Init(outputWindow);
		//OutputDebugStringA(std::string(reinterpret_cast<char*>(g_shaderByteCode)).c_str());
	}

	void Resize(U32 width, U32 height)
	{
		Singleton<D3DManager>::Get().Resize(width, height);
		Singleton<D3DManager>::Get().Reset();
	}

	void Shutdown()
	{
		Singleton<D3DManager>::Destroy();
	}

	bool IsInitialized()
	{
		return Singleton<D3DManager>::Get().IsInitialized();
	}
}
