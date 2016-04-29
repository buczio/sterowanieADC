
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
#include "Board_LED.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Main Thread'
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC0_queue;
extern osMessageQId ADC1_queue;
extern osMessageQId ADC2_queue;
extern osMessageQId ADC3_queue;
extern osEvent  result1;
extern osEvent  result2;
extern osEvent  result3;
extern osEvent  result4;
void main_Thread (void const *argument);                             // thread function
osThreadId tid_main_Thread;                                          // thread id
osThreadDef (main_Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_main_Thread (void) {

  tid_main_Thread = osThreadCreate (osThread(main_Thread), NULL);
  if (!tid_main_Thread) return(-1);
  
  return(0);
}

void main_Thread (void const *argument) {
	
volatile	uint32_t ADC1value;
volatile	uint32_t ADC2value;
volatile	uint32_t ADC3value;	
volatile	uint32_t ADC4value;	
	
while (1) {
result1 = osMessageGet(ADC0_queue,osWaitForever);
result2 = osMessageGet(ADC1_queue,osWaitForever);
result3 = osMessageGet(ADC2_queue,osWaitForever);
result4 = osMessageGet(ADC3_queue,osWaitForever);
ADC1value=result1.value.v;
ADC2value=result2.value.v;
ADC3value=result3.value.v;
ADC4value=result4.value.v;
if(ADC4value>0x76FF){	
if(ADC3value>0x7FFF){
	if(ADC1value>0x7FFF&ADC2value>0x7FFF){								//for testing purpose
	LED_On(0);
	LED_Off(1);
	LED_Off(2);
	}else if(ADC1value<=0x7FFF&ADC2value>0x7FFF){
	LED_Off(0);
	LED_On(1);
	LED_Off(2);
	}else if(ADC1value<=0x7FFF&ADC2value<=0x7FFF){
	LED_Off(0);
	LED_Off(1);
	LED_On(2);
	}else{
	LED_On(0);
	LED_On(1);
	LED_Off(2);
	}
}else{
	if(ADC1value>0x7FFF&ADC2value>0x7FFF){
	LED_On(0);
	LED_On(1);
	LED_Off(2);
	}else if(ADC1value<=0x7FFF&ADC2value>0x7FFF){
	LED_Off(0);
	LED_On(1);
	LED_On(2);
	}else if(ADC1value<=0x7FFF&ADC2value<=0x7FFF){
	LED_On(0);
	LED_On(1);
	LED_On(2);
	}else{
	LED_On(0);
	LED_Off(1);
	LED_On(2);
	}	
}
}else{
		LED_Off(0);
	LED_Off(1);
	LED_Off(2);
}
	
   osDelay(15);             
	
  }
}
