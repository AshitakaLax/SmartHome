/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the different dampers it will need to store in memory, 
* all the correct values.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/delay.h>


#include "../include/GlobalVar.h"
#include "../include/damper_Control.h"

uint8_t dVerbose = 0;
/**
*	initializes all of the dampers outputs.
*/
void InitializeDamper()
{
	MOTORENABLEHIGH;//DISABLE THE PIN
	MOTORENABLECONFIG;// A4988 driver us this one
	MOTORDIRCONFIG;// unused
	MOTORSTEPCONFIG;
	ISBUTTONCLOSE_CONFIG;
	ISBUTTONOPEN_CONFIG;
}



/**
*	this activates the correct damper
* 	Then while checking the Damper's status
*	runs the stepper motor.
*/
void OpenDamper( uint8_t damper)
{
	if(dVerbose)
	{
		
		send_str(PSTR("\r\nDamper: "));
		usb_serial_putchar((damper + '0'));
		send_str(PSTR("\r\nEnter OpenDamper Section \r\n"));
	}
	//set the Damper Demux correctly
	ActivateDamper(damper);
//	for(int i; i < 10000; i++);// wait for signal propagation 0.5 seconds is
	_delay_ms(1000);
	//step the device open
	uint8_t DamperStatus = CheckDamper(damper);
	// if already open, return
	if(DamperStatus == '0')
	{
		if(dVerbose)
			send_str(PSTR("Damper Status == 0"));
		else
			send_str(PSTR("0"));
		return;
	}
	else if(DamperStatus == '1')
	{		
		if(dVerbose)
			send_str(PSTR("DamperStatus == 1, running motor"));
		else
			send_str(PSTR("1"));
		//send_str(PSTR("DamperStatus == 1, running motor"));
	}
	if(dVerbose)
		send_str(PSTR("\r\n"));
	// if not run the motor
	MOTORENABLELOW;
	MOTORDIRHIGH;
//	MotorDirection2LOW; // current setting for A4988
	// set a timeout
	uint16_t timeout = 0; // make sure the motor doesn't spin forever
	// timeout is 5 seconds is 100ms * 50 = 5 seconds
	while(DamperStatus != '0' && timeout < 50)
	{
		// step a lot// need a pause
		MOTORSTEPHIGH;
		DamperStatus = CheckDamper(damper);
		// add ~500 milisec plenty of time for stepper motor not to crush everything 
		_delay_ms(100);
		
		MOTORSTEPLOW;
		timeout++;
	}
	if(timeout > 49)
	{
		// report error to main Could be the wrong board that
		// we are trying to access.
		if(dVerbose)
		{
			send_str(PSTR(" Damper Open Error:TIMEOUT\r\n"));
		}
		else
			send_str(PSTR("error"));
	}
	else
	{
		if(dVerbose)
		{
			send_str(PSTR("Damper Closed complete"));
		}
		else
			send_str(PSTR("done"));
	}
	MOTORENABLEHIGH;
}
/**
*	Closes the damper shut.
*/
void CloseDamper( uint8_t damper)
{
	if(dVerbose)
	{
		send_str(PSTR("\r\nDamper: "));
		usb_serial_putchar((damper + '0'));
		send_str(PSTR("\r\nEnter CloseDamper Section \r\n"));
	}
	
	ActivateDamper(damper);
	
	_delay_ms(1000);
	MOTORENABLELOW;
	//step the device close
	uint8_t DamperStatus = CheckDamper(damper);
	// if already close, return
	if(DamperStatus == '1')
	{
		send_str(PSTR("done"));
		return;
	}
	unsigned int timeout = 0; // make sure the motor doesn't spin 
	MOTORDIRLOW;// go in opposite direction
	//MotorDirection2HIGH;
	// set a timeout to 5 seconds
	while(DamperStatus != '1' || timeout == 50)
	{
		// step a lot
		MOTORSTEPHIGH;
		DamperStatus = CheckDamper(damper);
		_delay_ms(100);
		MOTORSTEPLOW;
		timeout++;
	}
	if(timeout == 10000)
	{
		// report error to main Could be the wrong board that
		// we are trying to access.
		if(dVerbose)
		{
			send_str(PSTR(" Damper Close Error:TIMEOUT\r\n"));
		}
		else
			send_str(PSTR("error"));
	}
	MOTORENABLEHIGH;
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
	//double make sure it is good.
	ActivateDamper(damper);
	
	// check if it is in the middle
	for(int i; i < 10000; i++);// wait for signal propagation 0.5 seconds is
	// enough
	// it is in the middle of turning
	uint8_t openBus;
	if(ISBUTTONOPENBUSHIGH != 0)
	{
		openBus = 1;
	}
	else
		openBus = 0;
	uint8_t closeBus;
	if(ISBUTTONCLOSEBUSHIGH != 0)
		closeBus = 1;
	else	
		closeBus = 0;
	if(dVerbose)
	{
		send_str(PSTR("Open Bus = "));
		usb_serial_putchar((openBus+'0'));
		send_str(PSTR("\r\n"));
		send_str(PSTR("Close Bus = "));
		usb_serial_putchar((closeBus+'0'));
		send_str(PSTR("\r\n"));
	}
	if(openBus && closeBus)
	{
		if(dVerbose)
		{
			send_str(PSTR("Both inputs are HIGH\r\n"));
		}	
		return '2';
	}
	else if((!openBus) && closeBus)
	{
		if(dVerbose)
		{
			send_str(PSTR("OPENBUS is LOW and CLOSE Button is HIGH\r\n"));
		}	
		return '0';
	}
	else 
	{
		if(dVerbose)
		{
			send_str(PSTR("CloseButton is Low and Open Button is HIGH\r\n"));
		}	
		return '1';
	}
}
/**
*	Sets the correct damper to be active.
* Damper is high when button isn't pressed
* for simplicity it would be better to switch this to string comparisons.
*/
void ActivateDamper(uint8_t damper)
{
// converts decimal to binary digits for ABCD
// could be backwards.
// example input A hex or 1010 bin or 10 dec
	uint8_t D = damper / 8;//  D = 1bin or 1 dec
	uint8_t temp = damper % 8;// temp = 10bin and 2 dec
	
	uint8_t C = temp / 4;// C = (2)/4 = 0
	temp = temp % 4;// temp = (2)%4 = 2
	
	uint8_t B = temp / 2;// 1
	temp = temp % 2;// 0
	
	uint8_t A = temp; 
	
	char tempBuf[4];
		//itoa(Command, tempBuf,10);
					
		tempBuf[0] = (char)A + '0';
		tempBuf[1] = (char)B + '0';
		tempBuf[2] = (char)C + '0';
		tempBuf[3] = (char)D + '0';
					
		if(dVerbose)
		{
			send_str(PSTR("Test Outputs of ABCD:\r\n"));
			usb_serial_write(tempBuf, 4);
			//usb_serial_write(itemBuf, 2);
			send_str(PSTR("\r\n"));
		}
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


void DampVerbose(uint8_t verbose)
{
	dVerbose = verbose;
}
