/*----------------------------------------------------------------------------
 * Program realizujacy odczyt polozenia dwoch 'grzybkow' od joysticka
 * poprzez 4 kanaly ADC oraz komunikacje miedzywatkowa typu 'message queue'
 * RTX RTOS
 * Bartlomiej Buczek
 * Freedom KL25Z Board
 *---------------------------------------------------------------------------*/

#define osObjectsPublic		// define objects in main module
#include "osObjects.h"		// RTOS object definitions
#include "MKL25Z4.h"
#include "Board_LED.h"		//board led support for testing purpose

osMessageQId ADC0_queue;		//define the message queues for ADCx threads
osMessageQDef (ADC0_queue,0x1,uint32_t);							
osEvent  result1;

osMessageQId ADC1_queue;																		
osMessageQDef (ADC1_queue,0x1,uint32_t);							
osEvent  result2;

osMessageQId ADC2_queue;																		
osMessageQDef (ADC2_queue,0x1,uint32_t);							
osEvent  result3;

osMessageQId ADC3_queue;																		
osMessageQDef (ADC3_queue,0x1,uint32_t);							
osEvent  result4;

int ADC_Init (void);																			//ADC initialization prototype
extern int Init_ThreadADC0(void);													//ADC threads
extern int Init_ThreadADC1(void);	
extern int Init_ThreadADC2(void);
extern int Init_ThreadADC3(void);
extern int Init_main_Thread(void);												//'main' thread 


int main (void) {
	osKernelInitialize ();		// initialize CMSIS-RTOS
	LED_Initialize();		// LED initialize
	ADC0_queue=osMessageCreate(osMessageQ(ADC0_queue),NULL);// initialize queue for adc0
	ADC1_queue=osMessageCreate(osMessageQ(ADC1_queue),NULL);// initialize queue for adc1
	ADC2_queue=osMessageCreate(osMessageQ(ADC2_queue),NULL);// initialize queue for adc2
	ADC3_queue=osMessageCreate(osMessageQ(ADC3_queue),NULL);// initialize queue for adc2
	if(ADC_Init()!=0)while(1){;/*error*/};									//initialize ADC
	if(Init_ThreadADC0()!=0)while(1){;/*error*/};					  //initialize ADC0 Thread
	if(Init_ThreadADC1()!=0)while(1){;/*error*/};					  //initialize ADC1 Thread
	if(Init_ThreadADC2()!=0)while(1){;/*error*/};					  //initialize ADC2 Thread
	if(Init_ThreadADC3()!=0)while(1){;/*error*/};					  //initialize ADC2 Thread
	if(Init_main_Thread()!=0)while(1){;/*error*/};					//initialize 'main' Thread
	osKernelStart ();                         						// start thread execution 

}




int ADC_Init(void)
{
//volatile uint16_t calibration_variable =0;
// Clock to adc0
SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
// ADC0 Configuration register 1
ADC0->CFG1 = ADC_CFG1_ADICLK(0)		// 0-bus clock, 1- bus clock/2, 2-alternative, 3 - asynchronous
| ADC_CFG1_MODE(3)		// 3- 16bit, 2-10bit, 1-12bit, 0 - 8bit (for single ended mode)
| ADC_CFG1_ADLSMP(0)		// 0-short sample rate, 1 - long sample rate (lower power consumpt)
| ADC_CFG1_ADIV(0)		// Clock divide, 0 - div1, 1- div2, 2 - div4, 3 div8
| ADC_CFG1_ADLPC(0);		//0- normal, 1 -low power
// ADC0 Configuration register 2	
ADC0->CFG2 = ADC_CFG2_ADLSTS(0)		// time select for long sample rate (enabled in CFG1 reg), 0 - default
| ADC_CFG2_ADHSC(1)		// 1 - high speed conversion, 0 - normal conversion
| ADC_CFG2_ADACKEN(0)		// 1 - enable asynchrous clock output
| ADC_CFG2_MUXSEL(0);		// 0 -  channel A, 1 - channel B
	
// ADC Status and control register 2
ADC0->SC2 = ADC_SC2_REFSEL(0)		//voltage reference selection, 0 - default
| ADC_SC2_DMAEN(0);		// DMA 1 - enabled, 0 - disabled
// ew. compare function
	
// ADC Status and control register 2
ADC0->SC3 = ADC_SC3_AVGE(0)		// 1 - hardware average enable, 0 - disable
| ADC_SC3_AVGS(0)		// 0 - 4 samples, 1 - 8 samples, 3 - 32 samples
| ADC_SC3_ADCO(0);		// continuous conversion 1 - enabled, 0 - disabled

ADC0->SC3|=ADC_SC3_CAL_MASK;		// begin calibration
while((ADC0->SC3&ADC_SC3_CAL_MASK)==ADC_SC3_CAL_MASK){
	;	//wait until conversion complete
}

if((ADC0->SC3&ADC_SC3_CALF_MASK)==ADC_SC3_CALF_MASK){
	return(-1);	//calibration failed
}
//calibration_variable = (ADC0_CLP0+ADC0_CLP1+ADC0_CLP2+ADC0_CLP3+ADC0_CLP4+ADC0_CLPS)/2+((uint16_t)32768);

//ADC0->PG = ADC_PG_PG(calibration_variable);
return(0);	//calibration succesed
}
