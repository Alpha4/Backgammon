#ifndef _LISTE_H
	#define _LISTE_H

#include "backgammon.h"

typedef struct Data Data;

typedef struct STab STab;

typedef struct SCell SCell;
typedef struct SList SList;


struct Data {
	SMove moves[4];
	int nbMoves;
	int dice[4]; // représente les nombres disponibles sur les dés 
	SGameState gameState;
	/* EXEMPLE :
	les dé affichent 3 et 4 --> dice est rempli de la sorte : [3,4,-1,-1]
	( -1 représente un 'dé' inutilisable)
	le dé affichant 3 est utilisé --> dice devient [-1,4,-1,-1]
	les dés affichent 2 et 2 --> dice est rempli de la sort : [2,2,2,2]
	*/
	
};



struct SCell
{
	SCell *previous;
	Data value;
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

int IsEmpty(SList *list);

#endif
