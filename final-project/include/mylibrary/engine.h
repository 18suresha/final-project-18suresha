// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>

#include "mylibrary/keyboard.h"

namespace mylibrary {

struct FrameSize {
	int width;
	int height;
};

struct NeutralZone {
	int x;
	int y;
	FrameSize frame_size;
};

class Engine {
	public:
		Engine();
		void RunOpenCV();
		void DisplayNeutralZone();
		bool IsNeutralZoneValid();
		FrameSize GetCamFrameSize();
		void SetNeutralZone(int x, int y, int width, int height);

	private:
		void SetCamFrameSize();
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
		FrameSize cam_frame_size_;
		std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
		NeutralZone neutral_zone_;
		bool CheckNeutralStartingPoints();
		bool CheckNeutralWidth();
		bool CheckNeutralHeight();
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
