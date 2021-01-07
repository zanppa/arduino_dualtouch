# arduino_dualtouch
A simple capacitive touch sensing class for arduino, using two sensors

## Usage
Connect two touch sensors to any analog I/O pins of Arduino. I used two pieces of copper foil, about 1 x 2 cm, spaced 1 cm of each other. Insulate the sensors so that user can not directly touch the sensors (e.g. using a plastic sheet). No resistors or anything is needed.

Code example is below. Depending on the sensor sizes etc. you may need to tune the sensitivity (with ```setThreshold(float)```) or the filtering gains (with ```setFilterGains(float, float)```).

## Code example
This simple code (which I did not test ;) initializes the two sensors and then prints the press status over serial.

    #include "DualTouch.h"
    
    DualTouch touchSensors(18, 19);   // Sensors in pins 18 and 19 (Arduino pro micro A0 and A1)
    
    void setup() {
      Serial.being(9600);
    }
    
    void loop() {
      touchSensors.update();
      Serial.print(touchSensors.leftTouched());
      Serial.print('\t');
      Serial.println(touchSensors.rightTouched());
      delay(100);
    }

