/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */

#define CH3_TASKMANAGEMENT

void taskLed(void *pvParameters)
{
    char *cod = (char*)pvParameters;
    for (;;) {
        TickType_t xLastWaketime = xTaskGetTickCount();
        if(*cod == 'o'){

                printf("\ndrvLedsSet(DRV_LEDS_ORANGE, DRV_LEDS_TOGGLE)\n");
                printf("\nBlink LED Orange\n");
                vTaskDelayUntil(&xLastWaketime, 500/portTICK_RATE_MS);

        }else if(*cod == 'g'){

            printf("\ndrvLedsSet(DRV_LEDS_GREEN,  DRV_LEDS_TOGGLE)\n");
            printf("\nBlink LED Green\n");
            vTaskDelayUntil(&xLastWaketime, 500/portTICK_RATE_MS);

        }else{
            printf("\nERRO - LED INEXISTENTE\n");
        }
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

	xTaskCreate( taskLed, "TaskLed", 1000, "o", 1, NULL );
	xTaskCreate( taskLed, "TaskLed", 1000, "g", 1, NULL );

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
