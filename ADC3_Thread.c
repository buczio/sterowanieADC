
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
/*----------------------------------------------------------------------------
 *      ADC 3 conversion
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC3_queue;
void Thread_ADC3 (void const *argument);                           // thread function
osThreadId tid_ADC3;                                          			// thread id
osThreadDef (Thread_ADC3, osPriorityAboveNormal, 1, 0);            // thread priority Above Normal

int Init_ThreadADC3 (void) {

  tid_ADC3 = osThreadCreate (osThread(Thread_ADC3), NULL);
  if (!tid_ADC3) return(-1);
  
  return(0);
}



void Thread_ADC3 (void const *argument) {

	while(1){
		
       ADC0_SC1A=13;													//channel 13
       while ((ADC0_SC2 & 0x80));						//wait for conversion
       while (!(ADC0_SC1A & 0x80));
       osMessagePut(ADC3_queue,ADC0_RA,osWaitForever);	// use message queue to send result
			 osDelay(10);																			//every 1 ms
	}
}

