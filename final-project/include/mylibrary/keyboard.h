// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_KEYBOARD_H_
#define FINALPROJECT_MYLIBRARY_KEYBOARD_H_

#include <windows.h>

namespace mylibrary {

class Keyboard {
	public:
		Keyboard();
		void SwitchTabsRight();
		void ScrollUp();

	private:
		INPUT ip_;
		INPUT ip1_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_KEYBOARD_H_
