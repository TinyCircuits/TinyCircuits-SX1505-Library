/*
  Rotary Interrupt Example
  Prints the value of the direction the rotary arrow is pointing 0-9
  to the Serial Monitor on an interrupt trigger

  Written by: Hunter Hykes for TinyCircuits

  https://TinyCircuits.com
*/

#include <Wireling.h>
#include <SX1505.h>            // For interfacing with Joystick and Rotary Switch Wirelings

// Universal Serial Monitor Config
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

/* * * * * * * * * * Rotary * * * * * * * * * */
#define ROT_PORT 0
#define ROT_INT A0
TinyRotary rotary = TinyRotary();
uint8_t rotaryValue = 0;
uint8_t prevValue = 0;

void setup(void) {
  Wireling.begin();
  
  /* * * * * * Rotary Init * * * * */
  Wireling.selectPort(ROT_PORT); 
  rotary.begin();
  rotary.setInterrupt();
  pinMode(ROT_INT, INPUT_PULLUP);
  attachInterrupt(ROT_INT, ISR_Rotary, FALLING);

  SerialMonitorInterface.begin(9600);
}

void loop() {
  // do nothing until the interrupt occurs

  if(rotaryValue != prevValue) {
    SerialMonitorInterface.println(rotaryValue); // print the value acquired by the ISR
    prevValue = rotaryValue;
  }
}

void ISR_Rotary() {
  rotaryValue = rotary.getPosition(); // get the new value
  rotary.clearInt();                  // clear the interrupt flag
}
