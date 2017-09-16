#ifndef SORTC_H_
#define SORTC_H_


class sortC  {
	
	public:
		sortC(void *t);
		virtual ~sortC();
		void setTabSize(int = 1000);
		int getTabSize(void);
		void fillTab(void);
		void bubbleSort(void);
		void insertionSort(void);
		void showResult(void);
		
	private:
		int tabSize;
		int *tab;
};


#endif
