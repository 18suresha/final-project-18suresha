// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <chrono>
#include <vector>

#include "mylibrary/engine.h"

using namespace cv;

namespace mylibrary {

    Engine::Engine() : analyze_video_{ false }, cap_ { 0 }, keyboard_{}, prev_time_point_{ std::chrono::system_clock::now() } {}

    Mat FilterMat(const Mat& src_frame) {
        Mat hsv_frame, filter_frame;
        cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
        // filter orange
        // inRange(hsv_frame, Scalar(13, 100, 100), Scalar(17, 255, 255), filter_frame);
        // filter orange
        inRange(hsv_frame, Scalar(105, 225, 80), Scalar(132, 250, 225), filter_frame);
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
                cv::Size frame_size = frame.size();
                cv::Rect roi(frame_size.width / 3, frame_size.height / 4, frame_size.width / 3, frame_size.height / 3);
                Mat cropped_frame = frame(roi);
                Mat filter_frame = FilterMat(cropped_frame);
                std::vector<KeyPoint> keypoints;
                Mat feature_frame;
                Ptr<FeatureDetector> detector = ORB::create();
                detector->detectAndCompute(filter_frame, feature_frame, keypoints, noArray(), false);
                drawKeypoints(filter_frame, keypoints, feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
                //imwrite("assets/" + std::to_string(counter++) + "_" + std::to_string(keypoints.size()) + ".png", cropped_frame);
                imshow("filtered frame", feature_frame);
                imshow("cropped frame", cropped_frame);
                if (keypoints.size() > 10) {
                    keyboard_.SwitchTabsRight();
                    SetPrevTimePoint(std::chrono::system_clock::now());
                    analyze_video_ = false;
                }
            }
            if (std::chrono::duration<double>(std::chrono::system_clock::now() -
                GetPrevTimePoint()).count() > 1.5) {
                analyze_video_ = true;
            }
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

}  // namespace mylibrary
