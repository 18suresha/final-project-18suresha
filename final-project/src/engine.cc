// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

#include "mylibrary/engine.h"

using namespace cv;

namespace mylibrary {

    Engine::Engine() : keyboard_{} {}

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
        VideoCapture cap(0);
        if (!cap.isOpened()) {
            return;
        }
        int counter = 0;
        while (1) {
            Mat frame;
            cap >> frame;
            cv::Size frame_size = frame.size();
            cv::Rect roi(frame_size.width / 3, frame_size.height / 4, frame_size.width / 3, frame_size.height / 3);
            Mat cropped_frame = frame(roi);
            Mat filter_frame = FilterMat(cropped_frame);
            std::vector<KeyPoint> keypoints;
            Mat feature_frame;
            Ptr<FeatureDetector> detector = ORB::create();
            detector->detectAndCompute(filter_frame, feature_frame, keypoints, noArray(), false);
            drawKeypoints(filter_frame, keypoints, feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
            /*if (keypoints.size() > 0) {
                std::cout << keypoints.size() << std::endl;
            }*/
            //imwrite("assets/" + std::to_string(counter++) + "_" + std::to_string(keypoints.size()) + ".png", cropped_frame);
            imshow("filtered frame", feature_frame);
            imshow("cropped frame", cropped_frame);
            //Sleep(1000);
            //destroyAllWindows();
            if (waitKey(30) == 'q') {
                break;
            }
        }
        cap.release();
        destroyAllWindows();
        keyboard_.SwitchTabsRight();
	}

}  // namespace mylibrary
