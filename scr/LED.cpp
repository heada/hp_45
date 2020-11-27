/*
 * LED.cpp
 *
 *  Created on: 04.12.2016
 *      Author: C3PO
 */

#include <LED.h>
#include <constants.h>

LED::LED() {
	pinMode(GREEN_LED,OUTPUT);
	pinMode(RED_LED,OUTPUT);
}
void LED::blink(int times, int speed, int color)
{
	for(int i=0; i<times;i++)
		{
		digitalWrite(color,HIGH);
		delay(speed);
		digitalWrite(color,LOW);
		delay(speed);
		}
}
void LED::toggle(bool state,int color)
{
	digitalWrite(color,state);
}
LED::~LED() {

}

