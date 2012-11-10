 /* Simple Library for generating a PWM signal on one of the Teensy2.0++ pins.
 *	Author: Levi Balling
 *
 *	 Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "PWMTeensyTwoPlusPlus.h"
// all of the registers can be found at the following link. they all work to make
// specific outputs.
//(www.pjrc.com/teensy/at90usb1286.pdf) pg 429
// ORDER OF EXECUTION:
//	1st: PWMNew
//	2nd: PWMStart
//	3rd: PWMDuty
// 	4rd(optional): PWMStopTimer || PWMEnableDisablePin

// NOTE: all page(pg) references are to the datasheet, and sections.
//select which pins to enable.
/**
*	This creates a PWM pin ready to be set.
*/
void PWMNew(uint8_t pin)// we are going with the physical pin numbers
{
	if(pin == 2||pin == 3)// Timer 0: PB7(A), PD0(B)
	{
		if(pin == 2)
		{
			PWM0A_CONFIG;// check header file for more info
			TCCR0A |= (1 << COM0A1);
		}			
		else
		{
			PWM0B_CONFIG;
			TCCR0A |= (1 << COM0B1);
		}
		TCCR0A |= (1 << WGM01) | (1 << WGM00);// set fast PWM Mode pg 106(section 13.6.3)
	}
	else if(pin == 22 || pin == 23)// Timer 1: PB6(B), PB5(A)
	{
		if(pin == 23)
		{
			PWM1A_CONFIG;
			TCCR1A |= (1 << COM1A1);
		}
		else
		{
			PWM1B_CONFIG;
			TCCR1A |= (1 << COM1B1);
		}
		// both of the following 2 registers set the mode for the PWM Fast. Table 14.4 pg 142
		TCCR1A |= (1 << WGM10);
		TCCR1B |= (1 << WGM12);// set Fast PWM mode using ICR1 as top
		
		
	}
	else if(pin == 24 || pin == 4)// Timer 2: PB4(A), PD1(B)
	{
		// Configure pin to be an output
		if(pin == 24)
		{
			PWM2A_CONFIG;
			TCCR2A |= (1 << COM2A1);
		}
		else
		{
			PWM2B_CONFIG;
			TCCR2B |= (1 << COM2B1);// this may not be correct.
		}	
		TCCR2A |= (1 << WGM21) | (1 << WGM20);// set Fast PWM mode
	}
	else if(pin == 17 || pin == 18 || pin == 19)// Timer 3:PC4(C),PC5(B),PC6(C)
	{
		// Configure pin to be an output
		if(pin == 19)
		{
			PWM3A_CONFIG;
			TCCR3A |= (1 << COM3A1);
		}
		else if(pin == 18)
		{	
			PWM3B_CONFIG;
			TCCR3A |= (1 << COM3B1);			
		}
		else
		{
			PWM3C_CONFIG;
			TCCR3A |= (1 << COM3C1);			
		}
		TCCR3A |= (1<< WGM30);
		TCCR3B |= (1 << WGM32);
	}
		
}

/*
*	For people who don't care how fast or slow the PWM frequency is
*	They just care about the duty cycle. this will set all timers to 1/1024 of the CPU clk
* 	15.625Khz will be the frequency of their timer. based on a 16Mhz clock
*/
void PWMStart(uint8_t pin)
{
	if(pin == 2||pin == 3)// Timer 0: PB7(A), PD0(B)
	{
		PWMStartTimer(0,5);
	}
	else if(pin == 22 || pin == 23)// Timer 1: PB6(B), PB5(A)
	{
		PWMStartTimer(1,5);		
	}
	else if(pin == 24 || pin == 4)// Timer 2: PB4(A), PD1(B)
	{
		PWMStartTimer(2,7);		
	}
	else if(pin == 17 || pin == 18 || pin == 19)// Timer 3:PC4(C),PC5(B),PC6(C)
	{
		PWMStartTimer(3,5);
	}
}
/**
*	This will start the timer with a specific Frequency Prescalar(if available to that timer)
*	This assumes that everything is in the PWM FAST mode(set in PWMConfig). pg 113 section 13.8.1
*
*/
void PWMStartTimer(uint8_t timer, uint8_t CLKspeed)// we are going with the physical pin numbers
{
	// check which timer the pin belongs too.
	if(timer == 0)// Timer 0: PB7(A), PD0(B) This information can be found on pg 114 Section 13.8.2
	{
		// stop timer while configuring to disired configuration: Note: there is a quicker way.
		TCCR0B &= ~(1 << CS00);
		TCCR0B &= ~(1 << CS01);
		TCCR0B &= ~(1 << CS02);
		
		// Configure pin to be an output
		if(CLKspeed == 1)// No Prescaler 16 Mhz clock
		{
			TCCR0B |= (1<< CS00); 
		}
		else if(CLKspeed == 2)
		{
			TCCR0B |= (1 << CS01); // set prescalar to 1/8 of clock speed = 2Mhz
		}
		else if(CLKspeed == 3)
		{
			TCCR0B |= (1 << CS01) | (1<< CS00); // set prescalar to 1/64 of clk speed = 250 Khz
		}
		else if(CLKspeed == 4)// NOTE Timer 0 doesn't have 1/128 or 1/512
		{
			TCCR0B |= (1 << CS02); // set prescalar to 1/256 of CPU clk = 62.5Khz
		}
		else if(CLKspeed == 5)
		{
			TCCR0B |= (1 << CS02) | (1<< CS00); // set prescalar to 1/1024 of CPU clk = 15.625Khz
			//NOTE slower rates aren't possible with internal settings for Timer0.  
			// if you need slower rates check with a different Timer.
		}
		else if(CLKspeed == 6)// external clock, useful for own clock example 10Mhz, or 1Khz...
		{
			TCCR0B |= (1 << CS02) | (1<< CS01); // pin T0(PD7) tick on the Falling Edge
		}
		else if(CLKspeed == 7)
		{
			TCCR0B |= (1 << CS02) | (1<< CS01)| (1<< CS00); // Pin T0(PD7) tick on the rising Edge
		}
		else
			return;
	}
	else if(timer == 1)// Timer 1: PB6(B), PB5(A) This information can be found on pg 144 Section 14.10.4
	{	// this section assumes MODE is FAST PWM, with 8 bit counter.  Not for slower clock rates
		// change Mode from 8 bit to 9,10, or use a different register to compare against. 
		// stop timer while configuring to disired configuration: Note: there is a quicker way.
		TCCR1B &= ~(1 << CS10);
		TCCR1B &= ~(1 << CS11);
		TCCR1B &= ~(1 << CS12);
		
		// Configure pin to be an output
		if(CLKspeed == 1)// No Prescaler 16 Mhz clock
		{
			TCCR1B |= (1<< CS10); 
		}
		else if(CLKspeed == 2)
		{
			TCCR1B |= (1 << CS11); // set prescalar to 1/8 of clock speed = 2Mhz
		}
		else if(CLKspeed == 3)
		{
			TCCR1B |= (1 << CS11) | (1<< CS10); // set prescalar to 1/64 of clk speed = 250 Khz
		}
		else if(CLKspeed == 4)// NOTE Timer 0 doesn't have 1/128 or 1/512
		{
			TCCR1B |= (1 << CS12); // set prescalar to 1/256 of CPU clk = 62.5Khz
		}
		else if(CLKspeed == 5)
		{
			TCCR1B |= (1 << CS12) | (1<< CS10); // set prescalar to 1/1024 of CPU clk = 15.625Khz
			//NOTE slower rates aren't possible with internal settings for Timer0.  
			// if you need slower rates check with a different Timer.
		}
		else if(CLKspeed == 6)// external clock, useful for own clock example 10Mhz, or 1Khz...
		{
			TCCR1B |= (1 << CS12) | (1<< CS11); // pin T0(PD7) tick on the Falling Edge
		}
		else if(CLKspeed == 7)
		{
			TCCR1B |= (1 << CS12) | (1<< CS11)| (1<< CS10); // Pin T0(PD7) tick on the rising Edge
		}
		else
			return;
	}
	else if(timer == 2)// Timer 2: PB4(A), PD1(B) This information can be found on pg 164 Section 15.8.2
	{	// this section assumes MODE is FAST PWM, with 8 bit counter.  
		TCCR2B &= ~(1 << CS20);
		TCCR2B &= ~(1 << CS21);
		TCCR2B &= ~(1 << CS22);
		
		// Configure pin to be an output
		if(CLKspeed == 1)// No Prescaler 16 Mhz clock
		{
			TCCR2B |= (1<< CS20); 
		}
		else if(CLKspeed == 2)
		{
			TCCR2B |= (1 << CS21); // set prescalar to 1/8 of clock speed = 2Mhz
		}
		else if(CLKspeed == 3)
		{
			TCCR2B |= (1 << CS21) | (1<< CS20); // set prescalar to 1/32 of clk speed = 500 Khz
		}
		else if(CLKspeed == 4)
		{
			TCCR2B |= (1 << CS22); // set prescalar to 1/64 of CPU clk = 250Khz 
		}
		else if(CLKspeed == 5)
		{
			TCCR2B |= (1 << CS22) | (1<< CS20); // set prescalar to 1/128 of CPU clk = 125Khz
		}
		else if(CLKspeed == 6)
		{
			TCCR2B |= (1 << CS22) | (1<< CS21);  // set prescalar to 1/256 of CPU clk = 62.5Khz
		}
		else if(CLKspeed == 7)
		{
			TCCR2B |= (1 << CS22) | (1<< CS21)| (1<< CS20); // set prescalar to 1/1024 of CPU clk = 15.625Khz
		}
		else
			return;
	}
	else if(timer == 3)// Timer 3: PC4(C),PC5(B),PC6(C) Same as timer 1 but different registers
	{	
		//This information can be found on pg 144 Section 14.10.4
		// this section assumes MODE is FAST PWM, with 8 bit counter.  Not for slower clock rates
		// change Mode from 8 bit to 9,10, or use a different register to compare against. 
		// stop timer while configuring to disired configuration: Note: there is a quicker way.
		TCCR3B &= ~(1 << CS30);
		TCCR3B &= ~(1 << CS31);
		TCCR3B &= ~(1 << CS32);
		
		// Configure pin to be an output
		if(CLKspeed == 1)// No Prescaler 16 Mhz clock
		{
			TCCR3B |= (1<< CS30); 
		}
		else if(CLKspeed == 2)
		{
			TCCR3B |= (1 << CS31); // set prescalar to 1/8 of clock speed = 2Mhz
		}
		else if(CLKspeed == 3)
		{
			TCCR3B |= (1 << CS31) | (1<< CS30); // set prescalar to 1/64 of clk speed = 250 Khz
		}
		else if(CLKspeed == 4)// NOTE Timer 0 doesn't have 1/128 or 1/512
		{
			TCCR3B |= (1 << CS32); // set prescalar to 1/256 of CPU clk = 62.5Khz
		}
		else if(CLKspeed == 5)
		{
			TCCR3B |= (1 << CS32) | (1<< CS30); // set prescalar to 1/1024 of CPU clk = 15.625Khz
			//NOTE slower rates aren't possible with internal settings for Timer0.  
			// if you need slower rates check with a different Timer.
		}
		else if(CLKspeed == 6)// external clock, useful for own clock example 10Mhz, or 1Khz...
		{
			TCCR3B |= (1 << CS32) | (1<< CS31); // pin T0(PD7) tick on the Falling Edge
		}
		else if(CLKspeed == 7)
		{
			TCCR3B |= (1 << CS32) | (1<< CS31)| (1<< CS30); // Pin T0(PD7) tick on the rising Edge
		}
		else
			return;
	}
}

void PWMStopTimer(uint8_t timer)// we are going with the physical pin numbers Stop counter
{
	// check which timer the pin belongs too.
	if(timer == 0)// Timer 0: PB7(A), PD0(B)
	{
		TCCR0B &= ~(1 << CS00);
		TCCR0B &= ~(1 << CS01);
		TCCR0B &= ~(1 << CS02);
	}
	else if(timer == 1)// Timer 1: PB6(B), PB5(A)
	{
		TCCR1B &= ~(1 << CS10);
		TCCR1B &= ~(1 << CS11);
		TCCR1B &= ~(1 << CS12);
	}
	else if(timer == 2)// Timer 2: PB4(A), PD1(B)
	{
		TCCR2B &= ~(1 << CS20);
		TCCR2B &= ~(1 << CS21);
		TCCR2B &= ~(1 << CS22);
	}
	else if(timer == 3)// Timer 3:PC4(C),PC5(B),PC6(C)
	{
		TCCR3B &= ~(1 << CS30);
		TCCR3B &= ~(1 << CS31);
		TCCR3B &= ~(1 << CS32);
	}
		
}
/*
*	All this does is change the mode for each pin.  from either Clear 0C2B on Compare Match,
* 	to Normal Port operation(high/Low) OCnX is disconnected when it is Disabled.
*/
void PWMEnableDisablePin(uint8_t pin, uint8_t enable)// we are going with the physical pin numbers Stop counter
{
	// check which timer the pin belongs too.
	if(pin == 2||pin == 3)// Timer 0: PB7(A), PD0(B)
	{
		// Configure pin to be an output
		if(pin == 2)
			if(enable)
				TCCR0A |= (1 << COM0A1);
			else
				TCCR0A &= ~(1 << COM0A1);
		else
			if(enable)
				TCCR0A |= (1 << COM0B1);
			else
				TCCR0A &= ~(1 << COM0B1);
	}
	else if(pin == 22 || pin == 23)// Timer 1: PB6(B), PB5(A)
	{
		// Configure pin to be an output
		if(pin == 23)
			if(enable)
				TCCR1A |= (1 << COM1A1);
			else
				TCCR1A &= ~(1 << COM1A1);
		else
			if(enable)
				TCCR1A |= (1 << COM1B1);
			else
				TCCR1A &= ~(1 << COM1B1);
	}
	else if(pin == 24 || pin == 4)// Timer 2: PB4(A), PD1(B)
	{
		// Configure pin to be an output
		if(pin == 24)
			if(enable)
				TCCR2A |= (1 << COM2A1);
			else
				TCCR2A &= ~(1 << COM2A1);
		else
			if(enable)
				TCCR2A |= (1 << COM2B1);
			else
				TCCR2A &= ~(1 << COM2A1);
	}
	else if(pin == 17 || pin == 18 || pin == 19)// Timer 3:PC4(C),PC5(B),PC6(C)
	{
		// Configure pin to be an output
		if(pin == 19)
			if(enable)
				TCCR3A |= (1 << COM3A1);
			else
				TCCR3A &= ~(1 << COM3A1);
		else if(pin == 18)
			if(enable)
				TCCR3A |= (1 << COM3B1);
			else
				TCCR3A &= ~(1 << COM3B1);
		else
			if(enable)
				TCCR3A |= (1 << COM3C1);
			else
				TCCR3A &= ~(1 << COM3C1);
	}
		
}
/**
*	this will change the duty cycle from 0%(h0x00|d0) to 100%(h0xFF|d255)
*	
**/
void PWMDuty(uint8_t pin, uint8_t dutyCycle)
{
	if(pin == 2||pin == 3)// Timer 0: PB7(A), PD0(B)
	{
		if(pin == 2)
			OCR0A = dutyCycle;// this is an 8 bit register pg 100 (13)
		else
			OCR0B = dutyCycle;// this is an 8 bit register pg 100 (13)
			
	}
	else if(pin == 22 || pin == 23)// Timer 1: PB6(B), PB5(A)
	{
		if(pin == 23)
			OCR1A = dutyCycle;
		else
			OCR1B = dutyCycle;
	}
	else if(pin == 24 || pin == 4)// Timer 2: PB4(A), PD1(B)
	{
		if(pin == 24)
			OCR2A = dutyCycle;
		else
			OCR2B = dutyCycle;
	}
	else if(pin == 17 || pin == 18 || pin == 19)// Timer 3:PC4(C),PC5(B),PC6(C)
	{
		// Configure pin to be an output
		if(pin == 19)
			//OCR3A = dutyCycle;
			OCR3A = dutyCycle;
		else if(pin == 18)
			//OCR3B = dutyCycle;
			OCR3B = dutyCycle;
		else
			OCR3C = dutyCycle;
	}
}
