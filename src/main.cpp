#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "wifiClient.h"
#include "webServer.h"

#include "demos/beam.h"
#include "demos/k2000.h"
#include "demos/waveBeam.h"


Adafruit_NeoPixel strip(STRIP_LENGTH + 2, PIN, NEOPIXEL_TYPE);
uint8_t pixelsColor[STRIP_LENGTH * COLOR_PER_PIXEL];
boolean updateStrip = false;


void setup() {
  Serial.begin(115200);
  strip.begin();

  #if DEMO_MODE == 0
    Serial.println("WifiClient::connect");
    float t = 0.0;
    WifiClient::connect([&t]() {
      strip.setPixelColor(PRIMARY_INDICATOR_PIXEL_INDEX, strip.Color(0, 0, 0, (cos(t * 0.1)/2.0 + 0.5) * 40));
      strip.setPixelColor(SECONDARY_INDICATOR_PIXEL_INDEX, strip.Color(0, 0, 0, (cos((t * 0.1) + PI)/2.0 + 0.5) * 40));
      strip.show();
      t++;
      delay(20);
    });

    Serial.println("WifiClient::diplayIp");
    WifiClient::diplayIp(strip);
    strip.setPixelColor(PRIMARY_INDICATOR_PIXEL_INDEX, strip.Color(2, 0, 1, 0));
    strip.setPixelColor(SECONDARY_INDICATOR_PIXEL_INDEX, strip.Color(0, 0, 0, 0));
    strip.show();

    Serial.println("WebServer::begin");
    WebServer::begin(strip, [](uint8_t *data, uint count){
      for (uint8_t i = 0; i < count; i++) {
        pixelsColor[i * COLOR_PER_PIXEL] = (uint8_t)data[i * COLOR_PER_PIXEL];
        pixelsColor[i * COLOR_PER_PIXEL + 1] = (uint8_t)data[i * COLOR_PER_PIXEL + 1];
        pixelsColor[i * COLOR_PER_PIXEL + 2] = (uint8_t)data[i * COLOR_PER_PIXEL + 2];

        #if COLOR_PER_PIXEL == 4
          pixelsColor[i * COLOR_PER_PIXEL + 3] = (uint8_t)data[i * COLOR_PER_PIXEL + 3];
        #endif
      }

      updateStrip = true;
    });
  #endif

  Serial.println("loop");
}


void loop() {
  #if DEMO_MODE == 0
    if (updateStrip) {
      updateStrip = false;

      for (uint8_t i = 0; i < STRIP_LENGTH; i++) {    
        strip.setPixelColor(i, strip.Color(
          pixelsColor[i * COLOR_PER_PIXEL],
          pixelsColor[i * COLOR_PER_PIXEL + 1],
          pixelsColor[i * COLOR_PER_PIXEL + 2],

          #if COLOR_PER_PIXEL == 4
            pixelsColor[i * COLOR_PER_PIXEL + 3]
          #else
            0
          #endif
        ));
      }

      strip.show();
    }

    delay(1000 / FPS_MAX);
  #endif

  #if DEMO_MODE == 1
    Beam::render(strip);
  #endif

  #if DEMO_MODE == 2
    K2000::render(strip);
  #endif

  #if DEMO_MODE == 3
    WaveBeam::render(strip);
  #endif
}