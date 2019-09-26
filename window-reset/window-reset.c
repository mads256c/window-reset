#include "slim_win32.h"
#include <stdlib.h>
#include <stdio.h>

#include "config.h"


//Called for each window when calling EnumWindows.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM unused)
{
	if (IsWindowVisible(hwnd))
	{
		RECT rect;
		//If this function errors out goto next window.
		if (!GetWindowRect(hwnd, &rect)){
			char title[256];
			GetWindowTextA(hwnd, title, sizeof(title));

			printf("GetWindowRect failed on window: %sError: %lu\n", title, GetLastError());
			return TRUE;
		}

		HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);

		MONITORINFO monitor;
		monitor.cbSize = sizeof(monitor);
		if (!GetMonitorInfo(hMonitor, &monitor)){
			char title[256];
			GetWindowTextA(hwnd, title, sizeof(title));

			printf("GetMonitorInfo failed on window: %s Error: %lu\n", title, GetLastError());
			return TRUE;
		}

		if (monitor.dwFlags & MONITORINFOF_PRIMARY) return TRUE;

		
		if (!SetWindowPos(hwnd, NULL, -monitor.rcMonitor.left + rect.left, -monitor.rcMonitor.top + rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE))
		{
			char title[256];
			GetWindowTextA(hwnd, title, sizeof(title));

			printf("SetWindowPos failed on window: %s Error: %lu\n", title, GetLastError());
			return TRUE;
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
	
#ifndef NDEBUG
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	printf("Debug mode: ENABLED\n");
#endif
	
	HANDLE hMutex = OpenMutex(
		MUTEX_ALL_ACCESS, 0, L"window-reset");

	if (hMutex)
	{
		MessageBoxA(NULL, "Another instance of this program is running.", "Window Reset", MB_OK);
		return EXIT_FAILURE;
	}

	hMutex = CreateMutex(0, 0, L"window-reset");

	if (!hMutex)
	{
		return EXIT_FAILURE;
	}
	
	if (!RegisterHotKey(NULL, 1, modifiers, hotkey))
	{
		ReleaseMutex(hMutex);
		MessageBoxA(NULL, "Failed to create hotkey.", "Window Reset", MB_OK);
		return EXIT_FAILURE;
	}

	MSG msg = { 0, 0, 0, 0, 0, {0, 0} };
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