/*
 * @file    key.c
 * @brief   按键检测模块实现
 * @details 包含按键扫描和防抖算法
 * @author  yang
 * @date    2025-12-11
 * @note    实现了按键消抖和状态检测功能
 */

#include "key.h"

/**
 * @brief 按键扫描函数，带消抖功能
 * @param None
 * @retval uint8_t 按键状态，返回按键按下对应的值
 * 
 * @details
 * 该函数实现按键扫描和消抖功能，通过静态变量key_up跟踪按键释放状态，
 * 防止按键按下期间重复触发。当检测到按键按下时，延时5ms进行硬件消抖，
 * 然后判断具体是哪个按键被按下并返回相应值。
 */
uint8_t key_scan(void)
{
    uint8_t key_val = 0;              /* 按键返回值，0表示无按键，其他值对应不同按键 */
    static uint8_t key_up = 1;        /* 按键释放标志，1表示已释放，0表示按下中 */
    /* 检测是否有按键按下且之前处于释放状态 */
    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0))
    {
        HAL_Delay(5);                 /* 延时5ms进行硬件消抖 */
        key_up = 0;                   /* 设置按键为按下状态 */
        /* 判断具体是哪个按键被按下 */
        if (KEY0 == 0)
        {
            key_val = KEY0_PRES;      /* 按键0被按下 */
        }
        else if (KEY1 == 0)
        {
            key_val = KEY1_PRES;      /* 按键1被按下 */
        }
        else if (KEY2 == 0)
        {
            key_val = KEY2_PRES;      /* 按键2被按下 */
        }
    }
    /* 当所有按键都释放时，恢复按键释放标志 */
    else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
    {
        key_up = 1;                   /* 设置按键为释放状态，允许下次按键响应 */
    }
    return key_val;                   /* 返回按键状态值 */


}








