/*
!!!!!!!!!!!!!!
A FAIRE 
!!!!!!!!!!!!!!

--> différentes manieres de perdre la partie : nb de points gagnés varient
--> videau
--> verifier qu'il pouvait bien pas jouer les 2 coups pour autoriser 1 seul coup
*/




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"
#include "arbitre.h"
#include "liste.h"




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
déplacement ne correspond pas à un dé
mauvais sens d'avancement des pions
pas de piont du joueur sur la case départ
au moins 2 pionts adverses sur la cas arrivé
case arrivée ou départ n'existe pas


!! ATTENTION prendre en compte 
0 bar --> peut etre uniquement dest
25 out peut etre uniquement source

sil en a dans dans le out et src pas out
attention si dest est le bar


*/

SMove** movesPossibles(SGameState gameState, int nbMoves, Smove moves[4], Player player, char dices[2]){

	// NUMEROS DE CELLULES DEPUIS LESQUELLES LE PION PEUT PARTIR (cellules possédées par le joueur	)

	int srcCells[25]; // tableau contenant les cases desquelles le pion peut partir
	int indexSrc = 0; // indice de la premiere case vide du tableau
	int i;
	for (i=0; i < gameState.board.length(); i++){
		if ( SGameState.board[i].owner == player){
			srcCells[indexSrc] = i+1;
			indexSrc ++;
		}
	}
	// si le joueur a des pions dans le bar, celui ci peut etre un case de départ 
	if ( gameState.bar[player] > 0){
		srcCells[indexSrc] = 0;
		indexSrc ++;
	}


	// NUMEROS DE CELLULES SUR LESQUELLES LE PION PEUT ARRIVER
		
	int destCells[25];
	int indexDest = 0;

	for (i=0; i < gameState.board.length(); i++){
		//cellules possédées par personnes ou le joueur
		if ( ( gameState.board[i].owner == player) || (SGameState.board[i].owner == NOBODY) ){
			destCells[indexDest] = i+1;
			indexDest ++;
		}

		//cellules ou il n'y a qu'un pion ennemi
		else if( gameState.board[i].nbDames == 1){
			destCells[indexDest] = i+1;
			indexDest ++;
		}
	}

	// pour le joueur blanc
	// si tous les pions sont dans les 6 dernieres cases du joueur --> il peut mettre des pions dans out[]
	if (joueur == WHITE ){
		for (i=18; i < 34; i++){
			int add = 0; // nb de pionts dans les 6 dernieres cases
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[indexDest] = 25;
		}
	}


	//pour le joueur noir
	// si tous les pions sont dans les 6 dernieres cases --> il peut mettre des pions dans out[]
	if ( joueur == BLACK ){	
		for (i=0; i < 6; i++){
			int add = 0; // nb de pionts dans les 6 dernieres cases
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[indexDest] = 25;
		}	
	}

	

	//         ATTENTION
	// -- le joueur peut mettre dans bar que si le nb est pil celui du dé ou qu'il n'a plus de pions plus loin
	// -- si pion dans out, mouv obligatoire partant de out
	// -- si le joueur a un double il faut prévoir qu'il peut faire 4 mouvs




	/*
	// initialisation du tableau contenant les tableaux des différentes combinaisons de mouvement que peut faire le joueur
	Smove movesPossibles[512][4];
	int indexMovesPossibles = 0;

	// si le joueur a au moins 2 pions dans le out, les mouvements possibles ne sont que ceux qui partent du out
	//                 A FAIRE
	// joueur blanc
	if ( joueur == WHITE ){
		if (gameState.board[0] > 2 ){
			for ( i=0; i < indexDest; i++){ // on parcours les cases d'arrivée possible
				// si un dé permet le déplacement on l'ajoute dans les mouvs possibles
				if ( (destCells[indexDest] == dice[0]) ){

				}
			}
		}
	}

	// joueur noir
	if ( joueur == BLACK ){

	}
	//                 ^ A FAIRE ^
	*/




	/*
	A FAIRE :
	dice le rendre 4 cases


	*/




	// le joueur n'a pas de pions dans le bar et ne peut pas en mettre dans le out :


	Slist* movesPossibles; // liste chainée de mouvements possibes
	movesPossibles = CreateList();

	int j;
	for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
		for (j=0; j < indexDest; j++){ // j --> parcours de destCells
			// vérification que le mouvement est dans le bon sens suivant le joueur
			if ( (player == WHITE) && destCells[j] > srcCells[i] ){
				// vérification que le mouvement correspond à un dé
				if ( fabs(destCells[j] - srcCells[i]) == dice[0] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					
					Data data;
					data.moves[0].src_point= srcCells[i-1];
					data.moves[0].dest_point = destCells][i-1];
					data.dice = [-1, dice[1],;
					addElementBegin(movesPossibles, data)
				}
				else if ( fabs(destCells[j] - srcCells[i]) == dice[1] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					Data data;
					data.moves[0].src_point= srcCells[i-1];
					data.moves[0].dest_point = destCells][i-1];
					data.dice = monDice;
					addElementBegin(movesPossibles, data)
				}
			}
		}
	}
}

struct Data {               //  A SUPPRIMER 	!!!!!!!!!!!!! ( DEJA PRÉSENT DANS LISTE.C)
	Smove moves[4];
	char dice[4]; // représente les nombres disponibles sur les dés 
};





// mouvements valides ? 
int validMoves( int nbMoves, SMove moves[4], SGameState gameState, char dices[2], Player player){ // retourne 0 ( pas de pb) ou 1 ( au moins un mouvement n'est pas correct)
	

// Smove movesPossibles[512][4]




	int valide = 0; // initialisation à " mouvements correctes"
	int i;
	
	for ( i=0; i < nbMoves; i++){

		// numéros des cellules de départ et d'arriver du mouvement
		int numCellSrc = moves[i].src_point;
		int numCellDest = moves[i].dest_point; 

		// celulles de départ et d'arrivée du mouvement
		Square cellSrc = gameState.board[numCellSrc];
		Square cellDest = gameState.board[numCellDest];



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
 *	nom du gagnant
 * @param int pointsWin
 *	le nombre de points gagnés
 * @param int round
 *	le round gagné
 *
 *	FORMAT
 *	nomGagnant	points\n<-- une game
 */
void saveResult(char* winner, int pointsWin){

	FILE *file = NULL;  // Pointeur vers le fichier

    file = fopen("result.txt", "a");  // Ouverture du fichier en mode "ajout" (on ajoute du contenu à la fin du fichier)

    if (file != NULL)  // Le fichier s'est bien ouvert
    {
    	fprintf(file, "%s\t%d\n", winner,pointsWin);  // On écrit dans le fichier
    	fclose(file);  // On ferme le fichier
    }
}

/**
 * Sauvegarde du résultat du match
 * @param GameState gs
 *	l'état du jeu courant
 * @param char* p1Name
 *	nom joueur 1
 * @param char* p2Name
 *	nom joueur 2
 * @param Player player1
 *	couleur du joueur 1
 *
 *	FORMAT
 *	nomGagnant	points	nomPerdant	points\n
 */
void saveMatch(GameState gs, char* p1Name,char* p2Name,Player player1){

	FILE *file = NULL;  // Pointeur vers le fichier

    file = fopen("result.txt", "a");  // Ouverture du fichier en mode "ajout" (on ajoute du contenu à la fin du fichier)

    if (file != NULL)  // Le fichier s'est bien ouvert
    {
    	if (gs.whiteScore>gs.blackScore) //BLANC 
    	{
    		if(player1==WHITE)
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p1Name,gs.whiteScore,p2Name,gs.blackScore);
    		}
    		else
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p2Name,gs.blackScore,p1Name,gs.whiteScore);
    		}
    	}
    	else
    	{
    		if(player1==WHITE)
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p2Name,gs.blackScore,p1Name,gs.whiteScore);
    		}
    		else
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p1Name,gs.whiteScore,p2Name,gs.blackScore);
    		}
    	}
    	fclose(file);  // On ferme le fichier
    }

}