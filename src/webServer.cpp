#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

#include "config.h"
#include "wifiClient.h"


namespace WebServer
{
  Adafruit_NeoPixel _strip;
  AsyncWebServer server(80);
  AsyncWebSocket* websocketPtr;
  boolean isConnected = false;
  void (*_callback)(uint8_t *data, uint count);

  void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
      Serial.println("Websocket client connection received");
      isConnected = true;
      //client->printf("CLIENT_ID:%u", client->id());
      _strip.fill(_strip.Color(0, 0, 0, 0));
      _strip.setPixelColor(PRIMARY_INDICATOR_PIXEL_INDEX, _strip.Color(0, 1, 2, 0));
      _strip.show();

      char stripSpec[64];
      sprintf(stripSpec, "{ \"stripLength\": %u, \"colorPerPixel\": %u, \"fpsMax\": %u }", STRIP_LENGTH, COLOR_PER_PIXEL, FPS_MAX);
      client->text(stripSpec);
      client->ping();


    } else if(type == WS_EVT_DISCONNECT){
      Serial.println("Client disconnected");
      isConnected = false;
      _strip.setPixelColor(PRIMARY_INDICATOR_PIXEL_INDEX, _strip.Color(2, 0, 1, 0));
      WifiClient::diplayIp(_strip);



    } else if(type == WS_EVT_DATA){
      AwsFrameInfo * info = (AwsFrameInfo*)arg;

      if(isConnected && info->final && info->index == 0 && info->len == len){
        uint count = info->len / COLOR_PER_PIXEL;

        _callback(data, count);
      }
    }
  }


  void begin(Adafruit_NeoPixel &strip, void(*callback)(uint8_t *, uint)) {
    _strip = strip;
    _callback = callback;

    if (!websocketPtr) {
      websocketPtr = new AsyncWebSocket("/");
      server.addHandler(websocketPtr);
    }
    websocketPtr->onEvent(onEvent);
    server.addHandler(websocketPtr);
    server.begin();
  }
}