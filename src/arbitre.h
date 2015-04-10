#ifndef _ARBITRE_H
    #define _ARBITRE_H

#include "liste.h"
//#include "backgammon.h"



/**
 * Fonction qui renvoie le joueur ayant gagné ou NOBODY sinon
 * @param SGameState gameState
 *	l'état du jeu courant
 * @return Player
 *	le joueur gagnant
 * Le cas de gain par abandon est traité par le main.c
 */
Player isGameFinished(SGameState gameState,int penalty[2]);


 /**
  * Fonction qui rempli le tableau srcCells contenant les numéros de cellules pouvant être source d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être source d'un mouvement
  * @return int index
  *   index de la premiere case vide du tableau srcCells
 **/
int getSrcCells(SGameState gameState, Player player, int* srcCells );


/**
  * Fonction qui rempli le tableau destCells contenant les numéros de cellules pouvant être destination d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être destination d'un mouvement
  * @return int index
  *   index de la premiere case vide du tablea destCells
  */

int getDestCells( SGameState gameState, Player player, int* destCells );

/**
 * Fonction qui renvoie un booléen indiquant si le joueur possède des pions plus loin du out que la source du mouvement
 * @param int numSrcCell 
 *   numéro de la cellule sur laquelle se trouve le pion pour lequel on veut vérifier qu'il peut aller dans le out
 * @param Player player
 *   joueur qui veut effectuer le mouvement
 * @param SGameState gameState
 *   état du jeu courant
 * @return int thereIsFartherPiece
 *   booléen indiquant si le joueur possède ou non des pions plus loin du out que la source du mouvement
 */
int thereIsFartherPiece(int numSrcCell, Player player, SGameState gameState);

/**
 * Fonction qui renvoie un booléen indiquant si un pion peut etre déplacé dans le out ou non
 * @param SGameState
 *   état du jeu courant
 * @param Player player
 *   joueur qui veut effectuer le mouvement
 * @param int numDice
 *   valeur d'un dé testée
 * @param int numSrcCell
 *   cellule sur laquelle se trouve le pion pour lequel on veut savoir si il peut être déplacé dans le out ou non
 * @return int canGoToOut
 *   booléen indiquant si un pion peut etre déplacé dans le out ou non
 */
int canGoToOut(SGameState gameState, Player player, int numDice, int numSrcCell);

/**
 * Fonction qui initialise les composantes d'un élément de la structure Data
 * @param Data* data
 *   pointeur vers l'élément de structure Data à initialiser
 */
void initData(Data* data);

/**
 * Fonction qui actualise le gameState donné en appliquant le mouvement d'un pion
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int numDestCell
 *   numéro de la cellule d'arrivée du mouvement
 * @param SGameState* gameState
 *   pointeur vers l'état du jeu courant
 * @param Player player
 *   joueur effectuant le mouvement
 */

void actualizeGameState(int numSrcCell, int numDestCell, SGameState* gameState, Player player);

/**
 * Fonction qui créer et remplit les premiers mouvements possibles dans movesPossible
 * @param Player player
 *   joueur pour lequel les mouvements possibles sont calculés
 * @param : int dice[4]
 *   tableau contenant les valeurs disponibles de dé ( -1 = dé non disponible )
 * @param SGameState gameState
 *   état du jeu courant
 * @return SList* movesPossible
 *   pointeur vers la liste chainée dont les cellules contiennent, entre autres, un tableau de mouvements possible (SMove) (actualisé)
 */
SList* fillIn_1_MovesPossible( Player player, int dice[4], SGameState gameState);

/**
 * Fonction qui supprime les celulles contenant moins de nbMoves mouvements
 * @param SList* movesPossible
 *   liste de laquelle on veut supprimer les cellules ne contenant pas assez de mouvements
 * @param int nbMoves
 *   nombre de mouvements minimal pour qu'une cellule ne soit pas supprimée
 */
void deleteCellsLessMoves(SList* movesPossible, int nbMoves);

/**
 * Fonction qui remplie les mouvements possibles dans movesPossible (les cellules de movesPossible doivent contenir au moins un mouvement)
 * @param Player player
 *   joueur pour lequel les mouvements possibles sont calculés
 * @param SList* movesPossible
 *   liste chainée dont les cellules contiennent un tableau de mouvements possible
 * @param : int dice[4]
 *   tableau contenant les valeurs disponibles de dé ( -1 = dé non disponible )
 * @param SGameState gameState
 *   état du jeu courant
 * @param int nbMovesInCells
 *   nombre de mouvements contenus dans chaque cellule de movesPossible ( ce nombre est constant suivant les cellules)
 * @return int nbMovesInCells 
 *   nombre de mouvements contenus dans les cellules de movesPossible
 */
int fillIn_2_MovesPossible( Player player, SList* movesPossible, int nbMovesInCells );

/**
 * Fontion qui renvoie la liste de tous les mouvements possibles en fonction de l'état du jeu, du joueur et des dés
 * @param SGameState gameState
 *   état du jeu courant
 * @param Player player
 *   joueur pour lequel on souhaite avoir une liste de mouvements possibles
 * @param unsigned char diceGiven[2]
 *   tableau contenant les valeurs des dés 
 * @rparam int nbMovesPossible
 *   nombre de mouvement contenu dans les tableaux de movesPossible
 * @return SList* movesPossible
 *   liste chainée contenant tous les tableaux de mouvements possible
 */ 
SList* getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2], int* nbMovesPossible);

/**
 * Fonction qui renvoie un booléen indiquant si un tableau de mouvement est correcte ( suivant l'état du jeu)
 * @param SMove moves[4]
 *   tableau contenant la suite de mouvements à analyser
 * @param SGameState gameState
 *   état courant du jeu
 * @param unsigned char dice[2]
 *   tableau contenant les valeurs des dés
 * @param Player player
 *   joueur pour lequel on veut tester le tableau de mouvements
 */
int validMoves(int nbMoves, SMove moves[4], SGameState gameState, unsigned char dice[2], Player player);

/**
 * Fonction qui ne garde que les cellules de movesPossible dont le mouvement au rang 'rank' correspond au mouvement donné
 * @param SList* movesPossible
 *    liste contenant les mouvements possibles
 * @param int rank
 *    numéro du mouvement que l'on doit traiter dans le tableau de mouvement contenu dans chaque celulle de movesPossible
 * @ parma int numSrcCell
 *     numéro de la cellule du départ du mouvement que l'on veut conserver
 * @parma int numDestCell
 *     numéor de la cellule d'arrivée du mouvement que l'on veut conserver
 */
void keepCells(SList* movesPossible, int rank, int numSrcCell, int numDestCell);

/**
 * Fonction qui indique si un élément ( entier ) se trouve dans un tableau (d'entiers)
 * @param int elem
 *   élément pour lequel on veut savoir s'il se trouve dans le tableau
 * @param in index
 *   index de la premiere case vide dans le tableau
 * @param int* tab
 *   tableau dans lequel on cherche l'élément
 * @return int result
 *   0 : l'élément ne se trouve pas dans le tableau
 *   1 : l'élément se trouve dans le tableau
 */
int isIn(int elem, int index, int* tab);

/**
 * Fonction qui remplit le tableau des cellules d'arrivée possibles d'un mouvement
 * @param SLits* movesPossible
 *   liste de mouvements possibles
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int* destCells
 *   tableau contenant le numéro des cellules d'arrivée possibles à remplir
 */
int fillInDestCells(SList* movesPossible, int numSrcCell, int* destCells, int rank);


/**
 * Fonction qui renvoit l'indice du dé utilisé pour le mouvement
 * @param int* dice
 *   jeu de dés
 * @param Player player
 *   joueur qui effectue le mouvement
 * @param int numSrcCell
 *    numéro de la cellule de départ du mouvement
 * @param int numDestCell
 *    numéro de la cellule d'arrivée du mouvement
 * @return int diceUsed
 *   indice du dé utilisé
 */
int diceUsed(int* dice, Player player, int numSrcCell, int numDestCell);

/**
 * Fonction qui remplie les cellules sources possibles d'un mouvement avev movesPossible
 * @param SList* movesPossible
 *    liste chainée contenant les mouvements possibles
 * @param int rank
 *    numéro du mouvement que l'on doit traiter dans le tableau de mouvement contenu dans chaue cellule de movesPossible
 * @parma int srcCells
 *    tableau contenant les numéros des cellules sources, à remplir
 * @return int index
 *    index de la premiere case vide de srcCells
 */
int getRealSrcCells(SList* movesPossible, int rank, int* srcCells);

#endif