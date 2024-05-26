#include "SamplerState.h"

namespace Nui::Graphics
{
	constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearDesc()
	{
		D3D11_SAMPLER_DESC sampDesc{};

		sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias     = 0.0f;
		sampDesc.MaxAnisotropy  = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD         = 0;
		sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		return sampDesc;
	}

	constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearClampDesc()
	{
		D3D11_SAMPLER_DESC sampDesc{};

		sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.MipLODBias     = 0.0f;
		sampDesc.MaxAnisotropy  = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD         = 0;
		sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		return sampDesc;
	}

	constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearBorderDesc()
	{
		D3D11_SAMPLER_DESC sampDesc{};

		sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
		sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
		sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
		sampDesc.MipLODBias     = 0.0f;
		sampDesc.MaxAnisotropy  = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD         = 0;
		sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		return sampDesc;
	}

	constexpr D3D11_SAMPLER_DESC GetSamplerStatePointDesc()
	{
		D3D11_SAMPLER_DESC sampDesc{};

		sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.MipLODBias     = 0.0f;
		sampDesc.MaxAnisotropy  = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD         = 0;
		sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		return sampDesc;
	}

	constexpr D3D11_SAMPLER_DESC GetSamplerStateAnisotropicDesc()
	{
		D3D11_SAMPLER_DESC sampDesc{};

		sampDesc.Filter         = D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias     = 0.0f;
		sampDesc.MaxAnisotropy  = 16;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD         = 0;
		sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		return sampDesc;
	}
}