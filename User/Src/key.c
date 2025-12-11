//
// Created by yang on 2025/12/11.
//

#include "key.h"

uint8_t key_scan(void)
{
    uint8_t key_val = 0;
    static uint8_t key_up = 1;
    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0))
    {
        HAL_Delay(5);
        key_up = 0;
        if (KEY0 == 0)
        {
            key_val = KEY0_PRES;
        }
        else if (KEY1 == 0)
        {
            key_val = KEY1_PRES;
        }
        else if (KEY2 == 0)
        {
            key_val = KEY2_PRES;
        }
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
    {
        key_up = 1;
    }
    return key_val;


}








