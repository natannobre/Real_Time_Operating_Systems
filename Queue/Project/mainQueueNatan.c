/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"

#define CH3_TASKMANAGEMENT

QueueHandle_t messageQueue;

int sensorRead(){
    srand(time(NULL));
    return (rand()%99);
}


void vTask1(void* parameter)
{
    char *messageSent[200];
    while(1){
        printf("\n(task1) - Digite a frase:\n");
        while(scanf(" %s", messageSent) == 0);
        vTaskDelay(pdMS_TO_TICKS(1500));
        xQueueSend(messageQueue, messageSent, 200);

    }
}

void vTask2(void* parameter)
{
    //char *messageReceived[200];
    int *messageReceived;
    messageReceived = malloc(sizeof(int));
    while(1){
        //printf("\nQueueSize Before: %d\n", uxQueueMessagesWaiting(messageQueue));
        if (uxQueueMessagesWaiting(messageQueue) > 0){
            //printf("\nQueueSize: %d\n", uxQueueMessagesWaiting(messageQueue));
            xQueueReceive(messageQueue, messageReceived, pdMS_TO_TICKS(200));
            printf("\ntask2 - Mensagem Recebida: Leu do sensor: %d\n", *messageReceived);
        }
    }
}

void vTask3(void* parameter)
{
    char *messageSent = (char *)parameter;
    while(1){
        printf("\n(task3 - enviou)\n");
        xQueueSend(messageQueue, messageSent, 200);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask4(void* parameter)
{
    //char *messageSent = (char *)parameter;
    int * v;
    v = malloc(sizeof(int));
    while(1){
        v = sensorRead();
        printf("\n(task3 - enviou)\n");
        xQueueSend(messageQueue, &v, 200);
        vTaskDelay(pdMS_TO_TICKS(500));
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

    //char msg[100];
    //scanf("%[^\n]s", msg);

    #ifdef CH3_TASKMANAGEMENT

	//xTaskCreate( vTask1, "vTask1", 1000, NULL, 1, NULL );
	xTaskCreate( vTask2, "vTask2", 1000, NULL, 1, NULL );
    //xTaskCreate( vTask3, "vTask3", 1000, msg, 1, NULL );
    xTaskCreate( vTask4, "vTask4", 1000, NULL, 1, NULL );

    #endif

    /* create a new queue instance */
    int queueLenght = 50; //Conjunto finito de itens
    int itemSize = 64; //tamanho fixo de bytes
    messageQueue = xQueueCreate( queueLenght, itemSize);

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
