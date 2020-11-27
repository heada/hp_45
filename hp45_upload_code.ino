#include "HP_51645AE.h"
#include <Wire.h>


#define TWI_ADRESS 8
#define BAUDRATE 9600
#define COLSIZE 44

/* timing settings
 all values are in microseconds
 HEAT_TIME:       time to heat up one nozzle
 DELAY_SAME_TIME: time between activate the same nozzle
 DELAY_NEXT_TIME: time between activation of tow different nozzles
*/
#define HEAT_TIME 5 
#define DELAY_SAME_TIME 400
#define DELAY_NEXT_TIME 10

// Encoder input pins 
#define ENC_PIN_A 18
#define ENC_PIN_B 19
HP_51645AE hp45(HEAT_TIME,DELAY_SAME_TIME,DELAY_NEXT_TIME); 
int val;
uint8_t encoder0PinA = ENC_PIN_A;
uint8_t encoder0PinB = ENC_PIN_B;
volatile int16_t encoder0Pos = 0;
volatile int16_t encoder0PinALast = LOW;
int n = LOW;
int m = LOW;
TwoWire twiData;
uint8_t pData[COLSIZE];
uint8_t pDataToPrint[COLSIZE]; 
uint8_t num = 0;

void setup() {
  // put your setup code here, to run once:
  DDRE |= B00001000;
  DDRG |= B00100000;
  Serial.begin(BAUDRATE);
  twiData.begin(TWI_ADRESS);
  twiData.onReceive(receiveEventData);
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder0PinA), shot_line, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), shot_line, CHANGE);

}




void receiveEventData(int howMany) {
  
  while ( twiData.available()) { // loop through all but the last
   if(num<COLSIZE){
   pData[num++] = twiData.read();
   Serial.print("DATA: ");
   Serial.println(num);
   
   }
   else
   {
    hp45.addPrintDataToBuffer(pData);
    num=0;
    Serial.println("Line R");
    
    }
   
  }
  //Serial.println("DATA R");
 PORTE |= (1<<PE3);
}
void shot_line()
{
  PORTE &= ~(1<<PE3);
    n = digitalRead(encoder0PinA);
    m = digitalRead(encoder0PinB);
    Serial.print (n);
    Serial.print ("_");
    Serial.println (m);
    if ((encoder0PinALast == LOW) && (n == HIGH)) {
      if (m == LOW) {
        encoder0Pos--;
      } else {
        encoder0Pos++;
      }
    }
    encoder0PinALast = n;
  if(hp45.getPrintDataFromBuffer(pDataToPrint)){
    Serial.print("Begin der Daten");
  for(uint16_t i = 0 ;i<COLSIZE;i++)
    {
    Serial.print(pDataToPrint[i]);
    }
    Serial.println("Ende der Daten.");
    }
    else{Serial.println("Buffer Leer");}
  }



void loop() {
  delay(50);
}
