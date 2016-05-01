
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
#include "Board_LED.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Main Thread'
 *---------------------------------------------------------------------------*/

extern osTimerId id1;
extern osTimerId id2;     
extern osTimerId id3; 
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
	uint32_t ADC1value;
	uint32_t ADC2value;
	uint32_t ADC3value;	
	//uint32_t ADC4value;
	uint32_t sum;
	
while (1) {
result1 = osMessageGet(ADC0_queue,osWaitForever);
result2 = osMessageGet(ADC1_queue,osWaitForever);
result3 = osMessageGet(ADC2_queue,osWaitForever);
//result4 = osMessageGet(ADC3_queue,osWaitForever);
ADC1value=result1.value.v;
ADC2value=result2.value.v;
ADC3value=result3.value.v;
//ADC4value=result4.value.v;
	
sum=ADC3value+ADC2value+ADC1value;
																								//calculation for about 25ms period
ADC1value=(25*ADC1value)/sum;
ADC2value=(25*ADC2value)/sum;
ADC3value=(25*ADC3value)/sum;
	
	LED_On(0);																		//RGB diode blinking
osDelay(ADC1value);
	LED_Off(0);
	LED_On(1);
osDelay(ADC2value);
   LED_Off(1);
	LED_On(2);
osDelay(ADC3value);        
	LED_Off(2);
  }
}


