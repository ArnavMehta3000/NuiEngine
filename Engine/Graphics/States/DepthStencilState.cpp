#include "DepthStencilState.h"

namespace Nui::Graphics::States
{
	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthDisabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = false;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable                = false;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable                = false;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilReverseDepthEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable                = false;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthWriteEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable                = false;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilReverseDepthWriteEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable                = false;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthStencilWriteEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable                = true;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}

	constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilStencilEnabledDesc()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable                = true;
		dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask             = 0;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_EQUAL;
		dsDesc.BackFace                     = dsDesc.FrontFace;

		return dsDesc;
	}
}