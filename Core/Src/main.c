#include "smart_lock.h"

#define PASSWORD "1234"

int main(void) {
    gpio_init();
    keypad_init();
    lcd_init();

    char entered[5] = {0};
    int attempt = 0;

    lcd_clear();
    lcd_print("Waiting for User");

    while (1) {
        if ((GPIOC->IDR & (1 << IR_SENSOR_PIN)) == 0) { // Active LOW
            lcd_clear();
            lcd_print("Enter Password:");
            lcd_set_cursor(1,0);
            int idx = 0;

            while (idx < 4) {
                char key = keypad_getkey();
                if (key) {
                    entered[idx++] = key;
                    lcd_data('*');
                }
            }
            entered[4] = '\0';

            if (strcmp(entered, PASSWORD) == 0) {
                lcd_clear();
                lcd_print("Access Granted");
                attempt = 0;
            } else {
                attempt++;
                if (attempt >= 4) {
                    lcd_clear();
                    lcd_print("System Locked!");
                    while (1);
                }
                int delay_s = attempt * 10;
                lcd_clear();
                lcd_print("Wait ");
                for (int i = delay_s; i > 0; i--) {
                    lcd_set_cursor(0,5);
                    char buf[4];
                    sprintf(buf, "%2d", i);
                    lcd_print(buf);
                    delay_ms(1000);
                }
            }
            lcd_clear();
            lcd_print("Waiting for User");
        }
    }
}
