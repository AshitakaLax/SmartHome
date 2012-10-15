#ifndef damper_control_h__
#define damper_control_h__

#include <avr/pgmspace.h>

/**
* this file needs to be able to complete the following tasks.  
*
* 1. Open damper (specified by number 0 - 15 ( byte)
* 2. Close damper (specified by number 0 - 15 (byte)
* 3. Check damper status (false for open, and true for closed) (boolean)
*/
//define which pins are what.
//Pin 25(PB3)
//#define DEMUX_A_ON		(PORTB |= (1<<3))
//#define DEMUX_A_OFF		(PORTB &= ~(1<<3))
//Pin 26(PB2)
#define DEMUX_A_ON			(PORTB |= (1<<2))
#define DEMUX_A_OFF			(PORTB &= ~(1<<2))
//Pin 27(PB1)
#define DEMUX_B_ON			(PORTB |= (1<<1))
#define DEMUX_B_OFF			(PORTB &= ~(1<<1))
//Pin 28(PB0)
#define DEMUX_C_ON			(PORTB |= (1<<0))
#define DEMUX_C_OFF			(PORTB &= ~(1<<0))
//Pin 29(PE7)
#define DEMUX_D_ON			(PORTE |= (1<<7))
#define DEMUX_D_OFF			(PORTE &= ~(1<<7))
//Pin 30(PE6)
#define DEMUX_ENABLE_ON		(PORTE |= (1<<6))
#define DEMUX_ENABLE_OFF	(PORTE &= ~(1<<6))

//Pin 4(PD1) possible to use interrupts pd0
#define ISBUTTONOPENBUSHIGH		(PIND & (1<<1))
//Pin 3(PD0)
#define ISBUTTONCLOSEBUSHIGH	(PIND & (1<<0))
//Pin 22(PB6)
#define MotorDirection1HIGH	(PORTB |= (1<<6))
#define MotorDirection1LOW	(PORTB &= ~(1<<6))

//Pin 23(PB5)
#define MotorDirection2HIGH	(PORTB |= (1<<5))
#define MotorDirection2LOW	(PORTB &= ~(1<<5))
//Pin 24(PB4)
#define MotorStepHIGH	(PORTB |= (1<<4))
#define MotorStepLOW	(PORTB &= ~(1<<4))

#define DEMUX_A_CONFIG	(DDRB |= (1<<2))
#define DEMUX_B_CONFIG	(DDRB |= (1<<1))
#define DEMUX_C_CONFIG	(DDRB |= (1<<0))
#define DEMUX_D_CONFIG	(DDRE |= (1<<7))
#define DEMUX_ENABLE_CONFIG	(DDRE |= (1<<6))

#define MotorDirection1_CONFIG	(DDRB |= (1<<6))
#define MotorDirection2_CONFIG	(DDRB |= (1<<5))
#define MotorStep_CONFIG	(DDRB |= (1<<4))


void InitializeDamper(void);
void OpenDamper(unsigned char damper);
void CloseDamper(unsigned char damper);
unsigned char CheckDamper(unsigned char damper);
void ActivateDamper(unsigned char Damper);
#endif