#pragma once

void extensios_check();


float arrayAverage(const float* array, int len);
float calcDeviation(const float* array, int len, const float average);
float calcCorrelation(const float* x, const float* y, int len);

// SIMD version

float arrayAverageSIMD128(const float* arr, int len);
float calcDeviationSIMD128(float* arr, int len, float average);
float calcCorrelationSIMD128(float* x, float* y, int len);

float arrayAverageSIMD256(const float* arr, int len);
float calcDeviationSIMD256(float* arr, int len, float average);
float calcCorrelationSIMD256(float* x, float* y, int len);