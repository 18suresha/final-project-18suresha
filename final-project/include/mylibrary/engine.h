// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>
#include <vector>

#include "mylibrary/keyboard.h"

namespace mylibrary {

struct Section {
	int x;
	int y;
	cv::Size frame_size;
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Engine {
	public:
		Engine();
		void RunOpenCV();
		void DisplayNeutralZone();
		bool IsNeutralZoneValid();
		cv::Size GetCamFrameSize();
		void SetNeutralZone(Section neutral_zone);
		Section GetNeutralZone();

	private:
		cv::Size SetCamFrameSize();
		void StopOpenCV();
		bool CheckNeutralStartingPoints();
		bool CheckNeutralWidth();
		bool CheckNeutralHeight();
		void AnalyzeSection(Direction dir, const cv::Mat& src_frame);
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
		const cv::Size cam_frame_size_;
		std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
		Section neutral_zone_;	
		std::map<Direction, Section> frame_dims;
		std::map<Direction, std::vector<cv::KeyPoint>> section_keypoints;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
