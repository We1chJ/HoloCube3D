#include <TFT_eSPI.h>
#include <SPI.h>

// Uncomment exactly one mode:
#define BINARY_MODE   // 1-bit 128×64 frames from VideoFrame.h, centered on 240×240
// (neither = RGB565 from gif_frames.h)

// Uncomment to mirror the video horizontally (left↔right).
// Enable this if using a holographic beam-splitting cube (it mirrors the image).
#define FLIP_V

#ifdef BINARY_MODE
  #include "VideoFrame.h"
  // TOTAL_FRAMES and FRAME_DELAY come from VideoFrame.h

  // ── Change these two pairs to match your hardware and video ──
  #define DISP_W  240   // display width  (pixels)
  #define DISP_H  240   // display height (pixels)
  #define SRC_W   128   // source frame width  (pixels)
  #define SRC_H    64   // source frame height (pixels)
  // ─────────────────────────────────────────────────────────────

  // Scale to fit: whichever axis would overflow the display limits the scale.
  // The other axis is centered with black bars. No distortion.
  #if (SRC_W * DISP_H >= SRC_H * DISP_W)
    // Width-limited
    #define SCALED_W  DISP_W
    #define SCALED_H  (SRC_H * DISP_W / SRC_W)
  #else
    // Height-limited
    #define SCALED_H  DISP_H
    #define SCALED_W  (SRC_W * DISP_H / SRC_H)
  #endif
  #define X_OFF  ((DISP_W - SCALED_W) / 2)
  #define Y_OFF  ((DISP_H - SCALED_H) / 2)
#else
  #include "ColoredVideoFrame.h"
  #define DISP_W FRAME_WIDTH
  #define DISP_H FRAME_HEIGHT
  #define FRAME_DELAY 42
#endif

uint16_t frameBuf[DISP_W * DISP_H];

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(false);
}

void loop() {
#ifdef BINARY_MODE
  for (int f = 0; f < TOTAL_FRAMES; f++) {
    memset(frameBuf, 0, sizeof(frameBuf));
    const uint8_t* src = video_frames[f];
    for (int row = 0; row < SCALED_H; row++) {
      int sr = row * SRC_H / SCALED_H;
      for (int col = 0; col < SCALED_W; col++) {
#ifdef FLIP_V
        int sc = (SCALED_W - 1 - col) * SRC_W / SCALED_W;
#else
        int sc = col * SRC_W / SCALED_W;
#endif
        int i = sr * SRC_W + sc;
        uint8_t b = pgm_read_byte(src + (i >> 3));
        frameBuf[(row + Y_OFF) * DISP_W + (col + X_OFF)] =
          (b >> (7 - (i & 7))) & 1 ? 0xFFFF : 0x0000;
      }
    }
    tft.startWrite();
    tft.setAddrWindow(0, 0, DISP_W, DISP_H);
    tft.pushPixels(frameBuf, DISP_W * DISP_H);
    tft.endWrite();
    delay(FRAME_DELAY);
  }

#else
  for (int f = 0; f < FRAME_COUNT; f++) {
    const uint16_t* framePtr = (const uint16_t*)pgm_read_ptr(&frames[f]);
    memcpy_P(frameBuf, framePtr, DISP_W * DISP_H * sizeof(uint16_t));
    tft.startWrite();
    tft.setAddrWindow(0, 0, DISP_W, DISP_H);
    tft.pushPixels(frameBuf, DISP_W * DISP_H);
    tft.endWrite();
    delay(FRAME_DELAY);
  }
#endif
}
