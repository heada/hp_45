#include <Arduino.h>
#ifndef CONSTANTS_H_
#define CONSTANTS_H_


#define RED_LED   4	//Rote LED
#define GREEN_LED 5 //Gruene LED
 const uint8_t temp_Pin = A9;
 const uint8_t r10x_Pin = A8;
 //Düsenansteuerung
 const byte clkP_on_1_7 = B00001000;
 const byte clkP_off_1_7 = B11110111; ;
 const byte clrP_on_1_7 = B00000100;
 const byte clrP_off_1_7 = B11111011;

 const byte clrA_on  = B00010000;
 const byte clrA_off = B11101111;
 const byte clkA_off = B11011111;
 const byte clkA_on  = B00100000;

 const byte clkP_on_8_14  = B00000010;
 const byte clkP_off_8_14 = B11111101;
 const byte clrP_on_8_14  = B00000001;
 const byte clrP_off_8_14 = B11111110;
 const byte off 		  = B00000000;



#endif /* HP45LIB_CONSTANTS_H_ */
