#include "BlendState.h"

namespace Nui::Graphics::States
{
	constexpr D3D11_BLEND_DESC GetBlendStateBlendDisabledDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = false;

		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = false;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStateAdditiveBlendDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = false;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = true;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStateAlphaBlendDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = true;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = true;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		blendDesc.RenderTarget[0].BlendEnable = true;

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStatePreMultipliedAlphaBlendDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = false;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = false;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStateColorWriteDisabledDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = false;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = false;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = 0;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStateAlphaToCoverageDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = false;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}

	constexpr D3D11_BLEND_DESC GetBlendStateOpacityBlendDesc()
	{
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable  = false;
		blendDesc.IndependentBlendEnable = false;
		for (U32 i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable           = true;
			blendDesc.RenderTarget[i].BlendOp               = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend             = D3D11_BLEND_INV_SRC1_COLOR;
			blendDesc.RenderTarget[i].DestBlendAlpha        = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend              = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].SrcBlendAlpha         = D3D11_BLEND_ONE;
		}

		return blendDesc;
	}
}