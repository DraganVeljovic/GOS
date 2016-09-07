#include <krlev.h>
#include <event.h>
#include <ivtentry.h>
#include <dos.h>
#include <stdlib.h>


KernelEv :: KernelEv (IVTNo ivtNo) 
{
	asm {
		pushf
		cli
	}
	
	ivtNumber = ivtNo;
	entry = IVTEntry :: getIVTEntryByNumber(ivtNo);
	
	asm {
		popf
	}
}

KernelEv :: ~KernelEv () 
{
	asm {
		pushf
		cli
	}
	
	IVTEntry :: removeIVTEntry(ivtNumber);
	delete entry;
	
	asm {
		popf
	}
}

int KernelEv :: wait()
{
	return entry->wait();
}

void KernelEv :: signal()
{
	entry->signal();
}