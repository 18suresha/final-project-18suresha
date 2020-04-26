// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include "mylibrary/keyboard.h"

namespace mylibrary {

class Engine {
	public:
		Engine();
		void RunOpenCV();

	private:
		Keyboard keyboard_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
