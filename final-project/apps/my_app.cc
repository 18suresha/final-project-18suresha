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

	MyApp::MyApp() : neutral_x_{ 0 }, neutral_y_{ 0 }, neutral_width_{ 0 }, neutral_height_{ 0 }, engine_{} {
		cam_frame_size_ = engine_.GetCamFrameSize();
		cam_width_text_ = "Camera Width: " + std::to_string(cam_frame_size_.width);
		cam_height_text_ = "Camera Height: " + std::to_string(cam_frame_size_.height);
	}
	
	void MyApp::setup() {
		ImGui::Initialize();
	}

	void MyApp::update() {
		bool run_opencv = false;
		if (ImGui::BeginMenu("Component to Run")) {
			ImGui::MenuItem("ORB", nullptr, &run_opencv);
			ImGui::EndMenu();
		}
		ImGui::InputInt("Starting X Position of Neutral Zone", &neutral_x_);
		ImGui::InputInt("Starting Y Position of Neutral Zone", &neutral_y_);
		ImGui::InputInt("Width of Neutral Zone", &neutral_width_);
		ImGui::InputInt("Height of Neutral Zone", &neutral_height_);

		ImGui::Text(cam_width_text_.c_str());
		ImGui::Text(cam_height_text_.c_str());

		engine_.SetNeutralZone(neutral_x_, neutral_y_, neutral_width_, neutral_height_);

		if (run_opencv && engine_.IsNeutralZoneValid()) {
			engine_.RunOpenCV();
		}
	}

	void MyApp::draw() {
	}

	void MyApp::keyDown(KeyEvent event) {}

}  // namespace myapp
