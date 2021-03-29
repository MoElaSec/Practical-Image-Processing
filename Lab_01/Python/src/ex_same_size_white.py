import cv2
import numpy as np

# read colored image.
lena_img = cv2.imread("lena.jpg")

(height, width, channel)= lena_img.shape

# create a new image with the same size as the colored image
white_img = np.ones((height, width, 1), np.uint8)

#make it white: u can white_img[:] = (255)
white_img.fill(255)


cv2.imshow("lena", lena_img)
cv2.imshow("white", white_img)

cv2.waitKey(0)
cv2.destoryAllWindows()

