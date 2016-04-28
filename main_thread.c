
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Main Thread'
 *---------------------------------------------------------------------------*/
extern osMessageQId ADC0_queue;
extern osEvent  result;
void main_Thread (void const *argument);                             // thread function
osThreadId tid_main_Thread;                                          // thread id
osThreadDef (main_Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_main_Thread (void) {

  tid_main_Thread = osThreadCreate (osThread(main_Thread), NULL);
  if (!tid_main_Thread) return(-1);
  
  return(0);
}

void main_Thread (void const *argument) {
	
volatile	uint32_t ADC0value;
	
	
while (1) {
result = osMessageGet(ADC0_queue,osWaitForever);
ADC0value=result.value.v;
   osDelay(5);             
	;		
  }
}
