//
// Created by yang on 2026/3/24.
//

#include "../Inc/Filter.h"
// 简单的3点中值滤波
// 输入：current_raw (当前采样), prev_1 (上一拍), prev_2 (上上拍)
// 注意：实际工程中通常维护一个长度为3的环形缓冲区
float median_filter_3(MedianFilter3* filter, float new_value)
{
    float prev_1 = filter->prev_1;
    float prev_2 = filter->prev_2;
    float current_raw = new_value;

    // 更新滤波器状态
    filter->prev_2 = prev_1; // 上上拍更新为上一拍
    filter->prev_1 = current_raw; // 上一拍更新为当前采样)
    if ((current_raw >= prev_1 && current_raw <= prev_2) || (current_raw <= prev_1 && current_raw >= prev_2)) return
        current_raw;
    if ((prev_1 >= current_raw && prev_1 <= prev_2) || (prev_1 <= current_raw && prev_1 >= prev_2)) return prev_1;
    return prev_2;
}


void lpf_init(LowPassFilter* filter, float alpha)
{
    filter->alpha = alpha;
    filter->last_output = 0.0f;
    filter->initialized = 0;
}

float lpf_update(LowPassFilter* filter, float new_input)
{
    if (!filter->initialized)
    {
        // 第一次运行时，直接让输出等于输入，避免启动突变
        filter->last_output = new_input;
        filter->initialized = 1;
        return new_input;
    }

    // 核心公式：Y[n] = α * X[n] + (1-α) * Y[n-1]
    // 优化写法：Y[n] = Y[n-1] + α * (X[n] - Y[n-1])
    // 这种写法在某些定点数DSP上精度更高
    filter->last_output = filter->last_output + filter->alpha * (new_input - filter->last_output);

    return filter->last_output;
}
