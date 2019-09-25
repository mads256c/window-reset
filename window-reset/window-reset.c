#include <Windows.h>
#include <stdlib.h>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM unused)
{
	if (IsWindowVisible(hwnd))
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

		if (rect.right <= 0 || rect.left < -10)
		{
			SetWindowPos(hwnd, NULL, 1920 + rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	return TRUE;
}

_Releases_lock_(hMutex)
int CALLBACK WinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPSTR       lpCmdLine,
    int         nCmdShow
    )
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	HANDLE hMutex = OpenMutex(
      MUTEX_ALL_ACCESS, 0, L"window-reset");

	if (hMutex)
	{
		MessageBoxA(NULL, "Another instance of this program is running.", "Window Reset", MB_OK);
		return EXIT_FAILURE;
	}
	
	hMutex = CreateMutex(0, 0, L"window-reset");


	if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x4D))
	{
		MessageBoxA(NULL, "Failed to create hotkey.", "Window Reset", MB_OK);
		return EXIT_FAILURE;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			EnumWindows(EnumWindowsProc, 0);
		}
	}

	ReleaseMutex(hMutex);
	return EXIT_SUCCESS;
}