import numpy as np
import cv2


img = cv2.imread("lena.jpg", cv2.IMREAD_GRAYSCALE)

img+= 100

cv2.imshow("bright", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

