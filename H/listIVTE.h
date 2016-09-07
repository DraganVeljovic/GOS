#ifndef _LISTIVTENTRY_H
#define _LISTIVTENTRY_H

#include <event.h>

class IVTEntry;

class ListIVTEntry {

	struct Elem {
		IVTEntry* pod;
		Elem* sled;
		Elem (IVTEntry* p, Elem* s = 0)
		{
			pod = p;
			sled = s;
		}
	};
	
	Elem *prvi, *posl, *tek, *pret;
	
	int kap;

public:

	ListIVTEntry& isprazni();
	
	ListIVTEntry ();
	
	~ListIVTEntry ();
	
	ListIVTEntry& dodaj (IVTEntry* t);
	
	ListIVTEntry& naPrvi ();
	
	ListIVTEntry& naSled();
	
	int imaTek (); 
	
	IVTEntry* uzmiTek ();
	
	ListIVTEntry& izbaciTek ();
	
	IVTEntry* nadjiBroj (IVTNo broj);
	
	ListIVTEntry& izbaciBroj (IVTNo broj);	

};
	
	
#endif
		