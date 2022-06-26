# Simple MCP4922 SPI DAC library for Teensy 3.2 boards.

## Simple MCP4922

This library provides a simple and fast way to set up a MCP4922 DAC on a Teensy 3.2 board. The code is simple and short, it was designed to fulfill an specific need of disabling some of the hardware pins used by the SPI interface, but it ended up being *kind of* redundant since (*re*)setting it does the job in a similar way. 

This library is based on [Bob's Tillart MCP_DAC library](https://github.com/RobTillaart/MCP_DAC) while doing some of the operations in a slightly different way.

## Functionalities

### Constructor 

- **Simple_MCP4922()** The constuctor intializes some of the values to the logical default ones such as gain=1, alternatePins=false etc.
- **init()** Function called in the constructor to intialize the values.
- **begin(uint8_t cs, bool alternatePins)** Recieves a pin to use as CS or Chip Select since the SPI library doesn't take care of it. Also a boolean value that controls the use of some of the altenate pins designated in the interface.

### Settings

- **setBuffer(bool buffered)** *True* enables buffered outputs while *False* disables them.
- **setGain(uint8_t gain)** The attribute *_gain* should be either 1 or 2, usually I would have had 0 or 1 for gain 1 and 2 respectively, but the DAC sets gain to 1 when sent a high level bit, so I thought it was more convenient this way. 
- **end()** can be used to call SPI.end() to disable the interface.

### Disabling certain pins of the interface

A SPI hardware interface tipically has 4 pins: MISO for data input, MOSI for data output, CSK for the clock and CS for selecting the peripheral device.

For example, seting MISO pin as pinMode(CS, OUTPUT) will give the GPIO the control of the pin even tho the SPI interface was using it. Athough this *works*, I included dedicated functions to change the pin control "completely". A multiplexor sets which system controls each pin, so its possible to write to the register in order to modify which system can use it.

    // PORTx_PCRn, for pin 12 (default MOSI):
    PORTC_PCR7 = (PORTC_PCR7 & 0xFFFFF8FF) | (001 << 8);

Bits 8-10 in the register control the multiplexor as shown in the [manual](https://www.pjrc.com/teensy/K20P64M72SF1RM.pdf) for Tensy 3.2 on page 228.

- **freeMOSI()** Gives GPIO the control of pin 11 or pin 7 if alternatePins is *True*.
- **freeMISO()** Gives GPIO the control of pin 12 or pin 8 if alternatePins is *True*.
- **freeCSK()** Gives GPIO the control of pin 13 or pin 14 if alternatePins is *True*.
- Freeing CS is not necessary since its not controlled by the library.

## Ouputting data

- **analogWrite(uint16_t data, uint8_t channel)** Channel can be either 0 (OutputA) or 1 (OutputB).
- **analogWriteFastCH1(uint16_t data)** and **analogWriteFastCH2(uint16_t data)** ignore buffer and gain settings and use the default values (unbuffered and G=1).

## Setting up the DAC

**SHDN** pin should be connected to high level since it enables the output. If you have spare pins a microcontroller pin can be used to switch between *enabled* and *disabled* in order to save power.

The rest of the pins are listed in the datasheet, a full list of connections for basic operation is provided below.

| DAC   | PIN | Connection                |
|-------|-----|---------------------------|
| VDD   | 1   | VDD                       |
| NC    | 2   | x                         |
| !CS   | 3   | CS: Teensy3.2 Pin 10      |
| SCK   | 4   | SCK: Teensy 3.2 Pin 13    |
| SDI   | 5   | SDI: Teensy 3.2 Pin 11    |
| NC    | 6   | x                         |
| NC    | 7   | x                         |
| !LDAC | 8   | GND                       |
| !SHDN | 9   | VDD                       |
| VoutB | 10  | Channel 2 Ouput           |
| VrefB | 11  | Channel 2 Reference (VDD) |
| VSS   | 12  | GND                       |
| VrefA | 13  | Channel 1 Reference (VDD) |
| VrefB | 14  | Channel 1 Output          |

The code below shows *Simple_MCP4922_example.ino*. It's written to output values on channel A (or 1), connections to the DAC should be as shown in the table above.

    #include "Simple_MCP4922.h"

    #define T_PIN_CS 10

    uint16_t data_array[6] = {0, 819, 1638, 2457, 3276, 4095};
    uint8_t i = 0;

    // Create a Simple_MCP4922 object.
    Simple_MCP4922 DAC;

    void setup() 
    {    
      // Start the SPI hardware controller.
      // The Simple_MCP4922 library takes care of importing
      // the SPI and setting up the chip select pin.
      DAC.begin(T_PIN_CS, false);
    }

    void loop() 
    {

      // Writes to channel 1
      DAC.analogWrite(data_array[i], 1);
      if(i < 5) i++;
      else i = 0;

      delay(600); //msas
      
    }