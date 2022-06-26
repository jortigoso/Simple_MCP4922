//#pragma once

#ifndef __SIMPLE_MCP4922_H__
#define __SIMPLE_MCP4922_H__

//#if defined(__arm__) && defined(TEENSYDUINO) && defined(KINETISK)

#include "Arduino.h"
#include <SPI.h>

//#if defined(__MK20DX256__)
//#endif

class Simple_MCP4922
{ 
public:
  Simple_MCP4922();

  void begin(uint8_t CS, bool altenratePins);

  void init();

  //Altenernate CS pin considered as pin 9 only
  void setAlternatePins(bool MOSI, bool MISO, bool SCK, bool CS);
  
  bool analogWrite(uint16_t data, uint8_t channel);

  void analogWriteFastA(uint16_t data);

  void analogWriteFastB(uint16_t data);
  
  // Data outputs can be buffered (true) or non-buffered (false)
  void setBuffer(bool buffered);

  //  Gain can be 1 or 2,  (maximum output voltage can never exceed VCC level)
  void setGain(uint8_t gain);

  void freeMOSI();

  void freeMISO();

  void freeSCK();

  //SPI library doesn't control the CS pin.
  //void freeCS();

  void end();

protected:

  uint8_t _pinCS; //SPI library doesn't control the CS pin.

  bool    _buffered;
  uint8_t _gain;

  bool    _alternatePins;
  bool    _altMISO;
  bool    _altMOSI;
  bool    _altSCK;
  bool    _altCS;

  void    transfer(uint16_t data);

};

//#endif
#endif