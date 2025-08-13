#include "smart_lock.h"
#include "lcd.h"
#include "keypad.h"
#include "delay.h"
#include "stm32f4xx.h"

#define MAX_ATTEMPTS 4
#define PASSWORD "1234"

static uint8_t wrong_attempts = 0;

void SmartLock_Run(void)
{
    if (IR_UserDetected())
    {
        LCD_Clear();
        LCD_Print("Enter Password:");
        
        char entered_pass[5] = {0};
        Keypad_GetPassword(entered_pass);

        if (strcmp(entered_pass, PASSWORD) == 0)
        {
            LCD_Clear();
            LCD_Print("Access Granted");
            wrong_attempts = 0;
        }
        else
        {
            wrong_attempts++;
            LCD_Clear();
            LCD_Print("Access Denied");

            if (wrong_attempts < MAX_ATTEMPTS)
            {
                uint32_t wait_time = wrong_attempts * 10; // 10s, 20s, 30s
                LCD_SetCursor(1,0);
                LCD_Print("Wait ");
                LCD_PrintNum(wait_time);
                LCD_Print("s");
                Delay_Sec(wait_time);
            }
            else
            {
                LCD_Clear();
                LCD_Print("System Locked");
                while(1); // lock system
            }
        }
    }
}
