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


#include "Fans.h"
/**
*	initializes the pins for the fans.
*/
void InitializeFans()
{
	FAN_ZERO_CONFIG;
	
	TCCR2 = _BV(WGM20) // PWM, Phase Correct mode
	  | _BV(COM21) // Clear OC2 on compare match when up-counting. 
      | _BV(CS20); // prescaler 1
	//OCR1A = ((int8_t)PIND + 0x7F);// used to set PWM.
}

/**
*	Sets the speed of a fan
*  FanSelect 0-4 for the specific fan.
*  FanSpeed 0-255 0 for off and 255 for Max power.
*/
void SetFan(unsigned char FanSelect, unsigned short FanSpeed)
{

}

/*
*	gets the FanSpeed of a specific Fan
* returns 0 - 100 based on the fan speed.
*/
unsigned short FanStatus(unsigned char FanSelect)
{
	if(FanSelect == 0)
	{
		return FanZeroStatus;
	}
	else if(FanSelect == 1)
	{
		return FanOneStatus;
	}
	else if(FanSelect == 2)
	{
		return FanTwoStatus;
	}
	else if(FanSelect == 3)
	{
		return FanThreeStatus;
	}
	else 
	{
		return FanFourStatus;
	}
}