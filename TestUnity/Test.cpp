#include "Test.h"
#include <string>
#include <cstring>
#include <string.h>

extern "C" {
	void Test(char* c, int a) {
		//delete[] c;
		std::string str = std::to_string(a * a);
		//char str[] = { 'f','g','j','k'};
		int strl = str.length();
		//c = new char[strl];
		for (int i = 0; i < strl; i++) {
			c[i] = str[i];
		}
	}
}