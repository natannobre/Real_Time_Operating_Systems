#include "FreeRTOS.h"
#include "task.h"
// #include "drivers.h"
#include "stdio.h"
#include "stdlib.h"
#include "semphr.h"

SemaphoreHandle_t SMF;//Objeto do semaforo

void delay(){
    unsigned int i;
    for(i = 0; i < 200000000; i++)
        asm("nop");
}

void isr(void*z)
{
	BaseType_t aux = 0;//Variavel de controle para a Troca de Contexto
	xSemaphoreGiveFromISR(SMF, &aux);

	if (aux)
	{
		portYIELD_FROM_ISR(aux);//Se houver tarefas esperando pelo semaforo, deve ser forcado a Troca de Contexto (afim de minimizar latencia)
	}

}

void isr2(void*z)
{
	BaseType_t aux = 0;//Variavel de controle para a Troca de Contexto
	xSemaphoreTakeFromISR(SMF, &aux);

	if (aux)
	{
		portYIELD_FROM_ISR(aux);//Se houver tarefas esperando pelo semaforo, deve ser forcado a Troca de Contexto (afim de minimizar latencia)
	}

}

void taskISR(void *pvParameters){
    for (;;) {
        isr(NULL);
        vTaskDelay(pdMS_TO_TICKS(500));
        isr2(NULL);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void taskLed(void *pvParameters){
    BaseType_t aux = 0;//Variavel de controle para a Troca de Contexto
    for (;;) {
        if (xSemaphoreTakeFromISR(SMF, &aux) == 1)//Tenta obter o semaforo durante 200ms (Timeout). Caso o semaforo nao fique disponivel em 200ms, retornara FALSE
		{
			//Se obteu o semaforo entre os 200ms de espera, fara o toggle do pino 23

			for (uint8_t i = 0; i < 3; i++)
			{
				printf("\033[31mLED VERMELHO: ON\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(500));
				printf("\033[31mLED VERMELHO: OFF\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(500));
			}
            xSemaphoreGiveFromISR(SMF, &aux);//Libera o semaforo
            if (aux)
            {
                portYIELD_FROM_ISR(aux);//Se houver tarefas esperando pelo semaforo, deve ser forcado a Troca de Contexto (afim de minimizar latencia)
            }
		}
		else
		{
			//Se nao obter o semaforo entre os 200ms de espera, fara o toggle do pino 22

			for (uint8_t i = 0; i < 3; i++)
			{
				printf("\033[33mLED AMARELO: ON\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(500));
				printf("\033[33mLED AMARELO: OFF\n\033[0;0m");
				vTaskDelay(pdMS_TO_TICKS(500));
			}
		}

		vTaskDelay(pdMS_TO_TICKS(500));
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