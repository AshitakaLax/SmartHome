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

// #define DEMUX_A_CONFIG	(DDRB |= (1<<3))
//Configure Fan Pins PC6, PC5, PC4, PD0, PD3
//#define XTAL 160000000L // 16Mhz
#define FAN1_CONFIG		(DDRD |= (1<<0))
#define FAN1_OFF		(PORTD &= ~(1<<0))
#define FAN1_ON			(PORTD |= (1<<0))

#define FAN2_CONFIG		(DDRD |= (1<<3))
#define FAN2_OFF		(PORTD &= ~(1<<3))
#define FAN2_ON			(PORTD |= (1<<3))

#define FAN3_CONFIG		(DDRC |= (1<<4))
#define FAN3_OFF		(PORTC &= ~(1<<4))
#define FAN3_ON			(PORTC |= (1<<4))

#define FAN4_CONFIG		(DDRC |= (1<<5))
#define FAN4_OFF		(PORTC &= ~(1<<5))
#define FAN4_ON			(PORTC |= (1<<5))

#define FAN5_CONFIG		(DDRC |= (1<<6))
#define FAN5_OFF		(PORTC &= ~(1<<6))
#define FAN5_ON			(PORTC |= (1<<6))



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
