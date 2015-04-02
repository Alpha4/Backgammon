/*
!!!!!!!!!!!!!!
A FAIRE 
!!!!!!!!!!!!!!

--> différentes manieres de perdre la partie : nb de points gagnés varient
--> videau
--> verifier qu'il pouvait bien pas jouer les 2 coups pour autoriser 1 seul coup
*/

#include <math.h>


/**
 * Fonction qui renvoie le joueur ayant gagné ou NOBODY sinon
 * @param SGameState gameState
 *	l'état du jeu courant
 * @return Player
 *	le joueur gagnant
 * En cas de gain par abandon le cas est traité par le main.c
 */
Player isGameFinished(SGameState gameState,int penalty[2])
{
	if (gameState -> bar[BLACK] == 15)
		return BLACK;
	else if (gameState -> bar[WHITE] == 15)
		return WHITE;
	else if(penalty[BLACK]==3) // Trop de pénalités
		return WHITE;
	else if (penalty[WHITE]==3) // Trop de pénalités
		return BLACK
	return NOBODY; 
}



/*
sens d'avancement
pas de piont du joueur sur la case départ
au moins 2 pionts adverses sur la cas arrivé
case arrivée ou départ n'existe pas


!! ATTENTION prendre en compte 
0 bar --> peut etre uniquement dest
25 out peut etre uniquement source

sil en a dans dans le out et src pas out
attention si dest est le bar


*/


// mouvements valides ? 
// entrées : tableau de mouvements : taille 4 car au plus 4 mouvements peuvent être joués (4 si un double sort sur les dés)
//            + joueur qui demande les mouvements
int valideMoves( int nbMoves, SMoves moves[4] , Player player, char dices[2], SGameState gameState){ // retourne 0 ( pas de pb) ou 1 ( au moins un mouvement n'est pas correct)
	int valide = 0; // initialisation à " mouvements correctes"
	int i;
	
	for ( i=0; i < nbMoves; i++){

		// numéros des cellules de départ et d'arriver du mouvement
		int numCellSrc = moves[i]->src_point;
		int numCellDest = moves[i]->dest_point; 

		// celulles de départ et d'arrivée du mouvement
		Square cellSrc = gameState->board[numCellSrc];
		Square cellDest = gameState->board[numCellDest];



		// test si le mouvement est dans le bon sens ar rapport au joueur
		// les blancs parcours les cases dans le sens croissant et les noir dans le sens décroissant
		if ( (player == WHITE) && (numCellSrc > numCellDest) ){
			return 1;
		}
		else if ( (player == BLACK) && (numCellSrc < numCellDest) ){
			return 1;
		}


		//test si le déplacement correspond à un dé
		if ( (fabs(numCellSrc - numCellDest) != dices[0])  &&  (fabs(numCellSrc - numCellDest) != dices[1]) ){
			return 1; // mouvement non valide
		}


	return valide;
	}
}


/**
 * Sauvegarde du résultat du round
 * @param char* winner
 *	l'état du jeu courant
 * @param int pointsWin
 *	le nombre de points gagnés
 * @param int round
 *	le round gagné
 *
 *	FORMAT
 *	nomGagnant	points	<-- une game
 *	nomGagnant	points	nomGagnant	points	nomGagnant	points\n <-- un match de 3 games
 */
void saveResult(char* winner, int pointsWin,int round,int lastRound){

	FILE *file = NULL;  // Pointeur vers le fichier

    file = fopen("result.txt", "a");  // Ouverture du fichier en mode "ajout" (on ajoute du contenu à la fin du fichier)

    if (file != NULL)  // Le fichier s'est bien ouvert
    {
        if (round == lastRound)  // On est au dernier round, on termine la ligne
            fprintf(file, "%s\t%d\n", winner,pointsWin);  // On écrit dans le fichier
        else  // On n'est pas au dernier round, on continue sur la même ligne
            fprintf(file, "%s\t%d\t", winner,pointsWin);  // On écrit dans le fichier
        fclose(file);  // On ferme le fichier
    }

}