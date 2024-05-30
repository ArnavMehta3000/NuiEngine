#include "D3DManager.h"

namespace Nui::Graphics
{
	D3DManager::D3DManager()
		: m_backBufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM)
		, m_backBufferWidth(1280)
		, m_backBufferHeight(720)
		, m_multiSampleCount(1)
		, m_multiSampleQuality(0)
		, m_featureLevel(D3D_FEATURE_LEVEL_11_1)
		, m_minFeatureLevel(D3D_FEATURE_LEVEL_11_0)
		, m_vsync(true)
		, m_refreshRate({ 0, 0 })
		, m_numVSYNCIntervals(1)
		, m_hWnd(nullptr)
	{
	}

	D3DManager::~D3DManager()
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

	bool D3DManager::IsInitialized() const noexcept
	{
		// Return true if all objects are initialized
		return m_device != nullptr 
			&& m_immediateContext != nullptr 
			&& m_swapChain != nullptr;
	}

	bool D3DManager::Init(HWND outputWindow)
	{
		// Get window size
		m_hWnd = outputWindow;
		RECT rect = { 0 };
		::GetClientRect(m_hWnd, &rect);

		// Set back buffer size to window dimensions
		m_backBufferWidth  = rect.right - rect.left;
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
		desc.Flags                       = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.SwapEffect                  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.OutputWindow                = outputWindow;
		desc.Windowed                    = TRUE;  // TODO: Support proper fullscreen

		U32 creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef NUI_DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		ComPtr<IDXGISwapChain> swapChain;
		DXCall(::D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			creationFlags,
			&m_featureLevel,
			1,
			D3D11_SDK_VERSION,
			&desc,
			swapChain.ReleaseAndGetAddressOf(),
			device.ReleaseAndGetAddressOf(),
			NULL,
			context.ReleaseAndGetAddressOf())
		);

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

#if NUI_DEBUG
		{  // Scoped so COM objects can be safely released
			ComPtr<ID3D11Debug> d3dDebug;
			if (SUCCEEDED(m_device.As(&d3dDebug)))
			{
				ComPtr<ID3D11InfoQueue> d3dInfoQueue;
				if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
				{
					// Treat warnings as errors
					d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
					d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);

					D3D11_MESSAGE_ID hide[] =
					{
						// Add more message IDs here as needed
						D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					};

					D3D11_INFO_QUEUE_FILTER filter = {};
					filter.DenyList.NumIDs = _countof(hide);
					filter.DenyList.pIDList = hide;
					d3dInfoQueue->AddStorageFilterEntries(&filter);
				}
			}

			// Enabling DXGI debug layer
			// Ref: https://walbourn.github.io/dxgi-debug-device/

			ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
			typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);
			HMODULE dxgidebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

			if (dxgidebug)
			{
				auto dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
					reinterpret_cast<void*>(::GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));

				if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
				{
					dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
					dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
				}
			}
		}
#endif // NUI_DEBUG


		AfterReset();

		return true;
	}

	void D3DManager::Reset()
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

	void D3DManager::Present()
	{
		if (!m_device)
			throw std::runtime_error("Attempted to present with invalid graphics device!");

		const U32 interval = m_vsync ? m_numVSYNCIntervals : 0;
		DXCall(m_swapChain->Present(interval, 0));
	}

	void D3DManager::CheckForSuitableOutput()
	{
		HRESULT hr = ::CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
		if (FAILED(hr))
			throw std::runtime_error("Unable to create DXGI 1.1 factory");

		// Look for an adapter that supports Direct3D 11
		DXCall(m_factory->EnumAdapters1(0, m_adapter.ReleaseAndGetAddressOf()));
		

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

	void D3DManager::AfterReset()
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