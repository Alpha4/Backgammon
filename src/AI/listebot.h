#ifndef _LISTE_H
	#define _LISTE_H



typedef struct Data Data;

typedef struct STab STab;

typedef struct SCell SCell;
typedef struct SList SList;


struct Data {
	int nFleche;
	int nbDames; 
	SGameState gamestate;
};




struct SCell
{
	SCell *previous;
	int value;
	SCell *next;
};

struct STab
{
	SCell tab[5]; // Bloc d'allocation
	STab *next; // Tableau de SCell suivant
	int nb; // Nb SCell occupée
};

struct SList
{
	SCell *head; //Pointeur vers la tête de liste
	SCell *tail; //Pointeur vers la queue de liste
	SCell *freespots; // Pointeur vers liste de cellules libres
	STab *table; // Premier bloc
};


SList* CreateList();
void DeleteList(SList *list);

SCell* AddElementBegin(SList *list,Data elem);
SCell* AddElementEnd(SList *list,Data elem);
SCell* AddElementAfter(SList *list,SCell *cell,Data elem);
void DeleteCell(SList *list,SCell *cell);

SCell* GetFirstElement(SList *list);
SCell* GetLastElement(SList *list);
SCell* GetPrevElement(SCell *cell);
SCell* GetNextElement(SCell *cell);
Data GetData(SCell *cell);

#endif
