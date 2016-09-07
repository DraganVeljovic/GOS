#include <semaphor.h>
#include <krlsem.h>
#include <dos.h>

Semaphore :: Semaphore (int init) 
{
	asm {
		pushf
		cli
	}
	
	myImpl = new KernelSem (init);
	
	asm {
		popf
	}
}

Semaphore :: ~Semaphore ()
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

int Semaphore :: wait ()
{
	return myImpl->wait ();
}

void Semaphore :: signal ()
{
	myImpl->signal ();
}
	
int Semaphore :: val () const
{
	return myImpl->val ();
}