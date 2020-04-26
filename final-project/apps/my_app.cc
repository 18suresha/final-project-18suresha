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

    MyApp::MyApp() : engine_{} { }
	
	void MyApp::setup() {
        ImGui::Initialize();
	}

	void MyApp::update() { }

	void MyApp::draw() {
        bool run_orb = false;
        bool run_key_input = false;
        if (ImGui::BeginMenu("Component to Run")) {
            ImGui::MenuItem("ORB", nullptr, &run_orb);
            ImGui::EndMenu();
        }
        if (run_orb) {
            engine_.RunOpenCV();
        }
	}

	void MyApp::keyDown(KeyEvent event) { }

}  // namespace myapp
