#include <event.h>
#include <krlev.h>
#include <dos.h>



Event :: Event (IVTNo ivtNo)
{
	asm {
		pushf
		cli
	}
	
	myImpl = new KernelEv(ivtNo);
	
	asm {
		popf
	}
}

Event :: ~Event ()
{
	asm {
		pushf
		cli
	}
	
	delete myImpl;
	
	asm {
		popf
	}
}

int Event :: wait()
{
	return myImpl->wait();
}

void Event :: signal ()
{
	myImpl->signal();
}