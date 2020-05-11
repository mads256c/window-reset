#include "slim_win32.h"
#ifndef NDEBUG
#include <consoleapi.h>
#include <stdio.h>
#endif
#include <stdlib.h>

#include "hotkey.h"

_Releases_lock_(hMutex) int CALLBACK
    WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nCmdShow) {
  UNREFERENCED_PARAMETER(hInstance);
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

#ifndef NDEBUG
  AllocConsole();
  freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
  printf("Debug mode: ENABLED\r\n");
#endif

  HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, "window-reset");

  if (hMutex) {
    MessageBoxA(NULL, "Another instance of this program is running.",
                "Window Reset", MB_OK);
    return EXIT_FAILURE;
  }

  hMutex = CreateMutexA(0, 0, "window-reset");

  if (!hMutex)
    return EXIT_FAILURE;

  if (!register_hotkeys())
    return EXIT_FAILURE;

  MSG msg = {0, 0, 0, 0, 0, {0, 0}};
  while (GetMessage(&msg, NULL, 0, 0) != 0) {
    if (msg.message == WM_HOTKEY) {
      EnumWindows(enum_windows_proc, msg.wParam);
    }
  }

  ReleaseMutex(hMutex);

  return EXIT_SUCCESS;
}
