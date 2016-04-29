/*----------------------------------------------------------------------------
 * Program realizujacy odczyt polozenia dwoch 'grzybkow' od joysticka
 * poprzez 4 kanaly ADC oraz komunikacje miedzywatkowa typu 'message queue'
 * RTX RTOS
 * Bartlomiej Buczek
 * Freedom KL25Z Board
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "MKL25Z4.h"
#include "Board_LED.h"											//board led support for testing purpose

osMessageQId ADC0_queue;																		//define the message queues
osMessageQDef (ADC0_queue,0x1,uint32_t);							
osEvent  result1;

osMessageQId ADC1_queue;																		//define the message queues
osMessageQDef (ADC1_queue,0x1,uint32_t);							
osEvent  result2;

int ADC0_Init (void);																			//ADC Initialization prototype
extern int Init_ThreadADC0(void);													//ADC0 Conversion prototype
extern int Init_ThreadADC1(void);	
extern int Init_main_Thread(void);												//main thread prototype



int main (void) {
	osKernelInitialize ();  																// initialize CMSIS-RTOS
	LED_Initialize();
	ADC0_queue=osMessageCreate(osMessageQ(ADC0_queue),NULL);// initialize queue for adc0
	ADC1_queue=osMessageCreate(osMessageQ(ADC1_queue),NULL);// initialize queue for adc0
	if(ADC0_Init()!=0)while(1){;};													//initialize ADC0
	if(Init_ThreadADC0()!=0)while(1){;/*error*/};					  //initialize ADC0 Thread
	if(Init_ThreadADC1()!=0)while(1){;/*error*/};					  //initialize ADC0 Thread
	if(Init_main_Thread()!=0)while(1){;/*error*/};					//initialize 'main' Thread
		
		osKernelStart ();                         						// start thread execution 

}



int ADC0_Init(void)
{
//clock to adc0
SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
//bus clock/2 & singled-ended 16bit mode & div by 4
ADC0->CFG1 = ADC_CFG1_ADICLK(1) | ADC_CFG1_MODE(3) | ADC_CFG1_ADIV(2);
//select channel A //& high speed mode
ADC0->CFG2|=ADC_CFG2_MUXSEL(0);// | ADC_CFG2_ADHSC_MASK;
//hardware trigger & VREFH/VREFL
ADC0->SC2 = ADC_SC2_REFSEL(0);
//calibration and enable averaging
ADC0->SC3|=ADC_SC3_AVGE_MASK|ADC_SC3_AVGS_MASK;
ADC0->SC3|=ADC_SC3_CAL_MASK;
while((ADC0->SC3&ADC_SC3_CAL_MASK)==ADC_SC3_CAL_MASK){
	;	//wait until conversion complete
}
if((ADC0->SC3&ADC_SC3_CALF_MASK)==ADC_SC3_CALF_MASK){
	return(-1);	//calibration failed
}
return(0);	//initialization complete
}
