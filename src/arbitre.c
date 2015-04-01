/*
!!!!!!!!!!!!!!
A FAIRE 
!!!!!!!!!!!!!!

--> différentes manieres de perdre la partie : nb de points gagnés varient
--> videau
--> verifier qu'il pouvait bien pas jouer les 2 coups pour autoriser 1 seul coup
*/

#include <math.h>


// fin du jeu ?
Player isGameFinished(SGameState gameSate){ // retourne -1 ( partie non finie) 0 ( noir a gagné) ou 1 ( blanc a gagné)
	if (gameState -> bar[BLACK] == 15){
		return 0;
	}
	else if (gameState -> bar[WHITE] == 15){
		return 1;
	}
	else{ 
		return -1; 
	}
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
int valideMouvs( int nbMoves, SMoves moves[4] , Player player, char dices[2], SGameState gameState){ // retourne 0 ( pas de pb) ou 1 ( au moins un mouvement n'est pas correct)
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


// sauvegarde du résultat de la partie dans un fichier
void saveResult(Player winner, int pointsWin){

}