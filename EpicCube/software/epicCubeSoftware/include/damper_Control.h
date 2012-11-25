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

//Pin 5(PD2) possible to use interrupts pd0 TO DO make interrupt
#define ISBUTTONOPEN_CONFIG		(DDRD &= ~(1<<2))
#define ISBUTTONOPENBUSHIGH		(PIND & (1<<2))
//Pin 4(PD1) set to input TODO make interrupt enabled.
#define ISBUTTONCLOSE_CONFIG	(DDRD &= ~(1<<1))
#define ISBUTTONCLOSEBUSHIGH	(PIND & (1<<1))
// to configure input you calculate
// (0x21 +3 *3) &= ~(1 << 1or2);


//Pin 22(PB6)
#define MOTORENABLEHIGH	(PORTB |= (1<<6))
#define MOTORENABLELOW	(PORTB &= ~(1<<6))

//Pin 23(PB5) Keep for a spare for ideal setup
#define MOTORDIRHIGH	(PORTB |= (1<<5))
#define MOTORDIRLOW	(PORTB &= ~(1<<5))
//Pin 24(PB4)
#define MOTORSTEPHIGH	(PORTB |= (1<<4))
#define MOTORSTEPLOW	(PORTB &= ~(1<<4))

#define MOTORENABLECONFIG	(DDRB |= (1<<6))
#define MOTORDIRCONFIG	(DDRB |= (1<<5))
#define MOTORSTEPCONFIG	(DDRB |= (1<<4))


void InitializeDamper(void);
void OpenDamper(unsigned char damper);
void CloseDamper(unsigned char damper);
unsigned char CheckDamper(unsigned char damper);
void ActivateDamper(unsigned char Damper);
void DampVerbose(uint8_t verbose);
#endif