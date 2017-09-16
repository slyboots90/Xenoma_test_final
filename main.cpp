/*
 * main.cpp
 *
 *  Created on: 02-02-2016
 *      Author: asus
 */

#include <iostream>
#include <native/task.h>
#include <native/timer.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "pwmC.h"
#include "gpioC.h"
#include <stdlib.h>
#include <time.h>
#include "timeStampC.h"
#include "sortC.h"
#include "mathC.h"


#define TASK_PRIO 99
/* Highest RT priority */
#define TASK_MODE T_FPU|T_CPU(1)
/* Uses FPU, bound to CPU #0 */
//#define TASK_STKSZ 4096
//#define TASK_STKSZ 8192
#define TASK_STKSZ 32768
/* Stack size (in bytes) */
RT_TASK task_desc1;
RT_TASK task_desc2;
//#define TASK_MODE T_JOINABLE

double freq_z;
unsigned int duty_z;
unsigned int res_z = 256;
timeStampC *measurePtr;

void setMeasurePtr(timeStampC *measure) {
	measurePtr = measure;
}

void getStamp(char* description) {
	if(measurePtr != 0) {
		measurePtr->getTimeStamp(description);
	} else {
		cout << "Main::getStamp:: Measure pointer not exist" << endl;
	}
}

void task_body(void *cookie) {
	for (int i=0; i<10;i++) {
		std::cout << "I'm here" << std::endl;
		/* ... "cookie" should be NULL ... */
	}
}

void pwmThread(void *thread_id) {
	std::cout << "pwmThread started" << std::endl;
	rpiPWM1 pwm(freq_z, res_z, (double)duty_z, rpiPWM1::MSMODE);
//	rpiPWM1 pwm(1000.0, 256, 80.0, rpiPWM1::MSMODE);
	double dcycle = 0.0;
	while(dcycle < 99.99) {
		pwm.setDutyCycle(dcycle);
		dcycle += 1;
		usleep(200);
	}
}

void gpioThread(void *thread_id) {
	std::cout << "gpioThread started" << std::endl;
	gpioC test(7);
	if(test.gpioPWMInit(measurePtr,freq_z, duty_z)) {
		test.gpioPWMStart();
	}
}


int init_module(void) {
	int err1, err2;
	/* ... */
	/*err1 = rt_task_create(&task_desc1, "HardPWM", TASK_STKSZ, TASK_PRIO,
	TASK_MODE);
	std::cout << "debug2" << err1 << std::endl;*/
	err2 = rt_task_create(&task_desc2, "SoftPWM", TASK_STKSZ, TASK_PRIO,
	TASK_MODE);
//Start hardware PWM
	/*if (!err1) {
		std::cout << "debug4" << std::endl;
		rt_task_start(&task_desc1, &pwmThread, NULL);
	}*/
//Start software PWM
	if(!err2) {
		std::cout << "debug5" << std::endl;
		rt_task_start(&task_desc2, &gpioThread, NULL);
	}
	return 1;
}
void cleanup_module(void) {
	//rt_task_delete(&task_desc1);
	rt_task_delete(&task_desc2);
}


void *gpioPosixThread(void *thread_id) {
	gpioC test(8);
	if(test.gpioPWMInit(measurePtr,freq_z, duty_z)) {
		test.gpioPWMStart2();
	}

	pthread_exit((void*)thread_id);
}


void *bubbleSortThread(void *thread_id) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	sortC sort(thread_id);
	while(1) {
		sort.setTabSize();
		sort.fillTab();
		sort.bubbleSort();
	}
	pthread_exit((void*)thread_id);
	return NULL;
}

void *insertionSortThread(void *thread_id) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	sortC sort(thread_id);
	while(1) {
		sort.setTabSize();
		sort.fillTab();
		sort.insertionSort();
	}
	pthread_exit((void*)thread_id);
	return NULL;
}

void *permutationsThread(void *thread_id) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	mathC perm(thread_id);
	while(1) {
		perm.createArray();
		perm.permutation();
	}
	pthread_exit((void*)thread_id);
	return NULL;
}

void *trigonometryThread(void *thread_id) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	mathC sintan(thread_id);
	while(1) {
		sintan.trigonometry();
	}
	pthread_exit((void*)thread_id);
	return NULL;
}

int main(int argc, char* argv[]) {

	std::cout << "Podaj wypelnienie " << endl;
	std::cin >> duty_z;
	std::cout << "Podaj f [Hz]" << endl;
	std::cin >> freq_z;
	std::cout << "Podaj rozdzielczość (256)" << endl;
	std::cin >> res_z;


	/* Time Measure Init */
	timeStampC measure;
	setMeasurePtr(&measure);
	measure.createOutputFile();

	pthread_t 	threadsHP;
	pthread_t 	threadsOB1;
	pthread_t 	threadsOB2;
	pthread_t 	threadsOB3;
	pthread_t 	threadsOB4;
	pthread_attr_t 	attrHP;
	pthread_attr_t 	attrLP;

	pthread_attr_init(&attrHP);
	pthread_attr_init(&attrLP);

	int 	sched_policy = SCHED_FIFO;
	int 	priomax = sched_get_priority_max(sched_policy);
	int 	priomin = sched_get_priority_min(sched_policy);

	struct sched_param spLP;
	struct sched_param spHP;
	pthread_attr_setinheritsched(&attrLP, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attrLP, sched_policy);
	pthread_attr_setinheritsched(&attrHP, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attrHP, sched_policy);
	//spLP.sched_priority = priomin;  // for overload threads
	spLP.sched_priority = priomax /2; // for overload threads
	spHP.sched_priority = priomax;
	pthread_attr_setschedparam(&attrLP, &spLP);
	pthread_attr_setschedparam(&attrHP, &spHP);


	void 		*status;

	// Software POSIX PWM
	pthread_create(&threadsHP, &attrHP, gpioPosixThread, (void *)0);
	// Overload threads
	pthread_create(&threadsOB1, &attrLP, bubbleSortThread, (void *)1);
	pthread_create(&threadsOB2, &attrLP, insertionSortThread, (void *)2);
	pthread_create(&threadsOB3, &attrLP, permutationsThread, (void *)3);
	pthread_create(&threadsOB4, &attrLP, trigonometryThread, (void *)4);

	measure.getTimeStamp("Start");

	mlockall(MCL_CURRENT|MCL_FUTURE);

	std::cout << "Hello Pi 2" << std::endl;
	//Xenomai threads
	if(init_module()) {
		std::cout << "Init done" << std::endl;
	}

	//Run until type a char in console
	char a;
	std::cin >> a;
	cleanup_module();
	measure.getTimeStamp("Finish");
	measure.saveAllTimeStamps();
	measure.closeOutputFile();

	return 0;
}




