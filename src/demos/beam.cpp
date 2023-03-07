#include <Adafruit_NeoPixel.h>
#include "config.h"

namespace Beam
{
  int currentPixel = 0;
  int gap = 7;

  void render(Adafruit_NeoPixel &strip) {
    for(int i=0; i < STRIP_LENGTH; i++) {
      if (i == currentPixel) {
        strip.setPixelColor(i, strip.Color(80, 40, 0, 0));
      } else if (currentPixel % gap == i % gap) {
        strip.setPixelColor(i, strip.Color(0, 40, 80, 0));
      } else {
        strip.setPixelColor(i, strip.Color(1, 0, 2, 0));
      }
    }

    currentPixel = currentPixel + 1 < ((strip.numPixels() / gap) + 1) * gap
      ? currentPixel + 1
      : 0;
        
    strip.show();
    delay(40);
  }
}