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
#include <inttypes.h>
#include <stdio.h>
#include "include/GlobalVar.h"
#include "include/usb_serial.h"//added to test the built with it.
#include "include/HVACGarage.h"// works and compiles
#include "include/damper_Control.h"
#include "include/tempSense.h"
#include "include/Sprinkler.h"
#include "include/PWMTeensyTwoPlusPlus.h"
#include <stdlib.h>

//need to add fan.h later when I figure out how to generate a pwm

// Teensy 2.0++ 
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n)) // clock rate at 16Mhz
#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10))
void initializeGlobal(void);
void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);// still needs updating
void convert_by_division(uint16_t value, char *temp);
uint16_t convertAsciiToInt(char*temp, uint16_t size);
//global verbose adds lots of print statements
uint8_t verbose = 1;
uint8_t invalidInput = 0;
//Simple delay command
static void delay(uint16_t us)
{
	while(us) us--;
}


int main(void)
{
	//Start HeartBeat LED PB7
	PWMNew(2);//configures PB7
	PWMStart(2);
	PWMDuty(2, 128);//
	
	char buf[32];
	uint8_t n;

	CPU_PRESCALE(0);
	initializeGlobal();
	initializeHVACGarage();
	InitializeDamper();
	InitializeSprinkler();
	InitializeFans();
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
		send_str(PSTR("\r\nWelcome to Epic Cube Serial Control Version 0.0.1\r\n"));
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
	char itemBuf[2];
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
		//disable all other Devices and enable this
		//Disable Sprinkler
		//Enable Damper
		DAMPER_ENABLE_ON;
		
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
			Command = (uint8_t)buf[7] - (uint8_t)'0';//unsigned 8 bit makes command a value from 0 - 9
			//Command += ((uint8_t)buf[6] - (uint8_t)'0');//the higher digit either 1 or 0; 
			itemBuf[0] = buf[6];
			itemBuf[1] = buf[7];
			//Command = atoi(itemBuf);
			
			if(((uint8_t)buf[6] - (uint8_t)'0') == 1)
			{
				Command = Command + 10; // adds the correct value 0 - 15;
			}
			if(buf[8] == '0')// open
			{
				if(verbose)
				{
					/**
					char tempBuf[2];
					//itoa(Command, tempBuf,10);
					
					tempBuf[0] = (char)tempCommand + '0';//
					tempBuf[1] = (char)(Command - 10) + '0';
					send_str(PSTR("Starting to open Damper"));
					usb_serial_write(tempBuf, 2);
					//usb_serial_write(itemBuf, 2);
					send_str(PSTR("\r\n"));
					**/
				}
				OpenDamper(Command);
				if(verbose)
				{
					send_str(PSTR("Damper Open Complete.\r\n"));
				}
				return;
			}
			else // Close Damper
			{
			if(verbose)
				{				
					char tempBuf[2];
					//itoa(Command, tempBuf,10);
					
					tempBuf[0] = ((char)(Command)-10) + '0';//
					tempBuf[1] = (char)(Command) + '0';
					send_str(PSTR("Starting to close Damper"));
					usb_serial_write(tempBuf, 2);
					//usb_serial_write(itemBuf, 2);
					send_str(PSTR("\r\n"));
				}
				CloseDamper(Command);
				if(verbose)
				{
					send_str(PSTR("Damper Close Complete.\r\n"));
				}
				return;
			}
		}
	}
	if(buf[0] == 'H')// HVAC Section should be DONE debug
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
				if(GFanStatus == 1)
				{
					send_str(PSTR("4\r\n"));
					return;
				}
				
				send_str(PSTR("8\r\n"));// just the blower is on
				return;
			}
		}
		else // command
		{
			if(buf[4] == '1')// Thermo stat controlled
			{
				SetWHeat(0);
				SetYCool(0);
				SetGFan(0);
				SetHVAC(0);
				if(verbose)
				{	
					send_str(PSTR("Server controlled off\r\n"));
				}
			}
			else if(buf[4] == '2')// Blower on
			{
				SetWHeat(0);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				SetGFan(1);
				SetHVAC(1);
				if(verbose)
				{	
					send_str(PSTR("Server controlled Fan\r\n"));
				}
			}
			else if(buf[4] == '3')// heat and blower on
			{
				SetGFan(1);
				SetWHeat(1);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				SetHVAC(1);
				if(verbose)
				{	
					send_str(PSTR("Server controlled Fan and Heating\r\n"));
				}
			}
			else if(buf[4] == '4')// heat and blower on
			{
				SetGFan(1);
				SetWHeat(0);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				SetHVAC(1);
				if(verbose)
				{	
					send_str(PSTR("Server controlled Fan and Cooling\r\n"));
				}
			}
			else if(buf[4] == '5')// Server controlled off
			{
				SetGFan(0);
				SetWHeat(0);// may be overridden by Furnace
				SetYCool(0);// may be overridden by Furnace
				SetHVAC(1);
				if(verbose)
				{	
					send_str(PSTR("Server controlled OFF\r\n"));
				}
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
	
	if(buf[0] == 'T') // Temperature Section (DEBUG)
	{
			//disable all other Devices and enable this
		//disable Damper
		DAMPER_ENABLE_OFF;
		
		char tempStation[2];
		tempStation[0] = buf[4];
		tempStation[1] = buf[5];
		send_str(PSTR("\n\r"));
		
		uint16_t ADCResult = ReadTempSensor(tempStation);// query temperature section for info. this is a number from 0 to 1024
		char tempResultStr[4];
		//result is best left positive till C# level for easier conversion.
		convert_by_division(ADCResult, tempResultStr);
		usb_serial_write(tempResultStr, 4);
		send_str(PSTR("\r\n"));
		return;
	}
	else if(buf[0] == 'G')// Garage Section DONE
	{
		//this is only one thing. which is a pulse
		PulseGarage();
		return;
	}
	else if(buf[0] == 'S')// Sprinkler Section DONE
	{
		
		//disable Damper
		if(verbose)
		{
			send_str(PSTR("Sprinkler Section\r\n"));
		}
		//now we need to create a string, and find out
		//which format is 
		//Sprinkler##
		//        (#) = station
		//   	   (#) = Open/On(1) or Closed/Off(0)
		SprinklerCntrl(buf[9],buf[10]);
		return;
	}	
	else if(buf[0] == 'F')// Fan Section 
	{
		char CharStatus ='0';
		//Fan query Protocol fan 
		
		// FAN###
//	buf[3]	 (#) = Fan number(0,2,3,4)// only 4 fans.
//	buf[4]	  (#) = on(1)/off(0)
//	buf[5]	   (#) = Fan Speed(0-9)// represents a number from 0 - 255.
//					 if FAN#0x <- x is a don't care
// alternative Fan Status
//			FAN?##
//	buf[3]	  (?) = status request
//	buf[4]	   (#) = Fan number(0,2,3,4)// only 4 fans.
//	buf[5]		(#) = Query type 's'= speed and 'o'= on or off
		if(verbose)
			send_str(PSTR("Fan Section\r\n"));
		
		if(buf[3]== '?')//Check if status request
		{
			if(verbose)
				send_str(PSTR("\tFan Status section\r\n"));
		
			
			if(buf[5] == 's')// status request for speed 0 - 9
			{	
				if(verbose)
					send_str(PSTR("\t\tFan Speed Status Section\r\n"));
		
				CharStatus = FanSpeedStatus(buf[4]);// status of fan speed.
			}
			else if(buf[5] == 'o')
			{
				if(verbose)
					send_str(PSTR("\t\tFan On/Off Status Section\r\n"));
				CharStatus = FanStatus(buf[4]);// status whether on or off.
			}
			else
			{
				invalidInput = 1;// triggers request for printing out instructions.
			}
			if(verbose)
				send_str(PSTR("\tFan Print Status Section>>>\r\n"));
			char charPrintStatus[2];
			charPrintStatus[1] = CharStatus;
			charPrintStatus[2] = CharStatus;
			
			usb_serial_write(charPrintStatus, 2);//termination char
			send_str(PSTR("\r\n"));
			return;
		}
		else if(buf[3] < ('5') && buf[3] > ('0'-1))//valid input
		{
			if(verbose)
				send_str(PSTR("\tFan Command\r\n"));
		
			// can still use some valid input checking.
			if(buf[4] == '0')//turn off fan.
			{
				TurnFanOff(buf[3]);
			}
			//command
			else
			{
				SetFan(buf[3], buf[5]);//this will automatically turn on the PWM
			}
		}
		else
		{
			invalidInput = 1;
		}
		
	}
	if((buf[0] == '-' && buf[1] == 'h') || buf[0] == '?' || invalidInput)// Help Section
	{
		//help commands so write a bunch of things that help people.
		send_str(PSTR("\r\n***************************************\r\n"));
		send_str(PSTR("HELP I don't know what to do\r\n"));
		send_str(PSTR("List of available commands\r\n\r\n"));
		send_str(PSTR("-help:\t\tprovides this menu\r\n"));
		send_str(PSTR("Damper###:\tTurns on and off Damper\r\n"));
		send_str(PSTR("Damper?##:\tReplies whether damper is open or closed\r\n"));
		send_str(PSTR("Temp?##:\tgets the temperature value of the specific sensor\r\n"));
		send_str(PSTR("HVAC?:\t\tReturns HVAC status\r\n"));
		send_str(PSTR("HVAC#:\t\tSets the HVAC to a specific state\r\n"));
		send_str(PSTR("Garage:\t\tPulses the Garage open or closed\r\n"));
		send_str(PSTR("Sprinkler###:\tturns on/off section of sprinklers\r\n"));
		send_str(PSTR("Sprinkler?:\tGets status of sprinkler\r\n"));
		send_str(PSTR("***************************************\r\n"));
		send_str(PSTR("Examples (to-do)\r\n"));
		invalidInput = 0;
	}
		send_str(PSTR("\r\n"));
}

/**
*	initializes the variables used for Temperature Sense, Sprinkler, and Damper	
**/
void initializeGlobal(void)
{
	DEMUX_A_CONFIG;
	DEMUX_B_CONFIG;
	DEMUX_C_CONFIG;
	DEMUX_D_CONFIG;
	// initialize all of the idifferent pins
	DAMPER_ENABLE_CONFIG;
}
/**
*	This function was taken from 	Stackoverflow.com/questions/3694100/converting-to-ascii-in-c
*	It converts the int into a string.
**/
void convert_by_division(uint16_t value, char *temp)
{
	temp[0] = (value % 10) + '0';
	temp[1] = (value % 100) / 10 + '0';
	temp[2] = (value % 1000) / 100 + '0';
	temp[3] = (value % 1000) / 1000 + '0';
}

/**
*	This will convert the char string into a decimal. assuming correct formatting
 *if error it will return 0.
 */
uint16_t convertAsciiToInt(char*temp, uint16_t size)
{
	uint16_t result = 0;
	uint16_t i;
	uint16_t j;
	for(i = 0; i < size; i++)
    {
		uint16_t multiplier;
		uint16_t multipleOfTen;
		uint16_t tempResult;

		if(temp[i] > '9' || temp[i] < '0')
			return 0;
		
		multiplier = size - i - 1;
		// create a multiple of 10
		// 1,10,100,1000,10000.  for the digit
		multipleOfTen = 1;
		for(j = 0; j < multiplier; j++)
		{
			multipleOfTen *= 10;
		}

		// get uint16_t from 0 - 9;

		tempResult = temp[i] - '0';
		result += (tempResult * multipleOfTen);
	}
  return result;
}

