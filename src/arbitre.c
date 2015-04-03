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






// NUMEROS DE CELLULES DEPUIS LESQUELLES LE PION PEUT PARTIR (cellules possédées par le joueur	)
int getSrcCells(SGameState gameState, Player player, int* srcCells ){

	int index = 0; // indice de la premiere case vide du tableau
	int i;

	// si le joueur a au moins 1 pion dans le out il ne peut que partir du out
	if ( gameState.out[player] > 0){
		srcCells[0] = 0;
		index ++;
		return index;
	}
	for (i=0; i < 24; i++){ 
		if ( gameState.board[i].owner == player){
			srcCells[index] = i+1;
			index ++;
		}
	}
	return index;

}




// NUMEROS DE CELLULES SUR LESQUELLES LE PION PEUT ARRIVER
int getDestCells( SGameState gameState, Player player, int* destCells ){
	
	int index = 0; // indice de la premiere case vide du tableau
	int i;
	for (i=0; i < 24; i++){ 
		//cellules possédées par personne ou le joueur
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

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		FONCTION CANGOTOBAR A FAIRE
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/



// remplissage des premiers mouvements possibles dans la liste
SList* fillIn_1_MovesPossible( Player player, SList* movesPossible, int dice[4], SGameState gameState){

	// création des listes contenant les cases de départ et d'arrivée possibles
	int* srcCells[25];
	int indexSrc = getSrcCells(gameState, player, srcCells);

	int* destCells[25];
	int indexDest= getDestCells(gameState, player, destCells);

	movesPossible = CreateList();

	int i;
	int j;
	for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
		for (j=0; j < indexDest; j++){ // j --> parcours de destCells

			// vérification que le mouvement est dans le bon sens suivant le joueur
			// cas particulier : si la cellule de départ est out[]  ou celle d'arrivée le bar[] alors on ne compte pas le sens
			if ( (player == WHITE && destCells[j] < srcCells[i]) || (player == BLACK && destCells[j] > srcCells[i]) || srcCell[i] == 0 
					|| destCell[i] == 25 ) {

				// vérification que, si la cellule d'arrivée est le bar, le jouer a le droit d'y mettre son pion
				if ( (destCell[i] != 25) || (destCell[i] == 25 &&  canGoToBar()) ){		

					// vérification que le mouvement correspond à un dé
					if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (fabs(destCells[j] - srcCells[i]) == dice[1])  // cas "normal" 
						|| (fabs(destCells[j] - srcCells[i]) == dice[2]) || (fabs(destCells[j] - srcCells[i]) == dice[3])
						|| 25-destCell[j] == dice[0] || 25-destCell[j] == dice[1] // Traite 2 cas : le joueur blanc sort un pion du out
						|| 25-destCell[j] == dice[2] || 25-destCell[j] == dice[3] // et le cas ou le joueur noir met un pion dans le bar
						){  // remarque : les cas ou un pion noir sort du out et celui ou un pion blanc va dans le bar sont traité par le cas "normal"
	
	
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
						Data* data = malloc(sizeof(Data));
	
						//ajout du 1er move
						data->moves[0].src_point = srcCells[i-1];
						data->moves[0].dest_point = destCells[i-1];
						data->nbMoves = 1;
	
						// nouveau jeu de dé
	
						data->dice[0] = dice[0];
						data->dice[1] = dice[1];
						data->dice[2] = dice[2];
						data->dice[3] = dice[3];
	
						if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (player == WHITE && 25-destCell[j] == dice[0]) 
									|| (player == BLACK && destCell[j] == dice[0]) ){ // cas 1 : premier dé utilisé
	
							data->dice[0] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[1]) || (player == WHITE && 25-destCell[j] == dice[1]) 
									|| (player == BLACK && destCell[j] == dice[1]) ){ // cas 2 : 2e dé utilisé
	
							data->dice[1] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[2]) || (player == WHITE && 25-destCell[j] == dice[2]) 
									|| (player == BLACK && destCell[j] == dice[2]) ){ // cas 3 : 3e dé utilisé
	
							data->dice[2] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[3]) || (player == WHITE && 25-destCell[j] == dice[3]) 
									|| (player == BLACK && destCell[j] == dice[3]) ){ // cas 4 : 4e dé utilisé
	
							data->dice[3] = -1; // "supprime " le dé utilisé
						}
	
	
	
						//nouveau gameState  			PRENDRE CE BOUT DE CODE SUR AURELIEN
						// ATTENTION REMPLACER DANS LE CODE D'AURELIEN GAMESTATE PAR NEWGAMESTATE (OU DATA->GAMESTATE)
						data->gameState = gameState;
						data->gameState.board[srcCells[i-1]].nbDames --; // retirer le pion de la case de départ
	
						if ( data->GameState.board[srcCells[i-1]].nbDames ){ // s'il n'y a plus de pion sur la case de départ alors on change l'owner à personne
							data->GameState.board[srcCells[i-1]].owner = NOBODY;
						}
	
						data->GameState.board[destCells[i-1]].nbDames ++; // ajout du pion sur la case d'arrivée
						data->GameState.board[destCells[i-1]].owner = player;// actualisation de l'owner de la cellule ( au cas ou le joueur ne la possédait pas déjà)
	
						// ajout de la cellule
						AddElementBegin(movesPossible, *data);
					}
				}
			}
		}
		return movesPossible;
	}
}



// supprime les cellules qui ont moins de nbMoves mouvements
void deletCellsLessMoves(SList* movesPossible, int nbMoves){

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	while ( cellEnTraitement != GetLastElement(movesPossible)){ // parcours des cellules de la liste
		
		SCell* cellNext = cellEnTraitement->next;

		if ( cellEnTraitement->value.nbMoves < nbMoves ){
			DeleteCell(movesPossible, cellEnTraitement);
		}

		cellEnTraitement = cellNext;
	}

}

/*
fillIn 2 prend une liste avec que des cellules comprenant le meme nombre de mouvement
entre chaque utilisation du fillIN 2 on fait un deleteCellLessMoves pour n'avoir que le nb max de moves possibles dans la liste
puis on peut réappliquer fill in 2 en incrémentant le compteur du nombre de mouvement dans les cellules de la liste

*/

/*
principe de fillIn_2_MovesPossible :
	on copie le gameState et pour chaque cellule de la liste ( contenant pour l'instant 1 et un seul mouvement)
	on le change pour appliquer ce mouvement. On va ensuite déterminer quels sont les mouvements ( n°2 ) possibles
	pour chacun des mouvements n°2 possibles on créer une nouvelle cellule dans la liste avec move1 ET mov2
	a la fin de l'analyse des move2 possibles, on détruit la cellule ne contenant que mov1
*/
SList* fillIn_2_MovesPossible( Player player, SList* movesPossible, int dice[4], SGameState gameState, int nbMovesInCells ){

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	int movAdd =0; // pour savoir si on a rajouté des movesPossible ou pas
	
	int i,j;

	while ( cellEnTraitement != GetLastElement(movesPossible)){ // parcours des cellules de la liste
		
		// création des listes contenant les cases de départ et d'arrivée possibles
		int* srcCells[25];
		int indexSrc = getSrcCells(gameState, player, srcCells);

		int* destCells[25];
		int indexDest= getDestCells(gameState, player, destCells);

		int movAddCell =0; // pour savoir si on a rajouté des moves dans la cellule (combinaison de moves) ou pas

		for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
			for (j=0; j < indexDest; j++){ // j --> parcours de destCells


				// vérification que le mouvement est dans le bon sens suivant le joueur
				// cas particulier : si la cellule de départ est out[]  ou celle d'arrivée le bar[] alors on ne compte pas le sens
				if ( (player == WHITE && destCells[j] < srcCells[i]) || (player == BLACK && destCells[j] > srcCells[i]) || srcCell[i] == 0 
						|| destCell[i] == 25 ){
					
					// vérification que, si la cellule d'arrivée est le bar, le joueur en a bien le droit d'y mettre son pion
					if ( (destCell[i] != 25) || (destCell[i] == 25 &&  canGoToBar()) ){	

						// vérification que le mouvement correspond à un dé
						if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (fabs(destCells[j] - srcCells[i]) == dice[1])  // cas "normal" 
							|| (fabs(destCells[j] - srcCells[i]) == dice[2]) || (fabs(destCells[j] - srcCells[i]) == dice[3])
							|| 25-destCell[j] == dice[0] || 25-destCell[j] == dice[1] // Traite 2 cas : le joueur blanc sort un pion du out
							|| 25-destCell[j] == dice[2] || 25-destCell[j] == dice[3] // et le cas ou le joueur noir met un pion dans le bar
							){  // remarque : les cas ou un pion noir sort du out et ___ sont traité par le cas "normal"
							
							// remplissage de la liste chainée avec une nouvelle cellule
							Date* data = malloc(sizeof(data));
	
							//copie des moves déjà présents
							int x;
							for (x=0; x < nbMovesInCells; x++){
								data->moves[i] = cellEnTraitement->data.moves[i];
							}
							
	
							// ajout du dernier move
							data->moves[nbMovesInCells].src_point = srcCells[i-1];
							data->moves[nbMovesInCells].dest_point = destCells[i-1];
							data->nbMoves ++;
	
							// nouveau jeu de dé
							data->dice[0] = cellEnTraitement->value.dice[0];
							data->dice[1] = cellEnTraitement->value.dice[1];
							data->dice[2] = cellEnTraitement->value.dice[2];
							data->dice[3] = cellEnTraitement->value.dice[3];
	
							if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (player == WHITE && 25-destCell[j] == dice[0]) 
									|| (player == BLACK && destCell[j] == dice[0]) ){ // cas 1 : premier dé utilisé
	
								data->dice[0] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == dice[1]) || (player == WHITE && 25-destCell[j] == dice[1])
										|| (player == BLACK && destCell[j] == dice[1]) ){ // cas 2 : 2e dé utilisé
	
								data->dice[1] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == dice[2]) || (player == WHITE && 25-destCell[j] == dice[2]) 
										|| (player == BLACK && destCell[j] == dice[2]) ){ // cas 3 : 3e dé utilisé
	
								data->dice[2] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == dice[3]) || (player == WHITE && 25-destCell[j] == dice[3]) 
										|| (player == BLACK && destCell[j] == dice[3]) ){ // cas 4 : 4e dé utilisé
	
								data->dice[3] = -1; // "supprime " le dé utilisé
							}
	
	
							// nouveau GameState  						PRENDRE CE BOUT DE CODE SUR AURELIEN
							data->gameState = gameState;
							data->GameState.board[srcCells[i-1]].nbDames --; // retirer le pion de la case de départ
		
							if ( data->GameState.board[srcCells[i-1]].nbDames ){ // s'il n'y a plus de pion sur la case de départ alors on change l'owner à personne
								data->GameState.board[srcCells[i-1]].owner = NOBODY;
							}
		
							data->GameState.board[destCells[i-1]].nbDames ++; // ajout du pion sur la case d'arrivée
							data->GameState.board[destCells[i-1]].owner = player;// actualisation de l'owner de la cellule ( au cas ou le joueur ne la possédait pas déjà)
	
	
							//ajout de la cellule
							AddElementBegin(movesPossible, *data);
							movAddCell = 1;
						}
					}
				}
			}

			SCell* cellNext = cellEnTraitement->next;

			if (movAddCell != 0){ // si on a ajouté au moins un mouvement (une cellule ) alors on supprime la cell avec moins de moves
				DeleteCell(movesPossible, cellEnTraitement);
				movAdd = 1;
			}
			cellEnTraitement = cellNext;	
		}
	}


	if (movAdd != 0){ // si on a rajouté au moins un mouvement alors il faut voir si avec le nouveau gameState et le nouveau jeu de dé de ce move 
					  // on peut encore faire d'autres moves

		nbMovesInCells ++;
		deleteCellsLessMoves( movesPossible , nbMovesInCells);
		movesPossible = fillIn_2_movesPossible(player, movesPossible, gameState, nbMovesInCells); 
	}

	return movesPossible;
}



SList* getMovesPossible(SGameState gameState, int nbMoves, SMove moves[4], Player player, unsigned char diceGiven[2]){

	int dice[4];

	if (diceGiven[0] == diceGiven[1]){ // si cest un double on permet au joueur de de joueur 4 fois la valeur du dé
		dice[0] = (int)diceGiven[0];
		dice[1] = (int)diceGiven[0];
		dice[2] = (int)diceGiven[0];
		dice[3] = (int)diceGiven[0];
	}
	else{
		dice[0] = (int)diceGiven[0];
		dice[1] = (int)diceGiven[1];
		dice[2] = -1;
		dice[3] = -1;		
	}


	// tableau contenant les cases desquelles le pion peut partir
	int srcCells[25];
	int indexSrc = getSrcCells(gameState, player, srcCells); 

	// tableau contenant les cases sur lesquelles le pion peut arriver
	int destCells[25]; 
	int indexDest = getDestCells( gameState, player, destCells);

	

	//         ATTENTION

	// -- le joueur peut mettre dans bar que si le nb est pil celui du dé ou qu'il n'a plus de pions plus loin
	// -- si le joueur ne peut pas jouer tous ces dés il doit jouer le + élevé --> nouvelle fonction


	// liste chainée de combinaisons de mouvements possibes
	SList* movesPossible; 

	//remplissage des premiers mouvements possibles
	movesPossible = fillIn_1_MovesPossible( indexSrc, indexDest, srcCells, destCells, player, movesPossible, dice);


	// remplissage des mouvements suivants:
	movesPossible = fillIn_2_movesPossible( indexSrc, indexDest, srcCells, destCells, player, movesPossible, dice, 1);


	// A FAIRE --> SI LE JOUEUR NE PEUT PAS JOUER TOUS CES DES IL DOIT JOUER LE PLUS ELEVE


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