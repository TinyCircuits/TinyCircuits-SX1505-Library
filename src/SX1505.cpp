#include "SX1505.h"
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>

SX1505::SX1505() {
  // default constructor
}

int SX1505::begin() {
  init();
}

// Initialize SX1505 registers for usage with rotary & joystick
int SX1505::init(void) {
  write(SX1505_REGDIR, 0xFF);           // set I/O to all input
    delay(5);
  write(SX1505_REG_PLD_MODE, 0x00);     // disable PLD input (default)
    delay(5);
  write(SX1505_REG_DATA, 0x00);         // clear contents of register data
    delay(5);
  write(SX1505_REG_PULLUP, 0x9B);       // enable pull-up on IO3, 7, 1, 0, 4 (IO used by Joystick)
    delay(5);
  return debug;
}

// writes byte "regData" to register "regAddr"
void SX1505::write(uint8_t regAddr, uint8_t regData) {
  Wire.beginTransmission(SX1505_I2CADDR);
  Wire.write(regAddr);
  Wire.write(regData);
  Wire.endTransmission();
}

// returns byte stored within specified register
uint8_t SX1505::read(uint8_t regAddr) {
  Wire.beginTransmission(SX1505_I2CADDR);
  Wire.write(regAddr);
  debug = Wire.endTransmission(); // debug set to value returned (error checking)

  // Request one data byte
  Wire.requestFrom(SX1505_I2CADDR, 1);
  
  value = (uint8_t)Wire.read();
}

// return data from data register
uint8_t SX1505::getRegData(void) {
  value = 0x00;
  value = read(SX1505_REG_DATA);
  return value;
}

// enable interrupts using the mask passed (active low mask)
void SX1505::setInt(uint8_t mask) {
  write(SX1505_REG_INTMASK, mask);

  // since the switches are active low, for both Joystick and Rotary
  // Switch Wirelings, set the edge sensitivity to a falling edge
  if(mask == ROTARY_INT_MASK) {
    //write(SX1505_REG_SENSE_HI, 0x02); // falling edge for I/O[4]
    write(SX1505_REG_SENSE_HI, 0x03); // both edges for I/O[4]
  } else if(mask == JOYSTICK_INT_MASK) {
    //write(SX1505_REG_SENSE_HI, 0x82); // falling edge for I/O[7], I/O[4]
    write(SX1505_REG_SENSE_HI, 0xC3); // both edges for I/O[7], I/O[4]
  }

  //write(SX1505_REG_SENSE_LO, 0x8A);   // falling edge for I/O[3], I/O[1], I/O[0]
  write(SX1505_REG_SENSE_LO, 0xCF);   // both edges for I/O[3], I/O[1], I/O[0]

  // NOTE: falling edge variants aren't detecting "0" properly on Rotary Switch Wireling
}

// clear interrupts by writing 0xFF to SX1505_REG_INT_SRC
void SX1505::clearInt(void) {
  write(SX1505_REG_INT_SRC, 0xFF);
  // this action also clears the corresponding bits in SX1505_REG_EVENT_STATUS
}

/*TinyJoystick*/

TinyJoystick::TinyJoystick() {
  // default constructor
}

// enable interrupts for Joystick Wireling
void TinyJoystick::setInterrupt() {
  setInt(JOYSTICK_INT_MASK);
}

// return joystick position
void TinyJoystick::getPosition(void) {
  // reset direction variables before setting and sending new values
  up = 0;
  down = 0;
  left = 0;
  right = 0;
  press = 0;

  getRegData();

  uint8_t rawValue = ~value;  // flip bits since active low
  
  if(rawValue & JOYSTICK_UP) {
    up = 1;
  }
  if(rawValue & JOYSTICK_DOWN) {
    down = 1;
  }
  if(rawValue & JOYSTICK_LEFT) {
    left = 1;
  }
  if(rawValue & JOYSTICK_RIGHT) {
    right = 1;
  }
  if(rawValue & JOYSTICK_PRESS) {
    press = 1;
  }
  if (rawValue == JOYSTICK_NEUTRAL){
    up = 0;
    down = 0;
    left = 0;
    right = 0;
    press = 0;
  }
}

TinyRotary::TinyRotary() {
  // default constructor
}

// enable interrupts for Rotary Switch Wireling
void TinyRotary::setInterrupt() {
  setInt(ROTARY_INT_MASK);
}

// return rotary position
uint8_t TinyRotary::getPosition(void) {
  getRegData();

  uint8_t rotaryDirection = 0x00;          // initialize all 0
  uint8_t rawValue = ~value;               // flip bits since active low
  
  rotaryDirection |= (rawValue & 0x08);         // check bit 3, no shift
  rotaryDirection |= ((rawValue & 0x02) << 1);  // check bit 1, shift to bit 2
  rotaryDirection |= ((rawValue & 0x10) >> 3);  // check bit 4, shift to bit 1
  rotaryDirection |= (rawValue & 0x01);         // check bit 0, no shift

  return rotaryDirection;
}
