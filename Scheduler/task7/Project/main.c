/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */

#define CH3_TASKMANAGEMENT


static unsigned long int idle_tick_counter = 0;

void taskLed(void *pvParameters)
{
    char *cod = (char*)pvParameters;
    for (;;) {
        if(*cod == 'o'){

                printf("\ndrvLedsSet(DRV_LEDS_ORANGE, DRV_LEDS_TOGGLE)\n");
                printf("\nBlink LED Orange\n");
                vTaskDelay(500/portTICK_RATE_MS);

        }else if(*cod == 'g'){

            printf("\ndrvLedsSet(DRV_LEDS_GREEN,  DRV_LEDS_TOGGLE)\n");
            printf("\nBlink LED Green\n");
            vTaskDelay(500/portTICK_RATE_MS);

        }else{
            printf("\nERRO - LED INEXISTENTE\n");
        }
    }

}


void taskCPUUsage(void *pvParameters){
    unsigned long int idle_tick_last, ticks;
    idle_tick_last = idle_tick_counter = 0;
    for (;;) {
        /* wait for 3 seconds */
        vTaskDelay(3000/portTICK_RATE_MS);

        /* calculate quantity of idle ticks per second */
        if (idle_tick_counter > idle_tick_last)
            ticks = idle_tick_counter - idle_tick_last;
        else
            ticks = 0xFFFFFFFF - idle_tick_last + idle_tick_counter;
        ticks /= 3;
  
        /* print idle ticks per second */
        printf("%ld idle ticks per second (out of %ld)\n", ticks,
        configTICK_RATE_HZ);
        /* calc and print CPU usage */
        ticks = (configTICK_RATE_HZ - ticks)/10;
        printf("CPU usage: %d%%\n", ticks);
      
        /* update idle ticks */
        idle_tick_last = idle_tick_counter;
    }
}

void taskUseCPU(void *pvParameters){
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 0; i < 100000000; i++){
            j *= i + 12.34;
        }
        vTaskDelay(100/portTICK_RATE_MS);
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

    /* init uart driver */
    //drvUartInit(DRV_UART_CH_DEF);
    printf("\ndrvUartInit(DRV_UART_CH_DEF)\n");


    #ifdef CH3_TASKMANAGEMENT

	xTaskCreate( taskLed, "TaskLed", 1000, "o", 1, NULL );
	xTaskCreate( taskLed, "TaskLed", 1000, "g", 1, NULL );
    xTaskCreate( taskCPUUsage, "taskCPUUsage", 1000, NULL, 0, NULL );
    xTaskCreate( taskUseCPU, "taskUseCPU", 1000, NULL, 1, NULL );

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
    unsigned long int tick = xTaskGetTickCount();
    while (xTaskGetTickCount() == tick);
    idle_tick_counter++;
}
