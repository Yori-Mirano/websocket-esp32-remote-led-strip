#include <Adafruit_NeoPixel.h>
#include "config.h"

namespace WaveBeam
{
  double t_waveBeam = 0;
  void render(Adafruit_NeoPixel &strip) {
    for(int i=0; i < STRIP_LENGTH; i++) {
      double intensity = 0.5 + cos(i/5.0 - t_waveBeam)/2;
      uint32_t color = strip.Color(0, 0, 0, 1 + 40 * intensity);
      strip.setPixelColor(i, color);
    }
    
    strip.show();
    delay(20);
    t_waveBeam += 0.1;
  }
}