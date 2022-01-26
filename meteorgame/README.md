# Meteor game

A simple meteor game to play with Arduino.

## Hardware

- Arduino Uno
- 1602 LCD display
- Joystick
- Breadboard
- 3k ohm resistors
- wires

## Connections

### LCD display

- LCD rs pin to digital pin 4
- LCD Enable pin to digital pin 6
- LCD D4 pin to digital pin 10
- LCD D5 pin to digital pin 11
- LCD D6 pin to digital pin 12
- LCD D7 pin to digital pin 13
- LCD RW pin to breadboard negative
- LCD V0 pin to GND through 3k ohm resistor
- LCD VDD pin to breadboard positive
- LCD A pin to 5V
- LCD K pin to breadboard negative

### Joystick

- Joystick GND to GND
- Joystick 5V to breadboard positive
- Joystick VRx to analog pin A0
- Joystick VRy to analog pin A1
- Joystick SW to digital pin 2
