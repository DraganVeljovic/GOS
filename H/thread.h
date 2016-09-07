#ifndef _THREAD_H_
#define _THREAD_H_

class PCB; // Kernel's implementation of a user's thread

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2; // celobrojni umnozak 55ms
typedef int ID;
typedef char* TName;

extern void dispatch();


class Thread {
public:
	
	void start(); 
	int waitToComplete(); 
	int wakeUp(); 
	static int sleep(Time timeToSleep);
	
	ID getId(); 
	TName getName(); 
	static Thread* getThreadById(ID id); 
	static ID getIdOf(TName name); 
	
	
	virtual ~Thread(); 

protected:

	friend class PCB;
	
	Thread (TName name, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice); 
	virtual void run() {} 
	
	
private:
	PCB* myPCB;
};



#endif