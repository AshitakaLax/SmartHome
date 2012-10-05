/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the Fans with a PWM signal
*/
#ifndef HVACGarage_h__
#define HVACGarage_h__

#include <avr/pgmspace.h>
#include <util/delay.h>


//Pin 7(PD4)
#define GARAGE_SWITCH_ON		(PORTD |= (1<<4))
#define GARAGE_SWITCH_OFF		(PORTD &= ~(1<<4))

//Pin 8(PD5)
#define G_FAN_SWITCH_ON		(PORTD |= (1<<5))
#define G_FAN_SWITCH_OFF		(PORTD &= ~(1<<5))

//Pin 9(PD6)
#define W_HEAT_SWITCH_ON		(PORTD |= (1<<6))
#define W_HEAT_SWITCH_OFF		(PORTD &= ~(1<<6))

//Pin 10(PD7)
#define Y_COOL_SWITCH_ON		(PORTD |= (1<<7))
#define Y_COOL_SWITCH_OFF		(PORTD &= ~(1<<7))

//Pin 11 (PE0)
#define HVAC_SWITCH_ON			(PORTE |= (1<<0))
#define HVAC_SWITCH_OFF			(PORTE &= ~(1<<0))

// pin 14 (PC1)
#define G_FAN_TRIGGER_READ			(PINC & (1<<1))// 1 is high 0 low
// pin 12 (PE1)
#define W_HEAT_TRIGGER_READ			(PINE & (1<<1))// 1 is high
// pin 13 (PC0)
#define Y_COOL_TRIGGER_READ			(PINC & (1<<0))// 1 is high;


//configuration of pins
// GARAGE_SWITCH_CONFIG  == PD4 (output)
#define GARAGE_SWITCH_CONFIG	(DDRD |= (1<<4))
// G_FAN_SWITCH_CONFIG == PD5 (output)
#define G_FAN_SWITCH_CONFIG		(DDRD |= (1<<5))
// W_HEAT_SWITCH_CONFIG == PD6 (output)
#define W_HEAT_SWITCH_CONFIG	(DDRD |= (1<<6))
// Y_COOL_SWITCH_CONFIG == PD7 (output)
#define Y_COOL_SWITCH_CONFIG	(DDRD |= (1<<7))
// HVAC_OFF_SWITCH_CONFIG == PE0 (output) 
#define HVAC_OFF_SWITCH_CONFIG	(DDRE &= ~(1<<0))

// G_FAN_TRIGGER_CONFIG == PC1 (input)
#define G_FAN_TRIGGER_CONFIG	(DDRC &= ~(1<<1))
// W_HEAT_TRIGGER_CONFIG == PE1 (input)
#define W_HEAT_TRIGGER_CONFIG	(DDRE &= ~(1<<1))
// Y_COOL_TRIGGER_CONFIG == PC0 (input)
#define Y_COOL_TRIGGER_CONFIG	(DDRC &= ~(1<<0))

// Pin 3 (PC1)
unsigned short GFanStatus;
// Pin 4 (PE1)
unsigned short WHeatStatus;
// Pin 17 (PC0)
unsigned short YCoolStatus;
unsigned short HVACStatus;

void initializeHVACGarage(void);
void PulseGarage(void);
uint8_t ReadGFan(void);
uint8_t ReadWHeat(void);
uint8_t ReadYCool(void);
void SetGFan(uint8_t state);
void SetWHeat(uint8_t state);
void SetYCool(uint8_t state);
void SetHVAC(uint8_t state);




#endif
