#include <Adafruit_NeoPixel.h>
//defining neopixel matrix 
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 80 // Popular NeoPixel ring size
int brightness=1;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//defining encoders and their neopixels 
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#define SS_SWITCH        24      // this is the pin on the encoder connected to switch
#define SS_NEOPIX        6       // this is the pin on the encoder connected to neopixel
#define SEESAW_BASE_ADDR          0x37  // I2C address, starts with 0x36


// create 4 encoders!
Adafruit_seesaw encoders[2];
// create 4 encoder pixels
seesaw_NeoPixel encoder_pixels[2] = {
  seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800),
  seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800)
};

int32_t encoder_positions[] = {0, 0};
bool found_encoders[] = {false, false};

void setup() {
  Serial.begin(115200);

 
  for (uint8_t enc = 0; enc < sizeof(found_encoders); enc++) {
    // See if we can find encoders on this address
    if (! encoders[enc].begin(SEESAW_BASE_ADDR + enc) ||
        ! encoder_pixels[enc].begin(SEESAW_BASE_ADDR + enc)) {
      Serial.print("Couldn't find encoder #");
      Serial.println(enc);
    } else {
      Serial.print("Found encoder + pixel #");
      Serial.println(enc);

      uint32_t version = ((encoders[enc].getVersion() >> 16) & 0xFFFF);
      if (version != 4991) {
        Serial.print("Wrong firmware loaded? ");
        Serial.println(version);
        while (1) delay(10);
      }
      Serial.println("Found Product 4991");

      // use a pin for the built in encoder switch
      encoders[enc].pinMode(SS_SWITCH, INPUT_PULLUP);

      // get starting position
      encoder_positions[enc] = encoders[enc].getEncoderPosition();

      Serial.println("Turning on interrupts");
      delay(10);
      encoders[enc].setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
      encoders[enc].enableEncoderInterrupt();

      // set not so bright!
      encoder_pixels[enc].setBrightness(30);
      encoder_pixels[enc].show();

      found_encoders[enc] = true;
    }
  }

  Serial.println("Encoders started");
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {


  for (uint8_t enc = 0; enc < sizeof(found_encoders); enc++) {
    if (found_encoders[enc] == false) continue;
//encoder0
    int32_t new_position0 = encoders[0].getEncoderPosition();
    // did we move around?
    if (encoder_positions[0] != new_position0) {
      Serial.print("Encoder #");
      Serial.print(0);
      Serial.print(" -> ");
      Serial.println(new_position0);         // display new position
      encoder_positions[0] = new_position0;

      // change the neopixel color, mulitply the new positiion by 4 to speed it up
      encoder_pixels[0].setPixelColor(0, Wheel((new_position0 * 4) & 0xFF));
      encoder_pixels[0].show();
    }
//encoder1
    int32_t new_position1 = encoders[1].getEncoderPosition();
    // did we move around?
    if (encoder_positions[1] != new_position0) {
      Serial.print("Encoder #");
      Serial.print(1);
      Serial.print(" -> ");
      Serial.println(new_position0);         // display new position
      encoder_positions[1] = new_position1;

      // change the neopixel color, mulitply the new positiion by 4 to speed it up
      encoder_pixels[1].setPixelColor(0,255,255,255);
      brightness=(abs(new_position1)+1%125);
      encoder_pixels[1].setBrightness(brightness);
      encoder_pixels[1].show();
    }
 
//control de pixeles 
    for(int i=0; i<NUMPIXELS;i++){
     pixels.setPixelColor(i,Wheel((new_position0*4)&0xFF));
     pixels.setBrightness(brightness);
    }
    pixels.show();
  }

 // don't overwhelm serial port
  yield();
  delay(10);
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
