/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */

#define CH3_TASKMANAGEMENT

void delay()
{

    unsigned int i;
    for(i = 0; i < 200000000; i++) {
        asm("nop");
    }

}



void taskLed1(void *pvParameters)
{
    for (;;) {
        //drvLedsSet(DRV_LEDS_GREEN, DRV_LEDS_TOGGLE);
        printf("\ndrvLedsSet(DRV_LEDS_GREEN, DRV_LEDS_TOGGLE)\n");
        printf("\ntaskLed1 - Blink LED Green\n");
        delay();
    }

}



void taskLed2(void *pvParameters)
{
    for (;;) {
        //drvLedsSet(DRV_LEDS_ORANGE, DRV_LEDS_TOGGLE);
        printf("\ndrvLedsSet(DRV_LEDS_ORANGE, DRV_LEDS_TOGGLE)\n");
        printf("\ntaskLed2 - Blink LED Orange\n");
        delay();
    }

}


void vApplicationIdleHook(void);

int main ( void )
{
    /* init CPU clock */
    //drvCpuClockInit();
    printf("\ndrvCpuClockInit()\n");

    /* init leds driver */
    //drvLedsInit();
    printf("\ndrvCpuClockInit()\n");


    #ifdef CH3_TASKMANAGEMENT

	xTaskCreate( taskLed1, "TaskLed1", 1000, NULL, 1, NULL );
	xTaskCreate( taskLed2, "TaskLed2", 1000, NULL, 4, NULL );

    #endif

    /* start the scheduler */
	vTaskStartScheduler();


    /* should never reach here! */
    for(;;);

	return 0;
}


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


void vApplicationIdleHook(void)
{
//	printf("Idle\r\n");
}
/*-----------------------------------------------------------*/
