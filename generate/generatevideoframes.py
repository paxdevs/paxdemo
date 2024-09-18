import sys
import cv2
from tqdm import tqdm

# Open a video file from sys.argv[1] and iterate over its frames

# Create a VideoCapture object
cap = cv2.VideoCapture(sys.argv[1])

# Check if camera opened successfully
if (cap.isOpened() == False):
    print("Error opening video stream or file")

# get number of frames
frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

# Read until video is completed
data = []
t = tqdm(total=frame_count)

f = open('video.frames', 'wb')
while (cap.isOpened()):
    # Capture frame-by-frame
    ret, frame = cap.read()
    t.update(1)
    if ret == True:

        # Convert the frame to 85 x 64
        frame = cv2.resize(frame, (240, 180))

        # Convert to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Convert to C array format in packed binary 8 pixels per byte
        vid_rle_arr = b""
        prev_pix = -1
        cur_cnt = 0
        for row in gray:
            for i, col in enumerate(row):
                if cur_cnt == 127:
                    vid_rle_arr += bytes([127 | (prev_pix << 7)])
                    cur_cnt = 0

                if col > 127:
                    cur_pix = 1
                else:
                    cur_pix = 0 
                
                if cur_pix != prev_pix:
                    if cur_cnt > 0:
                        vid_rle_arr += bytes([cur_cnt | (prev_pix << 7)])
                    prev_pix = cur_pix
                    cur_cnt = 1
                else:
                    cur_cnt += 1

        if cur_cnt > 0:
            vid_rle_arr += bytes([cur_cnt | (prev_pix << 7)])

        f.write(vid_rle_arr)
    else:
        break
t.close()