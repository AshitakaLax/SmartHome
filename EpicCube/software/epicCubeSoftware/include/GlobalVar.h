#ifndef globalVar_h__
#define globalVar_h__

#include <avr/pgmspace.h>

/**
* this file needs to be able to complete the following tasks.  
*
* 1. Open damper (specified by number 0 - 15 ( byte)
* 2. Close damper (specified by number 0 - 15 (byte)
* 3. Check damper status (false for open, and true for closed) (boolean)
*/
//define which pins are what.
//Following pins are used for Sprinkler, Damper, and Temperature.

//Pin 26(PB2)
#define DEMUX_D_CONFIG		(DDRB |= (1<<2))
#define DEMUX_D_ON			(PORTB |= (1<<2))
#define DEMUX_D_OFF			(PORTB &= ~(1<<2))
//Pin 27(PB1)
#define DEMUX_C_CONFIG		(DDRB |= (1<<1))
#define DEMUX_C_ON			(PORTB |= (1<<1))
#define DEMUX_C_OFF			(PORTB &= ~(1<<1))
//Pin 28(PB0)
#define DEMUX_B_CONFIG		(DDRB |= (1<<0))
#define DEMUX_B_ON			(PORTB |= (1<<0))
#define DEMUX_B_OFF			(PORTB &= ~(1<<0))
//Pin 29(PE7)
#define DEMUX_A_CONFIG		(DDRE |= (1<<7))
#define DEMUX_A_ON			(PORTE |= (1<<7))
#define DEMUX_A_OFF			(PORTE &= ~(1<<7))

//Damper
//Pin 30(PE6)
#define DAMPER_ENABLE_CONFIG			(DDRE |= (1<<6))
#define DAMPER_ENABLE_OFF				(PORTE |= (1<<6))
#define DAMPER_ENABLE_ON				(PORTE &= ~(1<<6))
//Sprinkler this is a little different.  it is enabled high.
//Pin 25 (PB3)
#define SPRINKLER_DEMUX_EN_CONFIG		(DDRB |= (1<<3))
#define SPRINKLER_DEMUX_EN_ON			(PORTB |= (1<<3))//SPRINKLER_DEMUX_EN_ON
#define SPRINKLER_DEMUX_EN_OFF			(PORTB &= ~(1<<3))
// Temperature
//Pin 20(PC7)
#define TEMPERATURE_DEMUX_EN_CONFIG		(DDRC |= (1<<7))
#define TEMPERATURE_DEMUX_EN_OFF		(PORTC |= (1<<7))// when high it disables everything else
#define TEMPERATURE_DEMUX_EN_ON			(PORTC &= ~(1<<7))

//Configure Fan Pins PC6, PC5, PC4, PD0, PD3
//#define XTAL 160000000L // 16Mhz

#endif