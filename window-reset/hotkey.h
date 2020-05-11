#pragma once

#include "slim_win32.h"

BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM id);

BOOL register_hotkeys(void);