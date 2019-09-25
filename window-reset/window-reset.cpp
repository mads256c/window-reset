// window-reset.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include <Windows.h>

std::vector<HWND> windows;

BOOL CALLBACK EnumWindowsProc(const HWND hwnd, LPARAM)
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

    	if (rect.left < 0)
    	{
    		SetWindowPos(window, HWND_NOTOPMOST, 0, 0, -rect.left + rect.right, -rect.top + rect.bottom, NULL);
    	}
    }

	return 0;
}