#include "DeviceResources.h"

namespace Nui::Graphics
{
	DeviceResources::DeviceResources()
		: m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
		, m_backBufferWidth(1280)
		, m_backBufferHeight(720)
		, m_multiSampleCount(1)
		, m_multiSampleQuality(0)
		, m_featureLevel(D3D_FEATURE_LEVEL_11_1)
		, m_minFeatureLevel(D3D_FEATURE_LEVEL_10_0)
		, m_vsync(true)
		, m_refreshRate({ 0, 0 })
		, m_numVSYNCIntervals(1)
	{
	}

	DeviceResources::~DeviceResources()
	{
		if (m_immediateContext)
		{
			m_immediateContext->ClearState();
			m_immediateContext->Flush();
		}

		SafeReleaseCOM(m_bbRTView.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_bbTexture.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_swapChain.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_immediateContext.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_device.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_output.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_adapter.ReleaseAndGetAddressOf());
		SafeReleaseCOM(m_factory.ReleaseAndGetAddressOf());
	}

	bool DeviceResources::Init(HWND outputWindow)
	{
		// Get window size
		m_hWnd = outputWindow;
		RECT rect = { 0 };
		::GetClientRect(m_hWnd, &rect);

		// Set back buffer size to window dimensions
		m_backBufferWidth = rect.right - rect.left;
		m_backBufferHeight = rect.bottom - rect.top;

		CheckForSuitableOutput();

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferCount                 = 2;
		desc.BufferDesc.Format           = m_backBufferFormat;
		desc.BufferDesc.Width            = m_backBufferWidth;
		desc.BufferDesc.Height           = m_backBufferHeight;
		desc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.RefreshRate      = m_refreshRate;
		desc.SampleDesc.Count            = m_multiSampleCount;
		desc.SampleDesc.Quality          = m_multiSampleQuality;
		desc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.Flags                       = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
		desc.OutputWindow                = outputWindow;
		desc.Windowed                    = TRUE;  // TODO: Support proper fullscreen

		U32 flags = 0;
#if NUI_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		ComPtr<IDXGISwapChain> swapChain;
		DXCall(::D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
			NULL, 0, D3D11_SDK_VERSION, &desc, swapChain.ReleaseAndGetAddressOf(),
			device.ReleaseAndGetAddressOf(), NULL, context.ReleaseAndGetAddressOf()));

		DXCall(device.As(&m_device));
		DXCall(context.As(&m_immediateContext));
		DXCall(swapChain.As(&m_swapChain));

		SetDebugName(m_device, "Device");
		SetDebugName(m_immediateContext, "ImmediateContext");

		m_featureLevel = m_device->GetFeatureLevel();
		if (m_featureLevel < m_minFeatureLevel)
		{
			throw std::runtime_error("The device doesn't support the minimum feature level required to run this app");
			return false;
		}

		if (D3DPERF_GetStatus() == 0)
		{
			ID3D11InfoQueue* infoQueue;
			DXCall(device->QueryInterface(IID_PPV_ARGS(&infoQueue)));
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
		}

		AfterReset();

		return true;
	}

	void DeviceResources::Reset()
	{
		if (m_swapChain)
		{
			SafeReleaseCOM(m_bbTexture.ReleaseAndGetAddressOf());
			SafeReleaseCOM(m_bbRTView.ReleaseAndGetAddressOf());
			
			m_immediateContext->ClearState();

			// TODO: Handle fullscreen
			DXCall(m_swapChain->SetFullscreenState(FALSE, NULL));

			DXCall(m_swapChain->ResizeBuffers(2, m_backBufferWidth, m_backBufferHeight,
				m_backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

			AfterReset();
		}
	}

	void DeviceResources::Present()
	{
		if (!m_device)
			throw std::runtime_error("Attempted to present with invalid graphics device!");

		const U32 interval = m_vsync ? m_numVSYNCIntervals : 0;
		DXCall(m_swapChain->Present(interval, 0));
	}

	void DeviceResources::CheckForSuitableOutput()
	{
		HRESULT hr = ::CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
		if (FAILED(hr))
			throw std::runtime_error("Unable to create DXGI 1.1 factory");

		// Look for an adapter that supports Direct3D 11
		ComPtr<IDXGIAdapter1> currentAdapter;
		LARGE_INTEGER umdVersion;
		while (!m_adapter && SUCCEEDED(m_factory->EnumAdapters1(0, m_adapter.ReleaseAndGetAddressOf())))
		{
			if (SUCCEEDED(m_adapter->CheckInterfaceSupport(__uuidof(ID3D11Device), &umdVersion)))
			{
				m_adapter = currentAdapter;
			}
		}

		// We'll just use the first output
		DXCall(m_adapter->EnumOutputs(0, m_output.ReleaseAndGetAddressOf()));

		// Get refresh rate
		U32 numModes = 0;
		DXCall(m_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

		DXGI_MODE_DESC* displayModeList{ nullptr };
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
			throw std::runtime_error("Unable to create display mode list");

		DXCall(m_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));
		for (U32 i = 0; i < numModes; i++)
		{
			DXGI_MODE_DESC mode = displayModeList[i];
			if ((mode.Width = (U32)m_backBufferWidth) && (mode.Height == (U32)m_backBufferHeight))
			{
				m_refreshRate = mode.RefreshRate;
			}
		}

		// Display mode not found (fallback to any display)
		if (m_refreshRate.Numerator == 0 && m_refreshRate.Denominator == 0)
		{
			DXGI_MODE_DESC mode = displayModeList[numModes / 2];
			m_refreshRate = mode.RefreshRate;

			m_backBufferWidth = mode.Width;
			m_backBufferHeight = mode.Height;
		}

		delete[] displayModeList;
		displayModeList = nullptr;

		SetDebugName(m_output, "Output");
		SetDebugName(m_adapter, "Adapter");
	}

	void DeviceResources::AfterReset()
	{
		DXCall(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_bbTexture.ReleaseAndGetAddressOf())));
		DXCall(m_device->CreateRenderTargetView(m_bbTexture.Get(), NULL, m_bbRTView.ReleaseAndGetAddressOf()));

		// Set default render targets
		m_immediateContext->OMSetRenderTargets(1, m_bbRTView.GetAddressOf(), NULL);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width    = static_cast<F32>(m_backBufferWidth);
		vp.Height   = static_cast<F32>(m_backBufferHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);
	}
}