#ifndef _LISTPCB_H
#define _LISTPCB_H

#include <thread.h>

class PCB;

extern int pcbEqualById (const PCB* pcb1, const PCB* pcb2);
extern int pcbEqualId (const PCB* pcb, ID id);
extern int pcbEqualName (const PCB* pcb, TName name);


class ListPCB {
	struct Elem {
		PCB* pcb;
		Elem* sled;
		
		Elem (PCB* p, Elem* s = 0)
		{
			pcb = p;
			sled = s;
		}
	};
	
	Elem *prvi, *posl, *tek, *pret;
	
	int kap;
	
public:	
	
	ListPCB& isprazni ();
	
	ListPCB ();
	
	~ListPCB ();
	
	ListPCB& dodaj (PCB* p);
	
	ListPCB& naPrvi ();
	
	ListPCB& naSled();
	
	int imaTek ();
	
	PCB* uzmiTek ();
	
	ListPCB& izbaciTek ();
	
	ListPCB& izbaci (PCB* pcb);
	
	PCB* nadji (PCB* pcb);
	
	PCB* nadjiId (ID id);
	
	PCB* nadjiName (TName name);
	
	int prazna();
};

#endif