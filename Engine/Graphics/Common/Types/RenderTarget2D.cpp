#include "RenderTarget2D.h"

namespace Nui::Graphics
{
	RenderTarget2D::RenderTarget2D()
		: Width(0)
		, Height(0)
		, Format(DXGI_FORMAT_UNKNOWN)
		, NumMipLevels(0)
		, MultiSamples(0)
		, MSQuality(0)
		, AutoGenMipMaps(false)
		, ArraySize(1)
	{}

	RenderTarget2D::~RenderTarget2D()
	{
		SafeReleaseCOM(Texture.ReleaseAndGetAddressOf());
		SafeReleaseCOM(RTView.ReleaseAndGetAddressOf());
		SafeReleaseCOM(SRView.ReleaseAndGetAddressOf());
	}

	RenderTarget2D RenderTarget2D::Create(
		ID3D11Device1* device, 
		U32 width,
		U32 height,
		DXGI_FORMAT format,
		U32 numMipLevels,
		U32 multiSamples, 
		U32 msQuality,
		bool autoGenMipMaps, 
		U32 arraySize)
	{
		RenderTarget2D rt = RenderTarget2D();

		// Create texture description
		D3D11_TEXTURE2D_DESC desc;
		desc.Width              = width;
		desc.Height             = height;
		desc.ArraySize          = arraySize;
		desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags     = 0;
		desc.Format             = format;
		desc.MipLevels          = numMipLevels;
		desc.MiscFlags          = (autoGenMipMaps && numMipLevels != 1) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		desc.SampleDesc.Count   = multiSamples;
		desc.SampleDesc.Quality = msQuality;
		desc.Usage              = D3D11_USAGE_DEFAULT;

		// Create texture
		DXCall(device->CreateTexture2D(&desc, nullptr, rt.Texture.ReleaseAndGetAddressOf()));

		rt.RTVArraySlices.clear();
		for (U32 i = 0; i < arraySize; i++)
		{
			ComPtr<ID3D11RenderTargetView> rtv;
			D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
			rtDesc.Format = format;

			if (arraySize == 1)
			{
				if (multiSamples > 1)
				{
					rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				}
				else
				{
					rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					rtDesc.Texture2D.MipSlice = 0;
				}
			}
			else
			{
				if (multiSamples > 1)
				{
					rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
					rtDesc.Texture2DMSArray.FirstArraySlice = i;
					rtDesc.Texture2DMSArray.ArraySize = 1;
				}
				else
				{
					rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					rtDesc.Texture2DArray.FirstArraySlice = i;
					rtDesc.Texture2DArray.ArraySize = 1;
					rtDesc.Texture2DArray.MipSlice = 0;
				}
			}
			DXCall(device->CreateRenderTargetView(rt.Texture.Get(), &rtDesc, rtv.ReleaseAndGetAddressOf()));
		}

		rt.RTView = rt.RTVArraySlices[0];

		DXCall(device->CreateShaderResourceView(rt.Texture.Get(), nullptr, rt.SRView.ReleaseAndGetAddressOf()));

		rt.SRVArraySlices.clear();
		for (U32 i = 0; i < arraySize; ++i)
		{
			ComPtr<ID3D11ShaderResourceView> srView;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = format;

			if (arraySize == 1)
			{
				if (multiSamples > 1)
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				}
				else
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = -1;
					srvDesc.Texture2D.MostDetailedMip = 0;
				}
			}
			else
			{
				if (multiSamples > 1)
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					srvDesc.Texture2DMSArray.FirstArraySlice = i;
					srvDesc.Texture2DMSArray.ArraySize = 1;
				}
				else
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					srvDesc.Texture2DArray.FirstArraySlice = i;
					srvDesc.Texture2DArray.MostDetailedMip = 0;
					srvDesc.Texture2DArray.ArraySize = 1;
					srvDesc.Texture2DArray.MipLevels = -1;
				}
			}

			DXCall(device->CreateShaderResourceView(rt.Texture.Get(), &srvDesc, srView.ReleaseAndGetAddressOf()));
			rt.SRVArraySlices.push_back(srView);
		}

		rt.Width          = width;
		rt.Height         = height;
		rt.NumMipLevels   = numMipLevels;
		rt.MultiSamples   = multiSamples;
		rt.Format         = format;
		rt.AutoGenMipMaps = autoGenMipMaps;
		rt.ArraySize      = arraySize;

		return rt;
	}
}