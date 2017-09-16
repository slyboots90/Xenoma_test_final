#ifndef MATHC_H_
#define MATHC_H_

#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

class mathC {
	
	public:
		mathC(void *t);
		virtual ~mathC();
		void trigonometry(void);
		void permutation(void);
		void createArray(void);
		
	private:
		vector <int> vint;
				

};


#endif
