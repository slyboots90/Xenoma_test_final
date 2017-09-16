#include "mathC.h"
#include <algorithm>
#include <cstdlib>

using namespace std;

mathC::mathC(void *t) {

}

mathC::~mathC() {

}

void mathC::trigonometry() {
	double result = 0.0;
	for(int i=0; i < 10000; i++) {
		result = result + sin(i) * tan(i);
		result /= 1.788;
	}
}

void mathC::createArray() {
	vint.push_back(5);
	vint.push_back(8);
	vint.push_back(3);
	vint.push_back(7);
	vint.push_back(4);
	vint.push_back(9);
	vint.push_back(1);
	vint.push_back(6);
	vint.push_back(2);
	sort(vint.begin(), vint.end());
}

void mathC::permutation() {
	while(next_permutation(vint.begin(), vint.end()));
}
