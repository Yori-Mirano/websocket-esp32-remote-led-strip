#include <Adafruit_NeoPixel.h>
#include "config.h"

namespace K2000
{
  const int k2000_numOfLed = STRIP_LENGTH;
  const int k2000_tail = k2000_numOfLed;
  int k2000_trailPosition[k2000_tail];
  int k2000_position = 0;
  bool k2000_rightDirection = true;

  void render(Adafruit_NeoPixel &strip) {
    strip.fill(strip.Color(1, 0, 0, 0));

    if (k2000_rightDirection) {
      k2000_position++;
    } else {
      k2000_position--;
    }

    if (k2000_position > k2000_numOfLed - 2) {
      k2000_rightDirection = false;
    } else if (k2000_position <= 0) {
      k2000_rightDirection = true;
    }
    
    for(int i=0; i<k2000_tail; i++) {
      strip.setPixelColor(k2000_trailPosition[i], strip.Color(1 + 32 * ((double)i/(double)k2000_tail)*((double)i/(double)k2000_tail), 0, 0, 0));

      if (i == k2000_tail - 1) {
        k2000_trailPosition[i] = k2000_position;
      } else {
        k2000_trailPosition[i] = k2000_trailPosition[i+1];
      }
    }

    strip.setPixelColor(k2000_position, strip.Color(255, 0, 0, 0));
    
    
    strip.show();
    delay(20);
  }
}