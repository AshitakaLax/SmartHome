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

#define MotorDirection1_CONFIG	(DDRB |= (1<<6))
#define MotorDirection2_CONFIG	(DDRB |= (1<<5))
#define MotorStep_CONFIG	(DDRB |= (1<<4))


void InitializeDamper(void);
void OpenDamper(unsigned char damper);
void CloseDamper(unsigned char damper);
unsigned char CheckDamper(unsigned char damper);
void ActivateDamper(unsigned char Damper);
#endif