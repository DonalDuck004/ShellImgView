from sys import argv
from cv2 import imread
from os import get_terminal_size
from numpy import array, mean
from math import floor, ceil

DEBUG = False

scale = int(argv[2]) if len(argv) > 2 else 2

image: array = imread(argv[1])
height, width, _ = image.shape
w, h = get_terminal_size()

s = ceil(max(width / w, height / h))

for row in range(0, height, s):
    for column in range(0, width, s):
        pixel_group = image[row:row + s, column:column + s]
        b, g, r = map(floor, mean(pixel_group, axis=(0, 1)))
        if DEBUG:
            print(f"\x1b[48;2;{r};{g};{b}m#{hex((r << 16) + (g << 8) + b)[2:].zfill(6)}")
        else:
            print(f"\x1b[48;2;{r};{g};{b}m", end=" " * scale)

    if not DEBUG:
        print("\033[0m")

