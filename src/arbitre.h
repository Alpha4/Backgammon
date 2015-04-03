#ifndef _ARBITRE
    #define _ARBITRE

#include "liste.h"
//#include "backgammon.h"

Player isGameFinished(SGameState gameState,int penalty[2]);

SList* getMovesPossible(SGameState gameState, int nbMoves, SMove moves[4], Player player, unsigned char dice[2]);

int getSrcCells(SGameState gameState, Player player, int* srcCells );

SList* fillIn_1_MovesPossible(int indexSrc, int indexDest, int* srcCells, int* destCells, Player player, SList* movesPossible, int dice[4]);

SList* fillIn_2_movesPossible(int indexSrc, int indexDest, int* srcCells, int* destCells, Player player, SList* movesPossible, int dice[4] );

int getDestCells( SGameState gameState, Player player, int* destCells );

int validMoves( int nbMoves, SMove moves[4], SGameState gameState, unsigned char dices[2], Player player);

void saveResult(char* winner, int pointsWin);

void saveMatch(SGameState gs, char* p1Name,char* p2Name,Player player1);

#endif





