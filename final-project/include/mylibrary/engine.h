// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>

#include "mylibrary/keyboard.h"

namespace mylibrary {

class Engine {
	public:
		Engine();
		void RunOpenCV();

	private:
		void StopOpenCV();
		void SetPrevTimePoint(
			const std::chrono::time_point<std::chrono::system_clock>& time_point) {
			prev_time_point_ = time_point;
		}
		std::chrono::time_point<std::chrono::system_clock> GetPrevTimePoint() const {
			return prev_time_point_;
		}

	private:
		bool analyze_video_;
		cv::VideoCapture cap_;
		Keyboard keyboard_;
		std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
