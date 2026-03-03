#include <TFT_eSPI.h>
#include <SPI.h>
#include "gif_frames.h"   // single file contains all frames + pointer array

#define FRAME_DELAY 42    // ms per frame (~24 fps)

uint16_t frameBuf[FRAME_WIDTH * FRAME_HEIGHT];

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(false);
}

void loop() {
  for (int f = 0; f < FRAME_COUNT; f++) {
    // pgm_read_ptr reads the pointer itself out of PROGMEM
    const uint16_t* framePtr = (const uint16_t*)pgm_read_ptr(&frames[f]);
    memcpy_P(frameBuf, framePtr, FRAME_WIDTH * FRAME_HEIGHT * sizeof(uint16_t));

    tft.startWrite();
    tft.setAddrWindow(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    tft.pushPixels(frameBuf, FRAME_WIDTH * FRAME_HEIGHT);
    tft.endWrite();

    delay(FRAME_DELAY);
  }
}