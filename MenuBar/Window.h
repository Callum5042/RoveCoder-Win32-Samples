#pragma once

#include <string>

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
};