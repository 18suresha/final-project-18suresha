# Program To Read video
# and Extract Frames
import cv2


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

            cv2.imshow("What's good", frame)
            # Saves the frames with frame-count
            cv2.imwrite("frame%d.jpg" % count, frame)

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