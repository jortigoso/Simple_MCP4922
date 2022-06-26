#include "Simple_MCP4922.h"

Simple_MCP4922::Simple_MCP4922()
{
  init();
}

void Simple_MCP4922::init()
{
  _pinCS         = 10;

  _buffered      = false;
  _gain          = 1;

  _alternatePins = false;
  _altMISO       = false;
  _altMOSI       = false;
  _altSCK        = false;
  _altCS         = false;
}

void Simple_MCP4922::setAlternatePins(bool MOSI, bool MISO, bool SCK, bool CS)
{
  _altMOSI = MOSI;
  _altMISO = MISO;
  _altSCK  = SCK;
  //_altCS   = CS;
}

void Simple_MCP4922::begin(uint8_t CS, bool alternatePins)
{
  _pinCS         =  CS;
  _alternatePins = alternatePins;

  //Set CS to NOT-SELECTED (usually CS pin is active low)
  pinMode(_pinCS, OUTPUT);
  digitalWrite(_pinCS, HIGH);

  if(_alternatePins)
  {
    #if defined(__MK20DX256__)
      //SPI.end(); //Maybe delete for faster operation
      if(_altMOSI)  SPI.setMOSI(7);
      if(_altMISO)  SPI.setMISO(8);
      if(_altSCK)   SPI.setSCK(14);
      //if(_altCS)    SPI.setCS(9);
      SPI.begin();
    #endif
  }
  else
  {
    //According to https://forum.pjrc.com/threads/49562-Audio-functionality-and-SPI-communication-again!-(Teensy-3-2)
    //SPI library doesnt save alternate pin changes so no there is no need to reset them.
    //SPI.end();

    SPI.begin();
  }

  //Apparently this type of settings is deprecated https://arduino.stackexchange.com/questions/79728/when-is-spi-begintransaction-required
  //SPI.setClockDivider(SPI_CLOCK_DIV2); //24MHz (MPC4922 rated for 20MHz)
}

bool Simple_MCP4922::analogWrite(uint16_t data, uint8_t channel)
{

  //| CH | Buff | G  | SH | D11 | D10 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
  //| 15 | 14   | 13 | 12 | 11  | 10  | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |

  //data &= 0x0FFF;         //Cleans the top bits 
  data |= (channel  << 15);
  data |= (1        << 12); //Sets SHDN to 1 (enable output).
  
  if(_buffered)        data |= (1  << 14);
  else                 data |= (0  << 14);

  if(_gain == 1)       data |= (1 << 13);
  else if (_gain == 2) data |= (0 << 13);

  transfer(data);

  return true;
}

void Simple_MCP4922::analogWriteFastCH1(uint16_t data)
{
  // 0x3 = 0011 (CH=0, BUFF=0, GA=1, SHDN=1)
  transfer(data | 0x3000); 
}

void Simple_MCP4922::analogWriteFastCH2(uint16_t data)
{
  // 0x3 = 1011 (CH=1, BUFF=0, GA=1, SHDN=1)
  transfer(data | 0xB000);
}

void Simple_MCP4922::transfer(uint16_t data)
{
  //SPI.transfer sends a byte of data so we need to split it
  uint8_t data_high = data >> 8;   //Gets the 8 MSBs
  uint8_t data_low  = data & 0xFF; //Gets the 8 LSBs

  digitalWrite(_pinCS, LOW); //Select chip 

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(data_high);
  SPI.transfer(data_low);
  SPI.endTransaction();

  digitalWrite(_pinCS, HIGH);
}

void Simple_MCP4922::setBuffer(bool buffered)
{
  _buffered = buffered;
}

void Simple_MCP4922::setGain(uint8_t gain)
{
  _gain =  gain;
}

void Simple_MCP4922::freeMOSI()
{
  #if defined(__MK20DX256__)
  if(_alternatePins)
  {
    //PIN 7
    PORTD_PCR2 = (PORTD_PCR2 & 0xFFFFF8FF) | (001 << 8);
  }
  else
  {
    //PIN 11
    PORTC_PCR6 = (PORTC_PCR6 & 0xFFFFF8FF) | (001 << 8);
  }
  #endif
}

void Simple_MCP4922::freeMISO(){
  #if defined(__MK20DX256__)
  if(_alternatePins)
  {
    //PIN 8
    PORTD_PCR3 = (PORTD_PCR3 & 0xFFFFF8FF) | (001 << 8);
  }
  else
  {
    //PIN 12
    PORTC_PCR7 = (PORTC_PCR7 & 0xFFFFF8FF) | (001 << 8);
  }
  #endif
}

void Simple_MCP4922::freeSCK(){
  #if defined(__MK20DX256__)
  if(_alternatePins)
  {
    //PIN 14
    PORTD_PCR1 = (PORTD_PCR1 & 0xFFFFF8FF) | (001 << 8);
  }
  else
  {
    //PIN 13
    PORTC_PCR5 = (PORTC_PCR5 & 0xFFFFF8FF) | (001 << 8);
  }
  #endif
}

/*
void freeCS(){
  #if defined(__MK20DX256__)
  if(_alternatePins)
  {
    //PIN 9
    PORTC_PCR3 = (PORTC_PCR3 & 0xFFFFF8FF) | (001 << 8);
  }
  else
  {
    //PIN 10
    PORTC_PCR4 = (PORTC_PCR4 & 0xFFFFF8FF) | (001 << 8);
  }
  #endif
}
*/

void Simple_MCP4922::end()
{
  SPI.end();
}