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
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "usb_debug_only.h"
#include "print.h"
#include "analog.h"


// Teensy 2.0: LED is active high
#if defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)
#define LED_ON      (PORTD |= (1<<6))
#define LED_OFF     (PORTD &= ~(1<<6))

// Teensy 1.0: LED is active low
#else
#define LED_ON      (PORTD &= ~(1<<6))
#define LED_OFF     (PORTD |= (1<<6))
#endif

#define LED_CONFIG      (DDRD |= (1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define DIT 80      /* unit time for morse code */

//void morse_character(char c);
//void morse_P(const char *s);
//const unsigned char morse_code_table[];

// PORT AND BIT ARE EVALUTED MULTIPLE TIMES!
#define SET_BIT(port, bit, onoff) \
    ((onoff) ? ((port) |= 1 << bit) : ((port) &= ~(1 << bit)))

void write_to_usb_debug(int16_t num_to_print);


// Teensy 2.0: LED is active high
#if defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)
#define LED_ON      (PORTD |= (1<<6))
#define LED_OFF     (PORTD &= ~(1<<6))

// Teensy 1.0: LED is active low
#else
#define LED_ON      (PORTD &= ~(1<<6))
#define LED_OFF     (PORTD |= (1<<6))
#endif

#define LED_CONFIG      (DDRD |= (1<<6))


int 
main(void)
{
	unsigned counter;
	
	LED_CONFIG;
	LED_ON;
    usb_init();
	
	print("starting\n");
	counter = 0;
	do 
	{
		print("0x");
		phex16(counter);
		
		// unsigned counter2;
		// for (counter2 = 0; counter2 < (unsigned)-1; counter2++)
		// {
		// 	
		// 	print(" 0x");
		// 	phex16(counter2);
		// 	
		// 	print("\n");
		// }
		print("\n");
		counter++;
	} while (counter != 0);
	print("done\n");
	LED_OFF;
	
	while (1)
	{
	}
	
	return 0;
}





void
write_to_usb_debug(int16_t num_to_print)
{
    usb_debug_putchar('\n');
    usb_debug_flush_output();
}