// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "opencv2/opencv.hpp"
#include "mylibrary/engine.h"

using mylibrary::Engine;


TEST_CASE("Get Camera Size") {
    Engine engine{};
    cv::Size cam_size = engine.GetCamFrameSize();
    REQUIRE(cam_size == cv::Size{ 640, 480 });
}