# Simple MCP4922 SPI DAC library for Teensy 3.2 boards.

## Simple MCP4922

This library provides a simple and fast way to set up a MCP4922 DAC on a Teensy 3.2 board. The code is simple and short, it was designed to fulfill an specific need of disabling some of the hardware pins used by the SPI interface, but it ended up being *kind of* redundant since (*re*)setting it does the job in a similar way. 

This library is based on [Bob's Tillart MCP_DAC library](https://link-url-here.org) while doing some of the operations in a slightly different way.

## Functionalities

### Constructor 

- **Simple_MCP4922()** The constuctor intializes some of the values to the logical default ones such as gain=1, alternatePins=false etc.
- **init()** Function called in the constructor to intialize the values.
- **begin(uint8_t cs, bool alternatePins)** Recieves a pin to use as CS or Chip Select since the SPI library doesn't take care of it. Also a boolean value that controls the use of some of the altenate pins designated in the interface.

### Settings

- **setBuffer(bool buffered)** *True* enables buffered outputs while *False* disables them.
- **setGain(uint8_t gain)** 
