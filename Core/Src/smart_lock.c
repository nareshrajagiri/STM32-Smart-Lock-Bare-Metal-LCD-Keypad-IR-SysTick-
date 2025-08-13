#include "smart_lock.h"

// ==== Simple delay using SysTick ====
void delay_ms(uint32_t ms) {
    SysTick->LOAD = 16000 - 1;  // 1 ms delay at 16 MHz
    SysTick->VAL = 0;
    SysTick->CTRL = 5; // Enable SysTick, no interrupt
    for(uint32_t i = 0; i < ms; i++) {
        while(!(SysTick->CTRL & (1 << 16)));
    }
    SysTick->CTRL = 0;
}

// ==== LCD Helper ====
static void lcd_enable_pulse(void) {
    GPIOA->BSRR = (1 << LCD_EN_PIN);
    delay_ms(1);
    GPIOA->BSRR = (1 << (LCD_EN_PIN + 16));
    delay_ms(1);
}

static void lcd_send_nibble(uint8_t nibble) {
    if (nibble & 0x01) GPIOA->BSRR = (1 << LCD_D4_PIN); else GPIOA->BSRR = (1 << (LCD_D4_PIN + 16));
    if (nibble & 0x02) GPIOA->BSRR = (1 << LCD_D5_PIN); else GPIOA->BSRR = (1 << (LCD_D5_PIN + 16));
    if (nibble & 0x04) GPIOA->BSRR = (1 << LCD_D6_PIN); else GPIOA->BSRR = (1 << (LCD_D6_PIN + 16));
    if (nibble & 0x08) GPIOA->BSRR = (1 << LCD_D7_PIN); else GPIOA->BSRR = (1 << (LCD_D7_PIN + 16));
    lcd_enable_pulse();
}

void lcd_command(uint8_t cmd) {
    GPIOA->BSRR = (1 << (LCD_RS_PIN + 16)); // RS low
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    delay_ms(2);
}

void lcd_data(char data) {
    GPIOA->BSRR = (1 << LCD_RS_PIN); // RS high
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    delay_ms(2);
}

void lcd_init(void) {
    delay_ms(50);
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x02); // 4-bit mode

    lcd_command(0x28); // 4-bit, 2-line, 5x8 dots
    lcd_command(0x0C); // Display ON, cursor OFF
    lcd_command(0x06); // Entry mode
    lcd_command(0x01); // Clear display
}

void lcd_print(char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

void lcd_clear(void) {
    lcd_command(0x01);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_command(addr);
}

// ==== Keypad ====
void keypad_init(void) {
    // Rows as output
    for (int pin = ROW0_PIN; pin <= ROW3_PIN; pin++) {
        GPIOB->MODER |= (1 << (pin * 2));
        GPIOB->PUPDR &= ~(3 << (pin * 2));
    }
    // Columns as input with pull-down
    for (int pin = COL0_PIN; pin <= COL2_PIN; pin++) {
        GPIOB->MODER &= ~(3 << (pin * 2));
        GPIOB->PUPDR |= (2 << (pin * 2)); // Pull-down
    }
}

char keypad_getkey(void) {
    char keys[4][3] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'},
        {'*','0','#'}
    };
    for (int row = 0; row < 4; row++) {
        GPIOB->BSRR = (0x0F << 16);
        GPIOB->BSRR = (1 << (ROW0_PIN + row));
        delay_ms(1);
        for (int col = 0; col < 3; col++) {
            if (GPIOB->IDR & (1 << (COL0_PIN + col))) {
                while (GPIOB->IDR & (1 << (COL0_PIN + col)));
                return keys[row][col];
            }
        }
    }
    return 0;
}

// ==== GPIO Init ====
void gpio_init(void) {
    RCC->AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2); // GPIOA, GPIOB, GPIOC clock

    // LCD pins as output
    GPIOA->MODER |= (1 << (LCD_RS_PIN * 2)) | (1 << (LCD_EN_PIN * 2)) |
                    (1 << (LCD_D4_PIN * 2)) | (1 << (LCD_D5_PIN * 2)) |
                    (1 << (LCD_D6_PIN * 2)) | (1 << (LCD_D7_PIN * 2));

    // IR sensor as input
    GPIOC->MODER &= ~(3 << (IR_SENSOR_PIN * 2));
    GPIOC->PUPDR |= (1 << (IR_SENSOR_PIN * 2)); // Pull-up
}
