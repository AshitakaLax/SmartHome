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

#include "../include/GlobalVar.h"
#include "../include/tempSense.h"


static uint8_t aref = (1<<REFS0);// default reference

/*
*	Reads the sensor for this teensy board
*   sensor is from 0 to 63.
*   0-31 for board 1 and 
*	32 to 63 for board 2(if connected)
* 	if not connected returns max or min(currently unknown)
*/
uint16_t ReadTempSensor(uint8_t sensor)
{
	//set the channel demux as such
	//read from adc on such.
	// Goes from C B A
	// their are 8 sections with 8 sensors on each
	//there are up to 32 sensors per board
	//and there are 8 sections
	uint8_t subSensor = sensor % 8;//find the sensor within the section.
	
	uint8_t section = sensor / 8;// find which section it is in.
	
	uint8_t C = subSensor / 4;
	subSensor = subSensor % 4;
	
	uint8_t B = subSensor / 2;
	subSensor = subSensor % 2;
	
	uint8_t A = subSensor;
	char tempBuf[7];
	//itoa(Command, tempBuf,10);
		
	tempBuf[0] = (char)A + '0';
	tempBuf[1] = '\t';
	tempBuf[2] = (char)B + '0';
	tempBuf[3] = '\t';
	tempBuf[4] = (char)C + '0';
	tempBuf[5] = '\t';
	tempBuf[6] = (char)section + '0';
	send_str(PSTR("Test Outputs of Temp ABC and section\r\n"));
	usb_serial_write(tempBuf, 7);
	send_str(PSTR("\r\n"));
	
	if(C == 1)
	{
		DEMUX_C_ON;
	}
	else
	{
		DEMUX_C_OFF;
	}
	if(B == 1)
	{
		DEMUX_B_ON;
	}
	else
	{
		DEMUX_B_OFF;
	}
	if(A == 1)
	{
		DEMUX_A_ON;
	}
	else
	{
		DEMUX_A_OFF;
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
uint16_t ReadADC(uint8_t pin)// may give errors if so replace with unsigned char.
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