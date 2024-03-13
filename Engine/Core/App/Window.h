#pragma once
#include "Core/Common/NuiWin.h"
#include "Core/Common/CommonHeaders.h"
#include <map>
#include <functional>

namespace Nui
{
	class Window
	{
	public:

		/**
		* @brief Window styles
		*/
		enum class Style
		{
			/**
			* @brief Create resizeable window
			*/
			Windowed,
			
			/**
			* @brief Create maximized resizeable window
			*/
			WindowedFullscreen,

			/**
			* @brief Create a non-resizeable window without border
			*/
			Borderless,

			/**
			* @brief Create maximized non-resizeable window without border
			*/
			BorderlessFullscreen,
		};

		/**
		* @brief Window size
		* @note This is in pixels
		*/
		struct Size
		{
			/**
			* @brief Width
			*/
			I32 X;

			/**
			* @brief Height
			*/
			I32 Y;
		};
		
		/**
		* @brief Window position
		* @note This is same as Window::Size
		* @see Nui::Window::Size
		*/
		using Position = Size;

#define NUI_WNDPROC_ARGS Nui::Window*, UINT, WPARAM, LPARAM
#define NUI_WNDPROC_NAMED_ARGS Nui::Window* window, UINT uMsg, WPARAM wParam, LPARAM lParam
		using WndCallback = std::function<LRESULT(NUI_WNDPROC_ARGS)>;

	public:

		/**
		* @brief Create a new window
		* @param style Window style
		* @param size Window size
		* @param position Window position
		* @param title Window title
		*/
		explicit Window(Window::Style style, Window::Size size, Window::Position position, StringW title);
		
		/**
		* @brief Destroy the window
		* @note This will close the window
		*/
		virtual ~Window();

		/**
		* @brief Get the window handle
		* @return Win32 Window handle
		*/
		HWND GetHWND() const;

		/**
		* @brief Get the Hinstance of the window
		* @return Hinstance
		*/
		HINSTANCE GetHinstance() const;

		/**
		* @brief Get the client size
		* @return Client size
		*/
		Size GetClientSize() const;

		/**
		* @brief Set the window title
		* @param title Window title
		*/
		void SetWindowTitle(StringW title);

		/**
		* @brief Check if the window is maximized
		* @return True if maximized
		*/
		bool Maximized() const;
		
		/**
		* @brief Runs the window message pump and checks if the window wants to close
		* @return True if the window wants to close
		*/
		bool WantsToClose() const;

		/**
		* @brief Add a window message callback
		* @param msg Window message
		* @param callback Callback
		* @note The callback will be called when the message is received
		*/
		void AddCallback(U32 msg, WndCallback callback);

	private:
		/**
		* @brief Adjusts the client rect of the window based on the maximized state
		* @param rect Client rect
		*/
		void AdjustMaximizedClientRect(RECT& rect);

		/**
		* @brief Creates the window
		* @note This should only be called once internally
		*/
		void MakeWindow();

		/**
		* @brief Calls internal message handler or any registered callbacks
		* @param hWnd Window handle
		* @param uMsg Message
		* @param wParam
		* @param lParam
		*/
		LRESULT	MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		/**
		* @brief Static internal default window procedurce
		* @param hWnd Window handle
		* @param uMsg Message
		* @param wParam
		* @param lParam
		*/
		static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		/**
		* @brief Internal message handler
		* @param hWnd Window handle
		* @param uMsg Message
		* @param wParam
		* @param lParam
		*/
		LRESULT	MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		/**
		* @brief Identifies borders and corners to allow resizing the window
		* @param cursor Cursor position
		* @return Hit test message
		*/
		LRESULT HitTest(POINT cursor);

	private:
		/**
		* @brief Window handle
		*/
		HWND      m_hWnd;

		/**
		* @brief Window Hinstance
		*/
		HINSTANCE m_hInstance;
		
		/**
		* @brief Window style
		*/
		Style     m_style;

		/**
		* @brief Window size
		*/
		Size      m_size;

		/**
		* @brief Window position
		*/
		Position  m_position;

		/**
		* @brief Window title
		*/
		StringW   m_title;

		/**
		* @brief Window message callbacks
		* @note The key is the message
		* @note The value is the callback
		* @note The callback will be called when the message is received in the MessageRouter
		*/
		std::map<U32, WndCallback> m_callbacks;
	};
}