#pragma once
#define TESTDLLSORT_API __declspec(dllexport)

extern "C" {
	TESTDLLSORT_API void Test(char* c, int a);
}