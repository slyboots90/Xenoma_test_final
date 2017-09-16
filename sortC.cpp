#include "sortC.h"

#include <iostream>
#include <cstdlib>


using namespace std;

sortC::sortC(void *t) {

	tabSize = 0;
	tab = new int [0];
}

sortC::~sortC() {

}


void sortC::setTabSize(int x)	{
	tabSize = x;
	tab = new int [tabSize];
}
 
int sortC::getTabSize() {
	return tabSize;
}
 

void sortC::fillTab() {
	for(int i = 0; i < tabSize; i++) {
		tab[i] = rand() % 10000;
	}
}
 

void sortC::bubbleSort() {
	int shift , counter, temp;
 
	if(tabSize <= 1) {
		cout << "sortC::bubbleSort: Wrong range of table";
	}
	else{
		do {
			counter = tabSize-1;
			shift = 0;
			do {
				counter--;
 
				if(tab[counter] > tab[counter+1]) {                
					temp = tab[counter+1];
					tab[counter+1] = tab[counter];
					tab[counter] = temp;
					shift=1;
				}
			} while(counter!=0); 
		} while(shift!=0);  
	}
}

void sortC::insertionSort() {

	int i,j;
	int temp;
 
	if(tabSize <= 1) {
		cout << "sortC::insertionSort: Wrong range of table";
	}
	else{
		for (i = 1; i < tabSize; i++) {
			temp = tab[i];
			for (j = i-1; j >= 0; j--) {
				if (temp < tab[j])
					tab[j+1] = tab[j];
				else
					break;
			}
			tab[j+1] = temp;
		}
	}
}

void sortC::showResult() {
	cout << "sortC::showResult: ";
	for(int i = 0; i < tabSize; i++) {
		cout << tab[i] << " , ";
	}
cout << endl;
}


