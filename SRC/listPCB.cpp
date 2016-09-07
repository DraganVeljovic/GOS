#include <listPCB.h>
#include <pcb.h>
#include <thread.h>
#include <stdlib.h>


extern int pcbEqualById (const PCB* pcb1, const PCB* pcb2);
extern int pcbEqualId (const PCB* pcb, ID id);
extern int pcbEqualName (const PCB* pcb, TName name);

ListPCB& ListPCB :: isprazni ()
{
	while (prvi) 
	{
		Elem *stari = prvi;
		prvi = prvi->sled;
		delete stari;
	}
	posl = tek = pret = 0;
	kap = 0;
	return *this;
}


ListPCB :: ListPCB ()	
{	
	prvi = posl = tek = pret = 0;
	kap = 0;
}

ListPCB :: ~ListPCB ()
{
	isprazni();
}

ListPCB& ListPCB :: dodaj (PCB* p)
{
	posl = (!prvi ? prvi : posl->sled) = new Elem(p);
	kap++;
	return *this;
}

ListPCB& ListPCB :: naPrvi ()
{
	tek = prvi; 
	pret = 0;
	return *this;
}

ListPCB& ListPCB :: naSled()
{
	pret = tek;
	if (tek) tek = tek->sled;
	return *this;
}

int ListPCB :: imaTek () 
{
	if (tek != NULL) return 1;
	else return 0;
}


PCB* ListPCB :: uzmiTek ()
{
	return tek->pcb;
}


ListPCB& ListPCB :: izbaciTek ()
{
	Elem *stari = tek;
	tek = tek->sled;
	(!pret ? prvi : pret->sled) = tek;
	if (!tek) posl = pret;
	if (pret) tek = pret;
	delete stari;
	kap--;
	return *this;
}

ListPCB& ListPCB :: izbaci (PCB* pcb)
{
	for(naPrvi(); imaTek(); naSled())
	{
		if (pcbEqualById(tek->pcb, pcb)) 
			izbaciTek();
	}
	return *this;
}

PCB* ListPCB :: nadji (PCB* pcb)
{
	for(naPrvi(); imaTek(); naSled())
	{
		if (pcbEqualById(tek->pcb, pcb)) 
			return tek->pcb;
	}
	return NULL;
}

PCB* ListPCB :: nadjiId (ID id)
{
	for(naPrvi(); imaTek(); naSled())
	{
		if (pcbEqualId(tek->pcb, id)) 
			return tek->pcb;
	}
	return NULL;
}

PCB* ListPCB :: nadjiName (TName name)
{
	for(naPrvi(); imaTek(); naSled())
	{
		if (pcbEqualName(tek->pcb,name)) 
			return tek->pcb;
	}
	return NULL;
}

int ListPCB :: prazna()
{
	if (!kap) return 1;
	else return 0;
}