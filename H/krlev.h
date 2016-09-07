#ifndef _KERNELEV_H
#define _KERNELEV_H


#include <event.h>

class IVTEntry;

class KernelEv{
private:
	IVTEntry* entry;
	IVTNo ivtNumber;
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv ();
	
	int wait();
	void signal();

};

#endif