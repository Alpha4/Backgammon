#ifndef _LIBS_ARB
    #define _LIBS_ARB

// fin du jeu ?
Player isGameFinished(SGameState gameSate,int penalty[2]);

// mouvements valides ? 
int valideMoves( int nbMoves, SMove moves[4] , Player player, char dices[2], SGameState gameState);

// sauvegarde du résultat de la partie dans un fichier
void saveResult(Player winner, int pointsWin);

#endif





