import cv2
import numpy as np


def copy_orange(img):
    """A Function to Copy/extract Oranges only to a new img"""
    result = np.zeros(img.shape, np.uint8)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # v = np.array([])
    for i in range(result.shape[0]):
        for j in range(result.shape[1]):
            # v = np.append(v, np.array(img[r, c]), axis=0)
            c = img[i, j]
            c2 = hsv[i, j]
            if c2[0] < 30 and c2[1] > 5:  # only the Hue & Saturation to be guessed
                result[i, j] = c

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
