#ifndef SMART_LOCK_H
#define SMART_LOCK_H

#include "stm32f4xx.h"

// ==== LCD Pins (GPIOA) ====
#define LCD_RS_PIN     0
#define LCD_EN_PIN     1
#define LCD_D4_PIN     4
#define LCD_D5_PIN     5
#define LCD_D6_PIN     6
#define LCD_D7_PIN     7

// ==== Keypad Pins (GPIOB) ====
#define ROW0_PIN       0
#define ROW1_PIN       1
#define ROW2_PIN       2
#define ROW3_PIN       3
#define COL0_PIN       4
#define COL1_PIN       5
#define COL2_PIN       6

// ==== IR Sensor Pin (GPIOC) ====
#define IR_SENSOR_PIN  0

// ==== Function Prototypes ====
void delay_ms(uint32_t ms);
void lcd_command(uint8_t cmd);
void lcd_data(char data);
void lcd_init(void);
void lcd_print(char *str);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);

char keypad_getkey(void);
void keypad_init(void);

void gpio_init(void);

#endif
