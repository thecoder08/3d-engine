#!/bin/sh
ffmpeg -f rawvideo -video_size 640x480 -pixel_format rgba -framerate 60 -i offscreen.data -c:v libx264 -b:v 4000k -pix_fmt yuv420p out.mp4
rm offscreen.data
