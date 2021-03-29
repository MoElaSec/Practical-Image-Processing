import numpy as np
import cv2
import argparse

# from: https://docs.opencv.org/3.4/d3/dc1/tutorial_basic_linear_transform.html
# Read image given by user
parser = argparse.ArgumentParser(description='Code for Changing the contrast and brightness of an image! tutorial.')
parser.add_argument('--input', help='Path to input image.', default='lena.jpg')
args = parser.parse_args()

img = cv2.imread(cv2.samples.findFile(args.input))
if img is None:
    print('Could not open or find the image: ', args.input)
    exit(0)

"""Now, since we will make some transformations to this image, we need a new Mat object to store it.
 Also, we want this to have the following features:
  [-] Initial pixel values equal to zero
  [-] Same size and type as the original image
"""
light_img = np.zeros(img.shape, img.dtype)

"""We ask now the values of α and β to be entered by the user:
"""
alpha = 1.0 # Simple contrast control
beta = 0    # Simple brightness control

# Initialize values
print(' Basic Linear Transforms ')
print('-------------------------')
try:
    # alpha = float(input('* Enter the alpha value [1.0-3.0]: '))
    alpha = 1.0
    # beta = int(input('* Enter the beta value [0-100]: '))
    beta = 100
except ValueError:
    print('Error, not a number')


# Do the operation new_image(i,j) = alpha*image(i,j) + beta
# Instead of these 'for' loops we could have used simply:
# new_image = cv.convertScaleAbs(image, alpha=alpha, beta=beta)
# but we wanted to show you how to access the pixels :)
# for y in range(img.shape[0]):
#     for x in range(img.shape[1]):
#         for c in range(img.shape[2]):
#             light_img[y,x,c] = np.clip(alpha*img[y,x,c] + beta, 0, 255)
light_img = cv2.convertScaleAbs(img, alpha=alpha, beta=beta)



cv2.imshow("Doc lighting image way", light_img)
cv2.waitKey(0)
cv2.destroyAllWindows()


# from: https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_gui/py_video_display/py_video_display.html
# if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
