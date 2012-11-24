/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the different dampers it will need to store in memory, 
* all the correct values.
* 
* Steps to setup of a fan PWM signal
* 1. set pin as output(OCx)
* 2. Select PWM mode of timer
* 3. Set appropriate prescaler divider
* 4. Set Compare Output Mode to Clear or Set on compare match
* 5. Write Duty cycle value to PWM(OCRx)
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../include/HVACGarage.h"


void initializeHVACGarage(void)
{
	// configures all of the pins to be outputs or inputs
	GARAGE_SWITCH_CONFIG;//out
	G_FAN_SWITCH_CONFIG;// out
	W_HEAT_SWITCH_CONFIG;// out
	Y_COOL_SWITCH_CONFIG;// out
	G_FAN_TRIGGER_CONFIG;// in
	W_HEAT_TRIGGER_CONFIG;// in
	Y_COOL_TRIGGER_CONFIG;// in
	
}
/**
*	makes the Garage door relay turn on for 0.5 seconds, then back off.
*	this will either open, close or stop the garage door.
*/
void PulseGarage(void)
{
	GARAGE_SWITCH_ON;
	for(int i = 0; i < 1000; i++);//wait
	GARAGE_SWITCH_OFF;
}
/**
*	reads the trigger whether the GFan is on or off
*	We need to oversample for 100 ms made up. we really need it for 0.0166666
*	so for good measure we can try 32ms
*	16Mhz clock 0.0000000625 seconds
*	0.032/0.0000000625 seconds.
*	so we need at least 512000 cycles need uint32_t
*	really it is on.
*	returns 1 for on and 0 for off.
*/
uint8_t ReadGFan(void)
{
	uint32_t counter = 0;
	uint8_t result = 0;
	while(result == 0 && counter < 600000)
	{
		result = G_FAN_TRIGGER_READ;
		counter++;
	}
	return result;
}

/**
*	reads the trigger whether the GFan is on or off
* 	NOTE: possible issue with optocouplers sensing it is off when
*	really it is on.
*	returns 1 for on and 0 for off.
*/
uint8_t ReadWHeat(void)
{	
uint32_t counter = 0;
	uint8_t result = 0;
	while(result == 0 && counter < 600000)
	{
		result = W_HEAT_TRIGGER_READ;
		counter++;
	}
	return result;
}
/**
*	reads the trigger whether the GFan is on or off
* 	NOTE: possible issue with optocouplers sensing it is off when
*	really it is on.
*	returns 1 for on and 0 for off.
*/
uint8_t ReadYCool(void)
{
	uint32_t counter = 0;
	uint8_t result = 0;
	while(result == 0 && counter < 600000)
	{
		result = Y_COOL_TRIGGER_READ;
		counter++;
	}
	return result;
}
/**
*	Sets the state of the relay
*	Probably should have a failsafe on this.
*	if state equals 1 the relay is on, and if 0 for off.
*/
void SetGFan(uint8_t state)
{
	if(state == 1)
	{
		G_FAN_SWITCH_ON;
		GFanStatus = 1;
	}
	else
	{
		G_FAN_SWITCH_OFF;
		GFanStatus = 0;
	}
}
/**
*	Sets the state of the relay
*	Probably should have a failsafe on this.
*	if state equals 1 the relay is on, and if 0 for off.
*/
void SetWHeat(uint8_t state)
{
	if(state == 1)
	{
		W_HEAT_SWITCH_ON;
		WHeatStatus = 1;
	}
	else
	{
		W_HEAT_SWITCH_OFF;
		WHeatStatus = 0;
	}
}
/**
*	Sets the state of the relay
*	Probably should have a failsafe on this.
*	if state equals 1 the relay is on, and if 0 for off.
*/
void SetYCool(uint8_t state)
{
	if(state == 1)
	{
		Y_COOL_SWITCH_ON;
		YCoolStatus = 1;
	}
	else
	{
		Y_COOL_SWITCH_OFF;
		YCoolStatus = 0;
	}
}
/**
*	This can interrupt the current state of the furnance
*	to turn it on or to turn it off. We will almost always 
*	want to keep this off unless we want it on.
*
**/
void SetHVAC(uint8_t state)
{
	if(state == 1)
	{
		HVAC_SWITCH_ON;
		HVACStatus = 1;
	}
	else
	{
		HVAC_SWITCH_OFF;
		HVACStatus = 0;
	}
}