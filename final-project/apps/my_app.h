// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include "cinder/gl/gl.h"
#include "mylibrary/engine.h"


namespace myapp {

    class MyApp : public cinder::app::App {
    public:
        MyApp();
        void setup() override;
        void update() override;
        void draw() override;
        void keyDown(cinder::app::KeyEvent) override;

        cinder::gl::Texture2dRef mTex;

    private:
        int neutral_x_;
        int neutral_y_;
        int neutral_width_;
        int neutral_height_;
        std::string cam_width_text_;
        std::string cam_height_text_;
        mylibrary::FrameSize cam_frame_size_;
        mylibrary::Engine engine_;
    };

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
