#include "hotkey.h"
#include "config.h"
#include "slim_win32.h"

#include <stdio.h>

#define ID_ALL ' all'
#define ID_LEFT 'left'
#define ID_RIGHT 'rght'

/**
 * \brief Registers a hotkey.
 * \param hotkey The VK key to register.
 * \param modifiers The modifiers to use.
 * \param id The id of the hotkey.
 * \returns True if the hotkey got registered.
 */
BOOL register_hotkey(const UINT hotkey, const UINT modifiers, const LPARAM id) {
  if (!RegisterHotKey(NULL, id, modifiers, hotkey)) {
    const char c1 = (char)((id >> 0) & 0xFF);
    const char c2 = (char)((id >> 1) & 0xFF);
    const char c3 = (char)((id >> 2) & 0xFF);
    const char c4 = (char)((id >> 3) & 0xFF);

    char buffer[512];

    const DWORD error = GetLastError();

    LPSTR error_string = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                       FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&error_string, 0, NULL);

    sprintf_s(buffer, sizeof(buffer),
              "Failed to register hotkey with id: %c%c%c%c\r\nError: \"%s\" "
              "Code: %lu",
              c1, c2, c3, c4, error_string, error);

    LocalFree(error_string);

    puts(buffer);

    MessageBoxA(NULL, buffer, "window-reset", MB_ICONERROR);

    return FALSE;
  }

  return TRUE;
}

// Called for each window when calling EnumWindows.
BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM id) {
  // Don't do anything if the window is not visible.
  if (!IsWindowVisible(hwnd))
    return TRUE;

  // Ignore the taskbar
  char title[256];
  GetClassNameA(hwnd, title, sizeof(title));
  if (strcmp("Shell_TrayWnd", title) == 0 ||
      strcmp("Shell_SecondaryTrayWnd", title) == 0) {
    return TRUE;
  }

  MONITORINFO monitor;
  monitor.cbSize = sizeof(monitor);
  // Get monitorinfo from the monitor the window is on.
  if (!GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY),
                      &monitor)) {
    GetWindowTextA(hwnd, title, sizeof(title));

    printf("GetMonitorInfo failed on window: %s Error: %lu\n", title,
           GetLastError());
    return TRUE;
  }

  // Don't do anything if we already are on the primary display.
  if (monitor.dwFlags & MONITORINFOF_PRIMARY)
    return TRUE;

  RECT rect;
  // If this function errors out goto next window.
  if (!GetWindowRect(hwnd, &rect)) {
    GetWindowTextA(hwnd, title, sizeof(title));

    printf("GetWindowRect failed on window: %sError: %lu\n", title,
           GetLastError());
    return TRUE;
  }

  switch (id) {
    // Don't do anything if the window is not on the left monitor.
  case ID_LEFT:
    if (monitor.rcMonitor.left >= 0)
      return TRUE;
    break;
  case ID_RIGHT:
    // Don't do anything if the window is not on the right monitor.
    if (monitor.rcMonitor.left <= 0)
      return TRUE;
    break;
  default:
    break;
  }

  if (!SetWindowPos(hwnd, NULL, -monitor.rcMonitor.left + rect.left,
                    -monitor.rcMonitor.top + rect.top, 0, 0,
                    SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE)) {
    GetWindowTextA(hwnd, title, sizeof(title));

    printf("SetWindowPos failed on window: %s Error: %lu\n", title,
           GetLastError());
    return TRUE;
  }

  return TRUE;
}

BOOL register_hotkeys(void) {
  if (!register_hotkey(all_hotkey, all_modifiers | MOD_NOREPEAT, ID_ALL))
    return FALSE;
  if (!register_hotkey(left_hotkey, left_modifiers | MOD_NOREPEAT, ID_LEFT))
    return FALSE;
  if (!register_hotkey(right_hotkey, right_modifiers | MOD_NOREPEAT, ID_RIGHT))
    return FALSE;

  return TRUE;
}
