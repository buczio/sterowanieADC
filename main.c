/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "MKL25Z4.h"

void ADC0_Init (void);											//ADC Initialization


int main (void) {
	volatile	uint32_t result;
 // osKernelInitialize ();                    // initialize CMSIS-RTOS
	ADC0_Init();

	
for(;;) {  
       ADC0_SC1A=8;
       while ((ADC0_SC2 & 0x80));
       while (!(ADC0_SC1A & 0x80));
       result=ADC0_RA;
  }

	
 // osKernelStart ();                         // start thread execution 
	  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL)
}

void ADC0_Init(void)
{
//clock to adc0
SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
//bus clock/2 & singled-ended 16bit mode & div by 4
ADC0->CFG1 = ADC_CFG1_ADICLK(1) | ADC_CFG1_MODE(3) | ADC_CFG1_ADIV(2);
//select channel A //& high speed mode
ADC0->CFG2|=ADC_CFG2_MUXSEL(0);// | ADC_CFG2_ADHSC_MASK;
//hardware trigger & VREFH/VREFL
ADC0->SC2 = ADC_SC2_REFSEL(0);
//single shot mode
//ADC0->SC3 &= ~ADC_SC3_ADCO_MASK;
ADC0->SC3|=ADC_SC3_CAL_MASK;
while((ADC0->SC3&ADC_SC3_CAL_MASK)==ADC_SC3_CAL_MASK){
	;
}
if((ADC0->SC3&ADC_SC3_CALF_MASK)==ADC_SC3_CALF_MASK){
	while(1){
		;
	}
}

	
}
