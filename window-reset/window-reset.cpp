// window-reset.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include <Windows.h>

std::vector<HWND> windows;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM)
{
	windows.push_back(hwnd);

	return TRUE;
}

int main()
{
	EnumWindows(EnumWindowsProc, NULL);

	for (HWND window : windows)
	{
		if (!IsWindowVisible(window)) continue;

		RECT rect;
		GetWindowRect(window, &rect);

		if (rect.right <= 0 || rect.left < -10)
		{
			SetWindowPos(window, NULL, 1920 + rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	return 0;
}