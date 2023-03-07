#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

  #include <Adafruit_NeoPixel.h>

  namespace WifiClient
  {
    void connect(const std::function<void(void)> &callback);
    void diplayIp(Adafruit_NeoPixel &pixels);
  }

#endif