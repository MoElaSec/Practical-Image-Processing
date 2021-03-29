"""From: https://docs.opencv.org/3.4/d5/dc4/tutorial_adding_images.html
Using: g(x)=(1−α)f0(x)+αf1(x)
"""
import cv2

nine_tails = cv2.imread("nine_tails.png")
naruto_curse = cv2.imread("naruto_curse.jpg")

width, height, _ = nine_tails.shape

resize_naruto_curse = cv2.resize(naruto_curse, (height, width))

# this is how i knew were to put height and width inside resize()
print(naruto_curse.shape)
print(nine_tails.shape)
print(resize_naruto_curse.shape)

alpha = 0.7
beta = (0.5 - alpha)

#  numpy ver. but 2x slower: dst = np.uint8(alpha*(img1)+beta*(img2))
# 0.0 is for gamma and I don't use it in this equation output of addWeighted(): [dst= α⋅src1 + β⋅src2 + γ]
dst = cv2.addWeighted(nine_tails, alpha, resize_naruto_curse, beta, 0.0)
cv2.imshow("final", dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
