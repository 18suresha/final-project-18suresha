// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <chrono>
#include <vector>

#include "mylibrary/engine.h"

using namespace cv;

namespace mylibrary {

    Engine::Engine() : analyze_video_{ false }, cap_{ 0 }, keyboard_{}, prev_time_point_{ std::chrono::system_clock::now() } {
        SetCamFrameSize();
        neutral_zone_.x = cam_frame_size_.width / 3;
        neutral_zone_.frame_size.width = cam_frame_size_.width / 3;
        neutral_zone_.y = 7 * cam_frame_size_.height / 12;
        neutral_zone_.frame_size.height = 5 * cam_frame_size_.height / 32;
    }

    void Engine::SetCamFrameSize() {
        if (!cap_.isOpened()) {
            return;
        }
        Mat frame;
        cap_ >> frame;
        Size frame_size = frame.size();
        cam_frame_size_.width = frame_size.width;
        cam_frame_size_.height = frame_size.height;
    }

    FrameSize Engine::GetCamFrameSize() {
        return cam_frame_size_;
    }

    NeutralZone Engine::GetNeutralZone() {
        return neutral_zone_;
    }

    Mat FilterMat(const Mat& src_frame) {
        Mat hsv_frame, filter_frame;
        cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
        // filter orange
        // inRange(hsv_frame, Scalar(13, 100, 100), Scalar(17, 255, 255), filter_frame);
        // filter orange
        inRange(hsv_frame, Scalar(102, 210, 80), Scalar(134, 250, 225), filter_frame);
        return filter_frame;
    }

	void Engine::RunOpenCV() {
        if (!cap_.isOpened()) {
            return;
        }
        int counter = 0;
        while (1) {
            if (analyze_video_) {
                Mat frame;
                cap_ >> frame;
                Size frame_size = frame.size();
                Rect up(neutral_zone_.x, neutral_zone_.y / 2, neutral_zone_.frame_size.width, neutral_zone_.y / 2);
                Mat up_frame = frame(up);
                Rect right(neutral_zone_.x / 2, neutral_zone_.y, neutral_zone_.x / 2, neutral_zone_.frame_size.height);
                Mat right_frame = frame(right);
                Mat right_filter_frame = FilterMat(right_frame);
                Mat up_filter_frame = FilterMat(up_frame);
                std::vector<KeyPoint> up_keypoints;
                std::vector<KeyPoint> right_keypoints;
                Mat right_feature_frame;
                Mat up_feature_frame;
                Ptr<FeatureDetector> detector = ORB::create();
                detector->detectAndCompute(right_filter_frame, right_feature_frame, right_keypoints, noArray(), false);
                drawKeypoints(right_filter_frame, right_keypoints, right_feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                detector->detectAndCompute(up_filter_frame, up_feature_frame, up_keypoints, noArray(), false);
                drawKeypoints(up_filter_frame, up_keypoints, up_feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                // imwrite("assets/" + std::to_string(counter++) + "_" + std::to_string(keypoints.size()) + ".png", right_frame);
                imshow("right frame", right_feature_frame);
                imshow("up frame", up_feature_frame);
                if (up_keypoints.size() > 10) {
                    keyboard_.ScrollUp();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                } else if (right_keypoints.size() > 2) {
                    // keyboard_.ScrollUp();
                    keyboard_.SwitchTabsRight();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                }
            }
            if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                GetPrevTimePoint()).count() > 1) {
                analyze_video_ = true;
            }
            if (waitKey(30) == 'q') {
                StopOpenCV();
                break;
            }
        }
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

    void Engine::SetNeutralZone(NeutralZone neutral_zone) {
        neutral_zone_ = neutral_zone;
    }

}  // namespace mylibrary
