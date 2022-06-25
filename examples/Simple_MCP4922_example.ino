/*
  This example shows the most basic functionalities.
  begin() function sets the designated chip select 
  pin since the SPI library doesn't control it.

  When the second begin() parameter is false, the default 
  pins on the teensy 3.2 board are selected (the darker ones).
  When provided with true some of the alternate pins are 
  selected: 7,8,9,14
*/

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

// END OF FILE