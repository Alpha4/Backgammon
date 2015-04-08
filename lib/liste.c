/*LISTE AVEC ALLOCATION PAR BLOC*/
#include "liste.h"
#include <stdlib.h>
#include <stdio.h>



SList* CreateList()
{
	SList *list=malloc(sizeof(SList));
	list->head=NULL;
	list->tail=NULL;
	list->freespots=NULL;
	list->table=malloc(sizeof(STab));
	list->table->nb=0;
	list->table->next=NULL;
	list->nb=0;
	return list;
}


void DeleteList(SList *list)
{
	STab *tmp=list->table;
	while(tmp!=NULL)
	{
		
		STab *del=tmp;// Stockage du STab a supprimer
		tmp=tmp->next; // MAJ de la tete
		free(del); // Suppression de l'ancienne tête
	}
	free(list);
	list=NULL;
}

int IsEmpty(SList *list){
	int isEmpty = 0;
	if (list->head == NULL){
		isEmpty =1;
	}
	return isEmpty;
}


SCell* GetNewCell(SList *list) // Fonction renvoyant la cellule à ajouter à la liste
{
	SCell *temp;
	if(list->freespots!=NULL) // Il y a des cellules libre pour l'ajout
	{
		temp = list->freespots; //On renvoie la première cellule libre
		list->freespots = list->freespots->next; //La tete de la liste de cellules libres est la suivante
	}
	else if ((list->table->nb)<5) //cas où le dernier tableau n'est pas rempli
	{
		temp=&list->table->tab[list->table->nb]; //La cellule a renvoyer est la prochaine non remplie, a savoir nb-1+1=nb
		list->table->nb++; //On incrémente le nb d'éléments du tableau
	}
	
	else // Dernier tableau plein -> allocation d'un nouveau
	{
		
		STab *newTab=malloc(sizeof(STab));
		newTab->next=list->table;// On ajoute le tableau en tete de la liste de tableau
		list->table=newTab;//Màj de la tete de liste de tableau
		newTab->nb=0;
		temp=&newTab->tab[0]; //la cellule a renvoyer est alors la première du nouveau tableau
		list->table->nb++; //On incrémente le nb d'éléments du tableau
	}
	list->nb++;
	return temp;
}

SCell* AddElementBegin(SList *list,Data elem)
{
	SCell *cell=GetNewCell(list); //Allocation nouvelle Cellule
	cell->value=elem; // On stocke elem dans la nouvelle SCell

	if (list->head==NULL) // Si la liste est vide
		list->tail=cell; // Alors on modifie la queue de liste
	else // Sinon il faut mettre à jour le précédent de la cellule en tête
		list->head->previous=cell; 
	cell->previous=NULL; // La nouvelle cellule n'a pas de précédent
	cell->next=list->head; // La cellule suivante de la nouvelle est celle actuellement en tête
	
	list->head=cell; // cell est la nouvelle tete
	
	return cell;
}

SCell* AddElementEnd(SList *list,Data elem)
{
	SCell *cell=GetNewCell(list);
	cell->value=elem;

	if (list->head==NULL)// Liste vide
		list->head=cell; // MAJ tete de liste
	else 
	{
		list->tail->next=cell; // MAJ du suivant de la queue de liste
	}
	
	cell->next=NULL;
	cell->previous=list->tail;
	list->tail=cell;

	return cell;
}

SCell* AddElementAfter(SList *list,SCell *cell,Data elem)
{
	if (cell == list->tail)
	{
		return AddElementEnd(list,elem);
	}
	SCell *newcell=GetNewCell(list);
	newcell->value=elem;

	newcell->next=cell->next; // Le suivant de la nouvelle cellule est le suivant de cell
	cell->next->previous=newcell; // Le précédent du suivant de cell est newcell
	newcell->previous=cell; // Le précédant de la newcell est cell
	cell->next=newcell; // le suivant de cell est newcell

	return cell;
}

void DeleteCell(SList *list,SCell *cell)
{
		if(cell!=list->tail) //Si cell n'est pas la queue
		{
			//printf("NOT_QUEUE\n");
			cell->next->previous=cell->previous; // Le précédent du suivant est le précédent de cell
		}
		if(cell!=list->head) // Si cell n'est pas la tete
		{
			//printf("NOT_TETE\n");
			cell->previous->next=cell->next; // Le suivant du précédent est le suivant de cell
		}
		if (cell==list->head) // MAJ tete si cell a suppr est la tete
		{
			//printf("TETE\n");
			list->head=cell->next;
		}
		if (cell==list->tail) // MAJ queue si cell a suppr est la queue
		{
			//printf("QUEUE\n");
			list->tail=cell->previous;
		}
		
		cell->next=list->freespots; // Ajout de la cellule en tete de la liste de cellules libres
		list->freespots=cell; // MAJ de la tete de cells libres
		list->nb--;
}

void printList(SList *list)
{
	SCell* pCell=list->head;
	while(pCell!=NULL)
	{
		printf("@prev %d |@me %d | @next %d | %d | Move1 s=%d d=%d | Move2 s=%d d=%d | Move3 s=%d d=%d | Move4 s=%d d=%d\n",pCell->previous,pCell,pCell->next,pCell->value.nbMoves,pCell->value.moves[0].src_point,pCell->value.moves[0].dest_point,pCell->value.moves[1].src_point,pCell->value.moves[1].dest_point,pCell->value.moves[2].src_point,pCell->value.moves[2].dest_point,pCell->value.moves[3].src_point,pCell->value.moves[3].dest_point);
		pCell=pCell->next;
	}
}

void printListTab(SList *list)
{
	STab* pTab=list->table;
	while(pTab!=NULL)
	{
		printf("@me %d | @next %d | %d \n",pTab,pTab->next,pTab->nb);
		pTab=pTab->next;
	}
}

SCell* GetFirstElement(SList *list)
{
	return list->head;
}

SCell* GetLastElement(SList *list)
{
	return list->tail;
}

SCell* GetPrevElement(SCell *cell)
{
	return cell->previous;
}

SCell* GetNextElement(SCell *cell)
{
	return cell->next;
}

Data GetData(SCell *cell)
{
	return cell->value;
}