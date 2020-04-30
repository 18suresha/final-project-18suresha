// Copyright (c) 2020 Adarsh Suresh. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include "cinder/gl/gl.h"
#include <cinder/ImageIo.h>
#include <cinder/Surface.h>
#include <cinder/CinderImGui.h>

namespace myapp {

	using cinder::app::KeyEvent;

	MyApp::MyApp() : engine_{}, run_opencv_{ false }, color_{ mylibrary::ColorToUse_::ColorToUse_Blue } {
		cam_width_text_ = "Camera Width: " + std::to_string(engine_.GetCamFrameSize().width);
		cam_height_text_ = "Camera Height: " + std::to_string(engine_.GetCamFrameSize().height);
		neutral_zone_ = engine_.GetNeutralZone();
	}
	
	void MyApp::setup() {
		ImGui::Initialize();
	}

	void MyApp::update() {
		// bool run_opencv = false;
		bool display_neutral_zone = false;
		if (ImGui::BeginMenu("Component to Run")) {
			ImGui::MenuItem("ORB", nullptr, &run_opencv_);
			ImGui::MenuItem("Neutral Zone", nullptr, &display_neutral_zone);
			ImGui::EndMenu();
		}
		
		ImGui::Text("Color To Use");
		ImGui::RadioButton("Blue", &color_, mylibrary::ColorToUse_::ColorToUse_Blue);
		ImGui::SameLine();
		ImGui::RadioButton("Orange", &color_, mylibrary::ColorToUse_::ColorToUse_Orange);
		ImGui::SameLine();
		ImGui::RadioButton("Green", &color_, mylibrary::ColorToUse_::ColorToUse_Green);

		ImGui::InputInt("Starting X Position of Neutral Zone", &neutral_zone_.x);
		ImGui::InputInt("Starting Y Position of Neutral Zone", &neutral_zone_.y);
		ImGui::InputInt("Width of Neutral Zone", &neutral_zone_.frame_size.width);
		ImGui::InputInt("Height of Neutral Zone", &neutral_zone_.frame_size.height);

		ImGui::Text(cam_width_text_.c_str());
		ImGui::Text(cam_height_text_.c_str());

		engine_.SetColorToUse(color_);
		engine_.SetNeutralZone(neutral_zone_);

		if (run_opencv_ && engine_.IsNeutralZoneValid()) {
			engine_.RunOpenCV();
		} else if (display_neutral_zone && engine_.IsNeutralZoneValid()) {
			engine_.DisplayNeutralZone();
		}
	}

	void MyApp::draw() {
	}

	void MyApp::keyDown(KeyEvent event) {}

}  // namespace myapp
