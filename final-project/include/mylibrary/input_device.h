// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_KEYBOARD_H_
#define FINALPROJECT_MYLIBRARY_KEYBOARD_H_

#include <windows.h>

namespace mylibrary {

class InputDevice {
public:
  InputDevice();
  // Switches to the right tab in the current window.
  void SwitchTabsRight();
  // Switches to the left tab in the current window.
  void SwitchTabsLeft();
  // Scrolls up in the current window.
  void ScrollUp();
  // Scrolls down in the current window.
  void ScrollDown();

private:
  const size_t scroll_amount_;
  INPUT ip_;
  INPUT ip1_;
  INPUT ip2_;
};

} // namespace mylibrary

#endif // FINALPROJECT_MYLIBRARY_KEYBOARD_H_
