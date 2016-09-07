#include <thread.h>
#include <pcb.h>
#include <SCHEDULE.H>
#include <ListPCB.h>
#include <ivtentry.h>
#include <krlsem.h>
#include <inter.h>
#include <dos.h>
#include <stdio.h>

extern ListPCB* sleepList;
extern ListPCB* blockedList;

extern void createProcess(PCB*, void (*body)());
extern void myDispatch();
extern void exitThread();

extern void interrupt timer();
extern int counter;

extern unsigned tsp;
extern unsigned tss;



Thread :: Thread (TName name, StackSize stackSize, Time timeSlice)
{	
	asm {
		pushf
		cli
	}
	
	myPCB = new PCB(name, stackSize, timeSlice, this);
	createProcess(myPCB,PCB :: runWrap);
	
	asm {
		popf
	}
}

Thread :: ~Thread()
{
	asm {
		pushf
		cli
	}
	
	waitToComplete();
	delete myPCB;
	
	asm {
		popf
	}
}

ID Thread :: getId ()
{
	return myPCB->getID();
}

TName Thread :: getName ()
{
	return myPCB->getName();
}

int Thread :: wakeUp()
{
	asm {
		pushf
		cli
	}
	
	if (sleepList->prazna())
		if (blockedList->prazna()) runningThreadPCB->wakeUpReturnValue = 0;
	
	PCB* p = sleepList->nadji(myPCB);
	if (p != NULL)
	{
		sleepList->izbaci(p);
		p->timeToSleep = 0;
		p->toSleep = 0;
		p->sleepReturnValue = 0;
		runningThreadPCB->wakeUpReturnValue = 1;
		Scheduler :: put(p);
	}
	else
	{	
		p = blockedList->nadji(myPCB);
		if (p != NULL)
		{
			blockedList->izbaci(p);
			if (p->sem)
			{
				p->sem->incVal();
				p->sem->blockedOnThisSem->izbaci(p);
				p->sem = NULL;
			}
			if (p->entry)
			{
				p->entry->incVal();
				p->entry->blockedOnThisSem->izbaci(p);
				p->entry = NULL;
			}
			p->blocked = 0;
			p->blockedOnSem = 0;
			p->waitToCompleteReturnValue = 0;
			p->waitReturnValue = 0;
			runningThreadPCB->wakeUpReturnValue = 1;
			Scheduler :: put(p);
		}
	}
	
	asm {
		popf
	}
	
	return runningThreadPCB->wakeUpReturnValue;
}



Thread* Thread :: getThreadById(ID id)
{
	return PCB :: getThreadById(id);
}


ID Thread :: getIdOf(TName name)
{
	return PCB :: getIdOf(name);
}


void Thread :: start ()
{
	asm {
		pushf
		cli
	}
	
	Scheduler::put (myPCB);
	
	asm {
		popf
	}
}


int Thread :: sleep (Time timeToSleep)
{
	asm {
		pushf
		cli
	}

	runningThreadPCB->timeToSleep = timeToSleep;
	runningThreadPCB->toSleep = 1;
	
	
	dispatch();
	
 	asm {
		popf
	}
	
	return runningThreadPCB->sleepReturnValue;
}

int Thread :: waitToComplete()
{
	asm {
		pushf
		cli
	}
	
	if (!myPCB->isCompleted) 
	{
		runningThreadPCB->blocked = 1;
		myPCB->notifyOnTheEnd->wait();
	}
		
	asm {
		popf
	}
	
	return runningThreadPCB->waitToCompleteReturnValue;
}


void dispatch()
{
	asm {
		pushf
		cli
	}
	
	myDispatch();
	
	asm {
		popf
	}
}
