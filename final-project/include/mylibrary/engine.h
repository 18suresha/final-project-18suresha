// Copyright (c) 2020 Adarsh Suresh. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_ENGINE_H_
#define FINALPROJECT_MYLIBRARY_ENGINE_H_

#include <chrono>
#include <opencv2/opencv.hpp>
#include <vector>

#include "mylibrary/input_device.h"

namespace mylibrary {

typedef int ColorToUse;

enum ColorToUse_ {
  ColorToUse_Blue = 0,
  ColorToUse_Red = 1,
  ColorToUse_Pink = 2
};

struct ColorRange {
  cv::Scalar lower;
  cv::Scalar upper;
};

enum Direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

class Engine {
public:
  Engine();
  // Allows user to begin performing actions on windows by moving their finger.
  void StartFingerTracking();
  // Displays the four different sections of finger motion tracking along with
  // the neutral zone.
  void DisplaySections();
  // Checks if the dimensions of the neutral zone are valid.
  bool IsNeutralZoneValid() const;
  // Returns the dimensions of the frame produced by the main camera.
  cv::Size GetCamFrameSize() const;
  // Sets the neutral zone to the specified dimensions.
  void SetNeutralZone(cv::Rect neutral_zone);
  // Returns the current neutral zone dimensions.
  cv::Rect GetNeutralZone() const;
  // Sets the color to track and filter on the user's finger.
  void SetColorToUse(ColorToUse color);
  // Sets the dimensions of the different sections as per the dimensions of the
  // neutral zone and sets the thresholds of each section.
  void Calibrate();

private:
  cv::Size SetCamFrameSize();
  bool CheckNeutralStartingPoints() const;
  bool CheckNeutralWidth() const;
  bool CheckNeutralHeight() const;
  void AnalyzeSections(const cv::Mat &src_frame);
  void AnalyzeSection(Direction dir, const cv::Mat &src_frame);
  void SetThresholds();
  cv::Mat FilterMat(const cv::Mat &src_frame) const;
  void AnalyzeFingerMovement();
  void OnKeyboardInput();
  void SetSectionDims();
  void CloseOpenCVWindows();

private:
  bool analyze_video_;
  int num_threshold_samples_;
  int threshold_offset_;
  char display_exit_key_;
  double action_delay_secs_;
  ColorToUse color_to_use_;
  cv::VideoCapture cap_;
  InputDevice input_device_;
  const cv::Size cam_frame_size_;
  std::chrono::time_point<std::chrono::system_clock> prev_time_point_;
  cv::Rect neutral_zone_;
  std::vector<cv::Rect> section_dims_;
  std::vector<double> section_thresholds_;
  std::vector<int> section_pixels_;
  std::vector<ColorRange> color_ranges_;
};

} // namespace mylibrary

#endif // FINALPROJECT_MYLIBRARY_ENGINE_H_
