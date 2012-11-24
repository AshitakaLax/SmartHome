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

#include "../include/Fans.h"
#include "../include/PWMTeensyTwoPlusPlus.h"
#include "../include/usb_serial.h"

/*
*	This example is comprized of mainly arduino sources
// */
// uint8_t	FanZeroStatus = 0;// need to initialize it.
// uint8_t	FanOneStatus = 0;// need to initialize it.
// uint8_t	FanTwoStatus = 0;// need to initialize it.
// uint8_t	FanThreeStatus = 0;// need to initialize it.
// uint8_t	FanFourStatus = 0;// need to initialize it.
/**
*	initializes the pins for the fans.
*/
void InitializeFans()
{
	//configure the fans that we want to use.
	PWMNew(3);
	PWMNew(17);
	PWMNew(18);
	PWMNew(19);
	PWMDuty(3, 0);
	PWMDuty(17, 0);
	PWMDuty(18, 0);
	PWMDuty(19, 0);
	PWMStartTimer(3,4);
	PWMStartTimer(17,4);
	PWMStartTimer(18,4);
	PWMStartTimer(19,4);
	
	
}

/**
*	Sets the speed of a fan
*  FanSelect 0-4 for the specific fan.
*	the only valid fans are 0,2,3,4
*  FanSpeed 0-255 0 for off and 255 for Max power.
* 	returns 0 if successfully changed.
* 	else returns the fan number(OFFSET by +1) of the other fan that is currently on.
* 	this would be an Error
*/
uint8_t SetFan(char FanChar, char FanSpeedchar)
{
	uint8_t FanSelect = FanChar - '0';
	uint8_t FanSpeed = CharToSpeed(FanSpeedchar);
	//fan pins are Fan0-PD0(3),(DON'T USE Fan1-PD3(6)/Not a PWM(case fan)
	//Fan Fan2-PC4(17), Fan3-PC5(18), Fan4-PC6(19)
	
	//need to check status of fans 0,2,3,4 we don't want more than 1.5amps of draw.
	//can also draw off of TempSense power on power board. just wire directly to connector, and skip
	// soldering it. // then we can power 2 at the same time.
	
	//NOTE about circuit design.
		//bail one is already running, and we can't over draw circuit
		// can change if you supply 1.5 amp power supply to each fan.
		// to do this you would just need a L7812 voltage reg for each.
		// it would be good to have a heatsink on each of them.
	if(FanZeroStatus == '1' && FanChar != '0')//return 1 for fan 0
	{
		return 1;
	}
	else if(FanTwoStatus == '1' && FanChar != '2')//return 3 for fan 2
	{
		return 3;
	}	
	else if(FanThreeStatus=='1' && FanChar != '3')//return 4 for fan 3
	{
		return 4;
	}
	else if(FanFourStatus == '1' && FanChar != '4')//return 5 for fan 4
	{
		return 5;
	}
	else if(FanSelect == 0)
	{
		FanZeroStatus = '1';
		//set duty cycle
		PWMDuty(3, FanSpeed);
		FanZeroSpeed = FanSpeedchar;
	}
	else if(FanSelect == 2)
	{
		FanTwoStatus = '1';
		//set duty cycle
		PWMDuty(17, FanSpeed);
		FanTwoSpeed = FanSpeedchar;
	}
	else if(FanSelect == 3)
	{
		FanThreeStatus = '1';
		//set duty cycle
		PWMDuty(18, FanSpeed);
		FanThreeSpeed = FanSpeedchar;
	}
	else if(FanSelect == 4)
	{
		FanFourStatus = '1';
		//set duty cycle
		PWMDuty(19, FanSpeed);
		FanFourSpeed = FanSpeedchar;
	}
	else
	{
		return 255;//max invalid fan.
	}
}

/*
*	gets the FanSpeed of a specific Fan
* returns 0 - 100 based on the fan speed.
*/
char FanStatus(char FanChar)
{
	uint8_t FanSelect = FanChar - '0';
	if(FanSelect == 0)
	{
		return FanZeroStatus;
	}
	else if(FanSelect == 2)
	{
		return FanTwoStatus;
	}
	else if(FanSelect == 3)
	{
		return FanThreeStatus;
	}
	else 
	{
		return FanFourStatus;
	}
}
/*
*	gets the FanSpeed of a specific Fan
*	returns 0,1,2..8,9
*/
char FanSpeedStatus(char FanChar)
{
	if(FanChar == '0')
	{
		return FanZeroSpeed;
	}
	else if(FanChar == '2')
	{
		return FanTwoSpeed;
	}
	else if(FanChar == '3')
	{
		return FanThreeSpeed;
	}
	else if(FanChar == '4')
	{
		return FanFourSpeed;
	}
	return 'n';// if invalid input.
}
/*
*	This will change the fan status, and change the 
* 	duty cycle of the fan to represent off.
*	return 0 if sucessful, and 1 for invalid input.
*/
uint8_t TurnFanOff(char FanChar)
{
	uint8_t FanSelect = FanChar - '0';
	//	Fan 0 pin3(PD0),
	if(FanSelect == 0)
	{
		PWMDuty(3, 0);
		FanZeroStatus = '0';
		FanZeroSpeed = '0';
	}
	else if(FanSelect == 2)
	{
		PWMDuty(17, 0);
		FanTwoStatus = '0';
		FanTwoSpeed = '0';
	}
	else if(FanSelect == 3)
	{
		PWMDuty(18, 0);
		FanThreeStatus = 0;
		FanThreeSpeed = '0';
	}
	else if(FanSelect == 4)
	{
		PWMDuty(19, 0);
		FanFourStatus = '0';
		FanFourSpeed = '0';
		
	}
	else
		return 1;
	return 0;
	
}
/*
*	10 speeds 0 to 9
*	returns a value from 0(0) to 255(10)
* returns 0 if invalid.
*/
uint8_t CharToSpeed(char speed)
{
	if(speed == '0')
		return 0;
	else if(speed == '1')
		return 28;
	else if(speed == '2')
		return 56;
	else if(speed == '3')
		return 85;
	else if(speed == '4')
		return 113;
	else if(speed == '5')
		return 141;
	else if(speed == '6')
		return 170;
	else if(speed == '7')
		return 198;
	else if(speed == '8')
		return 226;
	else if(speed == '9')
		return 255;
	else
		return 0;
}
/**
*	This will print the status of all the fans.
*	0,2,3,4
*	format
*	"FanNumber:on/off:speed,"
*
**/
void AllFanStatus()
{
	send_str(PSTR("1:"));
	if(FanZeroStatus == '1')//if fan 0 is on
		send_str(PSTR("1:"));
	else 
		send_str(PSTR("0:"));
	usb_serial_putchar(FanZeroSpeed);
	send_str(PSTR(",2:"));
	if(FanTwoStatus == '1')//if fan 0 is on
		send_str(PSTR("1:"));
	else 
		send_str(PSTR("0:"));
	usb_serial_putchar(FanTwoSpeed);
	send_str(PSTR(",3:"));
	if(FanThreeStatus == '1')//if fan 0 is on
		send_str(PSTR("1:"));
	else 
		send_str(PSTR("0:"));
	usb_serial_putchar(FanThreeSpeed);
	send_str(PSTR(",4:"));
	if(FanFourStatus == '1')//if fan 0 is on
		send_str(PSTR("1:"));
	else 
		send_str(PSTR("0:"));
	usb_serial_putchar(FanFourSpeed);
}