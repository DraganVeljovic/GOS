#ifndef _IVTENTRY_H
#define _IVTENTRY_H

class KernelEv;
class ListIVTEntry;
class ListPCB;



typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

#define PREPAREENTRY(entryNum, callOld) \
	void interrupt newInterruptFunction ## entryNum ## (...); \
	IVTEntry* entry ## entryNum ## = new IVTEntry(0x ## entryNum, callOld, &newInterruptFunction ## entryNum ##);\
	void interrupt newInterruptFunction ## entryNum ## (...) { \
		IVTEntry :: interruptRoutine(entryNum); \
	}; 


class Event;

extern ListIVTEntry* allIVTEntries;


class IVTEntry{
//private: 
public:
	IVTNo entryNumber;
	pInterrupt oldInterruptFunction;

	int callOld;
	int value;
	ListPCB* blockedOnThisSem;

//public:
	IVTEntry(IVTNo number, int callOld, pInterrupt newInterruptFunction);

	~IVTEntry();
	
	IVTNo getIVTNumber ();
	
	static IVTEntry* getIVTEntryByNumber(IVTNo number);
	static void removeIVTEntry(IVTNo number);

	static void interruptRoutine(IVTNo number);
	
	int wait();
	void signal();
	
	void incVal();
};
	
#endif