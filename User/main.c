#include "debug.h"

/* Global define */
#define LED7SEG_CLK GPIO_Pin_0
#define LED7SEG_DIO GPIO_Pin_1

/* Global Variable */

void GPIO_Toggle_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

int main(void) {
  u8 i = 0;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  SystemCoreClockUpdate();
  Delay_Init();

#if (SDI_PRINT == SDI_PR_OPEN)
  SDI_Printf_Enable();
#else
  USART_Printf_Init(115200);
#endif
  printf("SystemClk: %d\r\n", SystemCoreClock);
  printf("ChipID: %08x\r\n", DBGMCU_GetCHIPID());
  printf("GPIO Toggle TEST\r\n");
    
  GPIO_Toggle_INIT();

  while (1) {
    Delay_Ms(250);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
  }
}
