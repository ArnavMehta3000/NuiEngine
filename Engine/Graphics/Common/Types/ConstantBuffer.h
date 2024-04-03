#pragma once
#include "Graphics/Common/Common.h"

namespace Nui::Graphics
{
	/**
	 * @brief Returns a size suitable for creating a constant buffer, by rounding to the next multiple of 16
	 * @param size The size of the constant buffer
	 */
	constexpr U32 CalculateCBSize(U32 size)
	{
		return ((size + 15) / 16) * 16;
	}

	/**
	 * @brief Represents a constant buffer
	 * @tparam T The data type of the constant buffer
	 */
	template <typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer()
			: GPUWritable(false)
		{
			ZeroMemory(&Data, sizeof(T));
		}

		/**
		 * @brief Creates a constant buffer
		 * @param device DX11 device
		 * @param gpuWritable If the constant buffer can be written to by the GPU
		 * @return The constant buffer of type T
		 */
		static ConstantBuffer<T> Create(ID3D11Device1* device, bool gpuWritable = false)
		{
			ConstantBuffer<T> cb;

			D3D11_BUFFER_DESC desc;
			desc.Usage          = D3D11_USAGE_DYNAMIC;
			desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags      = 0;
			desc.ByteWidth      = static_cast<uint32>(sizeof(T) + (16 - (sizeof(T) % 16)));

			if (gpuWritable)
			{
				desc.Usage          = D3D11_USAGE_DEFAULT;
				desc.CPUAccessFlags = 0;
			}
			GPUWritable = gpuWritable;

			DXCall(device->CreateBuffer(&desc, nullptr, cb.Buffer.ReleaseAndGetAddressOf()));

			return cb;
		}

		/**
		 * @brief Applies the changes made to the constant buffer
		 * @param ctx DX11 device context
		 */
		void ApplyChanges(ID3D11DeviceContext1* ctx)
		{
			if (!Buffer)
				throw std::runtime_error("Trying to apply changes on a constant buffer that was not created");

			if (GPUWritable)
			{
				ctx->UpdateSubresource(Buffer.Get(), 0, nullptr, &Data, 0, 0);
			}
			else
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource{};
				DXCall(ctx->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
				CopyMemory(mappedResource.pData, &Data, sizeof(T));
				ctx->Unmap(Buffer, 0);
			}
		}

		/**
		 * @brief Sets the constant buffer for the vertex shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetVS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for VS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->VSSetConstantBuffers(slot, 1, bufferArray);
		}

		/**
		 * @brief Sets the constant buffer for the pixel shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetPS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for PS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->PSSetConstantBuffers(slot, 1, bufferArray);
		}

		/**
		 * @brief Sets the constant buffer for the compute shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetCS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for CS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->CSSetConstantBuffers(slot, 1, bufferArray);
		}

		/**
		 * @brief Sets the constant buffer for the geometry shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetGS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for GS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->GSSetConstantBuffers(slot, 1, bufferArray);
		}

		/**
		 * @brief Sets the constant buffer for the hull shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetHS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for HS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->HSSetConstantBuffers(slot, 1, bufferArray);
		}

		/**
		 * @brief Sets the constant buffer for the domain shader
		 * @param ctx DX11 device context
		 * @param slot Shader slot for the constant buffer
		 */
		void SetDS(ID3D11DeviceContext1* ctx, U32 slot)
		{
			if (!Buffer)
				throw std::runtime_error("Constant buffer was nullptr when trying to set for DS");

			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = Buffer.Get();

			ctx->DSSetConstantBuffers(slot, 1, bufferArray);
		}

	public:
		T Data;
		ComPtr<ID3D11Buffer> Buffer;
		bool GPUWritable;
	};
}