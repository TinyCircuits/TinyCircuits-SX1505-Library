/*
  Joystick Interrupt Example
  Prints the direction the joystick is pushed
  (UP, DOWN, LEFT, RIGHT, PRESSED)
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

/* * * * * * * * * * Joystick * * * * * * * * * */
#define JS_PORT 0
#define JS_INT A0
TinyJoystick joystick = TinyJoystick();
uint8_t joystickValue = 0;
uint8_t prevValue = 0;
bool intFlag = 0;

void setup(void) {
  Wireling.begin();
  
  /* * * * * * Rotary Init * * * * */
  Wireling.selectPort(JS_PORT); 
  joystick.begin();
  joystick.setInterrupt();
  pinMode(JS_INT, INPUT_PULLUP);
  attachInterrupt(JS_INT, ISR_Joystick, FALLING);

  SerialMonitorInterface.begin(9600);
}

void loop() {
  // do nothing until the interrupt occurs

  if(intFlag) {
    writeToMonitor(); // print the value acquired by the ISR
    intFlag = 0;
  }
}

void writeToMonitor() {
  if(joystick.up) {
    SerialMonitorInterface.println("UP (direction facing 5-pin connector)");
  }
  else if(joystick.down) {
    SerialMonitorInterface.println("DOWN");
  }
  else if(joystick.left) {
    SerialMonitorInterface.println("LEFT");
  }
  else if(joystick.right) {
    SerialMonitorInterface.println("RIGHT");
  }
  else if(joystick.press) {
    SerialMonitorInterface.println("PRESSED");
  }

}

void ISR_Joystick() {
  intFlag = 1;
  joystick.getPosition(); // get the new value
  joystick.clearInt();    // clear the interrupt flag in the SX1505
}
