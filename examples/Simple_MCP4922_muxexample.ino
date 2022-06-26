  /*
   * This test is built to check 2 of the main capabilities of the library 
   * (I wrote it to make this specific functions). One of the objectives is 
   * to mux the MISO pin, most of the time it will function as a digital
   * input pin with an interrupt and internall pullup. 
   * 
   * NOTE: Refer to "Disabling certain pins of the interface" chapter of
   * the README.md for more info.
   */

#include "Simple_MCP4922.h"
#include <IntervalTimer.h>

#define T_PIN_CS = 10;
#define T_PIN_MISO 12
#define T_PIN_LED 6
#define T_PIN_PLAY 7


uint8_t led_flag = 0;
uint8_t prev_index = 0;
float t = 600000; //us 
uint8_t main_index = 0;
uint16_t data_array[6] = {0, 819, 1638, 2457, 3276, 4095};
uint8_t tx_flag = 0;
uint8_t play_flag = 0;
uint8_t first_play = 1;
uint8_t first_off = 1; 

IntervalTimer timer;
Simple_MCP4922 DAC;

void PIN12_IRQHandler(){
  led_flag ^= 1; //Toggles LED state
  Serial.print("Led Flag: ");
  Serial.println(led_flag);
}

void PIN7_IRQHandler(){
  play_flag ^= 1;
  tx_flag = 1;
  Serial.print("Play Flag: ");
  Serial.println(play_flag);
}

void timer_IRQHandler(){
  prev_index = main_index;
  if(main_index < 5){
    main_index++;
  }else{
    main_index = 0;
  }
  tx_flag = 1;
  timer.begin(timer_IRQHandler, t);
  Serial.println("Timer Interrupt!");
}

void WRITE_DAC(uint16_t data){
  bool aux = DAC.analogWrite(data,1);
  Serial.print("Analog Write: ");
  Serial.println(aux);
  tx_flag = 0;  
}

void setup() {

   Serial.begin(9600);
   while(!Serial);
   DAC.begin(T_PIN_CS, false);

   //Refer to "Disabling certain pins of the interface" chapter of the README.md for more info
   //about this function call.
   //DAC.freeMISO();

   #if defined(__MK20DX256__)
   Serial.println("Using MK20DX256, Teensy 3.2!");
   #else 
   Serial.println("Not compiled for a MK20DX256 (Teensy 3.2) board.");
   #endif

   #if defined(PORTC_PCR7)
   Serial.print("MISO Mux State: ");
   //Serial.println(PORTC_PCR7 & ~0xFFFFF8FF, BIN);
   Serial.println(PORTC_PCR7, BIN);
   #endif
  
   //pinMode(PIN_CS, OUTPUT); // SPI library doesn't control CS (also set in the MCP library) 
   pinMode(T_PIN_MISO, INPUT_PULLUP); //PIN we want to MUX
   attachInterrupt(digitalPinToInterrupt(T_PIN_MISO), PIN12_IRQHandler, FALLING);
   
   pinMode(T_PIN_LED, OUTPUT);
   digitalWrite(T_PIN_LED, LOW);

   pinMode(T_PIN_PLAY, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(T_PIN_PLAY), PIN7_IRQHandler, FALLING);
}

void loop() {
  
  if(play_flag){
    if(first_play){
        timer.begin(timer_IRQHandler, t);
        first_play = 0;
        first_off = 1; //Prepare for when actually off 
    }
   if(tx_flag)  WRITE_DAC(data_array[main_index]);
  }else{
   if(first_off) timer.end();
  }
  
  if(led_flag)  digitalWrite(T_PIN_LED, HIGH);
  else          digitalWrite(T_PIN_LED, LOW);
}