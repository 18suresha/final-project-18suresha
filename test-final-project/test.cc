// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "opencv2/core/types.hpp"
#include "mylibrary/engine.h"

using mylibrary::Engine;
using cv::Size;
using cv::Rect;


TEST_CASE("Get Camera Size") {
    Engine engine{};
    Size cam_size = engine.GetCamFrameSize();
    REQUIRE(cam_size == Size{ 640, 480 });
}

TEST_CASE("Neutral Zones") {
    Engine engine{};
    Size cam_size = engine.GetCamFrameSize();
    SECTION("Default Neutral Zone") {
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ 213, 200, 186, 120 });
        REQUIRE(engine.IsNeutralZoneValid());
    }
    SECTION("Custom Neutral Zone") {
        engine.SetNeutralZone(Rect{ 0, 0, 640, 480 });
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ 0, 0, 640, 480 });
        REQUIRE(engine.IsNeutralZoneValid());
    }
    SECTION("Neutral Zone with Invalid X Starting Point") {
        engine.SetNeutralZone(Rect{ -1, 0, 640, 480 });
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ -1, 0, 640, 480 });
        REQUIRE(!engine.IsNeutralZoneValid());
    }

    SECTION("Neutral Zone with Invalid Y Starting Point") {
        engine.SetNeutralZone(Rect{ 0, -1, 640, 480 });
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ 0, -1, 640, 480 });
        REQUIRE(!engine.IsNeutralZoneValid());
    }

    SECTION("Neutral Zone with Invalid Width") {
        engine.SetNeutralZone(Rect{ 0, 0, 641, 480 });
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ 0, 0, 641, 480 });
        REQUIRE(!engine.IsNeutralZoneValid());
    }

    SECTION("Neutral Zone with Invalid Height") {
        engine.SetNeutralZone(Rect{ 0, 0, 640, 481 });
        Rect neutral_zone = engine.GetNeutralZone();
        REQUIRE(neutral_zone == Rect{ 0, 0, 640, 481 });
        REQUIRE(!engine.IsNeutralZoneValid());
    }
}