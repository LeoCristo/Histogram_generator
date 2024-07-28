/*##############################################################################
##############################################################################*/

//TivaWare uC: Usado internamente para identificar o uC em alguns .h da TivaWare
#define PART_TM4C1294NCPDT 1

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "inc/hw_types.h"
#include "images.h" // Para funcionar no keil, no path deve estar adicionado o caminho .\ , que é o diretório raiz do projeto (images.h e images.c estão lá)


uint32_t ui32SysClkFreq;

uint16_t EightBitHistogram_C(uint16_t width, uint16_t height, uint8_t * p_image, uint16_t *p_histogram);
void initSerial();
void printCSV(uint16_t string[], int len);
void UART0_SendString(const char *str);
void uint16_to_string(uint16_t value, char *buffer, size_t buffer_size);


// Função em Assembly embutida usando __asm
 int asm_add(int a, int b) {
	 __asm(
     "MOV R2, b \n\t" 
	  "MOV R1, a \n\t"
	  "ADD R0, R1, R2\n\t"
		"POP {R14} \n\t"
		
	 );
	 //return 0;
}

int main(void)
{
	uint16_t p_histogram[256]; //Cada pixel em cinza pode ter a intensidade variando de 0 a 255 = 256 valoress
  //Configura o clock para utilizar o xtal interno de 16MHz com PLL para 40MHz
  ui32SysClkFreq = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320, 40000000);
  //habilitar gpio port N
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  //aguardar o periférico ficar pronto para uso
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
  //configura o pin_0 como saída
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
	initSerial();

	EightBitHistogram_C(WIDTH0, HEIGHT0, (uint8_t *)image0, p_histogram);//-------------------->descomente para a imagem 0<----------------------
//EightBitHistogram_C(WIDTH1, HEIGHT1, (uint8_t *)p_start_image1, p_histogram);//------------->descomente para a imagem 1<---------------------
	printCSV(p_histogram,256);// envia para a serial com a taxa de 115200 baud
	asm_add(1, 2);
	int b = 5;
  while (1)
        {	 
        }
}


uint16_t EightBitHistogram_C(uint16_t width, uint16_t height, uint8_t *p_image, uint16_t *p_histogram){
//int image_size,i,histogram[255],image[255];
	uint16_t i; 
	uint32_t image_size;
	image_size= width*height;
	
	if (image_size>65536) {
		return 0;
	}
// Clear histogram
	
	for(i=0; i<=255; i++){
		p_histogram[i]=0;
	}

// Calulate histogram

	for(i=0; i<=image_size-1;i++){
		++p_histogram[p_image[i]];
	}
	
	return image_size;
}

/***
*			Inicializa a serial em 115200 
*			
***/
void initSerial(){
	// Enable the UART0 and GPIOA peripherals (the UART pins are on GPIO Port A) 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	// Configure the pins for the receiver and transmitter
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
/***
*			Imprime na serial a string no formato CSV 
*			
***/
void printCSV(uint16_t *string, int len){
	int i = 0;
	int j = 0;
	// Buffer para armazenar a string do valor.
  char buffer1[10];
	char buffer2[10];
	while(j<=(len-1)){
		
    // Converter o valor de 16 bits para string.
		uint16_to_string(j, buffer1,10);
    uint16_to_string(string[j],buffer2,10);
		if(i%2 == 0){
			
			UART0_SendString(buffer1);
			UARTCharPut(UART0_BASE, ';');
		}else{
			UART0_SendString(buffer2);
			//UARTCharPut(UART0_BASE, string[i]);
			UARTCharPut(UART0_BASE,'\n');
			j++;
		}
		i++;
	}
	UARTCharPut(UART0_BASE, '\n');
}
/***
*			Imprime na serial a string 
*			
***/
void UART0_SendString(const char *str) {
    // Enviar a string pela UART caractere por caractere.
    while (*str) {
        UARTCharPut(UART0_BASE, *str++);
    }
}
/***
*			Converte um valor uint16_t para string
*			
***/
void uint16_to_string(uint16_t value, char *buffer, size_t buffer_size) {
    // Limita o tamanho do buffer para evitar overflow
    if (buffer_size < 6) {
        return; // Tamanho insuficiente do buffer
    }

    // Converte o número para string
    int index = 0;
    char temp_buffer[6];  // Para armazenar os dígitos temporariamente

    // Trata o valor 0 separadamente
    if (value == 0) {
        temp_buffer[index++] = '0';
    } else {
        // Extrai cada dígito do valor
        while (value > 0) {
            temp_buffer[index++] = (value % 10) + '0';
            value /= 10;
        }
    }

    // Reverte a string para a ordem correta
    for (int i = 0; i < index; i++) {
        if (i >= buffer_size - 1) {
            break; // Evita overflow do buffer
        }
        buffer[i] = temp_buffer[index - i - 1];
    }
    buffer[index] = '\0'; // Adiciona o caractere nulo de término
}


