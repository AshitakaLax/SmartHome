/**
*Author: Levi Balling
*Date: 7/17/19
*
* Purpose: this file is to control all of the Fans with a PWM signal
*/
#ifndef fans_h__
#define fans_h__

#include <avr/pgmspace.h>
#include <util/delay.h>

// #define DEMUX_A_CONFIG	(DDRB |= (1<<3))
//Configure Fan Pins PC6, PC5, PC4, PD0, PD3
//#define XTAL 160000000L // 16Mhz


//their are 5 fans with a 
void InitializeFans(void);
uint8_t SetFan(char FanChar, char FanSpeedchar);
char FanStatus(char FanChar);
void AllFanStatus();
uint8_t TurnFanOff(char FanChar);
uint8_t CharToSpeed(char speed);
char FanSpeedStatus(char FanChar);

char	FanZeroStatus = '0';// need to initialize it.
char	FanOneStatus = '0';// need to initialize it.
char	FanTwoStatus = '0';// need to initialize it.
char	FanThreeStatus = '0';// need to initialize it.
char	FanFourStatus = '0';// need to initialize it.
char FanZeroSpeed = '0';//
char FanOneSpeed = '0';
char FanTwoSpeed = '0';
char FanThreeSpeed = '0';
char FanFourSpeed = '0';
// // Pin 3 (PD0)
// extern uint8_t FanZeroStatus;
// // Pin 4 (PD1)
// extern uint8_t FanOneStatus;//always on, but its for the case
// // Pin 17 (PC4)
// extern uint8_t FanTwoStatus;
// // Pin 18 (PC5)
// extern uint8_t FanThreeStatus;
// // Pin 19 (PC6)
// extern uint8_t FanFourStatus;

#endif
