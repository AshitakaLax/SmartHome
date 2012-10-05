/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the different dampers it will need to store in memory, 
* all the correct values.
* 
* Steps to setup of a fan PWM signal
* 1. set pin as output(OCx)
* 2. Select PWM mode of timer
* 3. Set appropriate prescaler divider
* 4. Set Compare Output Mode to Clear or Set on compare match
* 5. Write Duty cycle value to PWM(OCRx)
*/

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../include/tempSense.h"


static uint8_t aref = (1<<REFS0);// default reference
void initializeTempSense()
{
	// configure all pins
	CHANNEL_DEMUX_A_CONFIG;
	CHANNEL_DEMUX_B_CONFIG;
	CHANNEL_DEMUX_C_CONFIG;
	CHANNEL_DEMUX_EN_CONFIG;
	CHANNEL_DEMUX_EN_OFF;
	
}

/*
*	Reads the sensor for this teensy board
*   sensor is from 0 to 63.
*   0-31 for board 1 and 
*	32 to 63 for board 2(if connected)
* 	if not connected returns max or min(currently unknown)
*/
uint8_t ReadTempSensor(uint8_t sensor)
{
	//set the channel demux as such
	//read from adc on such.
	// Goes from C B A
	// their are 8 sections with 8 sensors on each
	uint8_t subSensor = sensor % 8;//find the sensor within the section.
	uint8_t section = sensor / 8;// find which section it is in.
	
	uint8_t C = subSensor / 4;
	subSensor = subSensor % 4;
	
	uint8_t B = subSensor / 2;
	subSensor = subSensor % 4;
	
	uint8_t A = subSensor / 1;
	if(C == 1)
	{
		CHANNEL_DEMUX_C_ON;
	}
	else
	{
		CHANNEL_DEMUX_C_OFF;
	}
	if(B == 1)
	{
		CHANNEL_DEMUX_B_ON;
	}
	else
	{
		CHANNEL_DEMUX_B_OFF;
	}
	if(A == 1)
	{
		CHANNEL_DEMUX_A_ON;
	}
	else
	{
		CHANNEL_DEMUX_A_OFF;
	}
	if(section > 7)
	{
		//return error
	}
	
	return ReadADC(section);
	
}
/**
*	Reads the ADC value on one of the ADC pins
*	0 - 7 pins.
*/
uint8_t ReadADC(uint8_t pin)// may give errors if so replace with unsigned char.
{
	//simple read on the adc analog pin.
	
	uint8_t low;

	ADCSRA = (1<<ADEN) | ADC_PRESCALER;		// enable ADC
	ADCSRB = (1<<ADHSM) | (pin & 0x20);		// high speed mode
	ADMUX = aref | (pin & 0x1F);			// configure mux input
	ADCSRA = (1<<ADEN) | ADC_PRESCALER | (1<<ADSC);	// start the conversion
	while (ADCSRA & (1<<ADSC)) ;			// wait for result
	low = ADCL;					// must read LSB first
	return (ADCH << 8) | low;			// must read MSB only once!
}