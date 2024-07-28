/*##############################################################################
Prof. Paulo Denis Garcez da Luz - 2023/2S
Desenvolvido para a placa EK-TM4C1294XL utilizando o SDK TivaWare no KEIL
Programa exemplo de blink(1s) usando apenas 1 led e Periférico SysTick.
##############################################################################*/

//TivaWare uC: Usado internamente para identificar o uC em alguns .h da TivaWare
#define PART_TM4C1294NCPDT 1

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

//variável que conta os ticks(1ms) - Volatile não permite o compilador otimizar o código 
static volatile unsigned int SysTicks;

//Protótipos de funções criadas no programa, código depois do main
void SetupSystick(void);
void SysTickIntHandler(void);

int main(void)
{
  //estado do led
  bool Led=false;
  //Configura o clock para utilizar o xtal interno de 16MHz com PLL para 40MHz
  SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320, 40000000);
  //habilitar gpio port N
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  //aguardar o periférico ficar pronto para uso
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)) {/*espera habilitar o port*/}
  //configura o pin_0 como saída
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
  //executa configuração e inicialização do SysTick
  SetupSystick();
  //loop infinito
  while (1)
        {
          //se já passou 1000ms = 1s, alterar o estado do led
          if (SysTicks>=1000)
             {
               //zera contagem do SysTick (uso exclusivo para o led)
               SysTicks=0;
               //inverte estado do led
               Led=!Led;
               //envia o estado do led para o pin 0 do port N
               GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, Led);
             }
        }
}

//função de tratamento da interrupção do SysTick
void SysTickIntHandler(void)
{
  SysTicks++;
}

//função para configurar e inicializar o periférico Systick a 1ms
void SetupSystick(void)
{
  SysTicks=0;
  //desliga o SysTick para poder configurar
  SysTickDisable();
  //clock 40MHz <=> SysTick deve contar 1ms=40k-1 do Systick_Counter - 12 trocas de contexto PP->IRQ - (1T Mov, 1T Movt, 3T LDR, 1T INC ... STR e IRQ->PP já não contabilizam atrasos para a variável)  
  SysTickPeriodSet(40000-1-12-6);
  //registra a função de atendimento da interrupção
  SysTickIntRegister(SysTickIntHandler);
  //liga o atendimento via interrupção
  SysTickIntEnable();
  //liga novamente o SysTick
  SysTickEnable();
}
