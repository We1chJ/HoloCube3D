#!/usr/bin/env python3
"""
stream_video.py — Stream screen or video file to HoloCube3D over USB Serial.

Requirements:
    pip install pyserial mss Pillow numpy
    pip install opencv-python   # only needed for --video (file playback)

Usage:
    # Stream your screen (default: full primary monitor)
    python stream_video.py --port COM3

    # Stream a specific screen region (x y width height)
    python stream_video.py --port COM3 --region 0 0 1280 720

    # Stream a video file (loops automatically)
    python stream_video.py --port COM3 --video myvideo.mp4

    # Linux / Mac
    python stream_video.py --port /dev/ttyACM0

Notes:
    - SRC_W / SRC_H below must match the values in HoloCube3D.ino.
    - THRESHOLD controls how bright a pixel must be to show as white (0-255).
    - The baud rate is ignored for ESP32-S3 native USB (it's USB CDC), but
      pyserial still requires a value — 921600 is used as a convention.
"""

import argparse
import sys
import time

import numpy as np
import serial
from PIL import Image

# ── Must match SRC_W / SRC_H in HoloCube3D.ino ────────────────────────────────
SRC_W = 128
SRC_H = 64
# ──────────────────────────────────────────────────────────────────────────────

FRAME_BYTES = (SRC_W * SRC_H + 7) // 8
THRESHOLD = 128  # pixels brighter than this → white; adjust for your content


def to_packed(img: Image.Image) -> bytes:
    gray = img.resize((SRC_W, SRC_H), Image.LANCZOS).convert("L")
    bw = np.array(gray) > THRESHOLD
    return np.packbits(bw).tobytes()


def stream_screen(ser: serial.Serial, region: list[int]) -> None:
    try:
        from mss import mss
    except ImportError:
        sys.exit("mss not found. Run: pip install mss")

    x, y, w, h = region
    monitor = {"top": y, "left": x, "width": w, "height": h}

    frame_count = 0
    t_start = time.time()

    with mss() as sct:
        while True:
            raw = sct.grab(monitor)
            img = Image.frombytes("RGB", (raw.width, raw.height), raw.rgb)
            ser.write(to_packed(img))
            ser.readline()          # wait for ACK ('\n') from device
            frame_count += 1
            if frame_count % 30 == 0:
                fps = frame_count / (time.time() - t_start)
                print(f"\r  {fps:.1f} fps ", end="", flush=True)


def stream_video_file(ser: serial.Serial, path: str) -> None:
    try:
        import cv2
    except ImportError:
        sys.exit("opencv-python not found. Run: pip install opencv-python")

    cap = cv2.VideoCapture(path)
    if not cap.isOpened():
        sys.exit(f"Cannot open video: {path}")

    frame_count = 0
    t_start = time.time()

    while True:
        ok, frame = cap.read()
        if not ok:
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)    # loop back to start
            continue
        img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        ser.write(to_packed(img))
        ser.readline()              # wait for ACK
        frame_count += 1
        if frame_count % 30 == 0:
            fps = frame_count / (time.time() - t_start)
            print(f"\r  {fps:.1f} fps ", end="", flush=True)


def main() -> None:
    parser = argparse.ArgumentParser(description="Stream video to HoloCube3D")
    parser.add_argument("--port", required=True,
                        help="Serial port, e.g. COM3 or /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=921600,
                        help="Baud rate (default 921600; ignored for native USB)")
    parser.add_argument("--video",
                        help="Path to a video file. Omit to stream your screen.")
    parser.add_argument("--region", type=int, nargs=4,
                        metavar=("X", "Y", "W", "H"),
                        default=None,
                        help="Screen region to capture. Omit to use full primary monitor.")
    args = parser.parse_args()

    print(f"Connecting to {args.port}...")
    try:
        ser = serial.Serial(args.port, args.baud, timeout=10)
    except serial.SerialException as e:
        sys.exit(f"Could not open port: {e}")

    print("Waiting for device READY signal...")
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if line == "READY":
            break

    print("Streaming — press Ctrl+C to stop.")

    # Default region: full primary monitor
    if args.region is None:
        try:
            from mss import mss
            with mss() as sct:
                m = sct.monitors[1]   # primary monitor
                region = [m["left"], m["top"], m["width"], m["height"]]
        except Exception:
            region = [0, 0, 1280, 720]
    else:
        region = args.region

    try:
        if args.video:
            stream_video_file(ser, args.video)
        else:
            stream_screen(ser, region)
    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()
