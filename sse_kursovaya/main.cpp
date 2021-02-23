#include <iostream>
#include "header.h"
#include <xmmintrin.h>
#include <ctime>
#include <functional>

void print_array4f(const float* arr) {
	printf("[");
	for (int j = 0; j < 3; ++j) {
		printf("%f ", arr[j]);
	}
	printf("%f]", arr[3]);
}

typedef void (*func)();

int runTest(const std::function<void()> f, int repeats) {
	clock_t fTimeStart, fTimeStop;
	fTimeStart = clock() * 1000 / CLOCKS_PER_SEC;
	for (int i = 0; i < repeats; ++i) {
		f();
	}
	fTimeStop = clock() * 1000 / CLOCKS_PER_SEC;
	return fTimeStop - fTimeStart;
}

void fill_random(float* r, int l) {
	for (int i = 0; i < l; ++i) {
		r[i] = rand() / 1000;
	}
}

void runTests() {
	// взаимосвязь двух переменных агрессивности и IQ у школьников
	float x[14] = { 24, 27, 26, 21, 20, 31, 26, 22, 20, 18, 30, 29, 24, 26 };
	float y[14] = { 100, 115, 117, 119, 134, 94, 105, 103, 111, 124, 122, 109, 110, 86 };

	// малая отрицательная корреляция чем выше агресивность тем ниже IQ
	printf("Correlation without SIMD usage: %f\n", calcCorrelation(x, y, sizeof(x) / sizeof(float))); // -0.4211
	printf("Correlation with SIMD 128 usage:    %f\n", calcCorrelationSIMD128(x, y, sizeof(x) / sizeof(float)));
	printf("Correlation with SIMD 256 usage:    %f\n\n", calcCorrelationSIMD256(x, y, sizeof(x) / sizeof(float)));

	const int RUNS = 100;
	const int SIZE = 1000000;
	float *test_x = new float[SIZE];
	float *test_y = new float[SIZE];
	fill_random(test_x, SIZE);
	fill_random(test_y, SIZE);

	int time1 = runTest([&]() {
		calcCorrelation(test_x, test_y, SIZE);
	}, RUNS);

	int time2 = runTest([&]() {
		calcCorrelationSIMD128(test_x, test_y, SIZE);
	}, RUNS);

	int time3 = runTest([&]() {
		calcCorrelationSIMD256(test_x, test_y, SIZE);
	}, RUNS);

	printf("Speed test %i runs for %i values:\n", RUNS, SIZE);
    printf("Real time simple verison %i(ms)\n", time1);
    printf("Real time simd 128 verison %i(ms), acceleration: %.1f\n", time2, float(time1) / time2);
    printf("Real time simd 256 verison %i(ms), acceleration: %.1f\n", time3, float(time1) / time3);
}

int main() {    
	runTests();
	/* Output:
	Correlation without SIMD usage: -0.421092
	Correlation with SIMD 128 usage:    -0.421092
	Correlation with SIMD 256 usage:    -0.421092

	Speed test 100 runs for 1000000 values:
	Real time simple verison 1532(ms)
	Real time simd 128 verison 822(ms), acceleration: 1.9
	Real time simd 256 verison 665(ms), acceleration: 2.3
	*/
}
