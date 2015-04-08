#ifndef _BOT_H
	#define _BOT_H
	
void triMoves (int i, SGameState gameState, SCell* currentCell,SList* attMoves, SList* defMoves,SList* neutralMoves,SList* harmingMoves);

SList* getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2], int* nbMovesPossible);

SMove choseMove(SList* listMoves, int i);

int movesAreEquals(SMove move1, SMove move2);

void deleteUnchosen(SMove move, SList* liste, int i);

int getDistanceToWin(const SGameState * const gameState, Player p);

#endif
