// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <chrono>
#include <vector>
#include <thread>
#include <initializer_list>
#include <cinder/app/App.h>

#include "mylibrary/engine.h"

using namespace cv;
using std::thread;

namespace mylibrary {

    Engine::Engine() : analyze_video_{ false }, cap_{ 0 }, keyboard_{}, prev_time_point_{ std::chrono::system_clock::now() }, cam_frame_size_{ SetCamFrameSize() }, section_thresholds_{ {UP, 0}, {RIGHT, 0}, {DOWN, 0}, {LEFT, 0} } {
        neutral_zone_.x = cam_frame_size_.width / 3;
        neutral_zone_.frame_size.width = cam_frame_size_.width / 3;
        neutral_zone_.y = 7 * cam_frame_size_.height / 12;
        neutral_zone_.frame_size.height = cam_frame_size_.height / 5;
        frame_dims_[UP] = Section{ neutral_zone_.x, neutral_zone_.y / 2, Size{neutral_zone_.frame_size.width, neutral_zone_.y / 2} };
        frame_dims_[RIGHT] = Section{ neutral_zone_.x / 2, neutral_zone_.y, Size{neutral_zone_.x / 2, neutral_zone_.frame_size.height } };
        frame_dims_[DOWN] = Section{ neutral_zone_.x, neutral_zone_.y + neutral_zone_.frame_size.height, Size{neutral_zone_.frame_size.width, (cam_frame_size_.height) - ((neutral_zone_.y + neutral_zone_.frame_size.height)) } };
        frame_dims_[LEFT] = Section{ neutral_zone_.x + neutral_zone_.frame_size.width, neutral_zone_.y, Size{(cam_frame_size_.width / 2) - ((neutral_zone_.x + neutral_zone_.frame_size.width) / 2), neutral_zone_.frame_size.height } };
        directions_ = { UP, RIGHT, DOWN, LEFT };
        SetThresholds();
    }

    void Engine::SetThresholds() {
        if (!cap_.isOpened()) {
            return;
        }
        for (Direction dir : directions_) {
            for (int i = 50; i < 50; i++) {
                Mat frame;
                cap_ >> frame;
                Mat filter_frame = FilterMat(frame);
                AnalyzeSection(dir, filter_frame);
                section_thresholds_[dir] += section_keypoints_[dir].size();
            }
            section_thresholds_[dir] /= 50.0;
        }
    }

    Size Engine::SetCamFrameSize() {
        if (!cap_.isOpened()) {
            return Size{ 0, 0 };
        }
        Mat frame;
        cap_ >> frame;
        Size frame_size = frame.size();
        return frame_size;
    }

    Size Engine::GetCamFrameSize() {
        return cam_frame_size_;
    }

    Section Engine::GetNeutralZone() {
        return neutral_zone_;
    }

    Mat Engine::FilterMat(const Mat& src_frame) const {
        Mat hsv_frame, filter_frame;
        cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
        // filter orange
         inRange(hsv_frame, Scalar(5, 210, 120), Scalar(10, 255, 225), filter_frame);
        // filter blue
        // inRange(hsv_frame, Scalar(110, 210, 80), Scalar(120, 255, 225), filter_frame);
        return filter_frame;
    }

    void Engine::AnalyzeSections(const cv::Mat& src_frame) {
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

    void Engine::AnalyzeSection(Direction dir, const Mat& src_frame) {
        Section sec = frame_dims_[dir];
        Rect roi(sec.x, sec.y, sec.frame_size.width, sec.frame_size.height);
        Mat cropped_frame = src_frame(roi);
        Ptr<FeatureDetector> detector = ORB::create();  
        FAST(cropped_frame, section_keypoints_[dir], 0, false);
        // take feature_frame out?
        Mat feature_frame;
        //detector->detectAndCompute(cropped_frame, feature_frame, section_keypoints[dir], noArray(), false);
    }

    void Engine::OnKeyboardInput() {
        prev_time_point_ = std::chrono::system_clock::now();
        analyze_video_ = false;
    }

    void Engine::AnalyzeFingerMovement() {
        if (section_keypoints_[UP].size() > 10 + ((int)section_thresholds_[UP])) {
            keyboard_.ScrollUp();
            OnKeyboardInput();
        }
        else if (section_keypoints_[RIGHT].size() > 4 + ((int)section_thresholds_[RIGHT])) {
            keyboard_.SwitchTabsRight();
            OnKeyboardInput();
        }
        else if (section_keypoints_[DOWN].size() > 10 + ((int)section_thresholds_[DOWN])) {
            keyboard_.ScrollDown();
            OnKeyboardInput();
        }
        else if (section_keypoints_[LEFT].size() > 4 + ((int)section_thresholds_[LEFT])) {
            keyboard_.SwitchTabsLeft();
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
                //Section sec = frame_dims[DOWN];
                //Rect roi(sec.x, sec.y, sec.frame_size.width, sec.frame_size.height);
                //Mat cropped_frame = filter_frame(roi);
                //// take feature_frame out?
                //Mat feature_frame;
                //drawKeypoints(cropped_frame, section_keypoints[DOWN], feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                //imshow("right", feature_frame);
                AnalyzeFingerMovement();
            }
            if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                prev_time_point_).count() > 1) {
                analyze_video_ = true;
            }
            /*if (waitKey(30) == 'q') {
                StopOpenCV();
                break;
            }*/
       // }
	}
    void Engine::DisplayNeutralZone() {
        if (!cap_.isOpened()) {
            return;
        }
        while (1) {
            Mat frame;
            cap_ >> frame;
            cv::Size frame_size = frame.size();
            cv::Rect roi(neutral_zone_.x, neutral_zone_.y, neutral_zone_.frame_size.width, neutral_zone_.frame_size.height);
            Mat cropped_frame = frame(roi);
            imshow("Neutral Zone", cropped_frame);
            if (waitKey(30) == 'q') {
                StopOpenCV();
                break;
            }
        }
    }

    void Engine::StopOpenCV() {
        // cap_.release();
        destroyAllWindows();
    }

    bool Engine::CheckNeutralStartingPoints() {
        return neutral_zone_.x >= 0 && neutral_zone_.y >= 0;
    }

    bool Engine::CheckNeutralWidth() {
        return (neutral_zone_.frame_size.width > 0) && (neutral_zone_.x + neutral_zone_.frame_size.width <= cam_frame_size_.width);
    }

    bool Engine::CheckNeutralHeight() {
        return (neutral_zone_.frame_size.height > 0) && (neutral_zone_.y + neutral_zone_.frame_size.height <= cam_frame_size_.height);
    }

    bool Engine::IsNeutralZoneValid() {
        return CheckNeutralStartingPoints() && CheckNeutralWidth() && CheckNeutralHeight();
    }

    void Engine::SetNeutralZone(Section neutral_zone) {
        neutral_zone_ = neutral_zone;
    }

}  // namespace mylibrary
