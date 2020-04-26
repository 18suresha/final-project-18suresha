// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <windows.h>

#include "mylibrary/keyboard.h"

namespace mylibrary {

    Keyboard::Keyboard() {
        ip_.type = INPUT_KEYBOARD;
        ip_.ki.wScan = 0;
        ip_.ki.time = 0;
        ip_.ki.dwExtraInfo = 0;
        ip1_.type = INPUT_KEYBOARD;
        ip1_.ki.wScan = 0;
        ip1_.ki.time = 0;
        ip1_.ki.dwExtraInfo = 0;
    }

	void Keyboard::SwitchTabsRight() {
        // Sleep(2000);
        // switch tabs
        ip_.ki.wVk = VK_CONTROL;
        ip_.ki.dwFlags = 0;
        ip1_.ki.wVk = VK_TAB;
        ip1_.ki.dwFlags = 0;
        SendInput(1, &ip_, sizeof(INPUT));
        SendInput(1, &ip1_, sizeof(INPUT));
        ip_.ki.dwFlags = KEYEVENTF_KEYUP;
        ip1_.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip_, sizeof(INPUT));
        SendInput(1, &ip1_, sizeof(INPUT));
	}


}  // namespace mylibrary
