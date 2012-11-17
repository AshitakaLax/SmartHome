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
 
#ifndef  PWMTEENSYTWOPLUSPLUS_H__
#define PWMTEENSYTWOPLUSPLUS_H__

#include <avr/pgmspace.h>
#include <util/delay.h>
// PWM Configure Pins
// Timer 0
#define PWM0A_CONFIG		(DDRB |= (1<<7))
#define PWM0B_CONFIG		(DDRD |= (1<<0))

// Timer 1
#define PWM1A_CONFIG		(DDRB |= (1<<5))
#define PWM1B_CONFIG		(DDRB |= (1<<6))

// Timer 2
#define PWM2A_CONFIG		(DDRB |= (1<<4))
#define PWM2B_CONFIG		(DDRD |= (1<<1))

// Timer 3
#define PWM3A_CONFIG		(DDRC |= (1<<4))
#define PWM3B_CONFIG		(DDRC |= (1<<5))
#define PWM3C_CONFIG		(DDRC |= (1<<6))
/**
*	This is to configure the setup of the pin
**/
void PWMNew(uint8_t pin);
/*
*	This function will start the PWM signal with a the Duty cycle
* 	either the duty cycle has already been specificied, or they need to do that.
*/
void PWMStart(uint8_t pin);
/*
*	This function allow more customizing with clock speed and source
*/
void PWMStartTimer(uint8_t timer, uint8_t CLKspeed);
/*
*	This will stop the PWMSignal and disable the output.
*
*/
void PWMStop(uint8_t pin);
/*
*	This will enabel or disable a pin.  it won't change the
* 	timer counting.
*/
void PWMEnableDisablePin(uint8_t pin, uint8_t enable);
/*
*	This will either configure the duty cycle or change the duty cycle.
* 	input: 0(0x00 or always low) to 255(0xFF or always high). 
*/
void PWMDuty(uint8_t pin, uint8_t dutyCycle);

#endif