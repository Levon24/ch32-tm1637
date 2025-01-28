#include "debug.h"
#include "tm1637.h"

/* Global define */


/* Global Variable */

void GPIO_Ports_Init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio_InitStructure = {0};
  gpio_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  gpio_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &gpio_InitStructure);
}

int main(void) {
  u16 counter = 0;
  u8 segments[TM1637_LENGTH] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d};

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
  printf("GPIO TM1637 TEST\r\n");
    
  GPIO_Ports_Init();
  
  while (1) {
    Delay_Ms(250);
    
    counter++;
    //tm1637_write_segments(segments);
    tm1637_write_int(counter);
  }
}
