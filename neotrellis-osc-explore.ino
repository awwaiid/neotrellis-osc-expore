
#include "Adafruit_NeoTrellis.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys

#define BRIGHTNESS 10

char ssid[] = "wifi2.42t.org"; // your network SSID (name)
//char pass[] = "****";             // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(10,0,0,184);        // remote IP of your computer
//const unsigned int outPort = 9999;          // remote port to receive OSC
//const unsigned int outPort = 18342;          // remote port to receive OSC
const unsigned int outPort = 8888;          // remote port to receive OSC
const unsigned int localPort = 8889;        // local port to listen for OSC packets (actually not used for sending)

void setup_wifi() {
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(10);
  // WiFi.begin(ssid, pass);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_osc_udp() {
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
}


void sendNoteOn(int channel, int note, int vel) {
  OSCMessage msg("/noteOn");
  msg.add(channel);
  msg.add(note);
  msg.add(vel);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void sendNoteOff(int channel, int note) {
  OSCMessage msg("/noteOff");
  msg.add(channel);
  msg.add(note);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

/*
void loop() {
    Serial.println("Sending noteOn");
    sendNoteOn(0, 67, 120);
    delay(500);
    Serial.println("Sending noteOff");
    sendNoteOff(0, 67);
    // OSCMessage msg("/test");
    // OSCMessage msg("/noteOn");
    // msg.add("hello, osc.");
    // msg.add(1);
    // msg.add(60);
    // msg.add(100);
    // Udp.beginPacket(outIp, outPort);
    // msg.send(Udp);
    // Udp.endPacket();
    // msg.empty();
    delay(5000);
}
*/

// #include <SparkFun_Alphanumeric_Display.h>
// HT16K33 display;


// Create a matrix of trellis panels
Adafruit_NeoTrellis trellis_array[Y_DIM/4][X_DIM/4] = {
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) },
  { Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32) }
};

// Pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)trellis_array, Y_DIM/4, X_DIM/4);

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t wheel(byte wheel_pos) {
  if(wheel_pos < 85) {
    return seesaw_NeoPixel::Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
  } else if(wheel_pos < 170) {
    wheel_pos -= 85;
    return seesaw_NeoPixel::Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  } else {
    wheel_pos -= 170;
    return seesaw_NeoPixel::Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  return 0;
}

void set_brightness(int level) {
	for (int x = 0; x < X_DIM / 4; x++) {
		for (int y = 0; y < Y_DIM / 4; y++) {
		  trellis_array[y][x].pixels.setBrightness(level);
		}
	}
}

// Define a callback for key presses
TrellisCallback blink(keyEvent evt) {

  // Rising == push, Falling == release
  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    int x = evt.bit.NUM % 8;
    int y = evt.bit.NUM / 8;

    Serial.print("Key: ");
    Serial.print(evt.bit.NUM);
    Serial.print(" x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.println("");

    //trellis.setPixelColor(x, y, 0xff0000);
    trellis.setPixelColor(evt.bit.NUM, random(0x1000000));
    trellis.show();
    sendNoteOn(0, evt.bit.NUM + 30, 120);

  } else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    trellis.setPixelColor(evt.bit.NUM, 0);
    trellis.show();
    sendNoteOff(0, evt.bit.NUM + 30);
  }

  return 0;
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_osc_udp();

  if(!trellis.begin()) {
    Serial.println("Failed to begin trellis!");
    while(1);
  } else {
    Serial.println("Trellis started!");
  }

  /* if (display.begin(0x70) == false) {
    Serial.println("AlphNum display did not acknowledge!");
  } else {
    display.print("HIYA");
  } */

  set_brightness(BRIGHTNESS);

  // Fancy random initialization display
  for(int n = 0; n < 10; n++) {
    for(int i=0; i < Y_DIM * X_DIM; i++) {
      trellis.setPixelColor(i, random(0x1000000));
    }
    trellis.show();
    delay(50);
  }

  // Hook up callbacks for every button
  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){
      // activate rising and falling edges on all keys
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      trellis.registerCallback(x, y, blink);
      trellis.setPixelColor(x, y, 0x000000); // Addressed with x,y
      trellis.show(); // Show all LEDs
    }
    delay(5);
  }

  // draw_board();
}

void loop() {
  trellis.read();
  delay(20);
}
