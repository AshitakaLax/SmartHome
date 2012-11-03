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

uint8_t SprinklerCntrl( uint8_t section);

//need to add the pins for th spare

// Sprinkler enable pin is changing to A,b,or C
//
//Pin 26(PB2)
#define DEMUX_D_CONFIG		(DDRB |= (1<<2))
#define DEMUX_D_ON			(PORTB |= (1<<2))
#define DEMUX_D_OFF			(PORTB &= ~(1<<2))

//Pin 26(PB2)
#define DEMUX_D_CONFIG		(DDRB |= (1<<2))
#define DEMUX_D_ON			(PORTB |= (1<<2))
#define DEMUX_D_OFF			(PORTB &= ~(1<<2))

//Pin 26(PB2)
#define DEMUX_D_CONFIG		(DDRB |= (1<<2))
#define DEMUX_D_ON			(PORTB |= (1<<2))
#define DEMUX_D_OFF			(PORTB &= ~(1<<2))

#endif