// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>
#include <vector>
#include <initializer_list>

#include "mylibrary/keyboard.h"

namespace mylibrary {

typedef int ColorToUse;

enum ColorToUse_ {
	ColorToUse_Blue = 0,
	ColorToUse_Orange = 1,
	ColorToUse_Green = 2
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct Section {
	int x;
	int y;
	cv::Size frame_size;
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
		void SetColorToUse(ColorToUse color);

	private:
		cv::Size SetCamFrameSize();
		void StopOpenCV();
		bool CheckNeutralStartingPoints();
		bool CheckNeutralWidth();
		bool CheckNeutralHeight();
		void AnalyzeSections(const cv::Mat& src_frame);
		void AnalyzeSection(Direction dir, const cv::Mat& src_frame);
		void SetThresholds();
		cv::Mat FilterMat(const cv::Mat& src_frame) const;
		void AnalyzeFingerMovement();
		void OnKeyboardInput();

	private:
		bool analyze_video_;
		ColorToUse color_;
		cv::VideoCapture cap_;
		Keyboard keyboard_;
		const cv::Size cam_frame_size_;
		std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
		Section neutral_zone_;	
		std::initializer_list<Direction> directions_;
		std::map<Direction, Section> frame_dims_;
		std::map<Direction, double> section_thresholds_;
		std::map<Direction, std::vector<cv::KeyPoint>> section_keypoints_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
