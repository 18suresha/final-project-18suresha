// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <opencv2/opencv.hpp>

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

        while (1) {
            Mat frame;
            cap >> frame;
            std::vector<KeyPoint> keypoints;
            FAST(frame, keypoints, 0, false);
            if (keypoints.size() > 0) {
                std::cout << keypoints.size() << std::endl;
            }
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
