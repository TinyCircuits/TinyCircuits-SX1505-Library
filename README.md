# TinyCircuits SX1505 Library

This Arduino library was designed for use with TinyCircuits' **[Joystick Wireling](https://tinycircuits.com/collections/wireling-input-output/products/joystick-wireling)** and **[Rotary Switch Wireling](https://tinycircuits.com/collections/wireling-input-output/products/rotary-switch-wireling)**.

*Support this library by buying products from **[TinyCircuits](https://tinycircuits.com/)***

## SX1505 Class

* **int begin(void)**  calls init() to initialize SX1505 for use with Rotary and Joystick - Set I/O to all input, disable PLD input, and enable pull-ups on IO3, 7, 1, 0, 4 (for Joystick) 
* **int init(void)** see begin() for description
* **void write(uint8_t regAddr, uint8_t regData)** writes byte *regData* to register *regAddr*
* **uint8_t read(uint8_t regAddr)** returns byte stored within specified register *regAddr*
* **uint8_t getRegData(void)** return data from SX1505 data register *SX1505_REG_DATA*


## Tiny Joystick Class

This class inherits SX1505 functions and implements specific functionality to use inputs for a discrete joystick.

* **void getPosition(void)** Reads SX1505 ands sets Tiny Joystick member variables *up*, *down*, *left*, and *right* with most recent data


## Tiny Rotary Class

This class inherits SX1505 functions and implements specific functionality to use inputs for a 10-position rotary encoder.

* **uint8_t getPosition(void)** Reads SX1505 and returns rotary direction 0-9 of the 10 available directions on the rotary switch

	
	
	
	
	
