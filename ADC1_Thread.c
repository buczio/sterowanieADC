
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
/*----------------------------------------------------------------------------
 *      Thread 1 - ADC 0 conversion
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC1_queue;
void Thread_ADC1 (void const *argument);                           // thread function
osThreadId tid_ADC1;                                          			// thread id
osThreadDef (Thread_ADC1, osPriorityAboveNormal, 1, 0);            // thread priority Above Normal

int Init_ThreadADC1 (void) {

  tid_ADC1 = osThreadCreate (osThread(Thread_ADC1), NULL);
  if (!tid_ADC1) return(-1);
  
  return(0);
}



void Thread_ADC1 (void const *argument) {

	while(1){
		
       ADC0_SC1A=9;													//channel 9
       while ((ADC0_SC2 & 0x80));						//wait for conversion
       while (!(ADC0_SC1A & 0x80));
       osMessagePut(ADC1_queue,ADC0_RA,osWaitForever);	// use message queue to send result
			 osDelay(1);																			//every 1 ms
	}
}

