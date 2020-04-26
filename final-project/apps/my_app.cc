// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include "cinder/gl/gl.h"
#include <cinder/ImageIo.h>
#include <cinder/Surface.h>
#include <cinder/CinderImGui.h>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
// have to include windows.h after cinder headers b/c windows.h declares macros that conflict with code in cinder headers
#include <windows.h>

namespace myapp {

	using cinder::app::KeyEvent;
    using namespace cv;

    void TestKeyboardInput() {
        INPUT ip;
        INPUT ip1;
        Sleep(5000);
        // switch tabs
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wVk = VK_CONTROL;
        ip.ki.dwFlags = 0;
        ip1.type = INPUT_KEYBOARD;
        ip1.ki.wScan = 0;
        ip1.ki.time = 0;
        ip1.ki.dwExtraInfo = 0;
        ip1.ki.wVk = VK_TAB;
        ip1.ki.dwFlags = 0;
        SendInput(1, &ip, sizeof(INPUT));
        SendInput(1, &ip1, sizeof(INPUT));
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        ip1.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        SendInput(1, &ip1, sizeof(INPUT));
    }

    Mat FilterMat(const Mat& src_frame) {
        Mat hsv_frame, filter_frame;
        cvtColor(src_frame, hsv_frame, COLOR_BGR2HSV);
        // filter orange
        // inRange(hsv_frame, Scalar(13, 100, 100), Scalar(17, 255, 255), filter_frame);
        // filter orange
        inRange(hsv_frame, Scalar(118, 100, 100), Scalar(132, 255, 255), filter_frame);
        return filter_frame;
    }

    void TestORB() {
        VideoCapture cap(0);
        if (!cap.isOpened()) {
            return;
        }
        int counter = 0;
        while (1) {
            Mat frame;
            cap >> frame;
            cv::Size frame_size = frame.size();
            cv::Rect roi(frame_size.width / 4, frame_size.height / 6, 2 * frame_size.width / 3, 3 * frame_size.height / 4);
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
            // imwrite("assets/" + std::to_string(counter++) + ".png", feature_frame);
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
    }

	MyApp::MyApp() { }
	
	void MyApp::setup() {
        // TestORB();
        // TestKeyboardInput();
		/*auto img = cinder::loadImage("assets/0.png");
		mTex = cinder::gl::Texture2d::create(img);*/
        ImGui::Initialize();
	}

	void MyApp::update() { }

	void MyApp::draw() {
		/*cinder::gl::clear();
		cinder::gl::draw(mTex);*/
        bool run_orb = false;
        bool run_key_input = false;
        if (ImGui::BeginMenu("Component to Run")) {
            ImGui::MenuItem("ORB", nullptr, &run_orb);
            ImGui::MenuItem("Keybaord Input", nullptr, &run_key_input);
            ImGui::EndMenu();
        }
        if (run_orb) {
            TestORB();
        }
        else if (run_key_input) {
            TestKeyboardInput();
        }
	}

	void MyApp::keyDown(KeyEvent event) { }

}  // namespace myapp
