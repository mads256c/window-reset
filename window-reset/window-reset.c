#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
//#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
//#define NOUSER            // All USER defines and routines
#define NONLS             // All NLS defines and routines
//#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
//#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

#include <Windows.h>
#include <stdlib.h>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM unused)
{
	if (IsWindowVisible(hwnd))
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

		HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);

		MONITORINFO monitor;
		monitor.cbSize = sizeof(monitor);
		GetMonitorInfo(hMonitor, &monitor);

		if (monitor.dwFlags & MONITORINFOF_PRIMARY) return TRUE;

		SetWindowPos(hwnd, NULL, -monitor.rcMonitor.left + rect.left, -monitor.rcMonitor.top + rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
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

	if (!hMutex)
	{
		return EXIT_FAILURE;
	}

	if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x4D))
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