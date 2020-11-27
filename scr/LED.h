/*
 * LED.h
 *
 *  Created on: 04.12.2016
 *      Author: C3PO
 */
#include <Arduino.h>
#ifndef HP45LIB_LED_H_
#define HP45LIB_LED_H_

class LED {
public:
	LED();
	void blink(int, int, int);
	void toggle(bool, int);
	virtual ~LED();
};

#endif /* HP45LIB_LED_H_ */
