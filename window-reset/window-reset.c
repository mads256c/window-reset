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

int main()
{
	if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x4D))
	{
		MessageBoxA(NULL, "Shit does not fucking work", "Window Reset", MB_OK);
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

	return EXIT_SUCCESS;
}