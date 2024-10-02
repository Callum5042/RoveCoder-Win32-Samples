#pragma once

#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Window
{
public:
	Window();
	virtual ~Window();

	bool Create(const std::wstring& title);

private:
	HWND m_Hwnd = NULL;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleEvents(UINT msg, WPARAM wParam, LPARAM lParam);

	// Menubar
	void CreateMenuBar();
	void HandleMenu(UINT msg, WPARAM wParam, LPARAM lParam);
	HMENU m_MenuBar = NULL;

	HMENU m_FileMenuItem = NULL;
	static const UINT m_MenuFileOpenId = 101;
	static const UINT m_MenuFileExitId = 102;

	HMENU m_OptionsMenuItem = NULL;
	static const UINT m_MenuOptionsToggleCheckboxId = 201;
	static const UINT m_MenuOptionsDisabledId = 202;
	static const UINT m_MenuOptionsToggleDisabledId = 203;

	HMENU m_SubmenuItem = NULL;

	HMENU m_Submenu1 = NULL;
	static const UINT m_SubMenu1Item = 3100;

	HMENU m_Submenu2 = NULL;
	static const UINT m_SubMenu2Item = 4100;
};