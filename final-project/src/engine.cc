// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <chrono>
#include <vector>
#include <thread>
#include <cinder/app/App.h>

#include "mylibrary/engine.h"

using namespace cv;
using std::thread;

namespace mylibrary {

    Engine::Engine() : analyze_video_{ false }, cap_{ 0 }, keyboard_{}, prev_time_point_{ std::chrono::system_clock::now() }, cam_frame_size_{SetCamFrameSize()} { 
        neutral_zone_.x = cam_frame_size_.width / 3;
        neutral_zone_.frame_size.width = cam_frame_size_.width / 3;
        neutral_zone_.y = 7 * cam_frame_size_.height / 12;
        neutral_zone_.frame_size.height = cam_frame_size_.height / 5;
        frame_dims[UP] = Section{ neutral_zone_.x, neutral_zone_.y / 2, Size{neutral_zone_.frame_size.width, neutral_zone_.y / 2} };
        frame_dims[RIGHT] = Section{ neutral_zone_.x / 2, neutral_zone_.y, Size{neutral_zone_.x / 2, neutral_zone_.frame_size.height } };
        frame_dims[DOWN] = Section{ neutral_zone_.x, neutral_zone_.y + neutral_zone_.frame_size.height, Size{neutral_zone_.frame_size.width, (cam_frame_size_.height) - ((neutral_zone_.y + neutral_zone_.frame_size.height)) } };
        frame_dims[LEFT] = Section{ neutral_zone_.x + neutral_zone_.frame_size.width, neutral_zone_.y, Size{(cam_frame_size_.width / 2) - ((neutral_zone_.x + neutral_zone_.frame_size.width) / 2), neutral_zone_.frame_size.height } };
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

    Mat FilterMat(const Mat& src_frame) {
        Mat hsv_frame, filter_frame;
        cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
        // filter orange
        // inRange(hsv_frame, Scalar(13, 100, 100), Scalar(17, 255, 255), filter_frame);
        // filter orange
        inRange(hsv_frame, Scalar(110, 210, 80), Scalar(120, 255, 225), filter_frame);
        return filter_frame;
    }

    void Engine::AnalyzeSection(Direction dir, const Mat& src_frame) {
        Section sec = frame_dims[dir];
        Rect roi(sec.x, sec.y, sec.frame_size.width, sec.frame_size.height);
        Mat cropped_frame = src_frame(roi);
        Ptr<FeatureDetector> detector = ORB::create();  
        FAST(cropped_frame, section_keypoints[dir], 0, false);
        // take feature_frame out?
        Mat feature_frame;
        //detector->detectAndCompute(cropped_frame, feature_frame, section_keypoints[dir], noArray(), false);
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
                Mat filter_frame = FilterMat(frame);
                thread up(&Engine::AnalyzeSection, this, UP, filter_frame);
                thread right(&Engine::AnalyzeSection, this, RIGHT, filter_frame);
                thread down(&Engine::AnalyzeSection, this, DOWN, filter_frame);
                thread left(&Engine::AnalyzeSection, this, LEFT, filter_frame);
                // drawKeypoints(right_filter_frame, right_keypoints, right_feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                // imwrite("assets/" + std::to_string(counter++) + "_" + std::to_string(keypoints.size()) + ".png", right_frame);
                up.join();
                right.join();
                down.join();
                left.join();
                Section sec = frame_dims[DOWN];
                Rect roi(sec.x, sec.y, sec.frame_size.width, sec.frame_size.height);
                Mat cropped_frame = filter_frame(roi);
                // take feature_frame out?
                Mat feature_frame;
                drawKeypoints(cropped_frame, section_keypoints[DOWN], feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                imshow("right", feature_frame);
                if (section_keypoints[UP].size() > 10) {
                    keyboard_.ScrollUp();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                }
                else if (section_keypoints[RIGHT].size() > 4) {
                    keyboard_.SwitchTabsRight();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                } else if (section_keypoints[DOWN].size() > 10) {
                    keyboard_.ScrollDown();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                } else if (section_keypoints[LEFT].size() > 4) {
                    keyboard_.SwitchTabsLeft();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                }
            }
            if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                GetPrevTimePoint()).count() > 1) {
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
