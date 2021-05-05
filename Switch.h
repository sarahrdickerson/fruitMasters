// Switch.h
// This software to input from switches or buttons
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/17/21
// Lab number: 10
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
#ifndef _Switch_h
#define _Switch_h
#include <stdint.h>

// Header files contain the prototypes for public functions 
// this file explains what the module does

void Switch_Init();
uint32_t Switch_In(void);

#endif

