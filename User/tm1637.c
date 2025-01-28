#include <debug.h>
#include "tm1637.h"

#define TM1637_BIT_DELAY 5

#define TM1637_WRITE_DATA_TO_DISPLAY 0x40
#define TM1637_REGISTER_ADDRESS 0xC0
#define TM1637_DISPLAY_CONTROL 0x80

const u8 tm1637_minus = 0x40;
const u8 tm1637_dot = 0x80;
const u8 tm1637_maping[] = {2, 1, 0, 5, 4, 3};
const u8 tm1637_digits[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

u8 tm1637_brightness = 0x08 | 0x07; // On + Brightness

/**
 * NOTE: on TM1637 reading keys should be slower than 250Khz (see datasheet p3)
 * for that reason the delay between reading bits should be more than 4us
 * When using a fast clock (such as ESP8266) a delay is needed to read bits correctly 
 */
void tm1637_delay() {
  Delay_Us(TM1637_BIT_DELAY);
}

/**
 * Start data sending
 */
void tm1637_start() {
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  tm1637_delay();
}

/**
 * Stop data sending
 */
void tm1637_stop() {
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
  tm1637_delay();
}

/**
 * Write data block
 * 
 * @param data 
 * @return acknowledge 
 */
u8 tm1637_write_byte(u8 data) {
  // Write 8 bit data
  for (u8 i = 0; i < 8; i++) {
    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
    tm1637_delay();
    
    if (data & 0x01) {
      GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
    } else {
      GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
    }
    tm1637_delay();

    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
    tm1637_delay();

    data = data >> 1;
  }

  // Wait for acknowledge
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
  tm1637_delay();
  
  u8 acknowledge = GPIO_ReadInputDataBit(LED7SEG_PORT, LED7SEG_DIO);
  if (acknowledge == 0) {
    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  }
  tm1637_delay();
  
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
  tm1637_delay();
  
  return acknowledge;
}

/**
 * Write data raw 
 * 
 * @param segments 
 * @param pos 
 */
void tm1637_write_segments(const u8 *segments) {
  // Write command #1: Write data to display register
  tm1637_start();
  tm1637_write_byte(TM1637_WRITE_DATA_TO_DISPLAY);
  tm1637_stop();
  
  // Write command #2: The command is used to set the display register address.
  tm1637_start();
  tm1637_write_byte(TM1637_REGISTER_ADDRESS);
  
  // write the data bytes
  for (u8 pos = 0; pos < TM1637_LENGTH; pos++) {
    u8 seg = tm1637_maping[pos];
    tm1637_write_byte(segments[seg]);
  }
  tm1637_stop();
  
  // Write command #2: Display control
  tm1637_start();
  tm1637_write_byte(TM1637_DISPLAY_CONTROL | tm1637_brightness);
  tm1637_stop();
}

/**
 * @brief 
 * 
 * @param value 
 */
void tm1637_write_int(u16 value) {
  char str[7];
  snprintf(str, sizeof(str) , "%6d", value);

  u8 segments[6] = {0};
  for (u8 pos = 0; pos < TM1637_LENGTH; pos++) {
    if (str[pos] == '-') {
      segments[pos] = tm1637_minus;
    } else if ((str[pos] >= '0') && (str[pos] <= '9')) {
      segments[pos] = tm1637_digits[str[pos] - 48];
    } else {
      segments[pos] = 0;
    }
  }

  tm1637_write_segments(segments);              
}

/**
 * @brief 
 * 
 * @param level 
 */
void tm1637_set_brightness(u8 level) {

  tm1637_brightness = 0x08 | (level & 0x07);
}