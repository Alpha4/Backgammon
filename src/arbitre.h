#ifndef _LIBS_ARB
    #define _LIBS_ARB

// fin du jeu ?
Player isGameFinished(SGameState gameSate);

// mouvements valides ? 
int valideMouvs( int nbMoves, SMoves moves[4] , Player player, char dices[2], SGameState gameState);

// sauvegarde du résultat de la partie dans un fichier
void saveResult(Player winner, int pointsWin);

#endif





