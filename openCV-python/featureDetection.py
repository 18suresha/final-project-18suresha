import cv2 as cv
import numpy as np
from PIL import Image
from matplotlib import pyplot as plt

def tryORB():
    vidObj = cv.VideoCapture(0)
    orb = cv.ORB_create()

    if (vidObj.isOpened()== False):
      print("Error opening video stream or file")

    counter = 0

    while (vidObj.isOpened()):
        ret, frame = vidObj.read()
        if ret == True:
            height, width = frame.shape[:2]
            frame = frame[int(height / 4):int(height / 2), int(width / 4):int((3 * width / 4))]
            kp = orb.detect(frame, None)
            kp, des = orb.compute(frame, kp)
            feature_frame = cv.drawKeypoints(frame, kp, None, color=(0,255,0), flags = 0)
            #print(format(len(kp)))
            # plt.imshow(feature_frame),plt.show()
            cv.imwrite("assets/ORB/%d_ORB_%d.jpg" % (counter, len(kp)), frame)
            counter += 1
            cv.imshow("What's good", feature_frame)
            # Press q on keyboard to  exit
            if cv.waitKey(25) & 0xFF == ord('q'):
                break
        else:
            break

    vidObj.release()
    cv.destroyAllWindows()

tryORB()
