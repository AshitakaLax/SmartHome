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
//Pin 14(PC1)
#define CHANNEL_DEMUX_A_ON		(PORTC |= (1<<1))
#define CHANNEL_DEMUX_A_OFF		(PORTC &= ~(1<<1))
//Pin 15(PC2)
#define CHANNEL_DEMUX_B_ON		(PORTC |= (1<<2))
#define CHANNEL_DEMUX_B_OFF		(PORTC &= ~(1<<2))
//Pin 16(PC3)
#define CHANNEL_DEMUX_C_ON		(PORTC |= (1<<3))
#define CHANNEL_DEMUX_C_OFF		(PORTC &= ~(1<<3))
//Pin 20(PC7)
#define CHANNEL_DEMUX_EN_ON		(PORTC |= (1<<7))// when high it disables everything else
#define CHANNEL_DEMUX_EN_OFF	(PORTC &= ~(1<<7))

//configuration of pins
#define CHANNEL_DEMUX_A_CONFIG	(DDRC |= (1<<1))
#define CHANNEL_DEMUX_B_CONFIG	(DDRC |= (1<<2))
#define CHANNEL_DEMUX_C_CONFIG	(DDRC |= (1<<3))
#define CHANNEL_DEMUX_EN_CONFIG	(DDRC |= (1<<7))

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

void initializeTempSense(void);
uint8_t ReadTempSensor(uint8_t sensor);
uint8_t ReadADC(uint8_t pin);


#endif