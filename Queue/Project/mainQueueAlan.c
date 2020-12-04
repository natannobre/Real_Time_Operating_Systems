#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "queue.h"

QueueHandle_t QueueDados;

typedef struct Dados{
    float dado1;   
    int dado2;
    char dado3; 
} Dados; 

void produtor(void *pvParameters) {
    //Inicializando
    float x1 = 0;
    int   x2 = 0;
    char  x3 = 'A';

    //Definindo a Struct
    Dados *dados;
    dados =  (Dados *) malloc(sizeof(Dados));

    //Produzindo dados para a fila QueueDados
	for (;;) {
        //Atualizando Dados
        x1 = x1 > 10000 ? 0 : x1 + 1.3;
        x2 = x2 > 10000 ? 0 : x2 + 2;
        x3 = x3 == 'Z' ? 'A' : x3 + 1;
        dados->dado1 = x1;
        dados->dado2 = x2;
        dados->dado3 = x3;
        
        //Enviando para a fila
        xQueueSendToBack(QueueDados, dados, 250/portTICK_RATE_MS);


        vTaskDelay(200/portTICK_RATE_MS);
	}
}

void consumidor(void *pvParameters) {
	
    //Defindo a struct que recebe dados
    Dados *dadosRe;
    dadosRe =  (Dados *) malloc(sizeof(Dados));

	for (;;) {
        if(uxQueueMessagesWaiting(QueueDados) > 0) {
			xQueueReceive(QueueDados, dadosRe, 500/portTICK_RATE_MS);
            printf("\n \nDados Recebidos (Struct):  \n | -> Dado 1 : %.2f \n | -> Dado 2 : %d\n | -> Dado 3 : %c\n", dadosRe->dado1,dadosRe->dado2, dadosRe->dado3);
        }
	}
}



int main(void)
{
	xTaskCreate(produtor, (signed char *)"Produtor",
	configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	xTaskCreate(consumidor, (signed char *)"Consumidor",
	configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Parametros : Tamanho da Fila / Tamanho de cada item
    QueueDados = xQueueCreate(25, 15);

	vTaskStartScheduler();
	for(;;);
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

void vApplicationIdleHook(void) {}