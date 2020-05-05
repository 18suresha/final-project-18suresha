// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <windows.h>

#include "mylibrary/input_device.h"

namespace mylibrary {

InputDevice::InputDevice() : scroll_amount_{300} {
  ip_.type = INPUT_KEYBOARD;
  ip_.ki.wScan = 0;
  ip_.ki.time = 0;
  ip_.ki.dwExtraInfo = 0;

  ip1_.type = INPUT_KEYBOARD;
  ip1_.ki.wScan = 0;
  ip1_.ki.time = 0;
  ip1_.ki.dwExtraInfo = 0;

  ip2_.type = INPUT_KEYBOARD;
  ip2_.ki.wScan = 0;
  ip2_.ki.time = 0;
  ip2_.ki.dwExtraInfo = 0;

  ip_.mi.dwFlags = MOUSEEVENTF_WHEEL;
  ip_.mi.time = 0;
}

void InputDevice::SwitchTabsRight() {
  ip_.type = INPUT_KEYBOARD;
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

void InputDevice::SwitchTabsLeft() {
  ip_.type = INPUT_KEYBOARD;
  ip_.ki.wVk = VK_CONTROL;
  ip_.ki.dwFlags = 0;

  ip1_.ki.wVk = VK_TAB;
  ip1_.ki.dwFlags = 0;

  ip2_.ki.wVk = VK_SHIFT;
  ip2_.ki.dwFlags = 0;

  SendInput(1, &ip_, sizeof(INPUT));
  SendInput(1, &ip2_, sizeof(INPUT));
  SendInput(1, &ip1_, sizeof(INPUT));

  ip_.ki.dwFlags = KEYEVENTF_KEYUP;
  ip1_.ki.dwFlags = KEYEVENTF_KEYUP;
  ip2_.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &ip_, sizeof(INPUT));
  SendInput(1, &ip1_, sizeof(INPUT));
  SendInput(1, &ip2_, sizeof(INPUT));
}

void InputDevice::ScrollUp() {
  ip_.type = INPUT_MOUSE;
  ip_.mi.mouseData = 1 * scroll_amount_;
  SendInput(1, &ip_, sizeof(ip_));
}

void InputDevice::ScrollDown() {
  ip_.type = INPUT_MOUSE;
  ip_.mi.mouseData = -1 * scroll_amount_;
  SendInput(1, &ip_, sizeof(ip_));
}

} // namespace mylibrary
