#include "Window.h"
#include <windowsx.h>
#include <sstream>

Window::Window()
{
}

Window::~Window()
{
	DestroyWindow(m_Hwnd);
}

bool Window::Create(const std::wstring& title)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	const wchar_t className[] = L"MyWindowClass";

	// Register class
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = className;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Window::WindowProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"RegisterClassEx failed!", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Create window
	m_Hwnd = CreateWindowEx(0, className, title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, this);
	if (m_Hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(m_Hwnd, SW_SHOWDEFAULT);
	CreateMenuBar();

	return true;
}

void Window::CreateMenuBar()
{
	m_MenuBar = CreateMenu();

	// Create file popup menu
	m_FileMenuItem = CreateMenu();
	AppendMenuW(m_FileMenuItem, MF_STRING, m_MenuFileOpenId, L"Open");
	AppendMenuW(m_FileMenuItem, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(m_FileMenuItem, MF_STRING, m_MenuFileExitId, L"Exit");
	AppendMenuW(m_MenuBar, MF_POPUP, reinterpret_cast<UINT_PTR>(m_FileMenuItem), L"&File");

	// Options
	m_OptionsMenuItem = CreateMenu();
	AppendMenuW(m_OptionsMenuItem, MF_STRING | MF_CHECKED, m_MenuOptionsToggleCheckboxId, L"Toggle Checkbox");
	AppendMenuW(m_FileMenuItem, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(m_OptionsMenuItem, MF_STRING | MF_DISABLED, m_MenuOptionsDisabledId, L"Disabled");
	AppendMenuW(m_OptionsMenuItem, MF_STRING, m_MenuOptionsToggleDisabledId, L"Toggle Disabled");
	AppendMenuW(m_MenuBar, MF_POPUP, reinterpret_cast<UINT_PTR>(m_OptionsMenuItem), L"&Options");

	// Submenus
	m_SubmenuItem = CreateMenu();
	AppendMenuW(m_MenuBar, MF_POPUP, reinterpret_cast<UINT_PTR>(m_SubmenuItem), L"&Submenus");

	// Submenu 1
	m_Submenu1 = CreateMenu();
	AppendMenuW(m_SubmenuItem, MF_POPUP, reinterpret_cast<UINT_PTR>(m_Submenu1), L"Sub Menu 1");

	for (int i = 1; i <= 10; ++i)
	{
		std::wstring title = L"Item " + std::to_wstring(i);
		AppendMenuW(m_Submenu1, MF_STRING, m_SubMenu1Item + i, title.c_str());
	}

	// Submenu 2
	m_Submenu2 = CreateMenu();
	AppendMenuW(m_SubmenuItem, MF_POPUP, reinterpret_cast<UINT_PTR>(m_Submenu2), L"Sub Menu 2");

	for (int i = 1; i <= 10; ++i)
	{
		std::wstring title = L"Item " + std::to_wstring(i);
		AppendMenuW(m_Submenu2, MF_STRING, m_SubMenu2Item + i, title.c_str());
	}

	// Assign menubar to window
	SetMenu(m_Hwnd, m_MenuBar);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Set or get the pointer to the window class
	Window* window = nullptr;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else
	{
		window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	// Use default window procedure if we do not have the Window pointer yet
	if (window == nullptr)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	// Pass into window class to handle events
	return window->HandleEvents(uMsg, wParam, lParam);
}

LRESULT Window::HandleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			HandleMenu(msg, wParam, lParam);
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_Hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(m_Hwnd, &ps);
			break;
		}
	}

	return DefWindowProc(m_Hwnd, msg, wParam, lParam);
}

void Window::HandleMenu(UINT msg, WPARAM wParam, LPARAM lParam)
{
	const UINT menu_id = LOWORD(wParam);
	switch (menu_id)
	{
		// File Menu
		case m_MenuFileOpenId:
			MessageBox(NULL, L"File Open", L"Menu Clicked", MB_OK);
			break;
		// Checkbox Menu
		case m_MenuOptionsToggleCheckboxId:
		{
			UINT flag = GetMenuState(m_OptionsMenuItem, m_MenuOptionsToggleCheckboxId, MF_BYCOMMAND);
			if ((flag & MF_CHECKED) == MF_CHECKED)
			{
				CheckMenuItem(m_OptionsMenuItem, m_MenuOptionsToggleCheckboxId, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				CheckMenuItem(m_OptionsMenuItem, m_MenuOptionsToggleCheckboxId, MF_BYCOMMAND | MF_CHECKED);
			}

			break;
		}
		// Disabled
		case m_MenuOptionsDisabledId:
			MessageBox(NULL, L"Disabled MenuItem", L"Menu Clicked", MB_OK);
			break;
		// Toggle Disabled
		case m_MenuOptionsToggleDisabledId:
		{
			UINT flag = GetMenuState(m_OptionsMenuItem, m_MenuOptionsDisabledId, MF_BYCOMMAND);
			if ((flag & MF_DISABLED) == MF_DISABLED)
			{
				EnableMenuItem(m_OptionsMenuItem, m_MenuOptionsDisabledId, MF_ENABLED);
			}
			else
			{
				EnableMenuItem(m_OptionsMenuItem, m_MenuOptionsDisabledId, MF_DISABLED);
			}

			break;
		}
	}
}