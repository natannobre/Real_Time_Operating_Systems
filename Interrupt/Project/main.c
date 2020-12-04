#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "stdlib.h"
#include "semphr.h"

SemaphoreHandle_t SMF;//Objeto do semaforo

void taskISR(void *pvParameters){
    BaseType_t aux = 0;//Variavel de controle para a Troca de Contexto
    for (;;) {

    	/* release a semaphore from an ISR */
	    xSemaphoreGiveFromISR(SMF, &aux);
        vTaskDelay(pdMS_TO_TICKS(100));

        /* obtain a semaphore from an ISR */
        xSemaphoreTakeFromISR(SMF, &aux);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void taskLed(void *pvParameters){
    BaseType_t aux = 0;
    for (;;) {
        if (xSemaphoreTakeFromISR(SMF, &aux) == 1)
		{

			for (uint8_t i = 0; i < 3; i++)
			{
				printf("\033[31mLED VERMELHO: ON\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(100));
				printf("\033[31mLED VERMELHO: OFF\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(100));
			}

			/* release a semaphore from an ISR */
            xSemaphoreGiveFromISR(SMF, &aux);

            if (aux)
            {
                portYIELD_FROM_ISR(aux);
            }
		}
		else
		{

			for (uint8_t i = 0; i < 3; i++)
			{
				printf("\033[33mLED AMARELO: ON\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(100));
				printf("\033[33mLED AMARELO: OFF\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(100));
			}
		}

		vTaskDelay(pdMS_TO_TICKS(100));
    }
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

int main(void){

    SMF = xSemaphoreCreateBinary();//Cria o semaforo binario e atribui ao objeto que criamos

    /* create task 1 */
    xTaskCreate(taskLed, "TaskLed", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(taskISR, "TaskISR", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    /* start the scheduler */
    vTaskStartScheduler();
    /* should never reach here! */
    for(;;);

}

void vApplicationIdleHook(void){
//	printf("Idle\r\n");
}
/*-----------------------------------------------------------*/