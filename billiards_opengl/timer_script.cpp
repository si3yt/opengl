#include "billiards_header.h"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::restart() {
	start = timeGetTime();
}

double Timer::elapsed() {
	double end = timeGetTime();
	return (end - start) / 1000;
}