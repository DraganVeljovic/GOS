
#include <thread.h>
#include <pcb.h>
#include <mnthr.h>
#include <dos.h>
#include <stdio.h>
#include <iostream.h>

PCB* runningThreadPCB;

extern void inic();
extern void restore();

extern void mainWaitToComplete();

extern int userMain(int argc, char** argv);


int main (int argc, char* argv[])
{	
	MainThread* main;
	int retValue;
	
	asm cli;
	
	inic();
	
	runningThreadPCB = new PCB();	
		
	main = new MainThread(argc,argv);
	
	main->start();
	
	dispatch();
	mainWaitToComplete();
	
	retValue = main->getMainReturnValue();
	
	delete main;
	
	//cout << "Nice!" << endl;

	restore();
	
	asm sti;
	
	
	return retValue;
}