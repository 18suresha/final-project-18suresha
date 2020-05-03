// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>
#include <vector>

#include "mylibrary/keyboard.h"

namespace mylibrary {

typedef int ColorToUse;

enum ColorToUse_ {
	ColorToUse_Blue = 0,
	ColorToUse_Red = 1
};

struct ColorRange {
	cv::Scalar lower;
	cv::Scalar upper;
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
		void StopOpenCV();
		void DisplayNeutralZone();
		bool IsNeutralZoneValid();
		cv::Size GetCamFrameSize();
		void SetNeutralZone(cv::Rect neutral_zone);
		cv::Rect GetNeutralZone();
		void SetColorToUse(ColorToUse color);

	private:
		cv::Size SetCamFrameSize();
		bool CheckNeutralStartingPoints();
		bool CheckNeutralWidth();
		bool CheckNeutralHeight();
		void AnalyzeSections(const cv::Mat& src_frame);
		void AnalyzeSection(Direction dir, const cv::Mat& src_frame);
		void AnalyzeSectionPixels(Direction dir, const cv::Mat& src_frame);
		void SetThresholds();
		cv::Mat FilterMat(const cv::Mat& src_frame) const;
		void AnalyzeFingerMovement();
		void OnKeyboardInput();

	private:
		bool analyze_video_;
		ColorToUse color_to_use_;
		cv::VideoCapture cap_;
		Keyboard keyboard_;
		const cv::Size cam_frame_size_;
		std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
		cv::Rect neutral_zone_;	
		std::map<Direction, cv::Rect> frame_dims_;
		std::map<Direction, double> section_thresholds_;
		std::map<Direction, std::vector<cv::KeyPoint>> section_keypoints_;
		std::map<Direction, int> section_pixels_;
		std::vector<ColorRange> color_ranges_;
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
