
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
/*----------------------------------------------------------------------------
 *      Thread 1 - ADC 0 conversion
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC0_queue;
void Thread_ADC0 (void const *argument);                           // thread function
osThreadId tid_ADC0;                                          			// thread id
osThreadDef (Thread_ADC0, osPriorityAboveNormal, 1, 0);            // thread priority Above Normal

int Init_ThreadADC0 (void) {

  tid_ADC0 = osThreadCreate (osThread(Thread_ADC0), NULL);
  if (!tid_ADC0) return(-1);
  
  return(0);
}



void Thread_ADC0 (void const *argument) {

	while(1){
		
       ADC0_SC1A=8;													//channel 8
       while ((ADC0_SC2 & 0x80));						//wait for conversion
       while (!(ADC0_SC1A & 0x80));
       osMessagePut(ADC0_queue,ADC0_RA,osWaitForever);	// use message queue to send result
			 osDelay(1);																			//every 1 ms
	}
}

