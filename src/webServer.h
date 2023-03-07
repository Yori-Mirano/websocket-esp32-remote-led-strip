#ifndef WEBSERVER_H
#define WEBSERVER_H

  #include <ESPAsyncWebServer.h>
  #include <Adafruit_NeoPixel.h>

  namespace WebServer
  {
    void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    void begin(Adafruit_NeoPixel &strip, void(*callback)(uint8_t *, uint));
  }

#endif