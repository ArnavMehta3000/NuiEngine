#include "RasterizerState.h"

namespace Nui::Graphics::States
{
	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_NONE;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = false;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateFrontFaceCullDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_FRONT;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = false;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateFrontFaceCullScissorDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_FRONT;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = true;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = true;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateBackFaceCullDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_BACK;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = false;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateBackFaceCullScissorDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_BACK;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = true;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullNoMSDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_NONE;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = false;
		rastDesc.ScissorEnable         = false;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullScissorDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_NONE;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = true;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}

	static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateWireframeDesc()
	{
		D3D11_RASTERIZER_DESC rastDesc{};

		rastDesc.AntialiasedLineEnable = false;
		rastDesc.CullMode              = D3D11_CULL_NONE;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0.0f;
		rastDesc.DepthClipEnable       = true;
		rastDesc.FillMode              = D3D11_FILL_WIREFRAME;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable     = true;
		rastDesc.ScissorEnable         = false;
		rastDesc.SlopeScaledDepthBias  = 0;

		return rastDesc;
	}
}