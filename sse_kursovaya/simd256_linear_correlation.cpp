#include <xmmintrin.h>
#include <cmath>
#include <immintrin.h>

const int FLOATS_8 = 8;

float arrayAverageSIMD256(const float* arr, int len) {
	if (len == 0)
		return 0;

	float result = 0;
	int rounded_len = len / FLOATS_8;

	if (len >= FLOATS_8) {
		float results[FLOATS_8];
		__m256 v1, v2;
		v1 = _mm256_load_ps(arr);
		for (int i = 1; i < rounded_len; ++i) {
			v2 = _mm256_load_ps(arr + i * FLOATS_8);
			v1 = _mm256_add_ps(v1, v2);
		}
		_mm256_store_ps(results, v1);
		for (int i = 0; i < FLOATS_8; ++i) {
			result += results[i];
		}
	}

	for (int i = rounded_len * FLOATS_8; i < len; ++i) {
		result += arr[i];
	}

	return result / len;
}

float calcDeviationSIMD256(float* arr, int len, float average) {
	float sum = 0, tmp;
	int rounded_len = len / FLOATS_8;
	__m256 v1, v2, v3, v4, v5;
	float results[FLOATS_8];
	v1 = _mm256_set1_ps(average);
	v5 = _mm256_set1_ps(0.f);
	for (int i = 0; i < rounded_len; ++i) {
		v2 = _mm256_load_ps(&arr[i * FLOATS_8]);
		v3 = _mm256_sub_ps(v2, v1);
		v4 = _mm256_mul_ps(v3, v3);
		v5 = _mm256_add_ps(v5, v4);
	}
	_mm256_store_ps(results, v5);
	for (int i = 0; i < FLOATS_8; ++i) {
		sum += results[i];
	}
	for (int i = rounded_len * FLOATS_8; i < len; ++i) {
		tmp = (arr[i] - average);
		sum += tmp * tmp;
	}

	return sqrt(1.f / (len - 1.f) * sum);
}

float calcCorrelationSIMD256(float* x, float* y, int len) {
	float xAverage = arrayAverageSIMD256(x, len);
	float yAverage = arrayAverageSIMD256(y, len);

	float xDeviation = calcDeviationSIMD256(x, len, xAverage);
	float yDeviation = calcDeviationSIMD256(y, len, yAverage);

	float sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += (x[i] - xAverage) * (y[i] - yAverage);
	}
	return (1.f / (len - 1.f)) * sum / xDeviation / yDeviation;
}