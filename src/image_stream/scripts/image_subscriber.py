#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import CompressedImage
import numpy as np
import cv2


def callback(data):
    try:
        # Decompress the image
        np_arr = np.frombuffer(data.data, np.uint8)
        cv_image = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

        rotated_image = cv2.rotate(cv_image, cv2.ROTATE_180)
    except Exception as e:
        print(e)

    cv2.imshow("Image Window", rotated_image)
    cv2.waitKey(1)

def main():
    rospy.init_node('image_subscriber', anonymous=True)
    # Subscribe to the compressed image topic
    rospy.Subscriber("/raspicam_node/image/compressed", CompressedImage, callback)
    rospy.spin()

    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
