#include "timeStampC.h"


#define DEFAULT_NUM_OF_SMP 		5020

using namespace std;

timeStampC::timeStampC() : NUM_OF_SMP(DEFAULT_NUM_OF_SMP) {
	timestampP =  new  timestampS [NUM_OF_SMP];
	fstampP = timestampP;
}

timeStampC::timeStampC(unsigned int NumOfSmp) : NUM_OF_SMP(NumOfSmp) {
	timestampP =  new  timestampS [NUM_OF_SMP];
	fstampP = timestampP;
}

timeStampC::~timeStampC() {

}

void timeStampC::setMeasureId(unsigned int measureid) {
	measureId = measureid;
	fstampP = timestampP;
}

int timeStampC::getTimeStamp(char* descriprion) {
	if(timestampP->stampId < NUM_OF_SMP ) { 
		gettimeofday(&timestampP->stampVal, NULL);
		timestampP->stampD = descriprion;
		if(timestampP != fstampP) {
			timestampP->stampId = ((timestampP - 1)->stampId) + 1;
		} else {
			timestampP->stampId = 1;
		}
		timestampP++;
		return 1;
	}
	else {
		cout << "timeStampC::getTimeStamp:: Can't add new stamp - too much stamples" << endl;
		return 0;
	}
}

double timeStampC::getTotalDiffTime() {
	if((timestampP-1)->stampId > fstampP->stampId) {
		double diffT = getDiffTime(fstampP->stampId, (timestampP-1)->stampId);		
		showDiffTime(diffT);
		return diffT;
	}
	else {
		cout << "timeStampC::getTotalDiffTime:: Not enough stamples" << endl;
		return 0;
	}
}


double timeStampC::getStampValue(unsigned int stampid) {
	if(stampid > this->getNumOfStamps()) {
		cout << "timeStampC::getStampValue:: Stamp " << stampid << " doesn't exist" << endl;
		return 0;
	} else {
		timeval const *ptr = &(fstampP + stampid - 1)->stampVal;
		return (double)((ptr->tv_sec) * 1000.0 + (ptr->tv_usec / 1000.0));
	}
}

double timeStampC::getDiffTime(unsigned int stampA, unsigned int stampB) {
	stampA--;
	stampB--;
	if(stampA > stampB) {
		cout << "timeStampC::getDiffTime:: Stamp A must be before stamp B" << endl;
		return -1;
	} else if(stampA == stampB) {
		return 0;
	} else if(stampA > NUM_OF_SMP || stampB > NUM_OF_SMP) {
		cout << "timeStampC::getDiffTime:: Stamp cannot be higher then MAX_VAL ! (" << NUM_OF_SMP << ")" << endl;
		return -1;
	} else if(stampB > this->getNumOfStamps()) {
		cout << "timeStampC::getDiffTime:: Stamp doesn't exist ! " << (fstampP + stampB)->stampId << endl;
		return -1;
	} else {
		timeval const *stampAV = &((fstampP + stampA)->stampVal);
		timeval const *stampBV =  &((fstampP + stampB)->stampVal);
		double diffT = (stampBV->tv_sec - stampAV->tv_sec) * 1000.0;
		diffT += (stampBV->tv_usec - stampAV->tv_usec) / 1000.0;
		return diffT;	
	}
}

unsigned int timeStampC::getNumOfStamps() {
	return (timestampP - fstampP);
}

void timeStampC::showDiffTime(double diffT) {
	cout << "timeStampC::showDiffTime:: Diff time equals " << diffT << " ms" << endl;
}

void timeStampC::createOutputFile() {	
	char filename[11] =  "result.txt";
	ofile = fopen(filename, "w");
	if(!ofile) {
		cout << "timeStampC::createOutputFile:: Cannot open file to write" << endl;
		return;
	}
	this->generateHeader(ofile);
}

void timeStampC::closeOutputFile() {
	if(!ofile) {
		cout << "timeStampC::closeOutputFile:: No file to close" << endl;
		return;
	}
	fclose(ofile);
}

void timeStampC::saveAllTimeStamps() {
	if(!ofile) {
		cout << "timeStampC::saveAllTimeStamps:: File not exist" << endl;
		return;
	}
	char linebuff[100];
	timestampS const *ptr = fstampP;
	while(ptr != timestampP) {
		if(ptr != fstampP && ptr != timestampP-1) {
			snprintf(linebuff, 100, "%d %d %s %f \n", this->measureId, ptr->stampId, ptr->stampD, this->getDiffTime((ptr-1)->stampId, ptr->stampId)); 
		} else {
			snprintf(linebuff, 100, "%d %d %s %f \n", this->measureId, ptr->stampId, ptr->stampD, this->getDiffTime(fstampP->stampId, ptr->stampId)); 
		}
		fputs(linebuff,ofile);
		ptr++;
	}
}

void timeStampC::generateHeader(FILE* ofile) {
	fputs("measureId stampId stampDescription stampVal[ms] \n", ofile);
}


