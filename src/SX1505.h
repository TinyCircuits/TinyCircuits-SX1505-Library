/*
  SX1505 Arduino Library
  Written for use with the Rotary Switch and Joystick Wirelings
  featuring the SX1505 I2C I/O Expander.

  Written July 2019
  By Hunter Hykes
  Modified Feb 2020
  By Laveréna Wienclaw
*/

#ifndef SX1505_h
#define SX1505_h

// Definitions for SX1505 I2C I/O Expander
#define SX1505_I2CADDR		          0x20

#define SX1505_REG_DATA	                  0x00
#define SX1505_REGDIR			  0x01	// 0 -> output, 1 -> input
#define SX1505_REG_PULLUP		  0x02 	// 0 -> pull-up disabled, 1 -> pull-up enabled
#define SX1505_REG_PULLDOWN		  0x03

#define SX1505_REG_INTMASK		  0x05
#define SX1505_REG_SENSE_HI		  0x06
#define SX1505_REG_SENSE_LO		  0x07
#define SX1505_REG_INT_SRC		  0x08
#define SX1505_REG_EVENT_STATUS	          0x09
#define SX1505_REG_PLD_MODE		  0x10
#define SX1505_REG_PLD_T0		  0x11
#define SX1505_REG_PLD_T1		  0x12
#define SX1505_REG_PLD_T2		  0x13
#define SX1505_REG_PLD_T3		  0x14
#define SX1505_REG_PLD_T4		  0x15

// Definitions of direction bits
// NOTE: UP is the direction if pushing the
// joystick towards the whisker connector.
#define JOYSTICK_UP			  0x02 	// 185 raw
#define JOYSTICK_DOWN		          0x80 	// 59  raw
#define JOYSTICK_LEFT		  	  0x01 	// 186 raw
#define JOYSTICK_RIGHT			  0x08 	// 179 raw
#define JOYSTICK_PRESS			  0x10 	// 171 raw
#define JOYSTICK_NEUTRAL		  0x44	// 187 raw

// use to enable interrupts on all joystick inputs (active low)
#define JOYSTICK_INT_MASK			0x64

// use to enable interrupts on all rotary inputs (active low)
#define ROTARY_INT_MASK				0xE4


#include <inttypes.h>

class SX1505 {
public:
	SX1505(void);
	int begin(void);
  int init(void);
	void write(uint8_t, uint8_t);
	uint8_t read(uint8_t);
	uint8_t getRegData(void);
	void setInt(uint8_t);
	void clearInt(void);

	uint8_t value;
	uint8_t debug;
	uint8_t I2Caddress = 0;
};

class TinyJoystick : public SX1505 {
public:
	TinyJoystick(void);
	void setInterrupt(void);
  void getPosition(void);
  uint8_t active = 0;
	uint8_t up = 0;
	uint8_t down = 0;
	uint8_t left = 0;
	uint8_t right = 0;
	uint8_t press = 0;
};

class TinyRotary : public SX1505 {
public:
	TinyRotary(void);
	void setInterrupt(void);
	uint8_t getPosition(void);
};

#endif
