# arduino-projects

## Stepper Motor Control

### Arduino Stepper library

- Datasheet: http://users.ece.utexas.edu/~valvano/Datasheets/L293d.pdf
- Arduino Library: https://www.arduino.cc/en/Reference/Stepper
- Adafruit Tutorial: https://learn.adafruit.com/adafruit-arduino-lesson-16-stepper-motors/overview
- L293D H-Bridge

### A4988 Microstepping Driver

- Datasheet: https://www.pololu.com/file/0J450/A4988.pdf
- Tutorial#1: https://www.youtube.com/watch?v=5CmjB4WF5XA#t=21.493344
- Tutorial#2: https://www.youtube.com/watch?feature=player_embedded&v=89BHS9hfSUk
- only 2 (-3) Arduino PINs neccessary to operate a bipolar stepper motor in full step mode
 - STEP PIN: simple HIGH-LOW digitalWrite to perform one step
 - DIRECTION PIN: LOW or HIGH for a particular direction
 - ENABLE PIN: Enable/ disable controller functions (energy consumption)

##  I/O-Pin Expansion

### MCP23017 16-Bit I/O-Expander

- Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf
- Adafruit Library: https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library 

## Arduino OneWire

### Arduino OneWire library to detect DS2401 silicon IDs

- Datasheet: https://www.maximintegrated.com/en/products/digital/memory-products/DS2401.html
- Arduino Library: http://playground.arduino.cc/Learning/OneWire

