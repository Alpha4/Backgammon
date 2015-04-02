/*
!!!!!!!!!!!!!!
A FAIRE 
!!!!!!!!!!!!!!

--> différentes manieres de perdre la partie : nb de points gagnés varient
--> videau
--> verifier qu'il pouvait bien pas jouer les 2 coups pour autoriser 1 seul coup


--> FAIRE LES FREES POUR LES MALLOCS !
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
	if (gameState.bar[BLACK] == 15)
		return BLACK;
	else if (gameState.bar[WHITE] == 15)
		return WHITE;
	else if(penalty[BLACK]==3) // Trop de pénalités
		return WHITE;
	else if (penalty[WHITE]==3) // Trop de pénalités
		return BLACK;
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



// NUMEROS DE CELLULES DEPUIS LESQUELLES LE PION PEUT PARTIR (cellules possédées par le joueur	)
int getSrcCells(SGameState gameState, Player player, int* srcCells ){

	int index = 0; // indice de la premiere case vide du tableau
	int i;
	for (i=0; i < 24; i++){ 
		if ( gameState.board[i].owner == player){
			srcCells[index] = i+1;
			index ++;
		}
	}
	// si le joueur a des pions dans le bar, celui ci peut etre un case de départ 
	if ( gameState.bar[player] > 0){
		srcCells[index] = 0;
		index ++;
	}
	return index;

}


// NUMEROS DE CELLULES SUR LESQUELLES LE PION PEUT ARRIVER
int getDestCells( SGameState gameState, Player player){
	
	int index = 0; // indice de la premiere case vide du tableau
	int i;
	for (i=0; i < 24; i++){
		//cellules possédées par personnes ou le joueur
		if ( ( gameState.board[i].owner == player) || (gameState.board[i].owner == NOBODY) ){
			destCells[index] = i+1;
			index ++;
		}

		//cellules ou il n'y a qu'un pion ennemi
		else if ( gameState.board[i].nbDames == 1){
			destCells[index] = i+1;
			index ++;
		}
	}

	// pour le joueur blanc
	// si tous les pions sont dans les 6 dernieres cases du joueur --> il peut mettre des pions dans out[]
	if (player == WHITE ){
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for ( i=18; i < 34; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[index] = 25;
			index ++;
		}
	}


	//pour le joueur noir
	// si tous les pions sont dans les 6 dernieres cases --> il peut mettre des pions dans out[]
	if ( player == BLACK ){	
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for (i=0; i < 6; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[index] = 25;
			index ++;
		}	
	}
	return index;
}



SList* fillIn_1_MovesPossible(int indexSrc, int indexDest, unsigned int* srcCells, unsigned int* destCells, Player player, SList* movesPossible ){

	movesPossible = CreateList();

	int i;
	int j;
	for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
		for (j=0; j < indexDest; j++){ // j --> parcours de destCells

			// vérification que le mouvement est dans le bon sens suivant le joueur
			if ( (player == WHITE && destCells[j] > srcCells[i]) || (player == BLACK && destCells[j] < srcCells[i]) ){
				
				// vérification que le mouvement correspond à un dé
				
				Data* data = malloc(sizeof(Data));
				//SMove* data.moves[0] = malloc(sizeof(SMove));
				// premier dé
				if ( fabs(destCells[j] - srcCells[i]) == dice[0] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					data->moves[0].src_point = srcCells[i-1];
					data->moves[0].dest_point = destCells[i-1];
					data->dice[0] = 100;
					data->dice[1] = dice[1];
					data->dice[2] = dice[2];
					data->dice[3] = dice[3];
					AddElementBegin(movesPossible, *data);
				}

				// deuxieme dé
				else if ( fabs(destCells[j] - srcCells[i]) == dice[1] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					data->moves[0].src_point= srcCells[i-1];
					data->moves[0].dest_point = destCells[i-1];
					data->dice[0] = dice[0];
					data->dice[1] = 100;
					data->dice[2] = dice[2];
					data->dice[3] = dice[3];
					AddElementBegin(movesPossible, *data);
				}

				//troisieme dé ( dans le cas d'un double --> on fait comme si il y avait 4 dés)
				else if ( fabs(destCells[j] - srcCells[i]) == dice[2] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					data->moves[0].src_point= srcCells[i-1];
					data->moves[0].dest_point = destCells[i-1];
					data->dice[0] = dice[0];
					data->dice[1] = dice[1];
					data->dice[2] = 100;
					data->dice[3] = dice[3];
					AddElementBegin(movesPossible, *data);
				}

				// 4e dé ( dans le cas d'un double --> on fait comme si il y avait 4 dés)
				else if ( fabs(destCells[j] - srcCells[i]) == dice[3] ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
					data->moves[0].src_point= srcCells[i-1];
					data->moves[0].dest_point = destCells[i-1];
					data->dice[0] = dice[0];
					data->dice[1] = dice[1];
					data->dice[2] = dice[2];
					data->dice[3] = 100;
					AddElementBegin(movesPossible, *data);
				}

			}
		}
		return movesPossible;
	}
}


/*
principe de fillIn_2_MovesPossible :
	on copie le gameState et pour chaque cellule de la liste ( contenant pour l'instant 1 et un seul mouvement)
	on le change pour appliquer ce mouvement. On va ensuite déterminer quels sont les mouvements ( n°2 ) possibles
	pour chacun des mouvements n°2 possibles on créer une nouvelle cellule dans la liste avec move1 ET mov2
	a la fin de l'analyse des move2 possibles, on détruit la cellule ne contenant que mov1
*/
SList* fillIn_2_movesPossible(int indexSrc, int indexDest, unsigned int* srcCells, unsigned int* destCells, Player player, SList* movesPossible ){

	SCell* cellEnTraitement;
	SCell* cellTemp;
	cellEnTraitement = GetFirstElement(movesPossible);
	int movAdd =0; // pour savoir si on a rajouté des movesPossible ou pas
	while ( cellEnTraitement != GetLastElement(movesPossible)){
		int movAddCell =0; // pour savoir si on a rajouté des moves dans la cellule (combinaison de moves) ou pas

		for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
			for (j=0; j < indexDest; j++){ // j --> parcours de destCells

				int newDice[4]; // dé qui prend en compte le premier mouvement
				newDice[0] = cellEnTraitement->value.dice[0];
				newDice[1] = cellEnTraitement->value.dice[1];
				newDice[2] = cellEnTraitement->value.dice[2];
				newDice[3] = cellEnTraitement->value.dice[3];

				// vérification que le mouvement est dans le bon sens suivant le joueur
				if ( (player == WHITE && destCells[j] > srcCells[i]) || (player == BLACK && destCells[j] < srcCells[i]) ){
					
					// vérification que le mouvement correspond à un dé
					
					Data* data = malloc(sizeof(data));

					// premier dé
					if ( fabs(destCells[j] - srcCells[i]) == newDice[0] ){
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
						data->moves[0].src_point = srcCells[i-1];
						data->moves[0].dest_point = destCells[i-1];
						data->dice[0] = 100;
						data->dice[1] = newDice[1];
						data->dice[2] = newDice[2];
						data->dice[3] = newDice[3];
						AddElementBegin(movesPossible, *data);
						movAddCell = 1;
					}
	
					// deuxieme dé
					else if ( fabs(destCells[j] - srcCells[i]) == newDice[1] ){
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
						data->moves[0].src_point = srcCells[i-1];
						data->moves[0].dest_point = destCells[i-1];
						data->dice[0] = newDice[0];
						data->dice[1] = 100;
						data->dice[2] = newDice[2];
						data->dice[3] = newDice[3];
						AddElementBegin(movesPossible, *data);
						movAddCell = 1;
					}
	
					//troisieme dé ( dans le cas d'un double --> on fait comme si il y avait 4 dés)
					else if ( fabs(destCells[j] - srcCells[i]) == newDice[2] ){
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
						data->moves[0].src_point = srcCells[i-1];
						data->moves[0].dest_point = destCells[i-1];
						data->dice[0] = newDice[0];
						data->dice[1] = newDice[1];
						data->dice[2] = 100;
						data->dice[3] = newDice[3];
						AddElementBegin(movesPossible, *data);
						movAddCell = 1;
					}
	
					// 4e dé ( dans le cas d'un double --> on fait comme si il y avait 4 dés)
					else if ( fabs(destCells[j] - srcCells[i]) == newDice[3] ){
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du dé
						data->moves[0].src_point= srcCells[i-1];
						data->moves[0].dest_point = destCells[i-1];
						data->dice[0] = newDice[0];
						data->dice[1] = newDice[1];
						data->dice[2] = newDice[2];
						data->dice[3] = 100;
						AddElementBegin(movesPossible, *data);
						movAddCell = 1;
					}
				}
			}
			if (movAddCell != 0){ // si on a ajouté au moins une combinaison (cell ) avec 2 moves alors on supprime la cell avec qu'un seul mov
				cellTemp = cellEnTraitement;
				DeleteCell(movesPossible, cellEnTraitement);
				cellEnTraitement = cellTemp;
				movAdd = 1;
			}
		}
	}
	return movesPossible;
}



SList* getMovesPossible(SGameState gameState, int nbMoves, SMove moves[4], Player player, unsigned char dice[2]){


	/*
	A FAIRE :
	dice le rendre 4 cases

	*/


	// tableau contenant les cases desquelles le pion peut partir
	unsigned int srcCells[25];
	int indexSrc = getSrcCells(gameState, player, srcCells); 

	// tableau contenant les cases sur lesquelles le pion peut arriver
	unsigned int destCells[25]; 
	int indexDest = getDestCells( gameState, player, destCells);

	

	//         ATTENTION
	// -- le joueur peut mettre dans bar que si le nb est pil celui du dé ou qu'il n'a plus de pions plus loin
	// -- si pion dans out, mouv obligatoire partant de out
	// -- si le joueur a un double il faut prévoir qu'il peut faire 4 mouvs




	// le joueur n'a pas de pions dans le bar et ne peut pas en mettre dans le out :

	// liste chainée de combinaisons de mouvements possibes
	SList* movesPossible; 

	//remplissage des premiers mouvements possibles
	movesPossibles = fillIn_1_MovesPossible( indexSrc, indexDest, srcCells, destCells, movesPossible);

	//     ATTENTION ACTUALISATION DU FAUX GAMESTATE


	// remplissage des deuxiemes mouvements possibles ( en fonction du premier)
	movesPossible = fillIn_2_movesPossible();

	//-------------------------------------------------------------------------------------------------------------------------------
	//REMPLISSAGE DES DEUXIEMES MOUVEMENTS POSSIBLES ( en fonction du premier)


	
	//----------------------------------------------------------------------------------------------------------------------------------------


}







// mouvements valides ? 
int validMoves( int nbMoves, SMove moves[4], SGameState gameState, unsigned char dices[2], Player player){ // retourne 0 ( pas de pb) ou 1 ( au moins un mouvement n'est pas correct)
	

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
void saveMatch(SGameState gs, char* p1Name,char* p2Name,Player player1){

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