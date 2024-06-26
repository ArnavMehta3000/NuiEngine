#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics
{
	/**
	 * @brief Class managing DirectX 11 device resources including device, context, swap chain, and render target.
	 */
	class D3DManager
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		D3DManager();

		/**
		 * @brief Destructor.
		 */
		~D3DManager();

		/**
		 * @brief Checks if the device resources have been initialized
		 */
		bool IsInitialized() const noexcept;

		/**
		 * @brief Initializes the device resources.
		 * @param outputWindow Output window handle.
		 */
		bool Init(HWND outputWindow);

		/**
		 * @brief Resets the device resources.
		 */
		void Reset();

		/**
		 * @brief Presents the rendered image to the screen.
		 */
		void Present() const;

		/**
		 * @brief Retrieves the Direct3D device.
		 * @return Pointer to the Direct3D device.
		 */
		[[nodiscard]] inline ID3D11Device1* GetDevice() const noexcept { return m_device.Get(); }

		/**
		 * @brief Retrieves the immediate context for the device.
		 * @return Pointer to the immediate context.
		 */
		[[nodiscard]] inline ID3D11DeviceContext1* GetImmediateContext() const noexcept { return m_immediateContext.Get(); }

		/**
		 * @brief Retrieves the swap chain associated with the device.
		 * @return Pointer to the swap chain.
		 */
		[[nodiscard]] inline IDXGISwapChain1* GetSwapChain() const noexcept { return m_swapChain.Get(); }

		/**
		 * @brief Retrieves the render target view for the back buffer.
		 * @return Pointer to the render target view.
		 */
		[[nodiscard]] inline ID3D11RenderTargetView* GetBackBuffer() const noexcept { return m_bbRTView.Get(); }

		/**
		 * @brief Retrieves the back buffer texture.
		 * @return Pointer to the back buffer texture.
		 */
		[[nodiscard]] inline ID3D11Texture2D* GetBackBufferTexture() const noexcept { return m_bbTexture.Get(); }

		/**
		 * @brief Retrieves the format of the back buffer.
		 * @return Format of the back buffer.
		 */
		[[nodiscard]] inline DXGI_FORMAT GetBackBufferFormat() const noexcept { return m_backBufferFormat; }

		/**
		 * @brief Retrieves the width of the back buffer.
		 * @return Width of the back buffer.
		 */
		[[nodiscard]] inline U32 GetBackBufferWidth() const noexcept { return m_backBufferWidth; }

		/**
		 * @brief Retrieves the height of the back buffer.
		 * @return Height of the back buffer.
		 */
		[[nodiscard]] inline U32 GetBackBufferHeight() const noexcept { return m_backBufferHeight; }

		/**
		 * @brief Retrieves the multi-sample count of the back buffer.
		 * @return Multi-sample count of the back buffer.
		 */
		[[nodiscard]] inline U32 GetBBMultiSampleCount() const noexcept { return m_multiSampleCount; }

		/**
		 * @brief Retrieves the multi-sample quality of the back buffer.
		 * @return Multi-sample quality of the back buffer.
		 */
		[[nodiscard]] inline U32 GetBBMultiSampleQuality() const noexcept { return m_multiSampleQuality; }

		/**
		 * @brief Checks if vertical synchronization is enabled.
		 * @return True if VSync is enabled, false otherwise.
		 */
		[[nodiscard]] bool IsVSyncEnabled() const noexcept { return m_vsync; }

		/**
		 * @brief Retrieves the number of vertical synchronization intervals.
		 * @return Number of VSync intervals.
		 */
		[[nodiscard]] U32 GetNumVSyncInterval() const noexcept { return m_numVSYNCIntervals; }

		/**
		 * @brief Resizes the back buffer.
		 * @param width New width of the back buffer.
		 * @param height New height of the back buffer.
		 */
		void Resize(U32 width, U32 height) noexcept { m_backBufferWidth = width; m_backBufferHeight = height; }

		inline void SetVSYNC(bool enableVSYNC) { m_vsync = enableVSYNC; }

	private:
		/**
		 * @brief Checks for a suitable display output for rendering.
		 */
		void CheckForSuitableOutput();

		/**
		 * @brief Performs actions after resetting the device resources.
		 */
		void AfterReset();

	private:
		ComPtr<IDXGIFactory2>          m_factory;
		ComPtr<IDXGIAdapter1>          m_adapter;
		ComPtr<IDXGIOutput>            m_output;

		ComPtr<ID3D11Device1>          m_device;
		ComPtr<ID3D11DeviceContext1>   m_immediateContext;
		ComPtr<IDXGISwapChain1>        m_swapChain;
		ComPtr<ID3D11Texture2D>        m_bbTexture;
		ComPtr<ID3D11RenderTargetView> m_bbRTView;

		DXGI_FORMAT                    m_backBufferFormat;     /// Format of the back buffer.
		U32                            m_backBufferWidth;      /// Width of the back buffer.
		U32                            m_backBufferHeight;     /// Height of the back buffer.
		U32                            m_multiSampleCount;     /// Multi-sample count of the back buffer.
		U32                            m_multiSampleQuality;   /// Multi-sample quality of the back buffer.
		bool                           m_vsync;                /// Flag indicating whether VSync is enabled.
		DXGI_RATIONAL                  m_refreshRate;          /// Refresh rate of the monitor.
		U32                            m_numVSYNCIntervals;    /// Number of VSync intervals.

		HWND                           m_hWnd;                 /// Handle to the output window.
		D3D_FEATURE_LEVEL              m_featureLevel;         /// Feature level of the device.
		D3D_FEATURE_LEVEL              m_minFeatureLevel;      /// Minimum feature level required.
	};
}
