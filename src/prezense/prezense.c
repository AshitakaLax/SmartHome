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


typedef enum {
	false = 0,
	true = 1
} bool;

	
bool is_light_on = false; 
bool occupied = false;
bool checking_occupancy = false;

void
turn_on_checking_occupancy(void)
{
	//LED_ON;
	checking_occupancy = true;
	PORTB |= 1 << 2;
}

void
turn_off_checking_occupancy(void)
{
	//LED_OFF;
	checking_occupancy = false;
	PORTB &= ~(1 << 2);
}
	

void
turn_on_light(void)
{
	is_light_on = true;
	LED_ON;
	PORTB |= 1 << 3;
}

void
turn_off_light(void)
{
	is_light_on = false;
	LED_OFF;
	PORTB &= ~(1 << 3);
}

void
update_signals(bool* motion, bool* tripwire)
{
    int16_t read_number = analog_read(0); // pin ADC0, I hope
    int16_t motion_sensor = analog_read(1); // pin ADC1, I hope
    // print("0x");
    // phex16(read_number);
    // print(" 0x");
    // phex16(motion_sensor);
        
    //print(" (0x");
    //phex16(reference);
                
    //if (read_number > reference)
    if ((unsigned)read_number >= 0x200)
    {
		if (tripwire)
			*tripwire = true;
        //LED_ON;
        PORTB |= 1 << 0;
    }
    else
    {
		if (tripwire)
			*tripwire = false;
        //LED_OFF;
        PORTB &= ~(1 << 0);
    }
        
    if ((unsigned)motion_sensor >= 0x200)
	{
		if (motion)
			*motion = true;
        PORTB |= 1 << 1;
	}
    else
	{
		if (motion)
			*motion = false;
        PORTB &= ~(1 << 1);
	}
}


void
delay(void)
{
	unsigned counter = 0;
	unsigned max = 0x3800;
	do 
	{
		//print("0x");
		//phex16(counter);
		
		// unsigned counter2;
		// for (counter2 = 0; counter2 < (unsigned)-1; counter2++)
		// {
		// 	
		// 	print(" 0x");
		// 	phex16(counter2);
		// 	
		// 	print("\n");
		// }
		
		
		//print("\n");
		bool tripwire;
		update_signals(NULL, &tripwire);
		
		if (tripwire)
			counter = 0;
		else
			counter++;
		
	} while (counter <= max);
	print("done\n");
}


int 
main(void)
{
	bool motion = false;
	bool tripwire = false;
	
    LED_CONFIG;
    LED_OFF;

    // initialize the USB, but don't want for the host to
    // configure.  The first several messages sent will be
    // lost because the PC hasn't configured the USB yet,
    // but we care more about blinking than debug messages!
    usb_init();
    
    /*
    // AVCC with external capacitor on AREF pin
    SET_BIT(ADMUX, REFS0, 1);
    SET_BIT(ADMUX, REFS1, 0);
    */
    
    analog_reference(1 << REFS0); // VCC voltage reference
    
    DDRB |= 1 << 0; // output on PORT B 0  tripwire
    DDRB |= 1 << 1; // output on PORT B 1  motion
	DDRB |= 1 << 2; // output on PORT B 2  occupancy
	DDRB |= 1 << 3; // output on PORT B 3  relay
    
    PORTB |= 1 << 1;
    
    while (1)
    {	
		update_signals(&motion, &tripwire);
		
		if (checking_occupancy)
		{
			if (motion)
				occupied = true;
			else
			{
				occupied = false;
				turn_off_light();
			}
			turn_off_checking_occupancy();
		}
		else if (tripwire)
		{
			turn_on_light();
			turn_on_checking_occupancy();
			delay();
		}
		else if (motion)
		{
			turn_on_light();
			occupied = true;
		}
    }
    
    return 0;
}





void
write_to_usb_debug(int16_t num_to_print)
{
    usb_debug_putchar('\n');
    usb_debug_flush_output();
}