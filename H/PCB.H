#ifndef _PCB_H
#define _PCB_H

#include <thread.h>
#include <dos.h>
#include <stdlib.h>

class KernelSem;
class ListPCB;
class IVTEntry;

// Liste odgovarajucih PCB - ova
extern ListPCB* allThreadsPCB;
extern ListPCB* sleepList;
extern ListPCB* blockedList;

// PCB tekuce nit (one koja se trenutno izvrsava)
extern PCB* runningThreadPCB;

// globalna fja koju poziva dispatch(), ona zove timer()
// prosledjujuci zahtev za promenom konteksta
extern void myDispatch();

// globalna fja koja kreira process nad zadatom fjom
extern void createProcess(PCB*, void (*body)());

extern void exitThread();

// smanjuje vreme spavanja niti koji se nalaze u listi uspavanih,
// poziva se prilikom svakog prekida od timer-a
extern void sleeping();

// usluzne globalne fje koje korisi klasa ListPCB, bez njih iz nekog
// razloga nije htelo da radi
extern int pcbEqualById (const PCB* pcb1, const PCB* pcb2);
extern int pcbEqualId (const PCB* pcb, ID id);
extern int pcbEqualName (const PCB* pcb, TName name);


class PCB {
public:
	
	// kontekst
	unsigned ss;
	unsigned sp;
	
	// stek
	unsigned *stack;
	
	// adekvatna polja definisana konstruktorom klase Thread
	StackSize stackSize;
	TName name;
	Time timeSlice; 
	
	// vreme dodeljeno nit za spavanje
	Time timeToSleep;
	
	// Thread kojoj pripada dati pcb
	Thread* myThread;
	
	ID id;
	static ID autoId;
	
	// staticka fja koja se prosledjuje fji createProcess, 
	// zapravno se nad njom kreira jedan proces
	static void runWrap();
	
	// vraca myPCB polje za zadatu nit
	static PCB* getPCB(Thread* t);
	
	// flag-ovi
	unsigned isCompleted;
	unsigned toSleep;
	unsigned blockedOnSem;
	unsigned blocked;
	//unsigned wokenUp;
	int noContextSwitch;
	int sleepReturnValue;
	int waitToCompleteReturnValue;
	int waitReturnValue;
	int wakeUpReturnValue;
	
	// Semafor na kome se blokiraju sve niti koje cekaju da
	// se zavrsi izvrsenje niti ciji je ovo PCB
	KernelSem* notifyOnTheEnd;
	
	KernelSem* sem;
	IVTEntry* entry;

	PCB (TName name, StackSize stackSize, Time timeSlice, Thread* myThread);
	
	// konstruktor za main
	PCB();
	
    ~PCB ();
	
	// izlazna rezija
	void finish();
	
	
	// interne metode:
	static PCB* getPCBById (ID id, ListPCB* lista);
	static PCB* getPCBByName (TName name, ListPCB* lista);

	
	// metode koje poziva Thread omotac:
	ID getID ();
	TName getName ();
	static Thread* getThreadById (ID id);
	static ID getIdOf (TName name);
	
	
}; // Kernel's implementation of a user's thread

#endif