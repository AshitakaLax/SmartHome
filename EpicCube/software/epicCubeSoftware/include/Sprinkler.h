/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the Fans with a PWM signal
*/
#ifndef sprinkler_h__
#define sprinkler_h__

#include <avr/pgmspace.h>
#include <util/delay.h>

uint8_t ReadTempSensor(uint8_t sensor);
uint8_t ReadADC(uint8_t pin);


#endif