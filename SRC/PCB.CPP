#include <pcb.h>
#include <thread.h>
#include <string.h>
#include <listPCB.h>
#include <SCHEDULE.H>
#include <krlsem.h>
#include <ivtentry.h>
#include <dos.h>
#include <stdlib.h>


extern PCB* runningThreadPCB;

extern unsigned demandContextSwitch;
extern void interrupt timer();
extern void exitThread();
extern void dispatch();


ListPCB* allThreadsPCB = new ListPCB();

void myDispatch()
{	 
	demandContextSwitch = 1;
	timer();	
}

void createProcess (PCB* pcb, void (*body)())
{	
	pcb->stack = new unsigned [pcb->stackSize];
	pcb->stack[pcb->stackSize-1] = 0x200;
	
	pcb->stack[pcb->stackSize-2] = FP_SEG(body);
	pcb->stack[pcb->stackSize-3] = FP_OFF(body);
	
	pcb->sp = FP_OFF(pcb->stack + pcb->stackSize-12);
	pcb->ss = FP_SEG(pcb->stack + pcb->stackSize-12);

	pcb->isCompleted = 0;
}

void mainWaitToComplete ()
{
	while(!allThreadsPCB->prazna()) dispatch();
}

int pcbEqualById (const PCB* pcb1, const PCB* pcb2)
{
	if (pcb1->id == pcb2->id) return 1;
	else return 0;
}

int pcbEqualId (const PCB* pcb, ID id)
{
	if (pcb->id == id) return 1;
	else return 0;
}

int pcbEqualName (const PCB* pcb, TName name)
{
	if (!strcmp(pcb->name, name)) return 1;
	else return 0;
}

void sleeping ()
{
	asm {
		pushf
		cli
	}
	
	if (!sleepList->prazna())
	{
		for (sleepList->naPrvi(); sleepList->imaTek(); sleepList->naSled())
		{
			
			PCB* p = sleepList->uzmiTek();
			p->timeToSleep--;
			if (p->timeToSleep == 0)
			{	
				p->sleepReturnValue = 1;
				p->toSleep = 0;
				sleepList->izbaciTek();
				Scheduler :: put(p);
			}
			
		}
	}
	
	asm {
		popf
	}
}


char* intToString (int i)
{
	char* string = new char[10];
	string = itoa(i,string,10);
	return string;
}

int PCB :: autoId = 0;

void PCB :: runWrap()
{
	runningThreadPCB->myThread->run();
	runningThreadPCB->finish();
}

PCB* PCB :: getPCBById (ID id, ListPCB* lista)
{
	return lista->nadjiId(id);
}


PCB* PCB :: getPCBByName (TName name, ListPCB* lista)
{
	return lista->nadjiName(name);
}

PCB* PCB :: getPCB (Thread* t)
{
	return t->myPCB;
}

PCB :: PCB (TName name, StackSize stackSize, Time timeSlice, Thread* myThread)
{
	id = ++autoId;
	
	if(name != NULL)
	{
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);
    }
    else 
	{				
		this->name = new char[sizeof(id)+7];
		char* sname = new char[7];
		char* sid = new char[sizeof(id)+1];
		sid = (char*)id;
		strcpy(sname,"Thread");
		itoa(id,sid,10);
		strcat(sname,sid);
		strcpy(this->name,sname);
		delete [] sname;
		delete [] sid;
    }
	
	this->myThread = myThread;
	this->stackSize = stackSize;	
	this->timeSlice = timeSlice;
	if (timeSlice == 0) noContextSwitch = 1;
	else noContextSwitch = 0;
	
	notifyOnTheEnd = new KernelSem(0);

	sem = NULL;
	entry = NULL;
	
	isCompleted = 0;
	toSleep = 0;
	blockedOnSem = 0;
	blocked = 0;
	sleepReturnValue = 0;
	waitToCompleteReturnValue = 0;
	waitReturnValue = 0;
	wakeUpReturnValue = 0;
	allThreadsPCB->dodaj(this);
}

PCB :: PCB ()
{
	stackSize = defaultStackSize;
	
	notifyOnTheEnd = new KernelSem(0);
	
	sem = NULL;
	entry = NULL;
	isCompleted = 0;
	toSleep = 0;
	blockedOnSem = 0;
	blocked = 0;
	noContextSwitch = 0;
	sleepReturnValue = 0;
	waitToCompleteReturnValue = 0;
	waitReturnValue = 0;
	wakeUpReturnValue = 0;
	timeSlice = defaultTimeSlice;
	stack = new unsigned [defaultStackSize];
	myThread = NULL;
}

	
PCB :: ~PCB() 
{
	allThreadsPCB->izbaci(this);
	delete [] stack;
	delete [] name;
}

ID PCB :: getID ()
{
	return id;
}


TName PCB :: getName ()
{
	return name;
}

Thread* PCB :: getThreadById(ID id)
{
	PCB* p = NULL;
	if (p = getPCBById(id,allThreadsPCB)) return p->myThread;
	return NULL;
}


ID PCB :: getIdOf(TName name)
{
	PCB* p = NULL;
	if (p = getPCBByName(name,allThreadsPCB)) return p->id;
	return -1; // nema trazene niti
}

void PCB :: finish ()
{
	asm {
		pushf
		cli
	}
	
	
	notifyOnTheEnd->signalWhile(0);
	
	isCompleted = 1;
	
	allThreadsPCB->izbaci(this);
	
	asm {
		popf
	}
	
	dispatch();
}


