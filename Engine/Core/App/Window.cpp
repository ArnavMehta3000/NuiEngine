#include "Window.h"
#include <windowsx.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace Nui
{
	namespace  // Anonymous namespace
	{
		static const WString s_windowClassName = L"NuiApp";

		bool HasStyleFlag(DWORD style, DWORD flag)
		{
			return (style & flag) == flag;
		}

		enum StyleInternal : DWORD
		{
			Windowed             = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			WindowedFullscreen   = Windowed | WS_MAXIMIZE,
			AeroBorderless       = WS_POPUP | /*WS_THICKFRAME |*/ WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
			BasicBorderless      = WS_POPUP | /*WS_THICKFRAME |*/ WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
			BorderlessFullscreen = WS_MAXIMIZE
		};

		bool IsCompositionEnabled()
		{
			BOOL compositionEnabled = FALSE;
			bool success = ::DwmIsCompositionEnabled(&compositionEnabled) == S_OK;
			return compositionEnabled && success;
		}

		StyleInternal GetBorderlessStyle()
		{
			return IsCompositionEnabled() ? StyleInternal::AeroBorderless : StyleInternal::BasicBorderless;
		}

		StyleInternal ConvertStyle(Window::Style style)
		{
			switch (style)
			{
			case Window::Style::Windowed:
				return StyleInternal::Windowed;

			case Window::Style::WindowedFullscreen:
				return StyleInternal::WindowedFullscreen;

			case Window::Style::Borderless:
			case Window::Style::BorderlessFullscreen:
				return GetBorderlessStyle();
			}

			return StyleInternal::Windowed;
		}

		Window::Style GetStyleProxy(Window::Style style, bool& outIsFullscreen)
		{
			Window::Style styleProxy = style;
			outIsFullscreen = false;

			if (styleProxy == Window::Style::WindowedFullscreen)
			{
				outIsFullscreen = true;
				styleProxy = Window::Style::Windowed;
			}
			else if (styleProxy == Window::Style::BorderlessFullscreen)
			{
				outIsFullscreen = true;
				styleProxy = Window::Style::Borderless;
			}
			return style;
		}

	}  // Anonymous namespace

	Window::Window(Window::Style style, WStringView title, Window::Size size)
		: m_style(style)
		, m_title(title)
		, m_size(size)
		, m_hWnd(nullptr)
		, m_isFocused(false)
		, m_hInstance(GetModuleHandle(NULL))
	{
		MakeWindow();
	}

	Window::~Window()
	{
		::DestroyWindow(m_hWnd);
		::UnregisterClass(s_windowClassName.c_str(), m_hInstance);
	}

	HWND Window::GetHWND() const { return m_hWnd; }

	HINSTANCE Window::GetHinstance() const { return m_hInstance; }

	bool Window::Maximized() const
	{
		NUI_ASSERT(m_hWnd, "Failed to check if window is maximized, window handle is nullptr");

		WINDOWPLACEMENT placement{};
		if (!::GetWindowPlacement(m_hWnd, &placement))
		{
			return false;
		}

		return placement.showCmd == SW_MAXIMIZE;
	}

	bool Window::Focused() const
	{
		return m_isFocused;
	}

	bool Window::WantsToClose() const
	{
		// Windows message pump
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);

			if (msg.message == WM_QUIT)
			{
				return true;
			}
		}
		return false;
	}

	void Window::AddCallback(U32 msg, WndCallback callback)
	{
		m_callbacks[msg] = std::move(callback);
	}

	void Window::AdjustMaximizedClientRect(RECT& rect)
	{
		NUI_ASSERT(m_hWnd, "Failed to adjust client rect, window handle is nullptr");

		if (Maximized())
			return;

		HMONITOR monitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
		if (!monitor)
			return;

		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(monitorInfo);
		if (!::GetMonitorInfo(monitor, &monitorInfo))
			return;

		// When maximized, make the client area fill just the monitor (without task bar) rect,
		// not the whole window rect which extends beyond the monitor.
		rect = monitorInfo.rcWork;
	}

	void Window::MakeWindow()
	{
		// Register window class
		WNDCLASSEXW wcx{};
		wcx.cbSize        = sizeof(wcx);
		wcx.style         = CS_HREDRAW | CS_VREDRAW;
		wcx.hInstance     = m_hInstance;
		wcx.lpfnWndProc   = WndProc;
		wcx.lpszClassName = s_windowClassName.c_str();
		wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcx.hCursor       = ::LoadCursorW(nullptr, IDC_ARROW);

		if (!::RegisterClassExW(&wcx))
		{
			// Log error but try to continue regardless
			NUI_LOG(Error, Window, "Failed to register window class. ", GetWin32ErrorString(GetLastError()));
		}

		// Get window centered position
		RECT desktopRect;
		::GetWindowRect(::GetDesktopWindow(), &desktopRect);
		I32 posX = (desktopRect.right / 2) - (m_size.X / 2);
		I32 posY = (desktopRect.bottom / 2) - (m_size.Y / 2);

		StyleInternal style = ConvertStyle(m_style);
		m_hWnd = ::CreateWindowExW(
			0,
			s_windowClassName.c_str(),
			m_title.c_str(),
			style,
			posX, posY,
			m_size.X, m_size.Y,
			nullptr,
			nullptr,
			m_hInstance,
			this
		);

		NUI_ASSERT(m_hWnd, "Failed to create window, handle is nullptr");

		// Apply style
		bool fullscreen = false;
		Style styleProxy = GetStyleProxy(m_style, fullscreen);


		if (HasStyleFlag((DWORD)styleProxy, (DWORD)Style::Borderless))
		{
			StyleInternal style = ConvertStyle(styleProxy);
			StyleInternal oldStyle = static_cast<StyleInternal>(::GetWindowLongW(m_hWnd, GWL_STYLE));

			if (style != oldStyle)
			{
				::SetWindowLongW(m_hWnd, GWL_STYLE, static_cast<LONG>(style));

				if (IsCompositionEnabled())
				{
					static const MARGINS shadowState[2]{ { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };
					::DwmExtendFrameIntoClientArea(m_hWnd, &shadowState[style != StyleInternal::Windowed]);
				}

				// Redraw frame
				::SetWindowPos(m_hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
			}
		}

		::ShowWindow(m_hWnd, fullscreen ? SW_MAXIMIZE : SW_SHOWNORMAL);

		// Update window size
		RECT rect{};
		AdjustWindowRect(&rect, (DWORD)m_style, FALSE);
		m_size.X = rect.right - rect.left;
		m_size.Y = rect.bottom - rect.top;
		::UpdateWindow(m_hWnd);
		NUI_LOG(Debug, Window, "Created window");
	}

	Window::Size Window::GetClientSize() const
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);

		return { rect.right - rect.left, rect.bottom - rect.top };
	}

	void Window::SetWindowTitle(WString title)
	{
		::SetWindowTextW(m_hWnd, title.c_str());
	}

	LRESULT Window::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return m_callbacks.contains(uMsg)
			? m_callbacks[uMsg](this, uMsg, wParam, lParam)
			: MessageHandler(hWnd, uMsg, wParam, lParam);

	}

	LRESULT WINAPI Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		{
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			Window* pObj                 = reinterpret_cast<Window*>(pCreateStruct->lpCreateParams);
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		}

		Window* pObj = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (pObj)
			return pObj->MessageRouter(hWnd, uMsg, wParam, lParam);
		else
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT Window::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCALCSIZE:
		{
			if (wParam == TRUE && HasStyleFlag((DWORD)m_style, (DWORD)Style::Borderless))
			{
				auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				AdjustMaximizedClientRect(params.rgrc[0]);
				return 0;
			}
			break;
		}

		case WM_NCHITTEST:
		{
			// When we have no border or title bar, we need to perform our
			// own hit testing to allow resizing and moving.
			if (HasStyleFlag((DWORD)m_style, (DWORD)Style::Borderless))
			{
				return HitTest(POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
			}
			break;
		}

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		}

		case WM_SETFOCUS:
		{
			m_isFocused = true;
			return 0;
		}

		case WM_KILLFOCUS:
		{
			m_isFocused = false;
			return 0;
		}
		}

		if (Input::Internal::ProcessInputWndProc(hWnd, uMsg, wParam, lParam))
		{
			// Message was processed by input system
			return 0;
		}


		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT Window::HitTest(POINT cursor)
	{
		// identify borders and corners to allow resizing the window.
		// Note: On Windows 10, windows behave differently and
		// allow resizing outside the visible window frame.
		// This implementation does not replicate that behavior.
		const POINT border
		{
			::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
			::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER)
		};

		RECT window;
		if (!::GetWindowRect(m_hWnd, &window)) {
			return HTNOWHERE;
		}

		enum region_mask
		{
			client = 0b0000,
			left   = 0b0001,
			right  = 0b0010,
			top    = 0b0100,
			bottom = 0b1000,
		};

		const auto result =
			left   * (cursor.x < (window.left + border.x)) |
			right  * (cursor.x >= (window.right - border.x)) |
			top    * (cursor.y < (window.top + border.y)) |
			bottom * (cursor.y >= (window.bottom - border.y));

		switch (result)
		{
		case left          : return HTLEFT;
		case right         : return HTRIGHT;
		case top           : return HTTOP;
		case bottom        : return HTBOTTOM;
		case top | left    : return HTTOPLEFT;
		case top | right   : return HTTOPRIGHT;
		case bottom | left : return HTBOTTOMLEFT;
		case bottom | right: return HTBOTTOMRIGHT;
		case client        : return HTCLIENT;
		default            : return HTNOWHERE;
		}
	}
}