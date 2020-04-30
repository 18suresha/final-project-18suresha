import cv2 as cv
import numpy as np

cap = cv.VideoCapture(0)

while(1):

    # Take each frame
    _, frame = cap.read()

    # Convert BGR to HSV
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    # define range of orange color in HSV
    # lower_orange = np.array([5,210,120])
    # upper_orange = np.array([10,255,225])

    # define range of green color in HSV
    lower_green = np.array([50, 100, 120])
    upper_green = np.array([70, 255, 255])

    # Threshold the HSV image to get only blue colors
    mask = cv.inRange(hsv, lower_green, upper_green)


    cv.imshow('frame',frame)
    cv.imwrite("assets/green.png", frame)
    cv.imshow('mask',mask)

    if cv.waitKey(25) & 0xFF == ord('q'):
        break

cv.destroyAllWindows()