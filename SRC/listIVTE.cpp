#include <listIVTE.h>
#include <event.h>
#include <ivtentry.h>
#include <stdlib.h>

ListIVTEntry& ListIVTEntry :: isprazni()
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

ListIVTEntry :: ListIVTEntry ()
{
	prvi = posl = tek = pret = 0;
	kap = 0;
}

ListIVTEntry :: ~ListIVTEntry ()
{
	isprazni();
}

ListIVTEntry& ListIVTEntry :: dodaj (IVTEntry* t)
{
	posl = (!prvi ? prvi : posl->sled) = new Elem(t);
	kap++;
	return *this;
}

ListIVTEntry& ListIVTEntry :: naPrvi ()
{
	tek = prvi; 
	pret = 0;
	return *this;
}

ListIVTEntry& ListIVTEntry :: naSled()
{
	pret = tek;
	if (tek) tek = tek->sled;
	return *this;
}

int ListIVTEntry :: imaTek () 
{
	if (tek != NULL) return 1;
	else return 0;
}

IVTEntry* ListIVTEntry :: uzmiTek ()
{
	return tek->pod;
}


ListIVTEntry& ListIVTEntry :: izbaciTek ()
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

IVTEntry* ListIVTEntry :: nadjiBroj (IVTNo broj)
{
	for (naPrvi(); imaTek(); naSled())
	{
		if (tek->pod->getIVTNumber() == broj)
			return tek->pod;
	}
	return NULL;
}

ListIVTEntry& ListIVTEntry :: izbaciBroj (IVTNo broj)
{
	for (naPrvi(); imaTek(); naSled())
	{
		if (tek->pod->getIVTNumber() == broj)
			izbaciTek();
	}
	return *this;
}