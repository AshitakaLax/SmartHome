/* LED Blink Example with USB Debug Channel for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008, 2010 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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

#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))

#define LED_CONFIG	(DDRD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

volatile uint32_t count;
volatile uint8_t dutyCount;

int main(){
	LED_CONFIG;
	
	/**	// help from www.pjrc.com/teensy/pinout.html
	// and www.arduino.cc/playground/Main/TimerPWMCheatsheat
	
	// pin refferences are for Ports
	// assume all CPU clks are 16Mhz
	// Pins PB7 PD0 belong to Timer 0
//		Setting 	Divisor 	Frequency
//		0x01 	 	1 	 		62500
//		0x02  		8 	 		7812.5
//		0x03  		64 	 		976.5625
//		0x04 	 	256 	 	244.140625
//		0x05 	 	1024 	 	61.03515625

//		TCCR0B = TCCR0B & 0b11111000 | <Setting>; //this is how to set the timer frequency
	// Pins PB6 PB5 belong to timer 1
//		Setting 	Divisor 	Frequency
//		0x01 	 	1 	 	31250
//		0x02 	 	8 	 	3906.25
//		0x03  		64 	 	488.28125
//		0x04  		256 	 	122.0703125
//		0x05 	 	1024 	 	30.517578125
// PWM on pin for timer 0
	//prescale is set to 64 by default for B// which is fine
	// for the A prescaler 
	
	//fast PWM_freq = (16000000)/(prescaler * 256) = 976
	// now we need to se the A prescaler
	// This is for FAST PWM 
//	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	// to use Phase-correct PWM on timer 0 (half the frequency of FAST PWM)
//	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
	
//		TCCR1B = TCCR1B & 0b11111000 | <setting>;
	// pins PD1 PB4 belong to timer 2
	//similar, to the above,but
	// TCCR2B = TCCR2B & 0b11111000 | <setting>;
	// pins PC4 PC5 PC6 belong to timer 6
	

       // set up 2 PWM channels on PB1 and PB2 using Timer1

	// TCCR1C = 0;     // disable all PWM on Timer1 whilst we set it up
	// //ICR1C = 19999;   // frequency is every 20ms


	// // Configure timer 1 for Fast PWM mode via ICR1, with no prescaling
	// //TCCR1C |= (1 << WGM11);
	// TCCR1C = (1 << WGM13) | (1<<WGM12) | (1 << WGM11);
	// TCCR1B = 0;
	// TCCR1B |= (1<<CS10);
	// TCCR1C |= (1<<COM1C1);
	// ICR1 = 1;
	// OCR1A = ICR1 / 2;
	
	// // Set PB1 and PB2 as outputs
	// DDRB |= _BV(1) |  _BV(2);
	// TCCR1A |= 2 <<  6;  // enable PWM on port B1 in non-inverted compare mode 2
 	// TCCR1A |= 2 <<  4;  // enable PWM on port B2 in non-inverted compare mode 2

	// OCR1A = ICR1 * 2 /20; // 2ms pulse to left motor on PB1
	// OCR1B = ICR1 * 2 /20; // 2ms pulse to right motor on PB2

	//simple attempt to drive PB7 and PD0
	// TCCR0A = 0;// disable
	// //ICR1 = 200;// made up number
	// //step 1 set the prescalar
	// TCCR0B = (TCCR0B & (0b11111000 | 0x04));// setting frequency to 244... we don't need anything fast
	// //since we don't need anything fast we will use the Phase-correct PWM
	// // step 2 set the comparator trigger.
	// TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
	
	// //need to enable the pins.
	// //Step 3 Set the duty cycle
	// OCR0A = 0xFF; // this is a duty cycle from 0 to 255
	
	// //Enable the output just like any pin
	// //Step enable pin, PB7
// //	DDRB |= (1<<7);// enable Port B 7
	// //same as 
	// DDRB |= _BV(7);
	// //since that didn't work we will try with the other set of pins
	// // on timer 1
// //	PB6 PB5 belong to timer 1
	// TCCR1A = 0;
	// ICR1 = 19999;//this is the frequency
	// TCCR1B = (TCCR1B & (0b11111000 | 0x03)); //first attempt may need to switch
	// TCCR1A = (1 << WGM11);
// //	TCCR1B = (1 << WGM13) | (1<<WGM12) | ( 1 << CS10);// this is the second attempt
	// //enable pins pb6 and pb5
	// DDRB |= _BV(5) | _BV(6);
	// TCCR1A |= 2 << 6;
	// TCCR1A |= 2 << 4;//
	
	// OCR1A = ICR1 *2 /20;
	// OCR1B = ICR1 * 2 /20;//these could be way off.
	//not sure if this will work one way to find out.
	//third attempt
	// DDRB |= (1 << 5);//this is PB5 which is a PWM pin
	// TCCR1A = ((1 << COM1A1) | (1<< COM1A0));
	// TCCR1B = ((1 <<CS10) | (1 << WGM13));
**/
	/**
	//fourth attempt
	// this time using extremeelectronics.co...
	TCCR0A |= (1<<CS02) | (1<<CS00);// set prescaler = FCPU/1024
	TCCR0B |= (1<<CS02) | (1<<CS00);
	TIMSK0 |= (1<<TOIE0);//enable overflow interrupts
	TCNT0= 0;//initialize counter
	count = 0;
	DDRC |= 0x0F; // port C 3,2,1,0 as outputs
	PORTC != PORTC;
	sei();// enable interrupts
	LED_ON;
	**/
	//fifth attempt IT works
	//use timer 2
	/**
	DDRB |= (1 <<7);// output
	OCR0A = 233; // Duty cycle
	TCCR0A |= (1 << COM0A1);// set none-inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);// //Set Fast PWM mode 
	TCCR0B |= (1 << CS01);// Set prescalar to 8 and starts PWM

	while(1)
	{
		//	OCR1A = 0xa;
		if(OCR0A < 255)
		{
			OCR0A++;
		}
		else
		{
			OCR0A = 0;
		}
   	}
	**/
	dutyCount = 0xFF;
	//Timer 0
	PWMNew(2);
	PWMStart(2);
	PWMDuty(2, dutyCount);
	PWMNew(3);
	PWMStart(3);
	PWMDuty(3, dutyCount);
	
	//timer 1
	PWMNew(22);
	PWMStart(22);
	PWMDuty(22, dutyCount);
	PWMNew(23);
	PWMStart(23);
	PWMDuty(23, dutyCount);
	
	//Timer 2
	PWMNew(24);
	PWMStart(24);
	PWMDuty(24, dutyCount);
	PWMNew(4);
	PWMStart(4);
	PWMDuty(4, dutyCount);
	
	//Timer 3
	PWMNew(17);
	PWMStart(17);
	PWMDuty(17, dutyCount);
	PWMNew(18);
	PWMStart(18);
	PWMDuty(18, dutyCount);
	PWMNew(19);
	PWMStart(19);
	PWMDuty(19, dutyCount);
	
	count = 0;
	uint8_t toggle = 0;
	while(1)
	{
		if(count < 10000)
		{
			if(toggle == 0)
				LED_ON;
			else
				LED_OFF;
			count++;
			
		}
		else
		{
			toggle = ~toggle;
			count = 0;
			dutyCount++;
			PWMDuty(2, dutyCount);
			PWMDuty(3, (0xFF -dutyCount));
			PWMDuty(22, dutyCount);
			PWMDuty(23, dutyCount);
			PWMDuty(17, dutyCount);
		}
	}


return 0;

}

