// Switch.cpp
// This software to input from switches or buttons
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/14/21
// Lab number: 10
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Switch.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

void Switch_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x10;
	
	__asm__ {
		NOP
		NOP
	}
	
	GPIO_PORTE_DIR_R &= ~(0x0F);
	GPIO_PORTE_DEN_R |= 0x0F;
}


uint32_t Switch_In(void){ 
  // write this
	uint32_t data = GPIO_PORTE_DATA_R&0x07;
  return data;
}
