#pragma once
/*
 * This is the configuration header for window-reset.
 * This way of configuring the program is inspired from the suckless.org
 * utilities.
 */

// ONLY EDIT BELOW THIS LINE

// Reset all windows hotkey
const static UINT all_hotkey = 'M';
const static UINT all_modifiers = MOD_CONTROL | MOD_ALT;

// Reset all windows on left display hotkey
const static UINT left_hotkey = VK_LEFT;
const static UINT left_modifiers = MOD_CONTROL | MOD_ALT;

// Reset all windows on right display hotkey
const static UINT right_hotkey = VK_RIGHT;
const static UINT right_modifiers = MOD_CONTROL | MOD_ALT;
