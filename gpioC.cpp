#include "gpioC.h"


#define PCM2708_PERI_BASE 	0x20000000
#define GPIO_BASE		(PCM2708_PERI_BASE + 0x200000) 	

#define PAGE_SIZE 	(4*1024)
#define BLOCK_SIZE 	(4*1024)

using namespace std;

gpioC::gpioC(unsigned int pin) {
	if(this->gpioDirectAccessInit()){
		cout << "gpioC::gpioC:: Error during init" << endl;
	}
	if(pin < 7 || 11 < pin) {
		cout << "gpioC::gpioC:: Pin should be between 7 & 11 " << endl;
		return;
	} 
	this->pin = pin;
	this->gpioAsInput();
	this->gpioAsOutput();

}


gpioC::~gpioC() {

}


int gpioC::gpioDirectAccessInit() {

	/* Set up direct GPIO register access */
	
	if((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		cout << "gpioC::Init:: Cannot open dev/mem" << endl;
		return -1;
	}
	
	gpio_map = mmap(	
			NULL,				/* Address in space - any */
			BLOCK_SIZE,			/* Map length */
			PROT_READ | PROT_WRITE,		/* READ & WRITE memory */
			MAP_SHARED,			/* Shared with othre processes */
			mem_fd,				/* File to map */
			GPIO_BASE 			/* Offset to GPIO */
		);

	if(gpio_map == MAP_FAILED) {
		cout << "gpioC::Init:: Mmap Error " << endl;
		return -1;
	}

	gpio = (volatile unsigned*)gpio_map;

	return 0;
}


void gpioC::gpioAsInput() {
	*(gpio+(pin/10)) &= ~(7<<((pin%10)*3));
}


void gpioC::gpioAsOutput() {
	*(gpio+(pin/10)) |= (1<<((pin%10)*3));
}

void gpioC::gpioSet() {
	*(gpio+7) = 1 << pin;
}

void gpioC::gpioClear() {
	*(gpio+10) = 1 << pin;	
}

int gpioC::gpioPWMInit(timeStampC *measurePtr, double freq, unsigned int duty) {
	if(freq > 0) {
		this->freq = freq;
	} else {
		cout << "gpioC::gpioPWMInit:: Frequency must be higher then 0 " << endl;
		return 0;
	}
	if(duty >= 0 && duty <= 100) {
		this->duty = duty;
	} else {
		cout << "gpioC::gpioPWMInit:: Duty must be between 0-100% " << endl;
		return 0;
	}
	this->measurePtr = measurePtr;
	return 1;	
}

void gpioC::gpioPWMStart() {
	double period = 1 / freq;

	double timeOn  = (( period * duty ) / 100) * 1000000;
	double timeOff = (( period * (100 - duty)) / 100) * 1000000;

	cout << "DEbug timeon " << timeOn << " us"<< endl;
	gpioExportPin(this->pin);
	
	double timeOnX  = timeOn * 1000 * 4; // us->ns & (4) procesor tics scaling
	double timeOffX = timeOff * 1000 * 4;

	RTIME timeOnX_tic = (RTIME)rt_timer_ns2tsc(timeOnX);
	RTIME timeOffX_tic = (RTIME)rt_timer_ns2tsc(timeOffX);

	int count =50;

	while(count) {
		gpioSet();
		//while(!gpioReadPin(this->pin));
		rt_task_sleep(timeOnX_tic);
		gpioClear();
		//while(gpioReadPin(this->pin));
		rt_task_sleep(timeOffX_tic);
		//count--;
	}
	gpioUnExportPin(this->pin);
}

void gpioC::gpioPWMStart2() {
	double period = 1 / freq;
	double timeOn  = (( period * duty ) / 100) * 1000000;
	double timeOff = (( period * (100 - duty)) / 100) * 1000000;

	cout << "DEbug timeon " << timeOn << " us"<< endl;
	gpioExportPin(this->pin);

	int count =50;

	while(count) {
		gpioSet();
		//while(!gpioReadPin(this->pin));
		usleep(timeOn);
		gpioClear();
		//while(gpioReadPin(this->pin));
		usleep(timeOff);
		//count--;
	}

	gpioUnExportPin(this->pin);
}

int gpioC::gpioReadPin(unsigned int pin) {
	char path[30];
	char value_str[3];
	int fd;

	snprintf(path, 30, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if(-1 == fd) {
		cout << "gpioC::gpioReadPin:: Cannot open file gpio" << endl;
		return -1;
	}
	if(-1 == read(fd,value_str, 3)) {
		cout << "gpioC::gpioReadPin:: Failed to read value from file" << endl;
		return -1;
	}
	close(fd);
	return(atoi(value_str));
}

int gpioC::gpioExportPin(unsigned int pin) {
	char buffer[3];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(-1 == fd) {
		cout << "gpioC::gpioExportPin:: Cannot open file gpio" << endl;
		return -1;
	}
	bytes_written = snprintf(buffer, 3, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return 0;
}

int gpioC::gpioUnExportPin(unsigned int pin) {
	char buffer[3];
	ssize_t bytes_written;
	int fd;
	cout << "gpioC::gpioExportPin" << endl;
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if(-1 == fd) {
		cout << "gpioC::gpioExportPin:: Cannot open file gpio" << endl;
		return -1;
	}
	bytes_written = snprintf(buffer, 3, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return 0;
}











