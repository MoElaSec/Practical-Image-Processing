#Tut: https://medium.com/@rndayala/getting-started-with-opencv-2a2bc80ba2d6
import cv2
import numpy as np

#! (1)  cv2.IMREAD_COLOR : Loads a color image. Any transparency of image will be neglected. It is the default flag.
#! (0)  cv2.IMREAD_GRAYSCALE : Loads image in grayscale mode
#! (-1) cv2.IMREAD_UNCHANGED : Loads image as such including alpha channel
img = cv2.imread("lena.jpg", 1)

# Check for invalid input
if img is None:  # Check for invalid input
    print("Could not open or find the image")

print("width of the image in pixels is: {}".format(img.shape[1]))
print("height of the image in pixels is: {}".format(img.shape[0]))
print("channels present the image is: {}".format(img.shape[2]))

# load image into cv2 window
# wait for key press
# write image into another format
cv2.imshow("Image Title", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
