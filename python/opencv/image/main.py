# -*- coding: utf-8 -*-
import cv2 as cv

def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


if __name__ == '__main__':
    img = cv.imread("F:\python\opencv\image\yidian_11831983921.jpg")
    cv.namedWindow("Image")
    cv.imshow("Image",img)
    cv.waitKey(0)
    cv.destroyAllWindows() 
    exit_code()
    