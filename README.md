# Websocket ESP32 remote LED strip

Arduino program for ESP32 to control the display of a strip of neopixels via websocket in wifi.


## Installation

Use [PlatformIO](https://platformio.org/) to compile and upload the program. This will take care of downloading the necessary dependencies.

Create a `src/config.h` file based on the `src/config.h.sample` sample.

The wifi ssid and password, for the moment, is hardcoded in the file `src/config.h`

It is also in this file that you should define with `STRIP_LENGTH` the number of pixels of the strip that you want to be able to control via websocket. To this, plan to keep in addition the 2 last pixels which will serve as a strip status indicator.

That is, if your strip has 90 pixels, STRIP_LENGTH should be 88. And pixels 89 and 90 will serve as status indicators


## Demo
Still in file `src/config.h`, the `DEMO_MODE` constant is used to display various animations to test the correct operation of the pixels of the strip.


## Usage
### Connecting to wifi
When the program starts, the two status indicator LEDs light up alternately until the strip is connected to wifi.

### Connected to wifi
So the band displays its ip like this:

![](ip_example.jpg)

  There are 4 groups of dots, each split into hundreds, tens, and units. This forms the IP.

  - Pink dots: **hundreds**
  - Purple dots: **tens**
  - Green dots: **units**

  In this example, it will be the IP 192.168.1.43

NOTE: It takes 79 pixels to be able to display the longest ip (i.e. 199.199.199.199). In practice, 50 pixels are enough to display the majority of local IPs (ex: 192.168.1.199)

### On websocket connection
The strip sends the following data to the client who just connected to it:

```json
{
  "stripLength": 88,
  "colorPerPixel": 4,
  "fpsMax": 50
}
```

### Data to send from client

```js
// Create pixel data array. Each byte is for one color component of a pixel.
// ex: [ r, g, b, w,  r, g, b, w,  r, g, b, w,  r, g, b, w, ... , r, g, b, w ]
//       <--px 0-->   <--px 1-->   <--px 2-->   <--px 3-->  ...   <--px n-->
new Uint8Array(stripLength * colorPerPixel);
```


## Examples

Once you have a strip connected to your wifi network, you can open in your browser one of the files in the "examples" folder to test.

### Minimal example
![](minimal_example.jpg)

```js
const hostname  = prompt('remote strip ip', '192.168.1.43');
const socket    = new WebSocket(`ws://${hostname}`);

socket.onmessage = event => {    
  try {
    const jsonData = JSON.parse(event.data);
    const rawData = new Uint8Array(jsonData.stripLength * jsonData.colorPerPixel);

    for (let i = 0; i < jsonData.stripLength; i++) {
      const r = 255 * (i / jsonData.stripLength);
      const g = 64;
      const b = 255 * (1 - (i / jsonData.stripLength));
      const w = 32;

      rawData[i * jsonData.colorPerPixel]    = Math.floor(r);
      rawData[i * jsonData.colorPerPixel +1] = Math.floor(g);
      rawData[i * jsonData.colorPerPixel +2] = Math.floor(b);
      rawData[i * jsonData.colorPerPixel +3] = Math.floor(w);
    }
    
    socket.send(rawData);
  } catch (e) { }
};
```


### Basic example
![](basic_example.gif)


## Known issues
If you attempt to render an animation via websocket, either at 30 fps or 50 fps, you will notice stuttering every 30 seconds. It seems to be related to the webserver which seems to be running a background task periodically. I have yet to find out exactly where it came from and if there is a way to fix it. Maybe the ESP32 just isn't powerful enough for this type of use, or at least not in this way: ie sending an animation via websocket. I am open to any idea.