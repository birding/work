# -*- coding: utf-8 -*-
import cv2


def exit_code():
    input("press ENTER key to exit")
    exit(0)


# https://www.jianshu.com/p/2b79012c0228
def show_camera():
    capture = cv2.VideoCapture(0)
    while True:
        ret, frame = capture.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame', gray)
        if cv2.waitKey(1) == ord('q'):
            break


# https://blog.csdn.net/huanglu_thu13/article/details/52337013
def show_camera2():
    capture = cv2.VideoCapture(0)
    while True:
        ret, frame = capture.read()
        cv2.imshow('frame', frame)
        if cv2.waitKey(1) >= 0:
            break
    capture.release()
    cv2.destroyAllWindows()


def camera_record():
    capture = cv2.VideoCapture(0)
    fourcc = cv2.VideoWriter_fourcc(*'MJPG')
    outfile = cv2.VideoWriter('output.avi', fourcc, 25., (640, 480))
    while capture.isOpened():
        ret, frame = capture.read()
        if ret:
            outfile.write(frame)
            cv2.imshow('frame', frame)
            if cv2.waitKey(1) == ord('q'):
                break
        else:
            break
    capture.release()
    cv2.destroyAllWindows()


def camera_take_picture():
    capture = cv2.VideoCapture(0)
    while 1:
        ret, frame = capture.read()
        cv2.imshow('frame', frame)
        if cv2.waitKey(1) == ord('q'):
            cv2.imwrite("pic.jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), 100])
            break
    capture.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    # show_camera2()
    # camera_record()
    camera_take_picture()
    # exit_code()
