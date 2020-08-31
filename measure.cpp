/*
 * measure.cpp
 *
 *  Created on: 08.12.2016
 *      Author: C3PO
 */
#include <measure.h>
#include <constants.h>
#include <Arduino.h>
Measure::Measure()
	{
	pinMode(temp_Pin,INPUT);
	pinMode(r10x_Pin,INPUT);

	}
float Measure::measure_temp(bool a)
	{
	int temperature;
	if (a)
	{
		temperature = analogRead(temp_Pin);

		return temperature;}
	else
	{
		temperature = analogRead(r10x_Pin);

		return temperature;
	}
}

Measure::~Measure() {
	// TODO Auto-generated destructor stub
}

