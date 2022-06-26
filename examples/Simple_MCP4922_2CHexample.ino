#include "Simple_MCP4922.h"

#define CS 10

uint16_t data_array_1[6] = {0, 819, 1638, 2457, 3276, 4095};
uint16_t data_array_2[6] = {0, 819, 1638, 2457, 3276, 4095};

uint8_t i = 0;
uint8_t j = 0;

Simple_MCP4922 DAC;

void setup() {
  DAC.begin(CS, false);
}

void loop() {

  //Using analogWrite
  DAC.analogWrite(data_array_1[i],0);
  DAC.analogWrite(data_array_2[j],1);

  //Using analogWriteFast
  DAC.analogWriteFastCH1(data_array_1[i]);
  DAC.analogWriteFastCH2(data_array_2[j]);
  
  if(i < 5) i++;
  else i = 0;
   
  if(j < 5) j++;
  else j = 0;

  delay(600); 
}