// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <cinder/Rand.h>
#include <iostream>
#include <filesystem>

#include "mylibrary/example.h"


TEST_CASE("Random sanity test", "[random]") {
    const float random = cinder::randFloat();
    REQUIRE(0. <= random);
    REQUIRE(random <= 1.);
}