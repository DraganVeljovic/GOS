#ifndef _MAINTHREAD_H
#define _MAINTHREAD_H

#include <pcb.h>
#include <thread.h>
#include <listPCB.h>
#include <stdio.h>

typedef int (*mainFunction)();

extern PCB* runningThreadPCB;
extern void mainWaitToComplete();
extern ListPCB* allThreadsPCB;


extern int userMain(int argc, char* argv[]);

class MainThread : public Thread{
	int argc;
	char** argv;
	int mainReturnValue;
public:
	MainThread (int argc, char** argv) : Thread(NULL)
	{
		this->argc = argc;
		this->argv = argv;
		mainReturnValue = 0;
	}
	
	~MainThread() 
	{
		waitToComplete();
	}
	
	void run()
	{
		mainReturnValue = userMain(argc,argv);
	}

	int getMainReturnValue()
	{
		return mainReturnValue;
	}
	
	friend class PCB;
};

#endif