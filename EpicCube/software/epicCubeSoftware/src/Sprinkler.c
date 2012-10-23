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

/**
*	this section requires that the section is 6 or less.
* returns 1 if there is an error, and return 0 if not.
* timing will be handled by the server/software
*/
uint8_t SprinklerCntrl( uint8_t section)
{
	if(section > 6)
		return 1;
	// if there are 6 stations
	// if we want 1
	// A0 needs to be 1
	// and A1 and A2 need to be 0
	//  demux corresponds chip 74AC138
	// A0 == A
	// A1 == B
	// A2 == C
	uint8_t A2 = section / 4;
	section = section % 4;
	
	uint8_t A1 = section / 2;
	section = section % 4;
	
	uint8_t A0 = section / 1;

	if(A2==1)
	{
		DEMUX_C_ON;
	}
	else
	{
		DEMUX_C_OFF;
	}
	if(A1==1)
	{
		DEMUX_B_ON;
	}
	else
	{
		DEMUX_B_OFF;
	}
	if(A0==1)
	{
		DEMUX_A_ON;
	}
	else
	{
		DEMUX_A_OFF;
	}
}
