/*
 * measure.h
 *
 *  Created on: 08.12.2016
 *      Author: C3PO
 */

#ifndef HP45LIB_MEASURE_H_
#define HP45LIB_MEASURE_H_

class Measure {
public:
	Measure();
	float measure_temp(bool);
	virtual ~Measure();
};

#endif /* HP45LIB_MEASURE_H_ */
