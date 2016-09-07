#ifndef _KERNELSEM_H
#define _KERNELSEM_H


class ListPCB;

class KernelSem {
public:
	KernelSem (int i);
	~KernelSem ();
	
	virtual int wait ();
	virtual void signal ();
	
	int val () const;
	
	void incVal();
	
	void signalWhile (int limit);
	
//private:
	int value;
	ListPCB* blockedOnThisSem;
};

#endif