#!/bin/bash

RAW_FILE="video/output_$(date +%Y%m%d_%H%M%S).h264"
MP4_FILE="${RAW_FILE%.h264}.mp4"

DURATION_MS=300000
FPS=30

echo "Video recording ($((DURATION_MS / 1000)) giây)..."
libcamera-vid -t $DURATION_MS --framerate $FPS -o "$RAW_FILE"

# check record
if [ ! -f "$RAW_FILE" ]; then
    echo "$RAW_FILE is no found"
    exit 1
fi

echo "Convert $RAW_FILE to $MP4_FILE ..."
ffmpeg -y -framerate $FPS -i "$RAW_FILE" -c:v libx264 -pix_fmt yuv420p "$MP4_FILE"

echo "Video saved at $MP4_FILE"
