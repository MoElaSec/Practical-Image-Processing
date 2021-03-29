import cv2
import numpy as np

img = np.zeros((2, 3, 3), np.uint8)

img[0, 0] = (0, 0, 255)
img[0, 1] = (0, 255, 0)
img[0, 2] = (255, 0, 0)

img[1, 0] = 0  # (0, 0, 0)
img[1, 1] = (39, 127, 255)
img[1, 2] = (0, 255, 255)


cv2.namedWindow("final", cv2.WINDOW_NORMAL)
cv2.resizeWindow("final", 800, 460)
cv2.imshow("final", img)
cv2.waitKey(0)
cv2.destroyAllWindows()



