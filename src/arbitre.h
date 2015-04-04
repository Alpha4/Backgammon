#ifndef _ARBITRE
    #define _ARBITRE

#include "liste.h"
//#include "backgammon.h"


Player isGameFinished(SGameState gameState,int penalty[2]);
int getSrcCells(SGameState gameState, Player player, int* srcCells );
int getDestCells( SGameState gameState, Player player, int* destCells );
int thereIsFartherPiece(int numSrcCell, Player player, SGameState gameState);
int canGoToBar(SGameState gameState, Player player, int numDice, int numSrcCell);
SList* fillIn_1_MovesPossible( Player player, int dice[4], SGameState gameState);
SList* fillIn_2_MovesPossible( Player player, SList* movesPossible, int nbMovesInCells );
SList* getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2]);
int validMoves( int nbMoves, SMove moves[4], SGameState gameState, unsigned char dices[2], Player player);
void saveResult(char* winner, int pointsWin);
void saveMatch(SGameState gs, char* p1Name,char* p2Name,Player player1);


#endif





