
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	UINT result = MessageBox(NULL, L"Do you want to save your work?", L"Exit Application", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (result)
	{
		case IDYES:
			MessageBox(NULL, L"Yes", L"Dialog Result", MB_OK | MB_ICONINFORMATION);
			break;

		case IDNO:
			MessageBox(NULL, L"No", L"Dialog Result", MB_OK | MB_ICONWARNING);
			break;

		case IDCANCEL:
			MessageBox(NULL, L"Cancel", L"Dialog Result", MB_OK | MB_ICONERROR);
			break;
	}

	return 0;
}