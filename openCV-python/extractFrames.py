# Program To Read video
# and Extract Frames
import cv2
import numpy as np


# Function to extract frames
def FrameCapture(path):
    # Path to video file
    vidObj = cv2.VideoCapture(path)

    # Used as counter variable
    count = 0

    # checks whether frames were extracted
    success = 1

    while (vidObj.isOpened()):
        ret, frame = vidObj.read()
        if ret == True:
            # up
            cropped_up = frame[0:186, 213:413]
            # down
            cropped_down = frame[376:480, 213:426]
            # right
            cropped_right = frame[280:376, 106:213]
            # left
            cropped_left = frame[280:376, 426:533]

            cropped_img = cropped_up

            # Convert BGR to HSV
            hsv = cv2.cvtColor(cropped_img, cv2.COLOR_BGR2HSV)

            # define range of orange color in HSV
            lower_orange = np.array([4,130,120])
            upper_orange = np.array([10,255,225])

            # define range of blue color in HSV
            lower_blue = np.array([105, 150, 80])
            upper_blue = np.array([118, 255, 255])

            # Threshold the HSV image to get only blue colors
            mask = cv2.inRange(hsv, lower_blue, upper_blue)
            cv2.imshow("filtered", mask)
            cv2.imshow("cropped", cropped_img)
            # Saves the frames with frame-count
            # cv2.imwrite("assets/pixelCount/%dframe%d.jpg" % (count, cv2.countNonZero(mask)), cropped_img)

            count += 1

            # Press Q on keyboard to  exit
            if cv2.waitKey(25) & 0xFF == ord('q'):
                break
        else:
            break



# Driver Code
if __name__ == '__main__':
    # Calling the function
    FrameCapture(0)