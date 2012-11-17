/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the different dampers it will need to store in memory, 
* all the correct values.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../include/GlobalVar.h"
#include "../include/sprinkler.h"



uint8_t sprinklerVerbose = 1;

void InitializeSprinkler( void)
{
	SPRINKLERDEMUX_C_CONFIG;
	SPRINKLERDEMUX_B_CONFIG;
	SPRINKLERDEMUX_A_CONFIG;
	SPRINKLERDEMUX_A_ON;//default all on
	SPRINKLERDEMUX_B_ON;
	SPRINKLERDEMUX_C_ON;
}

/**
*	this section requires that the section is 6 or less.
* returns 1 if there is an error, and return 0 if not.
* timing will be handled by the server/software
*/
void SprinklerCntrl( char section, char isOn)
{
	if(sprinklerVerbose)
	{
		send_str(PSTR("Entered SprinklerCntrl\r\n"));
	}
	if(section > '5')
	{
	
		if(sprinklerVerbose)
		{
			send_str(PSTR("Section is > 5\r\n"));
		}
		// if it is greater than 6 then
		// it will idle and do nothing.
		SPRINKLERDEMUX_C_ON;
		SPRINKLERDEMUX_B_ON;
		SPRINKLERDEMUX_A_ON;
		return ;
	}
	
	if(section == '0') // Section 0
	{
	
		if(sprinklerVerbose)
		{
			send_str(PSTR("Section == \'0\'\r\n"));
		}
		if(isOn == '1') // if true turn on
		{
		
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 1\r\n"));
			}
			SPRINKLERDEMUX_C_OFF;
			SPRINKLERDEMUX_B_OFF;
			SPRINKLERDEMUX_A_OFF;
			return;
		}
		else // else turn off
		{
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	else if(section == '1') // Section 1
	{
			if(sprinklerVerbose)
			{
				send_str(PSTR("Enter Section 1\r\n"));
			}
		if(isOn == '1') // if true turn on
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 1\r\n"));
			}
			SPRINKLERDEMUX_A_ON;
			SPRINKLERDEMUX_B_OFF;	
			SPRINKLERDEMUX_C_OFF;
			return;
		}
		else // else turn off
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 0\r\n"));
			}
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	else if(section == '2') // Section 2
	{
			if(sprinklerVerbose)
			{
				send_str(PSTR("Section 2\r\n"));
			}
		if(isOn == '1') // if true turn on
		{
			SPRINKLERDEMUX_A_OFF;
			SPRINKLERDEMUX_B_ON;	
			SPRINKLERDEMUX_C_OFF;
			return;
		}
		else // else turn off
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 0\r\n"));
			}
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	else if(section == '3') // Section 3
	{

		if(sprinklerVerbose)
		{
			send_str(PSTR("Section 3\r\n"));
		}
		if(isOn == '1') // if true turn on
		{
			SPRINKLERDEMUX_A_ON;
			SPRINKLERDEMUX_B_ON;	
			SPRINKLERDEMUX_C_OFF;
			return;
		}
		else // else turn off
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 0\r\n"));
			}
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	else if(section == '4') // Section 4
	{
	
		if(sprinklerVerbose)
		{
			send_str(PSTR("Section 4\r\n"));
		}
		if(isOn == '1') // if true turn on
		{
			SPRINKLERDEMUX_A_OFF;
			SPRINKLERDEMUX_B_OFF;	
			SPRINKLERDEMUX_C_ON;
			return;
		}
		else // else turn off
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 0\r\n"));
			}
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	else if(section == '5') // Section 5
	{
			if(sprinklerVerbose)
			{
				send_str(PSTR("Section 5\r\n"));
			}
		if(isOn == '1') // if true turn on
		{
			SPRINKLERDEMUX_A_ON;
			SPRINKLERDEMUX_B_OFF;	
			SPRINKLERDEMUX_C_ON;
			return;
		}
		else // else turn off
		{
			if(sprinklerVerbose)
			{
				send_str(PSTR("isOn == 0\r\n"));
			}
			SPRINKLERDEMUX_C_ON;
			SPRINKLERDEMUX_B_ON;
			SPRINKLERDEMUX_A_ON;
			return;
		}
	}
	
}
