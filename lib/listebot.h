#ifndef _LISTEBOT_H
	#define _LISTEBOT_H


typedef struct STab STab;

typedef struct SCell SCell;
typedef struct SList SList;


struct SCell_int
{
	SCell *previous;
	int value;
	SCell *next;
};

struct SCell_move
{
	SCell *previous;
	SMove value;
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

SCell* AddElementBegin_move(SList *list,SMove elem);
SCell* AddElementEnd_move(SList *list,SMove elem);
SCell* AddElementAfter_move(SList *list,SCell *cell,SMove elem);
void DeleteCell(SList *list,SCell *cell);

SCell* GetFirstElement(SList *list);
SCell* GetLastElement(SList *list);
SCell* GetPrevElement(SCell *cell);
SCell* GetNextElement(SCell *cell);
SMove GetData_move(SCell *cell);

SCell* AddElementBegin_int(SList *list,int elem);
SCell* AddElementEnd_int(SList *list,int elem);
SCell* AddElementAfter_int(SList *list,SCell *cell,int elem);
int GetData_int(SCell *cell);

#endif
