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

void SprinklerCntrl( char section, char isOn);
void InitializeSprinkler( void);

//need to add the pins for th spare

// Sprinkler enable pin is changing to A,b,or C
//
//Pin 15(PC2)
#define SPRINKLERDEMUX_C_CONFIG		(DDRC |= (1<<2))
#define SPRINKLERDEMUX_C_ON			(PORTC |= (1<<2))
#define SPRINKLERDEMUX_C_OFF		(PORTC &= ~(1<<2))

//Pin 16(PC3)
#define SPRINKLERDEMUX_B_CONFIG		(DDRC |= (1<<3))
#define SPRINKLERDEMUX_B_ON			(PORTC |= (1<<3))
#define SPRINKLERDEMUX_B_OFF		(PORTC &= ~(1<<3))

//Pin 25(PB3)
#define SPRINKLERDEMUX_A_CONFIG		(DDRB |= (1<<3))
#define SPRINKLERDEMUX_A_ON			(PORTB |= (1<<3))
#define SPRINKLERDEMUX_A_OFF		(PORTB &= ~(1<<3))

#endif