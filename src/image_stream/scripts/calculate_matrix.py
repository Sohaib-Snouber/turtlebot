import cv2
import numpy as np
import yaml


# Read the image
# image = cv2.imread('Distortion00001.png')
image = cv2.imread('/home/ubuntu/catkin_ws/src/image_stream/images/front_view.png')


# Convert to grayscale
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Define the chessboard size (width x height)
# board_size = (8, 6)  # Adjust based on your chessboard
board_size = (9, 6)  # Adjust based on your chessboard


# Find chessboard corners
pattern_found, corners = cv2.findChessboardCorners(gray, board_size, None)

if pattern_found:
    # Sort corners and extract the outer corners
    print(corners)
    corners = corners.squeeze()
    print(corners)
    # corners: 0 is top right, 8 is bottom right, 45 is top left and 53 is bottom left
    outer_corners = [
        corners[board_size[0]*board_size[1]-board_size[0]+1],  # Top-left corner
        corners[1],  # Top-right corner
        corners[-1],  # Bottom-left corner
        corners[board_size[0]-1],  # Bottom-right corner
        
    ]
    left_difference = outer_corners[0][0]-outer_corners[2][0] # first index indicate to tl,tr,bl,br respectively
    right_difference = outer_corners[3][0]-outer_corners[1][0] # second index indicates to x:width, y:height in pixels

    # Draw circles and labels for each corner on the original image
    for i, corner in enumerate(outer_corners):
        x, y = map(int, corner.ravel())  # Convert coordinates to integers        
        cv2.circle(image, (x, y), 5, (0, 0, 255), -1)  # Draw a red circle
        cv2.putText(image, f"Corner {i}", (x - 35, y - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)  # Label each corner

    # Display the image with corner numbers
    # Display the image in full screen
    cv2.namedWindow('Corners Numbered', cv2.WINDOW_NORMAL)
    cv2.setWindowProperty('Corners Numbered', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow('Corners Numbered', image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    # Print coordinates of each corner
    for i, corner in enumerate(outer_corners):
        print(f"Corner {i}: ({corner[0]}, {corner[1]})")


    # Define corresponding points for the top-down view (similar to previous code)
    tlnew = (outer_corners[0][0], outer_corners[0][1]-left_difference-right_difference)
    trnew = (outer_corners[1][0], outer_corners[1][1]-left_difference-right_difference)
    blnew = (outer_corners[2][0]+left_difference, outer_corners[2][1])
    brnew = (outer_corners[3][0]-right_difference, outer_corners[3][1])

    # Define coordinates of corners in the original image (front view)
    pts1 = np.array(outer_corners, dtype=np.float32).reshape(-1, 1, 2)

    # Define corresponding points for the top-down view
    pts2 = np.array([tlnew, trnew, blnew, brnew], dtype=np.float32).reshape(-1, 1, 2)

    # Calculate Homography
    H, _ = cv2.findHomography(pts1, pts2, cv2.RANSAC, 0)

    # Apply perspective transformation
    transformed_img = cv2.warpPerspective(image, H, (image.shape[1], image.shape[0]))

    # Display the transformed image
    cv2.namedWindow('Transformed Image', cv2.WINDOW_NORMAL)
    cv2.setWindowProperty('Transformed Image', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow('Transformed Image', transformed_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
else:
    print("Chessboard pattern not found.")

# Save the transformation matrix to a YAML file
with open('/home/ubuntu/catkin_ws/src/image_stream/config/transformation_matrix.yaml', 'w') as file:
    file.write("transformation_matrix:\n")
    for i in range(3):
        for j in range(3):
            file.write(f"  - {H[i, j]:.6f}\n")

