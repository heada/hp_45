#include "HP_51645AE.h"
#include <Arduino.h>
#include "8x8_horizontal_LSB_1.h"
#include "bmp.h"
#include "nozz_array.h"
#include "math.h"
#include "bit_arry.h"
#include "globals.h"
#include <stdint.h> 
#include "PROGMEM_reader.h"
//Der Konstruktor erh�lt drei Uebergabeparamater vom Typ uint. 
// 1. Parameter definiert die Heizzeit f�r eine D�se. Limitiert durch MAX_HEAT_TIME
// 2. Parameter definiert die Zeit zwischen der Aktivierung der selben Duese
// 3. Parameter definiert die Zeit zwischen Aktivierung aufeinanderfolgender Duesen

// Innerhalb des Konstruktors werden doi noetigen Outputregister fuer den Ausgewaehlten CPU gesetzt 
 
HP_51645AE::HP_51645AE(unsigned int _heat, unsigned int _wait_same, unsigned int _wait_next)
{
	// Variablen Definition

	bit = tmp_a = tmp_b = tmp_p =  t_s = status  = time =0;

	//led 	= new LED();		// LED Initialisierung	//
	pr_Array = new byte [44];
	if(_heat >= MAX_HEAT_TIME)
	{HEAT_TIME = MAX_HEAT_TIME;}
	else{
    HEAT_TIME = _heat;
	}
    wait_same = _wait_same;
    wait_next = _wait_next;
	
	/*for(int i = 0;i<BYTE_PER_LINE;i++)
	{
		for(int j = 0;j<BLOCK_SIZE;j++ )
		{
			NOZZLE_ARRAY_BUFFER [i][j] = 0;
		}
		
	}*/
	
	numOfBytesInCol = numOfBitsInRow = lastPrintedCol = 0;
	// Aktiviern der I/O fuer CPU AVR_MEGA_2650	
	#ifdef AVR_MEGA_2650
	DDRB |= 0xff; 
	DDRF |=	0xff;
	#endif
// set Output Pins for SMT32_F103 CPU
	#ifdef SMT32_F103
	 pinMode(PA0,OUTPUT);
	 pinMode(PA1,OUTPUT);
	 pinMode(PA2,OUTPUT);
	 pinMode(PA3,OUTPUT);
	 pinMode(PA4,OUTPUT); 
	 pinMode(PA5,OUTPUT);
	 pinMode(PA6,OUTPUT);
	 pinMode(PA7,OUTPUT);
	 pinMode(PB3,OUTPUT);
	 pinMode(PB4,OUTPUT);
	 pinMode(PB5,OUTPUT);
	 pinMode(PB6,OUTPUT);
	 pinMode(PB7,OUTPUT);
	 pinMode(PB8,OUTPUT);
	 pinMode(PB9,OUTPUT);
	 #endif
}
HP_51645AE::~HP_51645AE(){

delete[] pr_Array;

}
/*
Activates ports to trigger TLC59213x Source Driver and CD4051BE Multiplexer
	in : int prim
*/
void HP_51645AE::shot_all(int prim1_7, int prim8_14, int adr)		//Aktivieren einer D�se
{  
				#ifdef AVR_MEGA_2650
					PORTF &= clkA_off; 			//clk ADR 0
					PORTF |= clrA_on;				//clr ADR 1
					PORTB |= adr_Pins[adr];	//setzen der Adress Bits ->HIGH
					PORTF |= clkA_on; 			//clk ADR 1
					PORTF &= clkA_off;  			//clk ADR 0
					PORTB &= off;			//setzen der Adress Bits ->LOW
				#endif
				#ifdef SMT32_F103
					GPIOA->regs->ODR &= (0<<ADRESS_CLK_PIN);
					//delayMicroseconds(1);
					GPIOA->regs->ODR |= (1<<ADRESS_CLR_PIN);
					//delayMicroseconds(1);
					GPIOB->regs->ODR |= adr_Pins[adr];
					//delayMicroseconds(1);
					GPIOA->regs->ODR |= (1<<ADRESS_CLK_PIN);
					//delayMicroseconds(1);
					GPIOA->regs->ODR &= (0<<ADRESS_CLK_PIN);
					//delayMicroseconds(1);
					GPIOB->regs->ODR &= 0b1111110000000111; // set bit 3-9 Off
				#endif	 
				#ifdef AVR_MEGA_2650  
					//Prim 1_7
					PORTF &= clkP_off_1_7;	  			//CLK Prim 1-7 LOW
					PORTF |= clrP_on_1_7;					//CLR PRIM 1-7 HIGH
					PORTB |= prim1_7;						//setzen der Primaer Bits
					PORTF |= clkP_on_1_7;					//CLK Prim 1-7 HIGH
					PORTF &= clkP_off_1_7;				//CLK Prim 1-7 LOW
					delayMicroseconds(HEAT_TIME); 				//heat
					PORTF |= clrP_on_1_7;;
					PORTF &= clrP_off_1_7;
					  //Prim 8_14
					PORTF &= clkP_off_8_14;				//CLK Prim 8-14 LOW
					PORTF |= clrP_on_8_14;				//CLR PRIM 8-14 HIGH
					PORTB |= prim8_14;			//setzen der Primaer Bits
					PORTF |= clkP_on_8_14;				//CLK Prim 8-14 HIGH
					PORTF &= clkP_off_8_14;				//CLK Prim 8-14 LOW
					PORTB &= off;			// setzen der Prim Bits ->LOW
					delayMicroseconds(HEAT_TIME); 		//heat
					PORTF |= clrP_on_8_14;
					PORTF &= clrP_off_8_14;
					// Adr off
					PORTF |= clrA_on;
					PORTF &= clrA_off;
					PORTB &= off;
				#endif 
				#ifdef SAM3X8E

				
				#endif
	  delayMicroseconds(wait_next); 
}
void HP_51645AE::shot(int prim, int adr)		//Aktivieren einer D�se
{
if(prim%2!=0)
			{
			#ifdef AVR_MEGA_2650
				//ADR odd side
				  PORTF &= B11011111; 			//clk ADR 0
			      PORTF |= B00010000;			//clr ADR 1
				  PORTB |= adr_Pins_ODD[adr];	//setzen der Adress Bits ->HIGH
				  PORTF |= B00100000; 			//clk ADR 1
			      PORTF &= B11011111;  			//clk ADR 0
			      PORTB &= B00000000;			//setzen der Adress Bits ->LOW
			#endif
			  
			#ifdef SMT32_F103
			   	GPIOA->regs->ODR &= ~(1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<ADRESS_CLR_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR |= adr_Pins_ODD[adr];
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR &= ~(1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR &= 0b1111110000000111; // set bit 3-9 Off
			#endif
			   
			   
			      delayMicroseconds(1);
			}
		else
			{
			#ifdef AVR_MEGA_2650
				PORTF &= B11011111; 			//clk 0
			    PORTF |= B00010000;			//clr 1
				PORTB |= adr_Pins_EVEN[adr];	//setzen der Adress Bits ->HIGH
				PORTF |= B00100000; 			//clk 1
			    PORTF &= B11011111;  			//clk 0
			    PORTB &= B00000000;			//setzen der Adress Bits ->LOW
			#endif
				
			#ifdef SMT32_F103
			   	GPIOA->regs->ODR &= ~(1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<ADRESS_CLR_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR |= adr_Pins_EVEN[adr];
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR &= ~(1<<ADRESS_CLK_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR &= 0b1111110000000111; // set bit 3-9 Off
		   #endif
			   
			   	   delayMicroseconds(1);
			}		      	      
			 if(prim>6) 
			{
			#ifdef AVR_MEGA_2650
					//Prim 2
				  PORTF &= B11110111;	  		//CLK Prim 7-14 LOW
				  PORTF |= B00000100;			//CLR PRIM 7-14 HIGH
				  PORTB |= prim_Pins[prim];		//setzen der Primaer Bits
				  PORTF |= B00001000;			//CLK Prim 7-14 HIGH
			  	  PORTF &= B11110111;			//CLK Prim 7-14 LOW
				  PORTB &= B10000000;			// setzen der Prim Bits ->LOW
				  delayMicroseconds(HEAT_TIME); 		//heat im Durchschnitt 2,5 �s bei 2				 
				  PORTF |= B00000100;			
				  PORTF &= B11111011;  
			#endif
			
			#ifdef SMT32_F103
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_UPPER_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<PRIMITIV_UPPER_CLR_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR |= prim_Pins[prim];
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<PRIMITIV_UPPER_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_UPPER_CLK_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR &= 0b1111110000000111; // set bit 3-9 Off
				delayMicroseconds(HEAT_TIME); 
			
				GPIOA->regs->ODR |= (1<<PRIMITIV_UPPER_CLR_PIN);
				delayMicroseconds(1);				
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_UPPER_CLR_PIN);
				
				delayMicroseconds(1);
			#endif
	    	}
	    else
	   		{
			#ifdef AVR_MEGA_2650
			  // prim 1
				  PORTF &= B11111101;			//CLOCK LOW
				  PORTF |= B00000001;			//CLEAR HIGH
				  PORTB |= prim_Pins[prim];		//setzen der Primitiv Bits
				  PORTF |= B00000010;			//CLOCK HIGH
				  PORTF &= B11111101;			//CLOCK LOW
				  PORTB &= B00000000;	
			  	  delayMicroseconds(HEAT_TIME); 		//heat im Durchschnitt 2,5 �s bei 2
				  PORTF |= B00000001;			//CLEAR HIGH
				  PORTF &= B11111110;
			#endif
			
		    #ifdef SMT32_F103
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_LOWER_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<PRIMITIV_LOWER_CLR_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR |= prim_Pins[prim];
				delayMicroseconds(1);
				GPIOA->regs->ODR |= (1<<PRIMITIV_LOWER_CLK_PIN);
				delayMicroseconds(1);
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_LOWER_CLK_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR &= 0b1111110000000111; // set bit 3-9 Off
				delayMicroseconds(HEAT_TIME); 

				
				GPIOA->regs->ODR |= (1<<PRIMITIV_LOWER_CLR_PIN);
                delayMicroseconds(1);				
				GPIOA->regs->ODR &= ~(1<<PRIMITIV_LOWER_CLR_PIN);	
				delayMicroseconds(1);
			#endif
         	}	  
			
			#ifdef AVR_MEGA_2650
         		// Deaktivieren des Adresskontaktes
				  PORTF |= B00010000;
				  PORTF &= B11101111;
				  PORTB &= B10000000; 
			#endif
				
			#ifdef SMT32_F103
				GPIOA->regs->ODR |= (1<<ADRESS_CLR_PIN);
				delayMicroseconds(1);				
				GPIOA->regs->ODR &= ~(1<<ADRESS_CLR_PIN);
				delayMicroseconds(1);
				GPIOB->regs->ODR &= 0b1111110000000111;
			#endif
				
	  delayMicroseconds(wait_next);

}
//Allgemeine Funtionen



void HP_51645AE::test_print(uint32_t num)
{   uint32_t times;
	uint8_t adress, prim;
	delayMicroseconds(wait_same);
	for(times=0;times<num;times++)
		{
		for( prim=0;prim<PRIMTIVES;prim+=2)
			{	
			for(adress=0;adress<ADRESSES;adress++)
				{
						shot(prim,adress);
						delayMicroseconds(wait_next);
						//delay(2);
				}
			}
						//delayMicroseconds(400);
		for(prim=1;prim<PRIMTIVES;prim+=2)
			{
			for(adress=0;adress<ADRESSES;adress++)
				{
						shot(prim,adress);
						delayMicroseconds(wait_next);
					   //delay(2);
				}
			}
		}
}

uint8_t HP_51645AE::nozzle_test(uint8_t prim, uint8_t adr)
{
	if (prim>PRIMTIVES|| adr>ADRESSES)
	{
	return 1;			//Errorcode TODO
	}
	else 
	{
		shot(prim,adr);
		
	}
	
}

void HP_51645AE::test_fk(uint32_t num)
{   
uint32_t times;
uint8_t adress;
for(times=0;times<=num;times++)
		{
for(adress=0;adress<ADRESSES;adress++)
				{
						shot_all(B01111111,B01111111, adress);
						delayMicroseconds(wait_next);
				}
delayMicroseconds(wait_same);
		}
}

void HP_51645AE::char_shot(const byte data,int res) // Ausgabe eines Buchstaben
{		// Font bit und Aufl�sung (min 1 - max 18) werden �bergeben

	int run;
	byte tmp_b;		
	for(int l=0;l<2*res;l++)
				{//motor_y->step(false,1,sixteenthStep);// !!1INCREMENTGEBER LESEN!!!
	for(int i=0;i<8;i++)
		{


				tmp_b = data;
        		byte tmp_p = 1<<i;      //bit wird gesetzt
        		tmp_b = tmp_b & tmp_p;
          	    if(tmp_b == 1<<i)			
					{ 	 	
						for(int j=(i*res)/adr;j<=((i+1)*res)/adr;j++)
							{

							  run = 0;
								if(j!=((i+1)*res)/adr)
										{
											for(int k=(i*res)%adr;k<22;k++)
											{run++;

												 shot(j*2,k);
											}
											for(int k=0;k<res-run;k++)
											{

												 shot((j+1)*2,k);   										
											}
										} 
								else{		
										for(int k=(i*res)%adr;k<((i+1)*res)%adr;k++)
										{	

											 shot(j*2,k);
										}
								}			
		            		}
		           	}
		       else
			  		{	
					    delayMicroseconds(10);  	
					}   
			delayMicroseconds(wait_next);		  		               
			}
  delayMicroseconds(wait_same);

		}
}
void HP_51645AE::text(String sen, int res, char direction)          // bekommt eine Zeichenkette �bergeben. Mittels font Array werden die richtigen Bytes an die shot() Funktion �bergeben
{
	if(direction=='h')
		{	
		    for(unsigned int i=0;i<sen.length();i++)
		     {		
		         for(int j=0;j<8;j++){
		              
		                char_shot(static_cast<byte>pgm_read_byte(&(font_horizontal[sen[i]][j])),res);
						}
		         //motor_y->step(false,50,sixteenthStep); // Abstand zwischen Buchstaben // !!1INCREMENTGEBER LESEN!!!
		         }
		}
		else
		{
				{
		    for(unsigned int i=0;i<sen.length();i++)
		     {
		
		         for(int j=0;j<8;j++){
		                char_shot(pgm_read_byte(&(font_vertical[sen[i]][j])),res);	
									
						}
		       //  motor_y->step(false,50,sixteenthStep); // Abstand zwischen Buchstaben // !!1INCREMENTGEBER LESEN!!!
		         }
		}	
		}

     }


 //
 /////////////////////////////////////////////Funktionen zur Bebilderung der Druckwalze und Steuerung der Motoren////////////////////////////////////////////////////////////////////////////
 //

 void HP_51645AE::printBitmap()		//Vorbereiten des Drucksystems f�r den Druck
  {
  	int trs = Serial.read();
  	delay(25);
  	String tmp;
  	long width=0, height=0;
  	char dim[5];
  	for(int i=0;i<5;i++)
 		{
  		dim[i]=(char)Serial.read();
 		}
 	width = atof(dim);

 	for(int i=0;i<5;i++)
 		{
 		dim[i]=(char)Serial.read();
 		}
	height = atof(dim);
 	if(trs=='S')
 		{
 //		Serial.println("Schnelle Druckgeschwindigkeit ausgewaelt. Du traust dich aber was!	");
 		//Serial.println("S");
 		//store_bitmap_fast(width,height);
 		}
 	else
 		{
// 		Serial.println("Normale Druckgeschwindigkeit ausgewaelt. Gut gemacht!");

 		store_bitmap(width,height);
 		}

  }
 void HP_51645AE::shotBitmap()		// Drucken des Druckbildes
{
for(int i = 0; i<2;i++)
{
	 for(int k=0;k<19;k++)
	 {
		for(bit=0;bit<8;bit++)
		{
			{

		        		tmp_p = 1<<bit;      	//bit wird gesetzt
		        		tmp_p = pr_Array[k+19*i] & tmp_p;	//
		       	    if(tmp_p == 1<<bit)			//Bildpunkt: true | false
							{
							shot(((k*8+bit)/22)*2+i,(k*8+bit)%22);
							}
					
			}
		}
	 }
}
							}
void HP_51645AE::heatdelay(uint16_t times)
{
	uint8_t i;
	for( i=0;i<times;i++)
	{
		//asm("NOP");
		 __asm__ volatile ("nop"); 
		
	}
}

 uint8_t HP_51645AE::processControlData(uint8_t data)
 {
	 //
	 //...
	 switch(data)
	 {
		case 1: printResolutionFlag = data;  break;
	 }
	 
	 return SUCCESS;	
 }
void HP_51645AE::printCol(uint8_t* pData)
{
	uint8_t bit;
	uint8_t data;
	for (data = 0; data < 38; data++)
	{	
		for(bit = 0; bit < 8; bit++)
		{
			if (1 & pData[data] >> bit)
			{
				printNozzle((data*8)+bit);
			}
		}

	}


	// uint8_t adress, prim;	
	// for( prim=0;prim<PRIMTIVES;prim++)
	// 		{	
	// 		for(adress=0;adress<ADRESSES;adress++)
	// 			{
	// 				if(1 && pData[prim*adress/8]<<(1+prim)*adress%8 )
	// 					{Serial.print("test: ");
	// 					Serial.print(prim);
	// 					Serial.print(" : ");
	// 					Serial.print(adress);
	// 					Serial.print(" : ");

	// 					Serial.println(pData[prim*adress/8]);
	// 					// shot(prim,adress);
	// 					delayMicroseconds(wait_next);}
						
	// 			}

	// }
		
	
}

void HP_51645AE::printNozzle(uint16_t nozzle)


{
	// NOZZLE_T noz;
	PROGMEM_readAnything (&nozzle_array [nozzle], noz);
	// Serial.print (nozzle);
	Serial.print ("prim: ");
	Serial.print (noz.prim);
	Serial.print ("adr: ");
	Serial.println (noz.adr);
	shot(noz.prim, noz.adr);
	delayMicroseconds(wait_next);
	// // Serial.print (nozzle);
	// if (nozzle%2 == 0)
	// {
	
	

	// Serial.print("{");
	// Serial.print (nozzle/22);
	// 	Serial.print(",");	
	// Serial.print(nozzle/2%22);
	// 	Serial.println("},");

	// }
	// else
	// {
	// // shot((nozzle+1)/22, (nozzle+3)%22);
	// delayMicroseconds(wait_next);
	// 	Serial.print("{");
	// Serial.print ((nozzle/22)+1);
	// 	Serial.print(",");	
	// Serial.print((nozzle/2+4)%22);
	// 	Serial.println("},");
	// }
	
}
void HP_51645AE::printColFast(uint8_t* pData)
{uint8_t adress;

	for(adress=0;adress<ADRESSES;adress++)
				{
					
					
						shot_all(pData[adress<<1],pData[(adress+1)<<1], adress);
						delayMicroseconds(wait_next);
				}
			if(lastPrintedCol<=149){					
				lastPrintedCol++;
			}
			else 
			{
				lastPrintedCol=0;
			}
				
}

//
// Stellt 1 Byte in den Ringbuffer
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist voll. Es kann kein weiteres Byte gespeichert werden
//     BUFFER_SUCCESS    das Byte wurde gespeichert
uint8_t HP_51645AE::addPrintDataToBuffer(uint8_t* pData){

  if ( ( buffer.write + 1 == buffer.read ) ||
       ( buffer.read == 0 && buffer.write + 1 == BUFFER_SIZE ) )
    return BUFFER_FAIL; // voll

	for(uint8_t i =0;i<44;i++){
  buffer.data[i][buffer.write] = pData[i];
	}
  buffer.write++;
  if (buffer.write >= BUFFER_SIZE)
    buffer.write = 0;

  return BUFFER_SUCCESS;

}
//
// Holt 1 Byte aus dem Ringbuffer, sofern mindestens eines abholbereit ist
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist leer. Es kann kein Byte geliefert werden.
//     BUFFER_SUCCESS    1 Byte wurde geliefert
//    

uint8_t	HP_51645AE::getPrintDataFromBuffer(uint8_t *pByte)
{
  if (buffer.read == buffer.write)
  { return BUFFER_FAIL;}
Serial.print("HIER1");
for(uint8_t i =0;i<44;i++){
  pByte[i] = buffer.data[i][buffer.read];
	}
  Serial.print("HIER2");

  buffer.read++;
  if (buffer.read >= BUFFER_SIZE)
  { buffer.read = 0;}

  return BUFFER_SUCCESS;
}