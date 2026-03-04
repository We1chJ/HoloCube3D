# HoloCube3D

A compact, animated desktop companion powered by the ESP32-S3-WROOM-1 (N16R8). This project uses a high-speed IPS display and a beam-splitting cube to create a pseudo-holographic 3D animation effect.

![HoloCube Device](https://m.media-amazon.com/images/I/418SW+HhY1L._AC_UF894,1000_QL80_.jpg)

## Materials Used
- Microcontroller: ESP32-S3-WROOM-1 (N16R8), 16MB Flash, 8MB PSRAM
- Display: 1.3" IPS TFT LCD (240x240)
- Optics: Beam-splitting cube

## Overview
HoloCube3D renders an animation loop by drawing RGB565 frames from `gif_frames.h` to the TFT with `TFT_eSPI`.

## Current Animation Data
The included `gif_frames.h` currently defines:
- `FRAME_COUNT 5`
- `FRAME_WIDTH 240`
- `FRAME_HEIGHT 240`

## File Structure
- `HoloCube3D-main.ino`: Main Arduino sketch that initializes the display and plays frames.
- `gif_frames.h`: Auto-generated frame data header (all frames + frame pointer array).
- `gif-split.py`: GIF-to-RGB565 helper script for generating frame header files.
- `User_Setup.h`: Display/pin configuration for `TFT_eSPI`.

## Quick Start
1. Install [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) in Arduino IDE.
2. Configure `User_Setup.h` for your ESP32-S3 and 240x240 TFT.
3. Open `HoloCube3D-main.ino` in Arduino IDE.
4. Keep `gif_frames.h` in the same sketch folder.
5. Upload using ESP32-S3 board settings that match your hardware.

## Notes
- `gif-split.py` currently writes `frame_XXX.h` files. If you prefer a single `gif_frames.h`, combine generated frames into one header with a frame pointer array (matching how `HoloCube3D-main.ino` reads `frames[f]`).

## License
Open-source. Replace frame data to display your own hologram animations.
