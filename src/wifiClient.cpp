#include <Adafruit_NeoPixel.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <WiFiClient.h>
#include "config.h"

namespace WifiClient
{
  IPAddress ip;

  void connect(const std::function<void(void)> &callback) {
    WiFi.begin(_SSID_, _PASSWORD_);

    // Wait for connection.
    while (WiFi.status() != WL_CONNECTED) {
      callback();
    }

    ip = WiFi.localIP();
  }



  void diplayIp(Adafruit_NeoPixel &strip) {
    for (uint8_t i = 0; i < STRIP_LENGTH; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }

    uint pixelIndex = 0;

    // Calculating the left margin to center the display
    for (uint8_t ipPartIndex = 0; ipPartIndex < 4; ipPartIndex++) {
      uint8_t hundreds  = ip[ipPartIndex] / 100;
      uint8_t tens      = (ip[ipPartIndex] / 10) % 10;
      uint8_t units     = ip[ipPartIndex] % 10;

      if (ipPartIndex > 0) {
        pixelIndex += 2;
      }

      for (uint8_t i = 0; i < hundreds; i++) {
        pixelIndex++;
      }

      for (uint8_t i = 0; i < tens; i++) {
        pixelIndex++;
      }
    
      for (uint8_t i = 0; i < units; i++) {
        pixelIndex++;
      }
    }
    
    uint margin = (STRIP_LENGTH - pixelIndex - 1) / 2;


    // Display
    pixelIndex = margin;

    for (uint8_t ipPartIndex = 0; ipPartIndex < 4; ipPartIndex++) {
      uint8_t hundreds  = ip[ipPartIndex] / 100;
      uint8_t tens      = (ip[ipPartIndex] / 10) % 10;
      uint8_t units     = ip[ipPartIndex] % 10;

      if (ipPartIndex > 0) {
        pixelIndex += 2;
      }

      for (uint8_t i = 0; i < hundreds; i++) {
        strip.setPixelColor(pixelIndex, strip.Color(20, 0, 10, 0));
        pixelIndex++;
      }

      for (uint8_t i = 0; i < tens; i++) {
        strip.setPixelColor(pixelIndex, strip.Color(10, 0, 20, 0));
        pixelIndex++;
      }
    
      for (uint8_t i = 0; i < units; i++) {
        strip.setPixelColor(pixelIndex, strip.Color(0, 20, 10, 0));
        pixelIndex++;
      }
    }


    strip.show();
  }
}