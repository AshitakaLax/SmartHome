/* 
 * This code was made by Levi Balling
 * portions of code were provided from:
 * LED Blink Example with USB Debug Channel for Teensy USB Development Board
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
#include <string.h>
#include "include/usb_serial.h"//added to test the built with it.
#include "include/HVACGarage.h"// works and compiles
#include "include/damper_Control.h"
#include "include/tempSense.h"

//need to add fan.h later when I figure out how to generate a pwm

// Teensy 2.0++ 
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n)) // clock rate at 16Mhz

void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);// still needs updating

//global verbose adds lots of print statements
uint8_t verbose = 1;

int main(void)
{
	char buf[32];
	uint8_t n;

	CPU_PRESCALE(0);
	
	initializeHVACGarage();
	InitializeDamper();
	initializeTempSense();
	// still need to initialize fans

	usb_init();
	while(!usb_configured());
	_delay_ms(1000);
	//enter infinite loop
	while (1) 
	{
		while (!(usb_serial_get_control() & USB_SERIAL_DTR))// caught error with ";"
		usb_serial_flush_input();

		send_str(PSTR("\r\n***************************************"));
		send_str(PSTR("\r\nWelcome to Epic Cube Serial Control\r\n"));
		send_str(PSTR("***************************************"));
		send_str(PSTR("\r\nto receive general help type \"-help\"\r\n\r\n>"));

		while (1) 
		{
			send_str(PSTR("> "));
			n = recv_str(buf, sizeof(buf));
			if (n == 255) break;
			send_str(PSTR("\r\n"));
			parse_and_execute_command(buf, n);
			
		}
	}
}

/** 
*	Send a string to the USB serial port.  The string must be in
* 	flash memory, using PSTR
**/
void send_str(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
	
}

/** Receive a string from the USB serial port.  The string is stored
* in the buffer and this function will not exceed the buffer size.
* A carriage return or newline completes the string, and is not
* stored into the buffer.
* The return value is the number of characters received, or 255 if
* the virtual serial connection was closed while waiting.
**/
uint8_t recv_str(char *buf, uint8_t size)
{
	
	int16_t r;
	uint8_t count=0;
	//send_str(PSTR("\r\nReceiving String"));
	while (count < size) {
		r = usb_serial_getchar();
		if (r != -1) {
			if (r == '\r' || r == '\n') return count;
			if (r >= ' ' && r <= '~') {
				*buf++ = r;
				usb_serial_putchar(r);
				count++;
			}
		} else {
			if (!usb_configured() ||
			  !(usb_serial_get_control() & USB_SERIAL_DTR)) {
				// user no longer connected
				return 255;
			}
			// just a normal timeout, keep waiting
		}
	}
	return count;
	
	//return 0;
}

/**
*	This function will take the input command, and parse it, 
*	then execute the appropriate command.
**/
void parse_and_execute_command(const char *buf, uint8_t num)
{
	uint8_t Command, Status;// commands and status of each string
	
	// prints the same statement back to the user. 
	if(verbose)
	{
		send_str(PSTR("Received: "));
		usb_serial_write(buf, num);
		send_str(PSTR("\r\n"));
	}
	
	if(num < 4)
	{
		send_str(PSTR("ERROR:1 \r\nformat to small, see help for details\r\n"));
		return;
	}
	if(buf[0] == 'D')// damper section Command is Damper### ##1 for closed and ##0 for open
	{
		if(buf[6] == '?')// status request is Damper?## check to see if open or closed returns 1 for closed and 0 for open.
		{
			//not this will work for up to 9 dampers
			Command = buf[8] - '0';// convert ascii value to decimal
			Status = CheckDamper(Command);
			send_str((char*)(Status + '0'));// returns the status
			send_str(PSTR("\r\n"));
			return;
		}
		else
		{
			Command = buf[7] - '0';
			if(buf[8] == '0')// open
			{
				OpenDamper(Command);
				send_str(PSTR("Damper Open Complete.\r\n"));
				return;
			}
			else
			{
				CloseDamper(Command);
				send_str(PSTR("Damper Close Complete.\r\n"));
				return;
			}
		}
	}
	if(buf[0] == 'H')// HVAC Section
	{	
		if(verbose)
		send_str(PSTR("entering HVAC\r\n"));// H0V1A2V3?4
		if(buf[4] == '?')// status request
		{
		
			if(GFanStatus == 0)// if furnaced controlled
			{
				
				//read furnace for status
				if(ReadGFan() == 1)//if furnace is on
				{
					if(ReadWHeat() == 1)// and heat is on
					{
						send_str(PSTR("2\r\n"));
						return;
					}
					else if(ReadYCool() == 1)// and the cooler is on
					{
						send_str(PSTR("3\r\n"));
						return;
					}
					else
					{
						send_str(PSTR("1\r\n"));
						return;
					}
				}
				else //it is all off
				{
					send_str(PSTR("7\r\n"));
					return;
				}
			}
			else// Server is controlling furnace
			{
				//so GfanStatus is already on
				//check if hot or cold are on
				if(WHeatStatus == 1)
				{// 5
					send_str(PSTR("5\r\n"));// blower and heater.
					return;
				}
				if(YCoolStatus == 1)// blower and Cooler
				{
					send_str(PSTR("6\r\n"));
					return;
				}
				send_str(PSTR("4\r\n"));// just the blower is on
				return;
			}
		}
		else // command
		{
			if(buf[4] == '1')
			{
				SetWHeat(0);
				SetYCool(0);
				SetGFan(0);
				send_str(PSTR("Server controlled off\r\n"));
			}
			else if(buf[4] == '2')
			{
				SetWHeat(0);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				SetGFan(1);
				send_str(PSTR("Server controlled Fan\r\n"));
			}
			else if(buf[4] == '3')// heat and blower on
			{
				SetGFan(1);
				SetWHeat(1);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				send_str(PSTR("Server controlled Fan and Heating\r\n"));
			}
			else if(buf[4] == '4')// heat and blower on
			{
				SetGFan(1);
				SetWHeat(0);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				send_str(PSTR("Server controlled Fan and Cooling\r\n"));
			}
			else
			{
				// this should be for the server controlled off(need to add to hardware)
				// for now Turn crapper old thermostat to off.
			}
			return;
		}
	}
	
		// Documentation change from Project description
	 	// instead of Attic and Basement, we are gong with just Temperature
	 // sensors, and we will have to decide which temperature sensors are where
	 // in the server code.
	 // this is a Sts only so it is Temp?## from 0 - 63 for ##
	 // example Temp?16 will read from temperature sensor 16 and return the
	 // value in C
	
	if(buf[0] == 'T') // Temperature Section
	{
		Command = buf[5] - '0';// get the number room value
		Command = Command * 10;// ten's digit
		Command += (buf[6] - '0');// single digit.
		Status = ReadTempSensor(Command);// query temperature section for info.
		send_str((char *)Status);// return status.
		return;
	}
	else if(buf[0] == 'G')// Garage Section
	{
		//this is only one thing. which is a pulse
		PulseGarage();
		return;
	}
	else if(buf[0] == 'h')// Help Section
	{
		//help commands so write a bunch of things that help people.
	}
		send_str(PSTR("\r\n"));
}
