#ifndef _EVENT_H
#define _EVENT_H

typedef unsigned char IVTNo;

#include <ivtentry.h>

class KernelEv;

class Event {
public: 
	Event (IVTNo ivtNo);
	~Event();
	
	int wait();
	void signal();
	
private:
	KernelEv* myImpl;
};

#endif