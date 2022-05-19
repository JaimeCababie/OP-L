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

'  for (uint8_t enc = 0; enc < sizeof(found_encoders); enc++) {
    // See if we can find encoders on this address
    if (! encoders[enc].begin(SEESAW_BASE_ADDR + enc) ||
        ! encoder_pixels[enc].begin(SEESAW_BASE_ADDR + enc)) {
      Serial.print("Couldn't find encoder #");
      Serial.println(enc);
    } else {
      Serial.print("Found encoder + pixel #");
      Serial.println(enc);
      '







