import cv2
import numpy as np


def copy_orange(img):
    """A Function to Copy/extract Oranges only to a new img"""

    result = np.zeros(img.shape, np.uint8)
    
    for r in range(result.shape[0]):
        for c in range(result.shape[1]):
            v = img[r, c]
            # adjust the values to your image (trial and error method :( )
            # under the OpenCV the order of the channels is Blue, Green, Red
            if v[2] > 200 and v[1] < 600 and v[0] < 60:
                result[r, c] = v

    return result

def main():
    path = "Oranges.jpg"
    img = cv2.imread(path)
    if img is None:
        print('Could not open or find the image: ', path)
        exit(0)

    result = copy_orange(img)

    cv2.imshow("Oranges", result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


#####################################
if __name__ == '__main__':
    main()
