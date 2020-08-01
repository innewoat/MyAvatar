import os

s = "abcdefghijklmnopqrstuvwxyz"

for c in s:
    order = "ffmpeg -i output/pic_"+c + \
        ".ppm -loglevel panic -y output/bmp/pic_"+c+".bmp"
    print(order)
    os.system(order)
