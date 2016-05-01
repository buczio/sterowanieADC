
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "MKL25Z4.h"
/*----------------------------------------------------------------------------
 *      ADC 2 conversion
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC2_queue;
void Thread_ADC2 (void const *argument);                           // thread function
osThreadId tid_ADC2;                                          			// thread id
osThreadDef (Thread_ADC2, osPriorityAboveNormal, 1, 0);            // thread priority Above Normal

int Init_ThreadADC2 (void) {

  tid_ADC2 = osThreadCreate (osThread(Thread_ADC2), NULL);
  if (!tid_ADC2) return(-1);
  
  return(0);
}



void Thread_ADC2 (void const *argument) {

	while(1){
		
       ADC0_SC1A=12;													//channel 12
       while ((ADC0_SC2 & 0x80));						//wait for conversion
       while (!(ADC0_SC1A & 0x80));
       osMessagePut(ADC2_queue,ADC0_RA,osWaitForever);	// use message queue to send result
			 osDelay(10);																			//every 1 ms
	}
}

