// window-reset.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include <Windows.h>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, const LPARAM windows)
{
	if (IsWindowVisible(hwnd))
	{
		reinterpret_cast<std::vector<HWND>*>(windows)->push_back(hwnd);
	}
	
	return TRUE;
}

int main()
{
	std::vector<HWND> windows;
	windows.reserve(512);
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));

	for (const auto window : windows)
	{
		RECT rect;
		GetWindowRect(window, &rect);

		if (rect.right <= 0 || rect.left < -10)
		{
			SetWindowPos(window, nullptr, 1920 + rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	return EXIT_SUCCESS;
}