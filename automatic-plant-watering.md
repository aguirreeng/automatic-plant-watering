# Automatic Plant Watering System

The device will be an automatic plant watering system that will
activate a pump to water the plants from time to time.

## Project Requisites

- on/off lever
- water level control
  - must stop operation if the water level is below a minimum treshold
  - the water level control will use a liquid level on-off switch
    installed on the side of a bucket with a cover that will be the
    water reservoir
- reset button
  - the reset button must reset the time counter and perform an immediate
    watering
- time options (3h, 6h, 12h, 24h)
- on/off LED (green LED)
  - a green LED must be on to show that the system is working
- pump operation LED (yellow LED)
  - a yellow LED must be turned on every time the pump is activated
- water level LED (red LED)
  - A red LED must turn on when the water level is below the minimum
    value
- the system must have a fuse to protect it from power surges

## ATTiny85

This is the microcontroller that I'm going to use in this project.
It is small, consumes very little energy and can be programmed using
the Arduino Uno I have.

### Useful Data

![pintout](attiny-pinout.png)

- 10-bit analog to digital converter in pins PB2, PB3, and PB4
- system reset is activated by a low level on the reset pin

quantity | minimum | maximum
---------|---------|--------
voltage  |    2.7V |    5.5V

## References

I'm using an Arduino shield to program the ATTiny85 that I've build
following the tutorial from Arjun Ganesan that can be found
[here](https://create.arduino.cc/projecthub/arjun/programming-attiny85-with-arduino-uno-afb829).