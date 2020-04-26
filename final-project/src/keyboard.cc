// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <windows.h>

#include "mylibrary/keyboard.h"

namespace mylibrary {

    Keyboard::Keyboard() {}

	void Keyboard::SwitchTabsRight() {
        INPUT ip;
        INPUT ip1;
        Sleep(5000);
        // switch tabs
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wVk = VK_CONTROL;
        ip.ki.dwFlags = 0;
        ip1.type = INPUT_KEYBOARD;
        ip1.ki.wScan = 0;
        ip1.ki.time = 0;
        ip1.ki.dwExtraInfo = 0;
        ip1.ki.wVk = VK_TAB;
        ip1.ki.dwFlags = 0;
        SendInput(1, &ip, sizeof(INPUT));
        SendInput(1, &ip1, sizeof(INPUT));
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        ip1.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        SendInput(1, &ip1, sizeof(INPUT));
	}


}  // namespace mylibrary
