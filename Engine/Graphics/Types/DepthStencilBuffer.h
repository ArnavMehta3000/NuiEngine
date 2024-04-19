#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics
{
	/**
	 * @brief Represents a depth stencil buffer
	 */
	struct DepthStencilBuffer
	{
		/**
		 * @brief The depth stencil texture
		 */
		ComPtr<ID3D11Texture2D> Texture;

		/**
		 * @brief The depth stencil view
		 */
		ComPtr<ID3D11DepthStencilView> DSView;

		/**
		 * @brief The read only depth stencil view
		 */
		ComPtr<ID3D11DepthStencilView> ReadOnlyDSView;

		/**
		 * @brief The shader resource view
		 */
		ComPtr<ID3D11ShaderResourceView> SRView;

		/**
		 * @brief The width of the depth stencil buffer
		 */
		U32 Width;

		/**
		 * @brief The height of the depth stencil buffer
		 */
		U32 Height;

		/**
		 * @brief The number of multisamples
		 */
		U32 MultiSamples;

		/**
		 * @brief The multisample quality of the depth stencil buffer
		 */
		U32 MSQuality;

		/**
		 * @brief The format of the depth stencil buffer
		 */
		DXGI_FORMAT Format;

		/**
		 * @brief The number of array slices
		 */
		U32 ArraySize;

		/**
		 * @brief The array slices
		 */
		std::vector<ComPtr<ID3D11DepthStencilView>> ArraySlices;

		DepthStencilBuffer();
		~DepthStencilBuffer();

		/**
		 * @brief Creates a depth stencil buffer
		 * @param device DX11 device
		 * @param width The width of the depth stencil buffer
		 * @param height The height of the depth stencil buffer
		 * @param format The format of the depth stencil buffer
		 * @param useAsShaderResource Whether or not to use the depth stencil buffer as a shader resource
		 * @param multiSamples The number of multisamples
		 * @param msQuality The multisample quality
		 * @param arraySize The number of array slices
		 * @return The depth stencil buffer
		 */
		static DepthStencilBuffer Create(
			ID3D11Device1* device,
			U32 width,
			U32 height,
			DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT,
			bool useAsShaderResource = false,
			U32 multiSamples = 1,
			U32 msQuality = 0,
			U32 arraySize = 1);
	};
}