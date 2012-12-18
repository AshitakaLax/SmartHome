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
#include "../sampling.h"


#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10))

static uint8_t aref = (1<<REFS0);// default reference

void InitializeTemperature()
{
		//initialize my io pins to input.
		
//		analogenable;// this enables the analog input pins.
	adc_start(ADC_MUX_PIN_F0, ADC_REF_POWER);
	_delay_ms(1000);
	adc_read();
}
	
/*
*	Reads the sensor for this teensy board
*   sensor is from 0 to 63.
*   0-31 for board 1 and 
*	32 to 63 for board 2(if connected)
* 	if not connected returns max or min(currently unknown)
* Sensor Number must be 2 digits long
*/
uint16_t ReadTempSensor(char * SensorNumber)
{
		if(SensorNumber[0] == '0')//sensors 0 through 9
		{
			if(SensorNumber[1] == '0')
			{
				//case 1 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 2 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 3 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 3 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 4 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF0(pin 33)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 5 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF0(pin 33)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 6 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF0(pin 33)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 7 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF0(pin 33)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(0);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 8 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 9 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF0(pin 33)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(1);
			}
		}		
		else if(SensorNumber[0] == '1')//sensors 10 through 19
		{
			if(SensorNumber[1] == '0')
			{
				//case 10 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF1(pin 34)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 11 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF1(pin 34)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 12 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF1(pin 34)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 13 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF1(pin 34)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 14 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF1(pin 34)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 15 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF1(pin 34)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(1);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 16 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF2(pin 35)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 17 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF2(pin 35)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 18 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF2(pin 35)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 19 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF2(pin 35)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(2);
			}
		}		
		else if(SensorNumber[0] == '2')//sensors 20 through 29
		{
			if(SensorNumber[1] == '0')
			{
				//case 20 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF2(pin 35)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 21 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF2(pin 35)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 22 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF2(pin 35)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 23 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF2(pin 35)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(2);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 24 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF3(pin 36)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 25 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF3(pin 36)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 26 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF3(pin 36)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 27 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF3(pin 36)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 28 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF3(pin 36)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 29 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF3(pin 36)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(3);
			}
		}		
		else if(SensorNumber[0] == '3')//sensors 30 through 39
		{
			if(SensorNumber[1] == '0')
			{
				//case 30 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF3(pin 36)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 31 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF3(pin 36)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(3);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 32 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF4(pin 37)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 33 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF4(pin 37)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 34 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF4(pin 37)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 35 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF4(pin 37)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 36 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF4(pin 37)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 37 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF4(pin 37)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 38 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF4(pin 37)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(4);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 39 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF4(pin 37)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(4);
			}
		}		
		else if(SensorNumber[0] == '4')//sensors 40 through 49
		{
			if(SensorNumber[1] == '0')
			{
				//case 40 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF5(pin 38)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 41 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF5(pin 38)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 42 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF5(pin 38)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 43 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF5(pin 38)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 44 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF5(pin 38)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 45 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF5(pin 38)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 46 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF5(pin 38)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 47 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF5(pin 38)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(5);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 48 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF6(pin 39)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 49 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF6(pin 39)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(6);
			}
		}		
		else if(SensorNumber[0] == '5')//sensors 50 through 59
		{
			if(SensorNumber[1] == '0')
			{
				//case 50 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF6(pin 39)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 51 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF6(pin 39)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 52 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF6(pin 39)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 53 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF6(pin 39)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '4')
			{
				//case 54 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF6(pin 39)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '5')
			{
				//case 55 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF6(pin 39)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(6);
			}
			else if(SensorNumber[1] == '6')
			{
				//case 56 read adc 
				// set A = 0
				// set B = 0
				// set C = 0
				// Read PF7(pin 40)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '7')
			{
				//case 57 read adc 
				// set A = 1
				// set B = 0
				// set C = 0
				// Read PF7(pin 40)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_OFF;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '8')
			{
				//case 58 read adc 
				// set A = 0
				// set B = 1
				// set C = 0
				// Read PF7(pin 40)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '9')
			{
				//case 59 read adc 
				// set A = 1
				// set B = 1
				// set C = 0
				// Read PF7(pin 40)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_OFF;
				return ReadADC(7);
			}
		}		
		else if(SensorNumber[0] == '6')//sensors 60 through 63
		{
			if(SensorNumber[1] == '0')
			{
				//case 60 read adc 
				// set A = 0
				// set B = 0
				// set C = 1
				// Read PF7(pin 40)
				DEMUX_A_OFF;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '1')
			{
				//case 61 read adc 
				// set A = 1
				// set B = 0
				// set C = 1
				// Read PF7(pin 40)
				DEMUX_A_ON;
				DEMUX_B_OFF;
				DEMUX_C_ON;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '2')
			{
				//case 62 read adc 
				// set A = 0
				// set B = 1
				// set C = 1
				// Read PF7(pin 40)
				DEMUX_A_OFF;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(7);
			}
			else if(SensorNumber[1] == '3')
			{
				//case 63 read adc 
				// set A = 1
				// set B = 1
				// set C = 1
				// Read PF7(pin 40)
				DEMUX_A_ON;
				DEMUX_B_ON;
				DEMUX_C_ON;
				return ReadADC(7);
			}
		}
	
		send_str(PSTR("ERRORTEMP\n\r"));
	return 0;
}

void analogReference(uint8_t mode)
{
	aref = mode & 0xC0;
}
uint16_t adcToKelvin(uint16_t adcRaw)
{
	uint16_t code = adcRaw;
	int celsius;
  if (code <= 289) {
    celsius = 5 + (code - 289) / 9.82;
  }
  if (code > 289 && code <= 342) {
    celsius = 10 + (code - 342) / 10.60;
  }
  if (code > 342 && code <= 398) {
    celsius = 15 + (code - 398) / 11.12;
  }
  if (code > 398 && code <= 455) {
    celsius = 20 + (code - 455) / 11.36;
  }
  if (code > 455 && code <= 512) {
    celsius = 25 + (code - 512) / 11.32;
  }
  if (code > 512 && code <= 566) {
    celsius = 30 + (code - 566) / 11.00;
  }
  if (code > 566 && code <= 619) {
    celsius = 35 + (code - 619) / 10.44;
  }
  if (code > 619 && code <= 667) {
    celsius = 40 + (code - 667) / 9.73;
  }
  if (code > 667) {
    celsius = 45 + (code - 712) / 8.90;
  }
//  fahrenheit = celsius * 1.8 + 32;
	return celsius + 273;// return kelvin
  }
/**
*	Reads the ADC value on one of the ADC pins
*	0 - 7 pins. this returns a range from 0 to 1024
*/
uint16_t ReadADC(uint8_t pin)// may give errors if so replace with unsigned char.
{

	uint16_t temperature = 0;
	if(pin == 0)
		adc_start(ADC_MUX_PIN_F0, ADC_REF_POWER);
	else if(pin == 1)
		adc_start(ADC_MUX_PIN_F1, ADC_REF_POWER);
	else if(pin == 2)
		adc_start(ADC_MUX_PIN_F2, ADC_REF_POWER);
	else if(pin == 3)
		adc_start(ADC_MUX_PIN_F3, ADC_REF_POWER);
	else if(pin == 4)
		adc_start(ADC_MUX_PIN_F4, ADC_REF_POWER);
	else if(pin == 5)
		adc_start(ADC_MUX_PIN_F5, ADC_REF_POWER);
	else if(pin == 6)
		adc_start(ADC_MUX_PIN_F6, ADC_REF_POWER);
	else if(pin == 7)	
		adc_start(ADC_MUX_PIN_F7, ADC_REF_POWER);
	_delay_ms(1);
	
	char* stringBuf = "\r\nTemperature Value1: ";
	char* stringBuf1 = "\r\nTemperature Value2: ";
	char* stringBuf2 = "\r\nTemperature Value3: ";
	char buf[4];
	uint16_t val;
	
	uint16_t average  = adc_read();

	for(int i = 0; i < 10; i++)
	{
	
//	val = temperature;
	val = adc_read();
		temperature = val;
		buf[0] = HEX((val >> 8) & 15);
		buf[1] = HEX((val >> 4) & 15);
		buf[2] = HEX(val & 15);
		buf[3] = ' ';
		usb_serial_write(stringBuf, 21);
		usb_serial_write((unsigned char *)buf, 4);
			
		temperature = adc_read();// 0 - 1024
		average = (average + temperature) / 2;
		
	}
	
	//convert to kelvin
	
		
	return adcToKelvin(average);
	//simple read on the adc analog pin.

}