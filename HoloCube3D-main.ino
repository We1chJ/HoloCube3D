#include <TFT_eSPI.h>
#include <SPI.h>

#include "frame_000.h"
#include "frame_001.h"
#include "frame_002.h"
#include "frame_003.h"
#include "frame_004.h"
#include "frame_005.h"
#include "frame_006.h"
#include "frame_007.h"
#include "frame_008.h"
#include "frame_009.h"
#include "frame_010.h"
#include "frame_011.h"
#include "frame_012.h"
#include "frame_013.h"
#include "frame_014.h"
#include "frame_015.h"
#include "frame_016.h"
#include "frame_017.h"

#define FRAME_WIDTH   240
#define FRAME_HEIGHT  240
#define FRAME_DELAY   42

const uint16_t* const frames[] PROGMEM = {
  frame_000,
  frame_001,
  frame_002,
  frame_003,
  frame_004,
  frame_005,
  frame_006,
  frame_007,
  frame_008,
  frame_009,
  frame_010,
  frame_011,
  frame_012,
  frame_013,
  frame_014,
  frame_015,
  frame_016,
  frame_017,
};

uint16_t frameBuf[FRAME_WIDTH * FRAME_HEIGHT];

const int TOTAL_FRAMES = sizeof(frames) / sizeof(frames[0]);

TFT_eSPI tft = TFT_eSPI();
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(false);
}
void loop() {
  for (int f = 0; f < TOTAL_FRAMES; f++) {
    const uint16_t* framePtr = (const uint16_t*)pgm_read_ptr(&frames[f]);
    memcpy_P(frameBuf, framePtr, FRAME_WIDTH * FRAME_HEIGHT * sizeof(uint16_t));
    
    tft.startWrite();
    tft.setAddrWindow(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    tft.pushPixels(frameBuf, FRAME_WIDTH * FRAME_HEIGHT);
    tft.endWrite();
    
    delay(FRAME_DELAY);
  }
}