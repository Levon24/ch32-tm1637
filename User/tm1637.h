#ifndef _TM1637_H
#define _TM1637_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define LED7SEG_PORT GPIOC
#define LED7SEG_CLK GPIO_Pin_0
#define LED7SEG_DIO GPIO_Pin_1

#define TM1637_LENGTH 6

/* Exported Functions */
void tm1637_write_segments(const u8 *segments);
void tm1637_write_int(u16 value);


#ifdef __cplusplus
}
#endif

#endif