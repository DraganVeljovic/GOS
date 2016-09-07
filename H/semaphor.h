#ifndef _SEMAPHOR_H
#define _SEMAPHOR_H


class KernelSem;

class Semaphore {
public: 
	Semaphore (int init = 1);
	~Semaphore ();
	
	virtual int wait ();
	virtual void signal ();
	
	int val () const;
private: 
	KernelSem* myImpl;
};

#endif