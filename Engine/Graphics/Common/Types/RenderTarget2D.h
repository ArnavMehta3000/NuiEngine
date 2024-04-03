#pragma once
#include "Graphics/Common/Common.h"

namespace Nui::Graphics
{
	/**
	 * @brief Represents a 2D render target
	 */
	struct RenderTarget2D
	{
		/**
		 * @brief The texture of the render target
		 */
		ComPtr<ID3D11Texture2D> Texture;

		/**
		 * @brief The render target view
		 */
		ComPtr<ID3D11RenderTargetView> RTView;

		/**
		 * @brief The shader resource view for the render target
		 */
		ComPtr<ID3D11ShaderResourceView> SRView;

		/**
		 * @brief The width of the render target
		 */
		U32 Width;

		/**
		 * @brief The height of the render target
		 */
		U32 Height;

		/**
		 * @brief The number of mip levels of the render target
		 */
		U32 NumMipLevels;

		/**
		 * @brief The number of multisamples of the render target
		 */
		U32 MultiSamples;

		/**
		 * @brief The multisample quality of the render target
		 */
		U32 MSQuality;

		/**
		 * @brief The format of the render target
		 */
		DXGI_FORMAT Format;

		/**
		 * @brief Whether or not the render target automatically generates mipmaps
		 */
		bool AutoGenMipMaps;

		/**
		 * @brief The array slice size of the render target
		 */
		U32 ArraySize;

		/**
		 * @brief The array slices of the render target
		 */
		std::vector<ComPtr<ID3D11RenderTargetView>> RTVArraySlices;

		/**
		 * @brief The SRV array slices of the render target
		 */
		std::vector<ComPtr<ID3D11ShaderResourceView>> SRVArraySlices;

		RenderTarget2D();
		~RenderTarget2D();

		/**
		 * @brief Creates a render target
		 * @param device DX11 device
		 * @param width The width of the render target
		 * @param height The height of the render target
		 * @param format The format of the render target
		 * @param numMipLevels The number of mip levels
		 * @param multiSamples The number of multisamples
		 * @param msQuality The multisample quality
		 * @param autoGenMipMaps Whether or not the render target automatically generates mipmaps
		 * @param arraySize The array slice size
		 * @return The render target 2D
		 */
		static RenderTarget2D Create(
			ID3D11Device1* device,
			U32 width,
			U32 height,
			DXGI_FORMAT format,
			U32 numMipLevels = 1,
			U32 multiSamples = 1,
			U32 msQuality = 0,
			bool autoGenMipMaps = false,
			U32 arraySize = 1);
	};
}