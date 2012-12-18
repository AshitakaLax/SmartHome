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
#include "sampling.h"
#include "include/tempSense.h"
#include "include/Sprinkler.h"
#include "include/analog.h"
#include "include/PWMTeensyTwoPlusPlus.h"
#include <stdlib.h>

//need to add fan.h later when I figure out how to generate a pwm

// Teensy 2.0++ 
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n)) // clock rate at 16Mhz
#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10)) //I think this is unused.
void initializeGlobal(void);
void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);// still needs updating
void convert_by_division(uint16_t value, char *temp);
uint16_t convertAsciiToInt(char*temp, uint16_t size);
uint16_t ReadADCTemp(uint8_t pin);
void toggleVerbose(void);

static uint8_t aref = (1<<REFS0);// default reference
//global verbose adds lots of print statements
uint8_t verbose = 0;
uint8_t invalidInput = 0;
//Simple delay command
/**
static void (uint16_t us)
{
	while(us) us--;
}
**/

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
	InitializeTemperature();
	// still need to initialize fans

	usb_init();
	while(!usb_configured());
	_delay_ms(1000);
	//enter infinite loop
	while (1) 
	{
		while (!(usb_serial_get_control() & USB_SERIAL_DTR))// caught error with ";"
		usb_serial_flush_input();

		//if time permits
		   // +--------+
		  // /        /|
		 // /        / |
		// +--------+  |
		// |        |  |
		// |        |  +
		// |        | /
		// |        |/
		// +--------+
		
		send_str(PSTR("\r\n***************************************"));
		send_str(PSTR("\r\nWelcome to Epic Cube Serial Control Version 0.1.1\r\n"));
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
			//Command = buf[8] - '0';// convert ascii value to decimal
			Command = (uint8_t)buf[8] - (uint8_t)'0';//unsigned 8 bit makes command a value from 0 - 9
			char val = CheckDamper(Command);
			usb_serial_putchar(val);
			//send_str((char*)(Status + '0'));// returns the status
			//send_str(PSTR("\r\n"));
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
				if(verbose==1)
				{
					send_str(PSTR("10 or greater \n\r"));
				}
				Command = Command + 10; // adds the correct value 0 - 15;
			}
			if(buf[8] == '0')// open
			{
				if(verbose)
				{
					send_str(PSTR("Starting to open Damper"));
					
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
						send_str(PSTR("2"));
						return;
					}
					else if(ReadYCool() == 1)// and the cooler is on
					{
						send_str(PSTR("3"));
						return;
					}
					else
					{
						send_str(PSTR("1"));//just the blower is on.
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
					send_str(PSTR("5"));// blower and heater.
					return;
				}
				if(YCoolStatus == 1)// blower and Cooler
				{
					if(verbose)
						send_str(PSTR("6 Blower and Cooler are on"));
					send_str(PSTR("6"));
					return;
				}
				if(GFanStatus == 1)
				{
					send_str(PSTR("4\r\n"));// server controlled blower is on.
					return;
				}
				//no need to have a check here.
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
					send_str(PSTR("Thermostat controlled\r\n"));
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
				SetYCool(1);// may be overridden by Furnace
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
	/** Documentation change from Project description
	* instead of Attic and Basement, we are gong with just Temperature
	*  sensors, and we will have to decide which temperature sensors are where
	*  in the server code.
	*  this is a Sts only so it is Temp## from 0 - 63 for ##
	*  example Temp16 will read from temperature sensor 16 and return the
	*  value in C
	**/
	if(buf[0] == 'T') // Temperature Section (DEBUG)
	{
		for(int i = 0; i < 30; i++)
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
		}
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
	else if(buf[0] == 'A')// AllFanStatus Section 
	{
		AllFanStatus();
	}	
	else if(buf[0] == 'V')// Verbose Section 
	{
		if(verbose)//verbose is 1
			verbose = 0;
		else
			verbose = 1;
		DampVerbose(verbose);
	}
	else if(buf[0] == 'I')//test input
	{
		//syntax Input0 or Input1
		//This section is to test input values repeatedly
		//on the following inputs PD1 and PD2
		// to do this we will test our macros
		
		send_str(PSTR("PD1 or pin 4\r\n"));
		send_str(PSTR("PD2 or pin 5\r\n"));
		if(buf[5] == '0')//test open
		{
			DDRD &= ~(1<<2);
			uint8_t val;
			val = PIND & (1<<2);
			if(val != 0)
			{
				send_str(PSTR("OpenBUSHIGH == 1"));
			}
			else
			{
				send_str(PSTR("OpenBUSHIGH == 0"));
			}
			usb_serial_putchar((val+'0'));
		}
		else
		{
			if(ISBUTTONCLOSEBUSHIGH != 0)
			{
				send_str(PSTR("CloseBUSHIGH == 1"));
			}
			else
			{
				send_str(PSTR("CloseBUSHIGH == 0"));
			}
		}
		send_str(PSTR("\r\n"));
	}
	else if(buf[0] == 'W')//test input
	{
		
	}
	else if(buf[0] == 'Z')
	{
		uint16_t zTemp0 = ReadADCTemp(0);
		uint16_t zTemp1 = ReadADCTemp(1);
		uint16_t zTemp2 = ReadADCTemp(2);
		uint16_t zTemp3 = ReadADCTemp(3);
		char tempResultStr0[4];
		char tempResultStr1[4];
		char tempResultStr2[4];
		char tempResultStr3[4];
		//zTemp0 = 1024;
		//zTemp1 = 523;
		//zTemp2 = 23;
		//zTemp3 = 5;
		char* tempStringZ;
		
		//		itoa(zTemp0, tempStringZ, 10);
		
		//result is best left positive till C# level for easier conversion.
		convert_by_division(zTemp0, tempResultStr0);
		convert_by_division(zTemp1, tempResultStr1);
		convert_by_division(zTemp2, tempResultStr2);
		convert_by_division(zTemp3, tempResultStr3);
		
		//1023
//		zTemp0 = 389;
	
		uint8_t z10 = zTemp1 / 1024;//  D = 1bin or 1 dec
	uint8_t temp = zTemp1 % 1024;// temp = 10bin and 2 dec
	
	uint8_t z9 = temp / 512;//  D = 1bin or 1 dec
	temp = temp % 512;// temp = 10bin and 2 dec
	
	uint8_t z8 = temp / 256;//  D = 1bin or 1 dec
	temp = temp % 256;// temp = 10bin and 2 dec
	
	uint8_t z7 = temp / 128;//  D = 1bin or 1 dec
	temp = temp % 128;// temp = 10bin and 2 dec
	
	uint8_t z6 = temp / 64;//  D = 1bin or 1 dec
	temp = temp % 64;// temp = 10bin and 2 dec
	
	uint8_t z5 = temp / 32;//  D = 1bin or 1 dec
	temp = temp % 32;// temp = 10bin and 2 dec
	
	uint8_t z4 = temp / 16;//  D = 1bin or 1 dec
	temp = temp % 16;// temp = 10bin and 2 dec
	
	uint8_t z3 = temp / 8;//  D = 1bin or 1 dec
	temp = temp % 8;// temp = 10bin and 2 dec
	
	uint8_t z2 = temp / 4;// C = (2)/4 = 0
	temp = temp % 4;// temp = (2)%4 = 2
	
	uint8_t z1 = temp / 2;// 1
	temp = temp % 2;// 0
	
	uint8_t z0 = temp; 
	
	char tempBuf[11];
		//itoa(Command, tempBuf,10);
					
		tempBuf[0] = (char)z10 + '0';
		tempBuf[1] = (char)z9 + '0';
		tempBuf[2] = (char)z8 + '0';
		tempBuf[3] = (char)z7 + '0';
		
		tempBuf[4] = (char)z6 + '0';
		tempBuf[5] = (char)z5 + '0';
		tempBuf[6] = (char)z4 + '0';
		tempBuf[7] = (char)z3 + '0';
		
		tempBuf[8] = (char)z2 + '0';
		tempBuf[9] = (char)z1 + '0';
		tempBuf[10] = (char)z0 + '0';
		
		
		//printf("number %i", zTemp0);
		send_str(PSTR("\r\nTemp0 binary\r\n"));
		usb_serial_write(tempBuf, 11);
		send_str(PSTR("\r\nTemp0\r\n"));
		usb_serial_write(tempResultStr0, 4);
		send_str(PSTR("Temp1\r\n"));
		usb_serial_write(tempResultStr1, 4);
		send_str(PSTR("Temp2\r\n"));
		usb_serial_write(tempResultStr2, 4);
		send_str(PSTR("Temp3\r\n"));
		usb_serial_write(tempResultStr3, 4);
		
		send_str(PSTR("Temp4\r\n"));
	}
	else if(buf[0] == 'M')//test Motor
	{
		//syntax Motor
		//pins used are 
		//PB6 - Enable Enabled LOW = pulled low to enable
		//PB5 -	Step
		//PB4 - Direction
		
		send_str(PSTR("PB6 enable\r\n"));
		send_str(PSTR("PB5 Step\r\n"));
		send_str(PSTR("PB4 Direction\r\n"));
		
		MOTORDIRLOW;
		MOTORENABLELOW;//ENABLE
		
		for(uint16_t stepcount = 0; stepcount < 1000; stepcount++)
		{
			MOTORSTEPHIGH;
			send_str(PSTR("Step High\r\n"));
			_delay_ms(15);
			MOTORSTEPLOW;
			send_str(PSTR("Step Low\r\n"));
			_delay_ms(15);
			if(stepcount == 500)
			{
				MOTORDIRHIGH;
			}
		}
		MOTORENABLEHIGH;
		send_str(PSTR("disabled motor\r\n"));
	}
	if((buf[0] == '-' && buf[1] == 'h') || buf[0] == '?' || invalidInput)// Help Section
	{
		// -help Damper
		if(buf[6] == 'D' && num > 5)
		{
			send_str(PSTR("Damper Help\r\n"));
			send_str(PSTR("Damper?## - issues a status request to damper (00-15)\r\n"));
			send_str(PSTR("returns either 0 for open and 1 for closed.\r\n\r\n"));
			send_str(PSTR("Damper### - issues a command to damper (00-15)\r\n"));
			send_str(PSTR("     (##) - represents (00-15)\r\n"));
			send_str(PSTR("       (#) - represents 0 for open and 1 for closed\r\n\r\n"));
			send_str(PSTR("Example1:\r\n"));
			send_str(PSTR("\tCommand: Damper?12\r\n"));
			send_str(PSTR("\tReturns: 0 for open and 1 for closed,depending on Damper 12's state\r\n\r\n"));
			send_str(PSTR("Example2:\r\n"));
			send_str(PSTR("\tCommand: Damper141\r\n"));
			send_str(PSTR("\tReturns: done, when damper 14 is closed\r\n"));
		}
		// -help HVAC
		else if(buf[6] == 'H')
		{
			send_str(PSTR("HVAC Help\r\n"));
			send_str(PSTR("HVAC? - issues a status request to HVAC\r\n"));
			send_str(PSTR("returns a number from 0 to 7 depending on the HVAC status.\r\n"));
			send_str(PSTR("status 1: furnace Controller Blower on\r\n"));
			send_str(PSTR("status 2: furnace Controller Blower and heating are on\r\n"));
			send_str(PSTR("status 3: furnace Controller Blower and AC are onon\r\n"));
			send_str(PSTR("status 4: Server Controller Blower on\r\n"));
			send_str(PSTR("status 5: Server Controller Blower on\r\n"));
			send_str(PSTR("status 6: Server Controller Blower on\r\n"));
			send_str(PSTR("status 7: all off\r\n"));// there is a server controlled off
			send_str(PSTR("HVAC# - issues a command to HVAC\r\n"));
			send_str(PSTR("   (#) - represents (1-5)\r\n"));
			send_str(PSTR("Command 1: Thermostat controlled\r\n"));
			send_str(PSTR("Command 2: Server Controlled Blower on\r\n"));
			send_str(PSTR("Command 3: Server Controlled Blower and heating are on\r\n"));
			send_str(PSTR("Command 4: Server Controlled Blower and AC are on\r\n"));
			send_str(PSTR("Command 5: Thermostat controlled\r\n"));
			send_str(PSTR("Example1:\r\n"));
			send_str(PSTR("\tCommand: HVAC?\r\n"));
			send_str(PSTR("\tReturns: 3 when The AC is On\r\n\r\n"));
			send_str(PSTR("Example2:\r\n"));
			send_str(PSTR("\tCommand: HVAC3\r\n"));
			send_str(PSTR("\tReturns: done, when HVAC blower and Heating are done\r\n"));
		}		
		else if(buf[6] == 'G')// -help 
		{
			send_str(PSTR("Garage Help\r\n"));
			send_str(PSTR("Garage - Sends a Pulse to the Garage door\r\n"));
			send_str(PSTR("Example1:\r\n"));
			send_str(PSTR("\tCommand: Garage\r\n"));
			send_str(PSTR("\tReturns: done, when pulse is done\r\n\r\n"));
		}		
		else if(buf[6] == 'T')// -help Temp
		{
			send_str(PSTR("Temperature Help\r\n"));
			send_str(PSTR("TEMP## - issues a status request to TEMP\r\n"));
			send_str(PSTR("   (##) - Represents a number from 0 - 63\r\n"));
			send_str(PSTR("returns a number from 0 to 1024 as the ADC value\r\n"));
		}		
		else if(buf[6] == 'S')// -help Sprinkler
		{
			send_str(PSTR("Sprinkler Help\r\n"));
			send_str(PSTR("Sprinkler## - issues a status request to Sprinkler\r\n"));
			send_str(PSTR("   	   (#) - Represents a the station as a number from 0 - 5\r\n"));
			send_str(PSTR("   	    (#) - Represents 0 or 1, 1 = Open/on and 0 for Closed or Off \r\n"));
			send_str(PSTR("Doesn't return anything, due to station remaining on for 10+ mins\r\n"));
			send_str(PSTR("Server is in charge of when it is on, and off.\r\n"));
			send_str(PSTR("Example1:\r\n"));
			send_str(PSTR("\tCommand: Sprinkler31 This will turn on(1) the station(3) \r\n"));
		}	
		else if(buf[6] == 'F')// -help FAN
		{
			send_str(PSTR("Fan or FAN Help\r\n"));
			send_str(PSTR("To prevent damage to circuit, we have software protection of the fans\r\n"));
			send_str(PSTR("This means you can only control one fan at a time. \r\n\r\n"));
			send_str(PSTR("Fan?## - issues a status request about a specific Fan\r\n"));
			send_str(PSTR("   (#) - Represents a the Specific fan as a number: 0,2,3,4\r\n"));
			send_str(PSTR("    (#) - Represents 's' for fan Speed or 'o' for fan on or off\r\n"));
			send_str(PSTR("a Fan 's'speed query will return a number from 0 - 9 based on the duty cycle\r\n"));
			send_str(PSTR("a Fan 'o'on/off query will return 1 if it is on and 0 if it is off.\r\n"));
			send_str(PSTR("Fan### - issues a command to a specific Fan\r\n"));
			send_str(PSTR("  (#) - Represents a the Specific fan as a number: 0,2,3,4\r\n"));
			send_str(PSTR("   (#) - Represents a number 0 or 1, for off or on respectively\r\n"));
			send_str(PSTR("    (#) - Represents a number from 0 to 9 for the speed. 0 slow and 9 fastest\r\n"));
			send_str(PSTR("Don't care when setting to off Fan#0x<-don't care\r\n"));
			send_str(PSTR("Example1:\r\n"));
			send_str(PSTR("\tStatus: Fan?2s This will turn the speed (0-9) of fan 2.\r\n"));
			send_str(PSTR("Example2:\r\n"));
			send_str(PSTR("\tStatus: Fan?4o This will turn either 0 or 1 whether Fan 4 is on or off.\r\n"));
			send_str(PSTR("Example3:\r\n"));
			send_str(PSTR("\tCommand: Fan40x This will turn Fan 4 off and enabling.\r\n"));
			send_str(PSTR("Example4:\r\n"));
			send_str(PSTR("\tCommand: Fan015 This will turn Fan 4 on(if not on)(and/or) change the speed to 5.\r\n"));
		}
		else if(buf[6] == 'A')// -help AllFAN
		{
			send_str(PSTR("AllFan Help\r\n"));
			send_str(PSTR("Returns the status of all the Fans\r\n"));
			send_str(PSTR("Format: \"FanNumber:on/off:speed,\"\r\n"));
			send_str(PSTR("Example: 0:0:0,2:0:0,3:1:7,4:0:0\r\n"));
			send_str(PSTR("The result is that fan 3 is on at speed 7\r\n"));
			
		}
		else if(buf[6] == 'V')// -help Verbose
		{
			send_str(PSTR("Verbose Help\r\n"));
			send_str(PSTR("Changes whether in Verbose Mode or not\r\n"));
		}
		else{
		//help commands so write a bunch of things that help people.
		send_str(PSTR("\r\n***************************************\r\n"));
		send_str(PSTR("HELP when you say to yourself,\"I don't know what to do\"\r\n"));
		send_str(PSTR("List of available commands\r\n\r\n"));
		send_str(PSTR("-help:\t\tprovides this menu\r\n"));
		send_str(PSTR("Verbose:\t\This toggles the Verbose state\r\n"));
		send_str(PSTR("Damper###:\tTurns on and off Damper\r\n"));
		send_str(PSTR("Damper?##:\tReplies whether damper is open or closed\r\n"));
		send_str(PSTR("Temp##:\tgets the temperature value of the specific sensor\r\n"));
		send_str(PSTR("HVAC?:\t\tReturns HVAC status\r\n"));
		send_str(PSTR("HVAC#:\t\tSets the HVAC to a specific state\r\n"));
		send_str(PSTR("Garage:\t\tPulses the Garage open or closed\r\n"));
		send_str(PSTR("Sprinkler##:\tturns on/off section of sprinklers\r\n"));
		send_str(PSTR("FAN?##:\treturns either speed of a fan or if it is on.\r\n"));
		send_str(PSTR("FAN###:\teither turns on or changes the specfic fan speed.\r\n"));
		send_str(PSTR("AllFAN:\treturns status of all the fans\r\n"));
		send_str(PSTR("***************************************\r\n"));
		send_str(PSTR("for more specific help use: \"-help <command>\r\n"));
		send_str(PSTR("<command> = Damper, Temp, HVAC, Garage, Sprinkler, Fan, AllFAN\r\n"));
		send_str(PSTR("example: \"-help HVAC\"\r\n"));
		invalidInput = 0;
		}
		//buf[6] = '0';//prevent repeat help commands
		
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
	temp[3] = (value % 10) + '0';
	temp[2] = (value % 100) / 10 + '0';
	temp[1] = (value % 1000) / 100 + '0';
	temp[0] = (value % 1000) / 1000 + '0';
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
uint16_t ReadADCTemp(uint8_t pin)// may give errors if so replace with unsigned char.
{
	//simple read on the adc analog pin.
	
	uint8_t low;

	ADCSRA = (1<<ADEN) | ADC_PRESCALER;		// enable ADC
	ADCSRB = (1<<ADHSM) | (pin & 0x20);		// high speed mode
	ADMUX = aref | (pin & 0x1F);			// configure mux input
	ADCSRA = (1<<ADEN) | ADC_PRESCALER | (1<<ADSC);	// start the conversion
	while (ADCSRA & (1<<ADSC)) ;			// wait for result
	low = ADCL;					// must read LSB first
	return ((ADCH << 8) | low);			// must read MSB only once!
}
void toggleVerbose(void)
{
	//toggle all of the verbose files
	if(verbose)//verbose is 1
		verbose = 0;
	else
		verbose = 1;
	DampVerbose(verbose);
	
	
}