# python3
# -*- coding: utf-8 -*-
import cv2
import face_recognition
from glob import glob
from os.path import join
import os


def exit_code():
    input("press ENTER key to exit")
    exit(0)


def camera_take_picture():
    capture = cv2.VideoCapture(0)
    while 1:
        ret, frame = capture.read()
        cv2.imshow("Press 'q' to capture and exit", frame)
        if cv2.waitKey(1) == ord('q'):
            cv2.imwrite("tmp.jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), 100])
            break
    capture.release()
    cv2.destroyAllWindows()


# https://www.freebuf.com/articles/terminal/158484.html
def camera_take_picture2(pic_name):
    if pic_name.strip() == '':
        pic_name = "tmp.jpg"
    capture = cv2.VideoCapture(0)
    no_captured = 1
    while no_captured:
        ret, frame = capture.read()
        face_locations = face_recognition.face_locations(frame)
        for face_location in face_locations:
            top, right, bottom, left = face_location
            print(face_location)
            cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        cv2.imshow("Press any key to save", frame)
        if cv2.waitKey(1) >= 0:
            cv2.imwrite(pic_name, frame, [int(cv2.IMWRITE_JPEG_QUALITY), 100])
            break
    capture.release()
    cv2.destroyAllWindows()


# https://stackoverflow.com/questions/28834980/nameerror-global-name-cv-gui-normal-is-not-defined
def find_face():
    # file_name = "shanping.jpg"
    file_name = "children.jpg"

    img2 = cv2.imread(file_name)
    cv2.namedWindow("original", 0)  # CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO
    cv2.imshow("original", img2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    img = face_recognition.load_image_file(file_name)
    face_locations = face_recognition.face_locations(img)
    for face_location in face_locations:
        top, right, bottom, left = face_location
        face_image = img[top:bottom, left:right]
        cv2.imshow("face", face_image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()


def compare_img():
    chen_image = face_recognition.load_image_file("children.jpg")
    chen_face_encoding = face_recognition.face_encodings(chen_image)[0]
    # print("chen_face_encoding:{}".format(chen_face_encoding))

    unknown_image = face_recognition.load_image_file("shanping_2.jpg")
    unknown_face_encoding = face_recognition.face_encodings(unknown_image)[0]

    known_faces = [
        chen_face_encoding
    ]
    results = face_recognition.compare_faces(known_faces, unknown_face_encoding)
    print("result :{}".format(results))
    print("Are they matching?{}".format(results[0]))


def get_jpg_files():
    ret_files = glob("knownfaces/known_*.jpg")
    print(ret_files, os.path.basename(ret_files[0])[6:-6])
    return ret_files


def face_recognizing():
    face_encodings = []
    names = get_jpg_files()
    for name in names:
        # print(name)
        image = face_recognition.load_image_file(name)
        encoding = face_recognition.face_encodings(image)[0]
        face_encodings.append(encoding)

    capture = cv2.VideoCapture(0)
    while 1:
        ret, frame = capture.read()
        unknown_face_encodings = face_recognition.face_encodings(frame)
        face_locations = face_recognition.face_locations(frame)

        for i in range(len(unknown_face_encodings)):
            unknown_encoding = unknown_face_encodings[i]
            face_location = face_locations[i]
            top, right, bottom, left = face_location
            cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
            results = face_recognition.compare_faces(face_encodings, unknown_encoding, tolerance=0.45)
            result_name = []
            for j in range(len(results)):
                if results[j]:
                    print(names[j])
                    result_name.append(os.path.basename(names[j])[6:-6])
            result_name = list(set(result_name))
            cv2.putText(frame, ','.join(result_name), (left - 10, top - 10), cv2.FONT_HERSHEY_SIMPLEX,
                        0.5, (0, 255, 0), 2)
        print("recognized done\n\n")
        cv2.imshow("Press 'q' to exit", frame)
        if cv2.waitKey(1) == ord('q'):
            break
    capture.release()
    cv2.destroyAllWindows()


def face_recognizing_opt():
    face_encodings = []
    names = get_jpg_files()
    for name in names:
        # print(name)
        image = face_recognition.load_image_file(name)
        encoding = face_recognition.face_encodings(image)[0]
        face_encodings.append(encoding)

    capture = cv2.VideoCapture(0)
    while 1:
        ret, frame = capture.read()
        unknown_face_encodings = face_recognition.face_encodings(frame)
        face_locations = face_recognition.face_locations(frame)

        for i in range(len(unknown_face_encodings)):
            unknown_encoding = unknown_face_encodings[i]
            face_location = face_locations[i]
            top, right, bottom, left = face_location
            cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
            results = face_recognition.face_distance(face_encodings, unknown_encoding)
            print(results, results.min(), results.argmin())
            # ret_idx = results.index(min(results))
            ret_idx = results.argmin()
            result_name = os.path.basename(names[ret_idx])
            print(result_name)
            cv2.putText(frame, result_name[6:-6], (left - 10, top - 10), cv2.FONT_HERSHEY_SIMPLEX,
                        0.5, (0, 255, 0), 2)
        print("recognized done\n\n")
        cv2.imshow("Press 'q' to exit", frame)
        if cv2.waitKey(1) == ord('q'):
            break
    capture.release()
    cv2.destroyAllWindows()


def release_func():
    choice = input("Select function:\n\t1.take picture\n\t2.recognize faces\n>>")
    if choice == '1':
        name_save = input("input username>> ")
        print("capturing picture for " + name_save)
        name_save = "known_" + name_save + "00.jpg"
        camera_take_picture2(name_save)
        print(name_save + " is saved")
    else:
        face_recognizing_opt()


if __name__ == '__main__':
    # camera_take_picture()
    # camera_take_picture2()
    # find_face()
    # compare_img()
    # get_jpg_files()
    # face_recognizing()
    release_func()
    # exit_code()
