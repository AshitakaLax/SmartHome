/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the Fans with a PWM signal
*/
#ifndef tempSense_h__
#define tempSense_h__

#include <avr/pgmspace.h>
#include <util/delay.h>

// Pins on and off


//Setup Macros for ADC's
//Default us VCC as ADC reference point.
#define ADC_REF_POWER     (1<<REFS0)
#define ADC_REF_INTERNAL  ((1<<REFS1) | (1<<REFS0))
#define ADC_REF_EXTERNAL  (0)

// this is for a 16Mhz clock
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS1))
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#if !defined(ADHSM)
#define ADHSM (7)
#endif
#endif

uint16_t ReadTempSensor(char *SensorNumber);
uint16_t ReadADC(uint8_t pin);


#endif