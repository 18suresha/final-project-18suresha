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

    MyApp::MyApp() : engine_ {} { }
	
	void MyApp::setup() {
		ImGui::Initialize();
	}

	void MyApp::update() {
		bool run_opencv = false;

		if (ImGui::BeginMenu("Component to Run")) {
			ImGui::MenuItem("ORB", nullptr, &run_opencv);
			ImGui::EndMenu();
		}

		if (run_opencv) {
			engine_.RunOpenCV();
		}
	}

	void MyApp::draw() {
	}

	void MyApp::keyDown(KeyEvent event) {}

}  // namespace myapp
