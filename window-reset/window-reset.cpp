// window-reset.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM)
{
	if (IsWindowVisible(hwnd))
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

		if (rect.right <= 0 || rect.left < -10)
		{
			SetWindowPos(hwnd, nullptr, 1920 + rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	
	return TRUE;
}

int main()
{
	EnumWindows(EnumWindowsProc, 0);

	return EXIT_SUCCESS;
}