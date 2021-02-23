#include <cmath>

float arrayAverage(const float* array, int len) {
	if (len == 0)
		return 0;
	float result = 0;
	for (int i = 0; i < len; ++i) {
		result += array[i];
	}
	return result / len;
}

float calcDeviation(const float* array, int len, const float average) {
	float sum = 0;
	float tmp;
	for (int i = 0; i < len; ++i) {
		// sum += pow((array[i] - average), 2); // дает сумарно большую задержку по времени выполнения
		tmp = (array[i] - average);
		sum += tmp * tmp;
	}
	return sqrt(1.f / (len - 1.f) * sum);
}

float calcCorrelation(const float* x, const float* y, int len) {
	float xAverage = arrayAverage(x, len);
	float yAverage = arrayAverage(y, len);

	float xDeviation = calcDeviation(x, len, xAverage);
	float yDeviation = calcDeviation(y, len, yAverage);

	float sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += (x[i] - xAverage) * (y[i] - yAverage);
	}
	return (1.f / (len - 1.f)) * sum / xDeviation / yDeviation;
}