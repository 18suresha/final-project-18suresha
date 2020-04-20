// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

#include "my_app.h"
#include <vector>
#include <iostream>


using cinder::app::App;
using cinder::app::RendererGl;
using namespace cv;


namespace myapp {

    const int kSamples = 8;
    const int kWidth = 800;
    const int kHeight = 800;

    void SetUp(App::Settings* settings) {
        VideoCapture cap(0);
        if (!cap.isOpened()) {
            return;
        }
        int counter = 0;
        while (1) {
            Mat frame;
            cap >> frame;
            std::vector<KeyPoint> keypoints;
            Mat feature_frame;
            Ptr<FeatureDetector> detector = ORB::create();
            detector->detectAndCompute(frame, feature_frame, keypoints, noArray(), false);
            drawKeypoints(frame, keypoints, feature_frame, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
            /*if (keypoints.size() > 0) {
                std::cout << keypoints.size() << std::endl;
            }*/
            imwrite("assets/" + std::to_string(counter++) + ".png", feature_frame);
            imshow("Frame", frame);

            if (waitKey(30) >= 0) {
                break;
            }
        }
        cap.release();
        destroyAllWindows();
        settings->setWindowSize(kWidth, kHeight);
        settings->setTitle("My CS 126 Application");
    }

}  // namespace myapp


// This is a macro that runs the application.
CINDER_APP(myapp::MyApp,
    RendererGl(RendererGl::Options().msaa(myapp::kSamples)),
    myapp::SetUp)
