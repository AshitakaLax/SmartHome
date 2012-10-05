/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the Fans with a PWM signal
*/
#ifndef fans_h__
#define fans_h__

#include <avr/pgmspace.h>
#include <util/delay.h>

//Pin 25(PB3)
//#define DEMUX_A_ON		(PORTB |= (1<<3))
//#define DEMUX_A_OFF		(PORTB &= ~(1<<3))



//Pin 25(PB3)
// #define FAN_ONE_ON		(PORTD |= (1<<3))
// #define FAN_ONE_OFF		(PORTD &= ~(1<<3))


// #define DEMUX_A_CONFIG	(DDRB |= (1<<3))

#define FAN_ZERO_CONFIG	(DDRD |= (1<<0))
#define FAN_ONE_CONFIG	(DDRD |= (1<<1))
#define FAN_TWO_CONFIG	(DDRC |= (1<<4))
#define FAN_THREE_CONFIG (DDRC |= (1<<5))
#define FAN_FOUR_CONFIG	(DDRC |= (1<<6))



//their are 5 fans with a 
void InitializeFans(void);
void SetFan(unsigned char FanSelect, unsigned short FanSpeed);
unsigned short FanStatus(unsigned char FanSelect);

// Pin 3 (PD0)
extern unsigned short FanZeroStatus;
// Pin 4 (PD1)
extern unsigned short FanOneStatus;
// Pin 17 (PC4)
extern unsigned short FanTwoStatus;
// Pin 18 (PC5)
extern unsigned short FanThreeStatus;
// Pin 19 (PC6)
extern unsigned short FanFourStatus;

#endif
