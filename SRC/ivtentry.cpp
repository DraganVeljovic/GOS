
#include <pcb.h>
#include <thread.h>
#include <listPCB.h>
#include <listIVTE.h>
#include <event.h>
#include <ivtentry.h>
#include <inter.h>
#include <SCHEDULE.H>
#include <dos.h>
#include <stdlib.h>


extern int semPreempt;
extern unsigned tsp;
extern unsigned tss;
extern int counter;

extern PCB* runningThreadPCB;


ListIVTEntry* allIVTEntries = new ListIVTEntry();


IVTEntry* IVTEntry :: getIVTEntryByNumber (IVTNo number)
{
	return allIVTEntries->nadjiBroj(number);
}


void IVTEntry :: removeIVTEntry (IVTNo number)
{
	allIVTEntries->izbaciBroj(number);
}


IVTEntry :: IVTEntry(IVTNo number, int callOld, pInterrupt newInterruptFunction )
{
	asm {
		pushf
		cli
	}
	
	value = 1;
	entryNumber = number;
	allIVTEntries->dodaj(this);
	this->callOld = callOld;
	blockedOnThisSem = new ListPCB();
	
	oldInterruptFunction = getvect(entryNumber);
	setvect(entryNumber, newInterruptFunction);
	
	asm {
		popf
	}
}

IVTEntry :: ~IVTEntry ()
{
	asm {
		pushf
		cli
	}
		
	setvect(entryNumber,oldInterruptFunction);
	delete blockedOnThisSem;
	allIVTEntries->izbaciBroj(entryNumber);
	
	asm {
		popf
	}
}


IVTNo IVTEntry :: getIVTNumber()
{
	return entryNumber;
}

int IVTEntry :: wait()
{
	asm {
		pushf
		cli
	}
	
	if (--value<0)
	{
		runningThreadPCB->blockedOnSem = 1;
		runningThreadPCB->entry = this;
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

void IVTEntry :: signal()
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
			p->entry = NULL;
			p->blockedOnSem = 0;
			p->waitReturnValue = 1;
			blockedOnThisSem->izbaci(p);
			blockedList->izbaci(p);
			Scheduler :: put (p);	
		}
	}
	if (value>1) value = 1;
	if (semPreempt) dispatch();
	
	asm {
		popf
	}
}

void IVTEntry :: interruptRoutine(IVTNo number)
{

	IVTEntry* entry = allIVTEntries->nadjiBroj(number);
	if (entry != NULL)
	{
		if (entry->callOld)
		{
			entry->oldInterruptFunction();
		}
		else
		{
			asm {
				mov al, 0x20;
				out	0x20, al;
			}
		}
		entry->signal();
	}
	
}

void IVTEntry :: incVal()
{
	asm {
		pushf
		cli
	}
	
	value++;
	
	if (value>1) value = 1;
	
	asm {
		popf
	}
}