#include <TFT_eSPI.h>
#include <SPI.h>

// Uncomment exactly one mode:
#define BINARY_MODE   // 1-bit 128×64 frames from VideoFrame.h, centered on 240×240
// (neither = RGB565 from gif_frames.h)

// Uncomment to flip the video vertically.
// Enable this if using a holographic beam-splitting cube (it inverts the image).
// #define FLIP_V

#ifdef BINARY_MODE
  #include "VideoFrame.h"
  // TOTAL_FRAMES and FRAME_DELAY come from VideoFrame.h
  #define DISP_W    240
  #define DISP_H    240
  #define SRC_W     128
  #define SRC_H      64
  #define SCALED_W  240   // SRC_W scaled ×1.875 to fill display width
  #define SCALED_H  120   // SRC_H scaled ×1.875 (keeps aspect ratio)
  #define X_OFF       0
  #define Y_OFF      60   // (240 - 120) / 2
#else
  #include "gif_frames.h"
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
#ifdef FLIP_V
      int sr = (SCALED_H - 1 - row) * SRC_H / SCALED_H;
#else
      int sr = row * SRC_H / SCALED_H;
#endif
      for (int col = 0; col < SCALED_W; col++) {
        int sc = col * SRC_W / SCALED_W;
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
