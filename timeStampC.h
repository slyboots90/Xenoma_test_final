#ifndef TIMESTAMPC_H_
#define TIMESTAMPC_H_

/* 
Struktura timeval przechowuje czas w sekundach i mikrosekundach.
Aby uzyskac czas w ms trzeba obie zmienne przeliczyc na odpowiednie jednostki.
*/

#include <sys/time.h>
#include <iostream>
#include <cstring>
#include <cstdio>

class timeStampC {

	public:
		timeStampC();
		timeStampC(unsigned int);
		virtual ~timeStampC();
		void setMeasureId(unsigned int);
		int getTimeStamp(char*);
		double getTotalDiffTime(void);
		double getStampValue(unsigned int);
		double getDiffTime(unsigned int,unsigned int);
		unsigned int getNumOfStamps(void);
		void saveAllTimeStamps(void);
		void createOutputFile(void);
		void closeOutputFile(void);

		const unsigned int NUM_OF_SMP;

	private:

		void showDiffTime(double);
		void generateHeader(FILE*);
		
		unsigned int measureId;

		struct timestampS {
			unsigned int stampId;
			timeval stampVal;
			char* stampD;
		} *timestampP;
		const timestampS *fstampP;
		FILE* ofile;

};


#endif
