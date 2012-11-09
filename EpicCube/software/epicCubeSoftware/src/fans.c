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
/*
*	This example is comprized of mainly arduino sources
*/
void analogWrite(uint8_t, int val)
{
	//first call digitalPinToTimer(pin) found in Arduino.h
	
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
	// CPU frequency is 16,000,000
	//first Pre-scalar 8 *50
	
	TCCR1A = 0;
	ICR1 = 19999;
	//configure timer 1
	TCCR1A = (1<<WGM11)
		TCCR1B = (1 << WGM13) | (1<<WGM12) | (1<<CS11);
	DDRB |= _BV(1);
	TCCR1A |= 2 << 6;//enable non-invert mode-mode 2
	DDRB |= _BV(2); // make B2 and output pin
	TCCR1A |= 2 << 4; // enable PWM on ...
}
/**
*	initializes the pins for the fans.
*/
void InitializeFans()
{
	FAN1_CONFIG;
	FAN2_CONFIG;
	FAN3_CONFIG;
	FAN4_CONFIG;
	FAN5_CONFIG;
	DDRC |= _BV(PC4);// s
	TCCR3A = _BV(WGM
	
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