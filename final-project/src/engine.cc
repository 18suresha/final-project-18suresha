// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <chrono>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

#include "mylibrary/engine.h"

using cv::Mat;
using cv::Rect;
using cv::Scalar;
using cv::Size;
using std::thread;

namespace mylibrary {

Engine::Engine()
    : analyze_video_{false}, color_to_use_{ColorToUse_Blue}, cap_{0},
      action_delay_secs_{0.5}, input_device_{},
      prev_time_point_{std::chrono::system_clock::now()},
      cam_frame_size_{SetCamFrameSize()}, section_thresholds_{0, 0, 0, 0},
      section_dims_{Rect{}, Rect{}, Rect{}, Rect{}}, section_pixels_{0, 0, 0,
                                                                     0},
      color_ranges_{ColorRange{Scalar(105, 150, 80), Scalar(118, 255, 225)},
                    ColorRange{Scalar(0, 120, 120), Scalar(5, 255, 225)},
                    ColorRange{Scalar(170, 120, 120), Scalar(180, 255, 255)}},
      num_threshold_samples_{150}, threshold_offset_{100}, display_exit_key_{
                                                               'q'} {
  // default dimensions for neutral zone based on camera frame's dimensions
  neutral_zone_ =
      Rect{cam_frame_size_.width / 3, 5 * cam_frame_size_.height / 12,
           7 * cam_frame_size_.width / 24, cam_frame_size_.height / 4};
  Calibrate();
}

// Calculates the average number of pixels of the user-specified color present
// in each of the four sections.
void Engine::SetThresholds() {
  if (!cap_.isOpened()) {
    return;
  }
  section_thresholds_[UP] = 0;
  section_thresholds_[RIGHT] = 0;
  section_thresholds_[DOWN] = 0;
  section_thresholds_[LEFT] = 0;

  for (int i = 0; i < num_threshold_samples_; i++) {
    Mat frame;
    cap_ >> frame;
    Mat filter_frame = FilterMat(frame);
    thread up(&Engine::AnalyzeSection, this, UP, filter_frame);
    thread right(&Engine::AnalyzeSection, this, RIGHT, filter_frame);
    thread down(&Engine::AnalyzeSection, this, DOWN, filter_frame);
    thread left(&Engine::AnalyzeSection, this, LEFT, filter_frame);
    up.join();
    right.join();
    down.join();
    left.join();
    section_thresholds_[UP] += section_pixels_[UP];
    section_thresholds_[RIGHT] += section_pixels_[RIGHT];
    section_thresholds_[DOWN] += section_pixels_[DOWN];
    section_thresholds_[LEFT] += section_pixels_[LEFT];
  }
  section_thresholds_[UP] /= num_threshold_samples_;
  section_thresholds_[RIGHT] /= num_threshold_samples_;
  section_thresholds_[DOWN] /= num_threshold_samples_;
  section_thresholds_[LEFT] /= num_threshold_samples_;
}

// Calculates the dimensions of each of the four sections based on the
// dimensions of the neutral zone.
void Engine::SetSectionDims() {
  section_dims_[UP] =
      Rect{neutral_zone_.x, 0, neutral_zone_.width, neutral_zone_.y};
  section_dims_[RIGHT] =
      Rect{0, neutral_zone_.y, neutral_zone_.x, neutral_zone_.height};
  section_dims_[DOWN] = Rect{
      neutral_zone_.x, neutral_zone_.y + neutral_zone_.height,
      neutral_zone_.width,
      (cam_frame_size_.height) - ((neutral_zone_.y + neutral_zone_.height))};
  section_dims_[LEFT] =
      Rect{neutral_zone_.x + neutral_zone_.width, neutral_zone_.y,
           (cam_frame_size_.width) - ((neutral_zone_.x + neutral_zone_.width)),
           neutral_zone_.height};
}

void Engine::Calibrate() {
  SetSectionDims();
  SetThresholds();
}

// Returns the dimensions of a frame produced by the main camera.
Size Engine::SetCamFrameSize() {
  if (!cap_.isOpened()) {
    return Size{0, 0};
  }
  Mat frame;
  cap_ >> frame;
  Size frame_size = frame.size();
  return frame_size;
}

Size Engine::GetCamFrameSize() const { return cam_frame_size_; }

Rect Engine::GetNeutralZone() const { return neutral_zone_; }

// Returns a frame in which the pixels of the user-specified color are white and
// all other pixels are black.
Mat Engine::FilterMat(const Mat &src_frame) const {
  Mat hsv_frame, filter_frame;
  cvtColor(src_frame, hsv_frame, cv::COLOR_BGR2HSV);
  inRange(hsv_frame, color_ranges_[color_to_use_].lower,
          color_ranges_[color_to_use_].upper, filter_frame);
  return filter_frame;
}

// Calculate the number of pixels containing the user-specified color in each
// section.
void Engine::AnalyzeSections(const cv::Mat &src_frame) {
  Mat filter_frame = FilterMat(src_frame);
  thread up(&Engine::AnalyzeSection, this, UP, filter_frame);
  thread right(&Engine::AnalyzeSection, this, RIGHT, filter_frame);
  thread down(&Engine::AnalyzeSection, this, DOWN, filter_frame);
  thread left(&Engine::AnalyzeSection, this, LEFT, filter_frame);
  up.join();
  right.join();
  down.join();
  left.join();
}

// Calculate the number of pixels containing the user-specified color in the
// specified section.
void Engine::AnalyzeSection(Direction dir, const Mat &src_frame) {
  Mat cropped_frame = src_frame(section_dims_[dir]);
  section_pixels_[dir] = cv::countNonZero(cropped_frame);
}

// Prevents the application from analyzing finger movement right after the user
// performs a keyboard/mouse action with their finger.
void Engine::OnKeyboardInput() {
  prev_time_point_ = std::chrono::system_clock::now();
  analyze_video_ = false;
}

void Engine::SetColorToUse(ColorToUse color) { color_to_use_ = color; }

// Performs the specific keyboard action corresponding to the direction in which
// the user's finger moved.
void Engine::AnalyzeFingerMovement() {
  if (section_pixels_[UP] >
      threshold_offset_ + ((int)section_thresholds_[UP])) {
    input_device_.ScrollUp();
    OnKeyboardInput();
  } else if (section_pixels_[RIGHT] >
             threshold_offset_ + ((int)section_thresholds_[RIGHT])) {
    input_device_.SwitchTabsRight();
    OnKeyboardInput();
  } else if (section_pixels_[LEFT] >
             threshold_offset_ + ((int)section_thresholds_[LEFT])) {
    input_device_.SwitchTabsLeft();
    OnKeyboardInput();
  } else if (section_pixels_[DOWN] >
             threshold_offset_ + ((int)section_thresholds_[DOWN])) {
    input_device_.ScrollDown();
    OnKeyboardInput();
  }
}

void Engine::StartFingerTracking() {
  if (!cap_.isOpened()) {
    return;
  }
  if (analyze_video_) {
    Mat frame;
    cap_ >> frame;
    AnalyzeSections(frame);
    AnalyzeFingerMovement();
  }
  if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                                    prev_time_point_)
          .count() > action_delay_secs_) {
    analyze_video_ = true;
  }
}
void Engine::DisplaySections() {
  if (!cap_.isOpened()) {
    return;
  }
  while (1) {
    Mat frame;
    cap_ >> frame;
    // draw black rectangles around different sections
    cv::rectangle(frame, neutral_zone_, Scalar(0, 0, 0));
    cv::rectangle(frame, section_dims_[UP], Scalar(0, 0, 0));
    cv::rectangle(frame, section_dims_[RIGHT], Scalar(0, 0, 0));
    cv::rectangle(frame, section_dims_[DOWN], Scalar(0, 0, 0));
    cv::rectangle(frame, section_dims_[LEFT], Scalar(0, 0, 0));
    imshow("Neutral Zone", frame);
    if (cv::waitKey(30) == display_exit_key_) {
      CloseOpenCVWindows();
      break;
    }
  }
}

// Closes windows displaying video capture split into sections.
void Engine::CloseOpenCVWindows() { cv::destroyAllWindows(); }

// Returns whether the neutral zone starting points are valid.
bool Engine::CheckNeutralStartingPoints() const {
  return neutral_zone_.x >= 0 && neutral_zone_.y >= 0;
}

// Returns whether the neutral zone width is valid.
bool Engine::CheckNeutralWidth() const {
  return (neutral_zone_.width > 0) &&
         (neutral_zone_.x + neutral_zone_.width <= cam_frame_size_.width);
}

// Returns whether the neutral zone height is valid.
bool Engine::CheckNeutralHeight() const {
  return (neutral_zone_.height > 0) &&
         (neutral_zone_.y + neutral_zone_.height <= cam_frame_size_.height);
}

bool Engine::IsNeutralZoneValid() const {
  return CheckNeutralStartingPoints() && CheckNeutralWidth() &&
         CheckNeutralHeight();
}

void Engine::SetNeutralZone(Rect neutral_zone) { neutral_zone_ = neutral_zone; }

} // namespace mylibrary
