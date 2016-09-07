#include <pcb.h>
#include <ListPCB.h>
#include <Schedule.h>
#include <thread.h>
#include <inter.h>
#include <stdio.h>
#include <dos.h>


extern void tick();
extern void dispatch();

extern PCB* runningThreadPCB;

extern PCB* getNextPCBToExecute();

unsigned tss;
unsigned tsp;

extern ListPCB* allThreadsPCB;
ListPCB* sleepList = new ListPCB();
ListPCB* blockedList = new ListPCB();
extern void sleeping();
extern void dispatch();

unsigned demandContextSwitch = 0;
int counter = defaultTimeSlice; 




void interrupt timer()
{

	tick();
	
	if (!sleepList->prazna() && !demandContextSwitch) sleeping();
	
	if (!demandContextSwitch && !runningThreadPCB->noContextSwitch)
		counter--;
	
	
	
	if ((counter == 0) || (demandContextSwitch == 1) || (runningThreadPCB->isCompleted == 1) || (runningThreadPCB->toSleep == 1) || (runningThreadPCB->blockedOnSem == 1) || (runningThreadPCB->blocked == 1))
	{
		if (!runningThreadPCB->isCompleted )
		{
			asm {
				mov tsp, sp
				mov tss, ss
			}
		
			runningThreadPCB->sp = tsp;
			runningThreadPCB->ss = tss;
		
			if (!runningThreadPCB->toSleep && !runningThreadPCB->blockedOnSem && !runningThreadPCB->blocked)
			{
				Scheduler :: put(runningThreadPCB);
			}
			
			if (runningThreadPCB->toSleep) sleepList->dodaj(runningThreadPCB);
			if (runningThreadPCB->blockedOnSem || runningThreadPCB->blocked) blockedList->dodaj(runningThreadPCB);
		}
		
		runningThreadPCB = Scheduler :: get ();
				
		
		tsp = runningThreadPCB->sp;
		tss = runningThreadPCB->ss;
		
		if (runningThreadPCB->noContextSwitch) counter = -1;
		else counter = runningThreadPCB->timeSlice;
		
		asm {
			mov sp, tsp;
			mov ss, tss;
		}
	}
	
	if (!demandContextSwitch) asm int 60h;
	
	demandContextSwitch = 0;
	
	
}

unsigned oldTimerOFF;
unsigned oldTimerSEG;

void inic ()
{
	asm cli;
	
	asm {
		push es
		push ax
		
		mov ax,0
		mov es,ax
		
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax
		
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer
		
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax
		
		pop ax
		pop es
	}

	asm sti;
}

void restore()  
{

	asm cli;
	
	asm {
		push es
		push ax
		
		mov ax,0
		mov es,ax
		
		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax
		
		pop ax
		pop es
	}
	
	asm sti
}

void exitThread ()
{
	runningThreadPCB->isCompleted = 1;
	dispatch();
}

