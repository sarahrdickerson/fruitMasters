// SlidePot.cpp
// Runs on TM4C123
// Provide functions that initialize ADC0
// copy code from Lab 8 or Lab 9
// Last Modified: 1/16/2021 
// Student names: solutions

#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(uint32_t sac){ 
// lab 8
	SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0 
                                  
  SYSCTL_RCGCGPIO_R |= 0x08;			// 2) activate clock for Port D
  while((SYSCTL_PRGPIO_R&0x08) != 0x08){};
  GPIO_PORTD_DIR_R &= ~0x04;      // 3) make PD2 input
  GPIO_PORTD_AFSEL_R |= 0x04;     // 4) enable alternate function on PD2
  GPIO_PORTD_DEN_R &= ~0x04;      // 5) disable digital I/O on PD2
  GPIO_PORTD_AMSEL_R |= 0x04;     // 6) enable analog functionality on PD2
	
	ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 5;             //    set channel
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  uint32_t result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
// lab 8
	slope = m;
	offset = b;
	data = 0;
	flag = 0;
	distance = 0;
}

void SlidePot::Save(uint32_t n){
// lab 9
	data = n;
	distance = Convert(data);
	flag = 1;
}
uint32_t SlidePot::Convert(uint32_t n){
  // lab 8
  return (slope*n)/4096 + offset;
}

void SlidePot::Sync(void){
// lab 8
	while(flag == 0){};
	flag = 0;
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
  return data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
// lab 8
  return distance;
}


