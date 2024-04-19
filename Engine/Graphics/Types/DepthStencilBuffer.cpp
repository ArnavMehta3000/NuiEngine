#include "DepthStencilBuffer.h"

namespace Nui::Graphics
{
	DepthStencilBuffer::DepthStencilBuffer()
		: Width(0)
		, Height(0)
		, MultiSamples(0)
		, MSQuality(0)
		, Format(DXGI_FORMAT_UNKNOWN)
		, ArraySize(1)
	{
	}

	DepthStencilBuffer::~DepthStencilBuffer()
	{
		SafeReleaseCOM(Texture.ReleaseAndGetAddressOf());
		SafeReleaseCOM(SRView.ReleaseAndGetAddressOf());
		SafeReleaseCOM(DSView.ReleaseAndGetAddressOf());
		SafeReleaseCOM(ReadOnlyDSView.ReleaseAndGetAddressOf());
	}

	DepthStencilBuffer DepthStencilBuffer::Create(
		ID3D11Device1* device,
		U32 width,
		U32 height,
		DXGI_FORMAT format,
		bool useAsShaderResource,
		U32 multiSamples,
		U32 msQuality,
		U32 arraySize)
	{
		DepthStencilBuffer dsb = DepthStencilBuffer();
		
		// Choose bind flags
		U32 bindFlags = D3D11_BIND_DEPTH_STENCIL;
		if (useAsShaderResource)
			bindFlags |= D3D11_BIND_SHADER_RESOURCE;

		// Choose texture format
		DXGI_FORMAT dsTexFormat;
		if (!useAsShaderResource)
			dsTexFormat = format;
		else if (format == DXGI_FORMAT_D16_UNORM)
			dsTexFormat = DXGI_FORMAT_R16_TYPELESS;
		else if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
			dsTexFormat = DXGI_FORMAT_R24G8_TYPELESS;
		else
			dsTexFormat = DXGI_FORMAT_R32_TYPELESS;


		D3D11_TEXTURE2D_DESC desc;
		desc.Width              = width;
		desc.Height             = height;
		desc.ArraySize          = arraySize;
		desc.BindFlags          = bindFlags;
		desc.CPUAccessFlags     = 0;
		desc.Format             = dsTexFormat;
		desc.MipLevels          = 1;
		desc.MiscFlags          = 0;
		desc.SampleDesc.Count   = multiSamples;
		desc.SampleDesc.Quality = msQuality;
		desc.Usage              = D3D11_USAGE_DEFAULT;
		DXCall(device->CreateTexture2D(&desc, nullptr, dsb.Texture.ReleaseAndGetAddressOf()));

		// Create views
		dsb.ArraySlices.clear();
		for (U32 i = 0; i < arraySize; ++i)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			ComPtr<ID3D11DepthStencilView> dsView;
			dsvDesc.Format = format;

			if (arraySize == 1)
			{
				dsvDesc.ViewDimension = multiSamples > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice = 0;
			}
			else
			{
				if (multiSamples > 1)
				{
					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
					dsvDesc.Texture2DMSArray.FirstArraySlice = i;
					dsvDesc.Texture2DMSArray.ArraySize = 1;
				}
				else
				{
					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					dsvDesc.Texture2DArray.FirstArraySlice = i;
					dsvDesc.Texture2DArray.ArraySize = 1;
					dsvDesc.Texture2DArray.MipSlice = 0;
				}
			}

			dsvDesc.Flags = 0;
			DXCall(device->CreateDepthStencilView(dsb.Texture.Get(), &dsvDesc, dsView.ReleaseAndGetAddressOf()));
			dsb.ArraySlices.push_back(dsView);

			if (i == 0)
			{
				// Also create a read-only DSV
				dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
				if (format == DXGI_FORMAT_D24_UNORM_S8_UINT || format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
					dsvDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
				DXCall(device->CreateDepthStencilView(dsb.Texture.Get(), &dsvDesc, dsb.ReadOnlyDSView.ReleaseAndGetAddressOf()));
				dsvDesc.Flags = 0;
			}
		}

		dsb.DSView = dsb.ArraySlices[0];

		if (useAsShaderResource)
		{
			DXGI_FORMAT dsSRVFormat;
			if (format == DXGI_FORMAT_D16_UNORM)
				dsSRVFormat = DXGI_FORMAT_R16_UNORM;
			else if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
				dsSRVFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			else
				dsSRVFormat = DXGI_FORMAT_R32_FLOAT;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = dsSRVFormat;

			if (arraySize == 1)
			{
				srvDesc.ViewDimension = multiSamples > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
			}
			else
			{
				srvDesc.ViewDimension = multiSamples > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.ArraySize = arraySize;
				srvDesc.Texture2DArray.FirstArraySlice = 0;
				srvDesc.Texture2DArray.MostDetailedMip = 0;
				srvDesc.Texture2DArray.MipLevels = 1;
			}

			DXCall(device->CreateShaderResourceView(dsb.Texture.Get(), &srvDesc, dsb.SRView.ReleaseAndGetAddressOf()));
		}
		else
			dsb.SRView = nullptr;


		dsb.Width        = width;
		dsb.Height       = height;
		dsb.MultiSamples = multiSamples;
		dsb.Format       = format;
		dsb.ArraySize    = arraySize;

		return dsb;
	}
}