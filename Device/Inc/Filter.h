//
// Created by yang on 2026/3/24.
//

#ifndef MOTOR_FILTER_H
#define MOTOR_FILTER_H
#include "main.h"

typedef struct {
    float alpha;          // 滤波系数
    float last_output;    // 上一次输出值 Y[n-1]
    uint8_t initialized;  // 初始化标志
} LowPassFilter;

typedef struct {
    float prev_1; // 上一拍
    float prev_2; // 上上拍
} MedianFilter3;

float median_filter_3(MedianFilter3* filter, float new_value);
void lpf_init(LowPassFilter* filter, float alpha);
float lpf_update(LowPassFilter* filter, float new_input);
#endif //MOTOR_FILTER_H