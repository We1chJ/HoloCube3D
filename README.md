# HoloCube3D

A compact, animated desktop companion powered by the ESP32-S3-WROOM-1 (N16R8). This project uses a high-speed IPS display and a beam-splitting cube to create a pseudo-holographic 3D animation effect.

![HoloCube Device](assets/Picture1.gif)

[View Presentation](https://canva.link/rwkxsu6l7c77ncn)

## Materials Used
- Microcontroller: ESP32-S3-WROOM-1 (N16R8), 16MB Flash, 8MB PSRAM
- Display: 1.3" IPS TFT LCD (240x240)
- Optics: Beam-splitting cube

## Overview
HoloCube3D renders an animation loop by pushing pre-baked frames to the TFT with `TFT_eSPI`. Two display modes are supported, selected at compile time:

- **RGB565 (default):** Full color, 16 bits per pixel. ~70 frames fit in 8 MB flash at 240×240.
- **Binary (1-bit):** Black and white only, 1 bit per pixel. ~1,140 frames fit in the same flash — 16× more video for the same hardware.

## Current Animation Data
The included `gif_frames.h` currently defines:
- `FRAME_COUNT 5`
- `FRAME_WIDTH 240`
- `FRAME_HEIGHT 240`

## File Structure
- `HoloCube3D-main.ino`: Main Arduino sketch — initializes the display and plays frames in the selected mode.
- `gif_frames.h`: RGB565 frame data header (all frames + `uint16_t` pointer array). Used in color mode.
- `binary_frames.h`: 1-bit packed frame data header (all frames + `uint8_t` pointer array). Used in binary mode. **Not included** — you supply this.
- `gif-split.py`: GIF-to-RGB565 helper script for generating frame header files.
- `User_Setup.h`: Display/pin configuration for `TFT_eSPI`.

## Display Modes

### RGB565 Color Mode (default)
1. Install [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) in Arduino IDE.
2. Copy `User_Setup.h` into the TFT_eSPI library folder (see Setup Gotcha below).
3. Open `HoloCube3D-main.ino`. Confirm `// #define BINARY_MODE` is commented out.
4. Keep `gif_frames.h` in the same sketch folder.
5. Upload using ESP32-S3 board settings.

### Binary (1-bit) Mode
1. Complete steps 1–2 above.
2. Prepare `binary_frames.h` in the same sketch folder. It must define:
   - `#define FRAME_COUNT`, `FRAME_WIDTH`, `FRAME_HEIGHT`
   - Per-frame arrays: `const uint8_t frame_NNN[] PROGMEM` — bits packed MSB-first (8 pixels per byte)
   - Pointer table: `const uint8_t* const frames[FRAME_COUNT] PROGMEM`
3. In `HoloCube3D-main.ino`, uncomment `#define BINARY_MODE`.
4. Upload. Each pixel is expanded to `0xFFFF` (white) or `0x0000` (black) at runtime.

## Setup Gotcha
TFT_eSPI reads its config from `User_Setup.h` inside the **library folder**, not the sketch folder. Copy the repo's `User_Setup.h` into the installed library, or the display will not initialise correctly.

## Notes
- `gif-split.py` currently writes `frame_XXX.h` files. If you prefer a single `gif_frames.h`, combine the generated frames into one header with a `uint16_t` pointer array (matching how the sketch reads `frames[f]`).

## License
Open-source. Replace frame data to display your own hologram animations.
