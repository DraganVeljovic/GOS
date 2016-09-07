#ifndef _INTERRUPT_H
#define _INTERRUPT_H

extern void inic();
extern void restore();
extern void exitThread();

extern unsigned oldTimerOFF;
extern unsigned oldTimerSEG; 


// globalne promenjive koje se koriste  
// kao privremena odredista u timer()
extern unsigned tss;
extern unsigned tsp;

extern unsigned demandContextSwitch;
extern PCB* getNextPCBToExecute();
extern int nextThread;
extern int counter;
//extern unsigned toSleep;


#endif