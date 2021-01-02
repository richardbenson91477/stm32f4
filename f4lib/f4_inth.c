#include "f4lib.h"

void USART1_IRQHandler () {
    while (USART_GetITStatus (USART1, USART_IT_RXNE) != RESET) {
        uint8_t c = USART_ReceiveData (USART1);
        while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET)
            misc_udelay (1);
        USART_SendData (USART1, c);
        while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET)
            misc_udelay (1);
    }
}

void NMI_Handler () {
}

void HardFault_Handler ()
{
  while (1)
      ;
}

void MemManage_Handler ()
{
  while (1)
      ;
}

void BusFault_Handler ()
{
  while (1)
      ;
}

void UsageFault_Handler ()
{
  while (1)
      ;
}

void SVC_Handler () {
}

void DebugMon_Handler () {
}

void PendSV_Handler () {
}

void SysTick_Handler () {
}

void PPP_IRQHandler () {
}

