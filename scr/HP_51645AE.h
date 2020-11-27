//#include "8x8_horizontal_LSB_1.h"

#include <Arduino.h>
#include <stdlib.h>
#include "bit_arry.h"
// uncomment to use SMT32_F103 Microcontroller
//#define SMT32_F103
#ifdef SMT32_F103
#define PRIMITIV_LOWER_CLR_PIN 0x0	//0000000000000001 PA0
#define PRIMITIV_LOWER_CLK_PIN 0x1 	//0000000000000010 PA1
#define PRIMITIV_UPPER_CLR_PIN 0x2 	//0000000000000100 PA2
#define PRIMITIV_UPPER_CLK_PIN 0x4 	//0000000000001000 PA3
#define ADRESS_CLR_PIN 0x8   	//0000000000010000	PA4
#define ADRESS_CLK_PIN 0x10		//0000000000100000  PA5


#endif 

// uncomment to use AVR_MEGA_2650 Microcontroller
#define AVR_MEGA_2650
#ifdef AVR_MEGA_2650
 #define clkP_on_1_7  B00001000
 #define clkP_off_1_7  B11110111; 
 #define clrP_on_1_7  B00000100
 #define clrP_off_1_7  B11111011

 #define clrA_on   B00010000
 #define clrA_off  B11101111
 #define clkA_off  B11011111
 #define clkA_on   B00100000

 #define clkP_on_8_14   B00000010
 #define clkP_off_8_14  B11111101
 #define clrP_on_8_14   B00000001
 #define clrP_off_8_14  B11111110
 #define off 		   B00000000
#endif

//#define SAM3X8E
#ifdef SAM3X8E
	#define 


#endif

//General 
#define ADRESSES 22
#define PRIMTIVES 14
#define BYTE_PER_LINE 44
#define BLOCK_SIZE 150
//Buffer
#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1

#define BUFFER_SIZE 150

#define SUCCESS 0
#define FAIL 1
// MAX_HEAT_TIME legt die maximal einstellbare Heizzeit fest
#define MAX_HEAT_TIME 10

#define BAUDRATE 9600

#ifndef HP_51645AE_H
#define HP_51645AE_H
class HP_51645AE
{
private:
const uint8_t adr=22;
uint8_t HEAT_TIME;     // Zeit zum aufheitzen
uint8_t wait_same;  //Zeit zwischen dem nutzen der selben D�se
uint8_t wait_next;  // Zeit zwischen dem nuten verschiedener D�sen
int t_s,bit,tmp_a, tmp_b, time;
byte tmp_p, status;
byte* pr_Array;
NOZZLE_T noz;
	//Buffer variables
	
struct Buffer {
  uint8_t data[BYTE_PER_LINE][BLOCK_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem �ltesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
 // uint8_t read_block; 
 // uint8_t write_block;
} buffer = {{}, 0, 0};
	
//byte NOZZLE_ARRAY_BUFFER [BYTE_PER_LINE][BLOCK_SIZE];
uint8_t numOfBytesInCol;
uint8_t numOfBitsInRow;
uint8_t lastPrintedCol;
// There 2 modes 300 or 600 dpi 
// printResolutionFlag = 1 -> 300 dpi
// printResolutionFlag = 2 -> 600 dpi defalut
uint8_t printResolutionFlag;	
public:

    HP_51645AE();
    HP_51645AE(unsigned int _heat = 2,unsigned int _wait_same= 800,unsigned int _wait_next=2);
	void shot(int,int);			// Prim und Adresse werden �bergeben
	void shot_all(int,int, int);
	void shotBitmap();
	void shotBitmap_fast();
	void printBitmap();	
	void printBitmap_fast();
    bool button(int);
    void test_print(uint32_t);
    void char_shot(const byte, int);
    void text(String,int, char);
    void test_fk(uint32_t);
	uint8_t nozzle_test(uint8_t,uint8_t);
	void store_bitmap(int, int);
	void store_bitmap_fast(int, int);
	void heatdelay(uint16_t);
	uint8_t	addPrintDataToBuffer(uint8_t*);
	uint8_t getPrintDataFromBuffer(uint8_t*);
    uint8_t processControlData(uint8_t);
	void printCol(uint8_t*);
	void printColFast(uint8_t*);
	void printNozzle(uint16_t);
	 ~HP_51645AE();
};
#endif // HP_51645AE_H
