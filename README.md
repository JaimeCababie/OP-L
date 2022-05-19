# OP-L
##The OP-L is an portable progrmable RGB with a handle. 
![image](https://user-images.githubusercontent.com/98760075/169195585-2c031e6e-ce47-465b-a5a6-c0676219cad4.png)

It is designeg for work ilumination, outdoor ilumination, and for video recordings. 
- The case is 3D printed using ABS like resin. 
- It uses an acrylic panel as a difuser.
- The It uses a matrix ws2812b addressable LEDs, soldered to hand mande printed circuit board.
- The OP-L uses a QTPY  as the brains of the operations.
- To interact with the OP-L There are programable rotary enconders conected via I2C to the QTPY. 
- It has a 4000 mAh battery that lasts as for more than 2 hours in medium brightness.
- Its articulated handle can be adjusted for different positions, or removed to connect to a tripod.

###Programing: 
The OP-L was programmed using Arduino IDE
The I2C Encoders example from adafruit was used to connect multiple encoders to the QTPY usind Stemma Connect. 
Each Seesaw component needs to have a different address for the I2C protocol to work, you can find the Diferent available addresses in this [page](https://learn.adafruit.com/i2c-addresses/the-list) from Adafruit.

For rotary encoders the available addresses are: 
> 0x36 through 0x3D

![image](https://user-images.githubusercontent.com/98760075/169197235-8c63146c-09c5-4231-9703-6ce4a8748de4.png)

The address on the rotary encoder can be modified by clossing either the A0, A1, or A2 on the back of the circuit board. or a combination of them to get up to 8 different I2C addresses. For more informaion visit the [I2C QT Rotary Encoder Guide](https://learn.adafruit.com/adafruit-i2c-qt-rotary-encoder)  

The Sketch Loops through the diferent addresses and declare them as encoders to find which ones are connected and declare them as inputs. 

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

This part of the code inicialices the rotary encoder as well as the neopixel inside the rotary encooder board. 

To controll the Addressable LEDs on the OP-L, the Neopixel Library Is used as well as the Wheel functieon found in the Neopixel Sketch From Adafruit. 

The Wheel funnction maps the RGB values used by the LEDs in a Wheel to achive a smooth transition between the hues they are able to reproduce. 
        
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
To control the color of the Neopixels with the rotary encoder, it is necesary to map the position of the rotary encoder, with the position on the Wheel of colors. 

    for(int i=0; i<NUMPIXELS;i++){
     pixels.setPixelColor(i,Wheel((new_position0*4)&0xFF));
     pixels.setBrightness(brightness);
    }
    pixels.show();
  }
Check out the OP-L code to learn more about how it works!

##Electronics
The comoponents used in the OP-L are: 

- 1 QTPY MO 
- 1 Tp4056 USB C Charging Module
- 1 XI6009e1 Voltage Booster Module
- 2 Adafruit I2C QT Rotary Encoder
- 1 4000mAh Lithium Ion Battery
- 80 WS2812b LEDs 

##Mecanical components

The harware used in the OP-L: 

- 4 brass M3 inserts 
- 4 M3 silver plated screws
- 1 1/4" steel insert for the handle and Tripod Mount
- 1 1/4" Screw 
- 8 self tapping screws to cold the rotary encoders.

Check Out the StEP file for more information!







