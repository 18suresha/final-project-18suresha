# Development

 - **4/19/20** Got Cinder, OpenCV, Catch2 to work for the project in Visual Studio
   - Have to check keyboard input

- **4/20/20** Implemented keyboard input including typing a letter key and switching tabs
  - Create GUI implementation using Cinder so user can toggle between what type of finger action produces what event
  - Identify finger movement

- **4/21/20** Implemented basic Cinder functionality of setting an image as background
  - Create GUI implementation using Cinder so user can toggle between what type of finger action produces what event
  - Identify finger movement
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio

- **4/22/20** Implemented Cinder-ImGui and orange color filtering in OpenCV
  - Identify specific finger movement
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Modularize and improve code quality

- **4/25/20** Implemented HSV color filtering and found good range for blue filter
  - Identify specific finger movement
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Modularize and improve code quality
  - Work on user-specified neutral zone
  - Implement time delay after keyboard input occurs

- **4/26/20** Implemented user inputting neutral zone
  - Identify specific finger movement
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Improve code quality
  - Implement splitting of frame into different areas
  - Implement creating average base threshold of keypoints when user not moving finger

- **4/27/20** Modularized frame analysis, switched back to FAST feature detection, Used multithreading, identify all four finger movements with keyboard actions, implemented section thresholds to account for the user's physical setting
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Improve code quality
  - Add options for colors other than blue
  - Double check frame_dims calculations to make sure they never go beyond camera frame

- **4/28/20** Used mouse scroll input for scrolling up/down
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Improve code quality
  - Add options for colors other than blue
  - Double check frame_dims calculations to make sure they never go beyond camera frame
  - Write tests

- **4/29/20** Added option to use orange color on finger, Added radio buttons to GUI for color choice
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Improve code quality
  - Add third color option
  - Work on distinguishing between orange on finger and appearance of orange on bright skin.
  - Double check frame_dims calculations to make sure they never go beyond camera frame
  - Write tests

- **5/1/20** Used feature points instead of keypoints in order to detect change in number of pixels of chosen color
  - Configure IDE to follow Google C++ Style Guide
  - Create documentaion on how to set up project in Visual Studio
  - Mention specifications of netural zone preferably being user's head and using well-sized markers on finger in documentation
  - Improve code quality
  - Add third color option
  - Work on distinguishing between orange on finger and appearance of orange on bright skin.
  - Double check frame_dims calculations to make sure they never go beyond camera frame
  - Write tests
  - Check Map documentation for multithreading specifications
  - Count number of colors
  - Preview neutral zone with four sections

- **5/2/20** Formatted code to follow Google Style Guide, User can calibrate application any time they want to, Display different sections in preview, Replaced orange with red
  - Create documentaion on how to set up project in Visual Studio
  - Mention specifications of netural zone preferably being user's head and using well-sized markers on finger in documentation
  - Add third color option
  - Write tests
  - Check Map documentation for multithreading specifications

- **5/3/20** Created documentation, Added pink color, wrote tests
  - Check Map documentation for multithreading specifications

- **5/4/20** Updated naming

- **5/5/20** Updated blue image
  - Create video of project in use
---
