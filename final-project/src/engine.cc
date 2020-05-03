// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <chrono>
#include <cinder/app/App.h>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

#include "mylibrary/engine.h"

using namespace cv;
using std::thread;

namespace mylibrary {

Engine::Engine()
    : analyze_video_{false}, color_to_use_{ColorToUse_Blue}, cap_{0},
      keyboard_{}, prev_time_point_{std::chrono::system_clock::now()},
      cam_frame_size_{SetCamFrameSize()}, section_thresholds_{{UP, 0},
                                                              {RIGHT, 0},
                                                              {DOWN, 0},
                                                              {LEFT, 0}},
      section_pixels_{{UP, 0}, {RIGHT, 0}, {DOWN, 0}, {LEFT, 0}},
      color_ranges_{ColorRange{Scalar(0, 150, 100), Scalar(4, 255, 225)},
                    ColorRange{Scalar(0, 150, 100), Scalar(4, 255, 225)}} {
  neutral_zone_ =
      Rect{cam_frame_size_.width / 3, 5 * cam_frame_size_.height / 12,
           7 * cam_frame_size_.width / 24, cam_frame_size_.height / 4};
  frame_dims_[UP] =
      Rect{neutral_zone_.x, 0, neutral_zone_.width, neutral_zone_.y};
  frame_dims_[RIGHT] =
      Rect{0, neutral_zone_.y, neutral_zone_.x, neutral_zone_.height};
  frame_dims_[DOWN] = Rect{
      neutral_zone_.x, neutral_zone_.y + neutral_zone_.height,
      neutral_zone_.width,
      (cam_frame_size_.height) - ((neutral_zone_.y + neutral_zone_.height))};
  frame_dims_[LEFT] =
      Rect{neutral_zone_.x + neutral_zone_.width, neutral_zone_.y,
           (cam_frame_size_.width) - ((neutral_zone_.x + neutral_zone_.width)),
           neutral_zone_.height};
  SetThresholds();
}

void Engine::SetThresholds() {
  if (!cap_.isOpened()) {
    return;
  }
  for (int i = 0; i < 150; i++) {
    Mat frame;
    cap_ >> frame;
    Mat filter_frame = FilterMat(frame);
    thread up(&Engine::AnalyzeSectionPixels, this, UP, filter_frame);
    thread right(&Engine::AnalyzeSectionPixels, this, RIGHT, filter_frame);
    thread down(&Engine::AnalyzeSectionPixels, this, DOWN, filter_frame);
    thread left(&Engine::AnalyzeSectionPixels, this, LEFT, filter_frame);
    up.join();
    right.join();
    down.join();
    left.join();
    section_thresholds_[UP] += section_pixels_[UP];
    section_thresholds_[RIGHT] += section_pixels_[RIGHT];
    section_thresholds_[DOWN] += section_pixels_[DOWN];
    section_thresholds_[LEFT] += section_pixels_[LEFT];
  }
  section_thresholds_[UP] /= 150.0;
  section_thresholds_[RIGHT] /= 150.0;
  section_thresholds_[DOWN] /= 150.0;
  section_thresholds_[LEFT] /= 150.0;
}

Size Engine::SetCamFrameSize() {
  if (!cap_.isOpened()) {
    return Size{0, 0};
  }
  Mat frame;
  cap_ >> frame;
  Size frame_size = frame.size();
  return frame_size;
}

Size Engine::GetCamFrameSize() { return cam_frame_size_; }

Rect Engine::GetNeutralZone() { return neutral_zone_; }

Mat Engine::FilterMat(const Mat &src_frame) const {
  Mat hsv_frame, filter_frame;
  cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
  inRange(hsv_frame, color_ranges_[color_to_use_].lower,
          color_ranges_[color_to_use_].upper, filter_frame);
  return filter_frame;
}

void Engine::AnalyzeSections(const cv::Mat &src_frame) {
  Mat filter_frame = FilterMat(src_frame);
  thread up(&Engine::AnalyzeSectionPixels, this, UP, filter_frame);
  thread right(&Engine::AnalyzeSectionPixels, this, RIGHT, filter_frame);
  thread down(&Engine::AnalyzeSectionPixels, this, DOWN, filter_frame);
  thread left(&Engine::AnalyzeSectionPixels, this, LEFT, filter_frame);
  up.join();
  right.join();
  down.join();
  left.join();
}

void Engine::AnalyzeSection(Direction dir, const Mat &src_frame) {
  Mat cropped_frame = src_frame(frame_dims_[dir]);
  FAST(cropped_frame, section_keypoints_[dir], 0, false);
}

void Engine::AnalyzeSectionPixels(Direction dir, const Mat &src_frame) {
  Mat cropped_frame = src_frame(frame_dims_[dir]);
  section_pixels_[dir] = cv::countNonZero(cropped_frame);
}

void Engine::OnKeyboardInput() {
  prev_time_point_ = std::chrono::system_clock::now();
  analyze_video_ = false;
}

void Engine::SetColorToUse(ColorToUse color) {
  if (color_to_use_ != color) {
    color_to_use_ = color;
    SetThresholds();
  } else {
    color_to_use_ = color;
  }
}

void Engine::AnalyzeFingerMovement() {
  if (section_pixels_[UP] > 100 + ((int)section_thresholds_[UP])) {
    keyboard_.ScrollUp();
    OnKeyboardInput();
  } else if (section_pixels_[RIGHT] > 100 + ((int)section_thresholds_[RIGHT])) {
    keyboard_.SwitchTabsRight();
    OnKeyboardInput();
  } else if (section_pixels_[LEFT] > 100 + ((int)section_thresholds_[LEFT])) {
    keyboard_.SwitchTabsLeft();
    OnKeyboardInput();
  } else if (section_pixels_[DOWN] > 100 + ((int)section_thresholds_[DOWN])) {
    keyboard_.ScrollDown();
    OnKeyboardInput();
  }
}

void Engine::RunOpenCV() {
  if (!cap_.isOpened()) {
    return;
  }
  int counter = 0;
  // while (1) {
  if (analyze_video_) {
    Mat frame;
    cap_ >> frame;
    AnalyzeSections(frame);
    AnalyzeFingerMovement();
  }
  if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                                    prev_time_point_)
          .count() > 0.5) {
    analyze_video_ = true;
  }
}
void Engine::DisplayNeutralZone() {
  if (!cap_.isOpened()) {
    return;
  }
  while (1) {
    Mat frame;
    cap_ >> frame;
    cv::rectangle(frame, neutral_zone_, Scalar(0, 0, 0));
    cv::rectangle(frame, frame_dims_[UP], Scalar(0, 0, 0));
    cv::rectangle(frame, frame_dims_[RIGHT], Scalar(0, 0, 0));
    cv::rectangle(frame, frame_dims_[DOWN], Scalar(0, 0, 0));
    cv::rectangle(frame, frame_dims_[LEFT], Scalar(0, 0, 0));
    imshow("Neutral Zone", frame);
    if (waitKey(30) == 'q') {
      StopOpenCV();
      break;
    }
  }
}

void Engine::StopOpenCV() { destroyAllWindows(); }

bool Engine::CheckNeutralStartingPoints() {
  return neutral_zone_.x >= 0 && neutral_zone_.y >= 0;
}

bool Engine::CheckNeutralWidth() {
  return (neutral_zone_.width > 0) &&
         (neutral_zone_.x + neutral_zone_.width <= cam_frame_size_.width);
}

bool Engine::CheckNeutralHeight() {
  return (neutral_zone_.height > 0) &&
         (neutral_zone_.y + neutral_zone_.height <= cam_frame_size_.height);
}

bool Engine::IsNeutralZoneValid() {
  return CheckNeutralStartingPoints() && CheckNeutralWidth() &&
         CheckNeutralHeight();
}

void Engine::SetNeutralZone(Rect neutral_zone) { neutral_zone_ = neutral_zone; }

} // namespace mylibrary
