#include "stm32f4xx.h"
#include "lcd.h"
#include "keypad.h"
#include "delay.h"
#include "smart_lock.h"

int main(void)
{
    SystemInit();
    LCD_Init();
    Keypad_Init();
    IR_Init();
    Delay_Init();

    LCD_Clear();
    LCD_Print("Waiting for user");

    while (1)
    {
        SmartLock_Run();
    }
}
