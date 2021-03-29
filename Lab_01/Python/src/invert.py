# From: https://stackoverflow.com/questions/19580102/inverting-image-in-python-with-opencv
import cv2
import numpy as np

img = cv2.imread("invert_me.jpg", cv2.IMREAD_GRAYSCALE)

# Wrong: inverted_img = (img - 255)
# Another way: inverted_img = cv2.bitwise_not(img)
inverted_img = ~img   # That's one way to do it  x + (~x) = -1

cv2.imshow("Original", img)
cv2.imshow("Inverted", inverted_img)
cv2.waitKey(0)
cv2.destroyAllWindows()