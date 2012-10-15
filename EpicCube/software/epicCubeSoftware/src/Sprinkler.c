/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the different dampers it will need to store in memory, 
* all the correct values.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../include/GlobalVar.h"
#include "../include/sprinkler.h"

/**
*	this activates the correct damper
* 	Then while checking the Damper's status
*	runs the stepper motor.
*/
void OpenDamper( uint8_t damper)
{
	ActivateDamper(damper);
	for(int i; i < 10000; i++);// wait for signal propagation 0.5 seconds is
	
	//step the device open
	uint8_t DamperStatus = CheckDamper(damper);
	// if already open, return
	if(DamperStatus == 0)
	{
		return;
	}
	MotorDirection1HIGH;
	MotorDirection2LOW;
	// set a timeout
	unsigned int timeout = 0;
	while(DamperStatus != 0 || timeout == 10000)
	{
		// step a lot
		MotorStepHIGH;
		DamperStatus = CheckDamper(damper);
		MotorStepLOW;
		timeout++;
	}
	if(timeout == 10000)
	{
		// report error to main Could be the wrong board that
		// we are trying to access.
	}
	
	
}
/**
*	Closes the damper shut.
*/
void CloseDamper( uint8_t damper)
{

	ActivateDamper(damper);
	for(int i; i < 10000; i++);// wait for signal propagation 0.5 seconds is
	
	//step the device close
	uint8_t DamperStatus = CheckDamper(damper);
	// if already close, return
	if(DamperStatus == 1)
	{
		return;
	}
	MotorDirection1LOW;
	MotorDirection2HIGH;
	// set a timeout
	unsigned int timeout = 0;
	while(DamperStatus != 1 || timeout == 10000)
	{
		// step a lot
		MotorStepHIGH;
		DamperStatus = CheckDamper(damper);
		MotorStepLOW;
		timeout++;
	}
	if(timeout == 10000)
	{
		// report error to main Could be the wrong board that
		// we are trying to access.
	}
	
}
/**
* So the Damper has a procedure to access the status
* first to have to activate the section
* second you have to check if it is open = 0
* third you have to check if it is close = 1
* fourth you have to check if it is in the middle = 2
* damper is a number from 0 to 15
*/
uint8_t CheckDamper(uint8_t damper)
{
	//Go through each damper and check if the input on the Bus is high or low.
	ActivateDamper(damper);
	
	// check if it is in the middle
	for(int i; i < 10000; i++);// wait for signal propagation 0.5 seconds is
	// enough
	if(ISBUTTONOPENBUSHIGH & ISBUTTONCLOSEBUSHIGH)
	{
		
		return 2;
	}
	else if(!ISBUTTONOPENBUSHIGH & ISBUTTONCLOSEBUSHIGH)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
/**
*	Sets the correct damper to be active.
* Damper is high when button isn't pressed
*/
void ActivateDamper(uint8_t damper)
{
// converts decimal to binary digits for ABCD
// could be backwards.
	uint8_t D = damper / 8;
	uint8_t temp = damper % 8;
	
	uint8_t C = temp / 4;
	temp = temp % 4;
	
	uint8_t B = temp / 2;
	temp = temp % 4;
	
	uint8_t A = temp / 1;
	//set the output pins as such.
	//d to A
	if(D==1)
	{
		DEMUX_D_ON;
	}
	else
	{
		DEMUX_D_OFF;
	}
	if(C==1)
	{
		DEMUX_C_ON;
	}
	else
	{
		DEMUX_C_OFF;
	}
	if(B==1)
	{
		DEMUX_B_ON;
	}
	else
	{
		DEMUX_B_OFF;
	}
	if(A==1)
	{
		DEMUX_A_ON;
	}
	else
	{
		DEMUX_A_OFF;
	}
}
