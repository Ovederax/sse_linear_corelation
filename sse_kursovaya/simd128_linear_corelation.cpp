#include <xmmintrin.h>
#include <cmath>

const int FLOATS = 4;

float arrayAverageSIMD128(const float* arr, int len) {
	if (len == 0)
		return 0;

	float result = 0;
	int rounded_len = len / FLOATS;

	if (len >= FLOATS) {
		float results[FLOATS];
		__m128 v1, v2;
		v1 = _mm_load_ps(arr);
		/*_asm {
			mov         eax, dword ptr[arr]
			movups      xmm0, xmmword ptr[eax]
			movaps      xmmword ptr[ebp - 130h], xmm0
		}*/
		for (int i = 1; i < rounded_len; ++i) {
			v2 = _mm_load_ps(arr + i * FLOATS);
			v1 = _mm_add_ps(v1, v2);
			/*_asm {
				mov         eax, dword ptr[ebp - 3Ch]
				shl         eax, 2
				mov         ecx, dword ptr[arr]
				movups      xmm1, xmmword ptr[ecx + eax * 4]
				movaps      xmmword ptr [ebp-120h],xmm0
				addps xmm0, xmm1
			}*/
		}
		_mm_store_ps(results, v1);
		/*_asm {
			movups results[0], xmm0
		}*/
		result += results[0] + results[1] + results[2] + results[3];
	}

	for (int i = rounded_len * FLOATS; i < len; ++i) {
		result += arr[i];
	}

	return result / len;
}

float calcDeviationSIMD128(float* arr, int len, float average) {
	float sum = 0, tmp;
	int rounded_len = len / FLOATS;
	__m128 v1, v2, v3, v4, v5;
	float results[FLOATS];
	v1 = _mm_set_ps1(average);
	v5 = _mm_set_ps1(0.f);
	for (int i = 0; i < rounded_len; ++i) {
		v2 = _mm_load_ps(&arr[i * FLOATS]);
		v3 = _mm_sub_ps(v2, v1);
		v4 = _mm_mul_ps(v3, v3);
		v5 = _mm_add_ps(v5, v4);
	}
	_mm_store_ps(results, v5);
	for (int i = 0; i < FLOATS; ++i) {
		sum += results[i];
	}
	for (int i = rounded_len * FLOATS; i < len; ++i) {
		tmp = (arr[i] - average);
		sum += tmp * tmp;
	}
	return sqrt(1.f / (len - 1.f) * sum);
}

float calcCorrelationSIMD128(float* x, float* y, int len) {
	float xAverage = arrayAverageSIMD128(x, len);
	float yAverage = arrayAverageSIMD128(y, len);

	float xDeviation = calcDeviationSIMD128(x, len, xAverage);
	float yDeviation = calcDeviationSIMD128(y, len, yAverage);

	float sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += (x[i] - xAverage) * (y[i] - yAverage);
	}
	return (1.f / (len - 1.f)) * sum / xDeviation / yDeviation;
}