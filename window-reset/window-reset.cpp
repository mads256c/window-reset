// window-reset.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include <Windows.h>

std::vector<HWND> windows;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM)
{
	if (IsWindowVisible(hwnd))
	{
		windows.push_back(hwnd);
	}
	
	return TRUE;
}

int main()
{
	windows.reserve(512);
	EnumWindows(EnumWindowsProc, NULL);

	for (const auto window : windows)
	{
		RECT rect;
		GetWindowRect(window, &rect);

		if (rect.right <= 0 || rect.left < -10)
		{
			SetWindowPos(window, NULL, 1920 + rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	return EXIT_SUCCESS;
}