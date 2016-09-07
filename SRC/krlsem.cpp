#include <krlsem.h>
#include <pcb.h>
#include <listPCB.h>
#include <semaphor.h>
#include <thread.h>
#include <inter.h>
#include <SCHEDULE.H>
#include <thread.h>
#include <stdlib.h>
#include <dos.h>


extern int semPreempt;

extern void interrupt timer();
extern void dispatch();

extern ListPCB* blockedList;


KernelSem :: KernelSem (int i)
{
	asm {
		pushf
		cli
	}
	
	value = i;
	blockedOnThisSem = new ListPCB();
	
	asm {
		popf
	}
}

KernelSem :: ~KernelSem () 
{
	asm {
		pushf
		cli
	}
	
	delete blockedOnThisSem;
	
	asm {
		popf
	}
}

int KernelSem :: wait () 
{
	asm {
		pushf
		cli
	}
	
	if (--value<0)
	{
		runningThreadPCB->blockedOnSem = 1;
		runningThreadPCB->sem = this;
		blockedOnThisSem->dodaj(runningThreadPCB);
		dispatch();
	}
	
	runningThreadPCB->waitReturnValue = 1;
	
	if (semPreempt) dispatch();
	
	asm {
		popf
	}
	
	return runningThreadPCB->waitReturnValue;
}

void KernelSem :: signal () 
{
	asm {
		pushf
		cli
	}
	
	if (value++<0)
	{
		blockedOnThisSem->naPrvi();
		PCB* p = blockedList->nadji(blockedOnThisSem->uzmiTek());
		if (p != NULL)
		{
			p->sem = NULL;
			p->blockedOnSem = 0;
			p->waitReturnValue = 1;
			if (p->blocked)
			{	
				p->blocked = 0;
				p->waitToCompleteReturnValue = 1;
			}
			blockedOnThisSem->izbaci(p);
			blockedList->izbaci(p);
			Scheduler :: put (p);	
		}
	}
	if (semPreempt) dispatch();
	
	asm {
		popf
	}
}
	
int KernelSem :: val () const 
{
	return value;
}

void KernelSem :: incVal ()
{
	asm {
		pushf
		cli
	}
	
	value++;
	
	asm {
		popf
	}
}

void KernelSem :: signalWhile (int limit)
{
	asm {
		pushf
		cli
	}
	
	while (value < limit) signal();
	
	asm {
		popf
	}
}