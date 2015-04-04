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



 /**
  * Fonction qui rempli le tableau srcCells contenant les numéros de cellules pouvant être source d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être source d'un mouvement
  * @return int index
  *   index de la premiere case vide du tablea srcCells
 **/

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


 /**
  * Fonction qui rempli le tableau destCells contenant les numéros de cellules pouvant être destination d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être destination d'un mouvement
  * @return int index
  *   index de la premiere case vide du tablea destCells
  */

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



/**
 * Fonction qui renvoie un booléen indiquant si le joueur possède des pions plus loin du bar que la source du mouvement
 * @param int numSrcCell 
 *   numéro de la cellule surlaquelle se trouve le pion dont on veut vérifier qu'il puisse aller dans le bar
 * @param Player player
 *   joueur qui veut effectuer le mouvement
 * @param SGameState gameState
 *   état du jeu courant
 * @return int thereIsFartherPiece
 *   booléen indiquant si le joueur possède des pions plus loin du bar que la source du mouvement
 */

int thereIsFartherPiece(int numSrcCell, Player player, SGameState gameState){
	int thereIsFartherPiece = 0;

	// cas du joueur blanc
	if ( player == WHITE){
		int i;
		// de la 6e case jusqu'a la case source du mouvement voulu (exclue) ( sens décroissant)
		// on regarde si le joueur possède des pions
		for (i=6; i>numSrcCell; i--){
			if (gameState.board[numSrcCell-1].owner == player){
				thereIsFartherPiece = 1;
			}
		}
	}

	//cas du joueur noir
	if (player == BLACK){
		int i;
		// de la 19e case jusqu'a la case source du mouvement voulu (exclue) (sens croissant)
		// on regarde si le joueur possède des pions
		for (i=19; i<numSrcCell; i++){
			if (gameState.board[numSrcCell-1].owner == player){
				thereIsFartherPiece = 1;
			}
		}
	}

	return thereIsFartherPiece;
}


/**
 * Fonction qui renvoie un booléen indiquant si un pion peut etre déplacé dans le bar ou non
 * @param SGameState
 *   état du jeu courant
 * @param Player player
 *   joueur qui exécute le mouvement
 * @param int numDice
 *   valeur d'un dé testée
 * @param in numSrcCell
 *   cellule sur laquelle se trouve le pion dont on veut savoir si il peut être déplacé dans le bar ou non
 */

int canGoToBar(SGameState gameState, Player player, int numDice, int numSrcCell){

	int canGoToBar = 0; // initialisation à ne peut pas aller dans le bar
	
	int i;

	// vérification que tous les pions du joueur sont dans ses 6 dernieres cases
	int piecesWellPlaced = 0;
	// pour le joueur BLANC
	if (player == WHITE ){

		int add = 0; // nb de pionts dans les 6 dernieres cases

		for ( i=18; i < 34; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			piecesWellPlaced = 1;
		}
	}

	//pour le joueur NOIR
	if ( player == BLACK ){	
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for (i=0; i < 6; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			piecesWellPlaced = 1;
		}	
	}

	// si tous les pions sont dans les 6 dernieres cases du joueur : on regarde les conditions suivantes
	if (piecesWellPlaced){
		// si le mouvement correspond pile au dé le mouvement est autorisé
		if ( ((player = WHITE) && (numSrcCell == numDice)) || ((player = BLACK) &&  ((25-numSrcCell) == numDice) )){
			canGoToBar = 1;
		}
		// sinon s'il n'y pas de pions plus loin et que le dé est plus grand que la distance entre le pion et le bar, le mouvement est également autorisé
		else if( (!(thereIsFartherPiece(numSrcCell, player, gameState))) && ( (player == WHITE && numDice>numSrcCell) || (player == BLACK && numDice > (25-numSrcCell)) ) ){
			canGoToBar = 1;
		}
	}

	return canGoToBar;
}


/**
 * Fonction qui créer remplit les premiers mouvements possibles dans movesPossible
 * @param Player player
 *   joueur pour lequel les mouvements possibles sont calculés
 * @param : int dice[4]
 *   tableau contenant les valeurs disponibles de dé ( -1 = dé non disponible )
 * @param SGameState gameState
 *   état du jeu courant
 * @return SList* 
 *   pointeur vers la liste chainée dont les cellules contiennent, entre autres, un tableau de mouvements (SMove) (actualisé)
 */

SList* fillIn_1_MovesPossible( Player player, int dice[4], SGameState gameState){

	SList* movesPossible;

	// création des listes contenant les cases de départ et d'arrivée possibles
	int srcCells[25];
	int indexSrc = getSrcCells(gameState, player, srcCells);

	int destCells[25];
	int indexDest= getDestCells(gameState, player, destCells);

	movesPossible = CreateList();

	int i;
	int j;
	for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
		for (j=0; j < indexDest; j++){ // j --> parcours de destCells

			// vérification que le mouvement est dans le bon sens suivant le joueur
			// cas particulier : si la cellule de départ est out[]  ou celle d'arrivée le bar[] alors on ne compte pas le sens
			if ( (player == WHITE && destCells[j] < srcCells[i]) || (player == BLACK && destCells[j] > srcCells[i]) || srcCells[i] == 0 
					|| destCells[i] == 25 ) {

				// cas où la cellule destination n'est pas le bar
				if ( destCells[i] != 25){					

					if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (fabs(destCells[j] - srcCells[i]) == dice[1])  // cas "normal" 
						|| (fabs(destCells[j] - srcCells[i]) == dice[2]) || (fabs(destCells[j] - srcCells[i]) == dice[3])
						|| 25-destCells[j] == dice[0] || 25-destCells[j] == dice[1] // cas : le joueur blanc sort un pion du out
						|| 25-destCells[j] == dice[2] || 25-destCells[j] == dice[3]
						){  // remarque : cas ou un pion noir sort du out est traité dans "normal"
	
	
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
						Data* data = malloc(sizeof(Data));
	
						//ajout du 1er move
						data->moves[0].src_point = srcCells[i];
						data->moves[0].dest_point = destCells[j];
						data->nbMoves = 1;
	
						// nouveau jeu de dé
	
						data->dice[0] = dice[0];
						data->dice[1] = dice[1];
						data->dice[2] = dice[2];
						data->dice[3] = dice[3];
	
						if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || ( (player == WHITE) && (25-destCells[j] == dice[0])) 
									|| ( (player == BLACK) && (destCells[j] == dice[0])) ){ // cas 1 : premier dé utilisé
	
							data->dice[0] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[1]) || (player == WHITE && 25-destCells[j] == dice[1]) 
									|| (player == BLACK && destCells[j] == dice[1]) ){ // cas 2 : 2e dé utilisé
	
							data->dice[1] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[2]) || (player == WHITE && 25-destCells[j] == dice[2]) 
									|| (player == BLACK && destCells[j] == dice[2]) ){ // cas 3 : 3e dé utilisé
	
							data->dice[2] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[3]) || (player == WHITE && 25-destCells[j] == dice[3]) 
									|| (player == BLACK && destCells[j] == dice[3]) ){ // cas 4 : 4e dé utilisé
	
							data->dice[3] = -1; // "supprime " le dé utilisé
						}
	

	
						//nouveau gameState  			PRENDRE CE BOUT DE CODE SUR AURELIEN
						// ATTENTION REMPLACER DANS LE CODE D'AURELIEN GAMESTATE PAR NEWGAMESTATE (OU DATA->GAMESTATE)
						
	
						// ajout de la cellule
						AddElementBegin(movesPossible, *data);
					}
				}
				// cas ou la cellule destination est le bar ET le mouvement est possible avec 1er dé
				else if ( destCells[j] == 25 && canGoToBar(gameState, player, dice[0], srcCells[i]) ){

					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data* data = malloc(sizeof(Data));

					//ajout du 1er move
					data->moves[0].src_point = srcCells[i];
					data->moves[0].dest_point = destCells[j];
					data->nbMoves = 1;

					// nouveau jeu de dé
					data->dice[0] = -1;
					data->dice[1] = dice[1];
					data->dice[2] = dice[2];
					data->dice[3] = dice[3];

					// nouveau gameState  A PRENDRE SUR AURELIEN
				}
				// cas ou la cellule destination est le bar ET le mouvement est possible avec 2e dé
				else if ( destCells[j] == 25 && canGoToBar(gameState, player, dice[1], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data* data = malloc(sizeof(Data));

					//ajout du 1er move
					data->moves[0].src_point = srcCells[i];
					data->moves[0].dest_point = destCells[j];
					data->nbMoves = 1;

					// nouveau jeu de dé
					data->dice[0] = dice[0];
					data->dice[1] = -1;
					data->dice[2] = dice[2];
					data->dice[3] = dice[3];

					// nouveau gameState  A PRENDRE SUR AURELIEN
				}
				// cas ou la cellule destination est le bar ET le mouvement est possible avec 3e dé
				else if( destCells[j] == 25 && canGoToBar(gameState, player, dice[2], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data* data = malloc(sizeof(Data));

					//ajout du 1er move
					data->moves[0].src_point = srcCells[i];
					data->moves[0].dest_point = destCells[j];
					data->nbMoves = 1;

					// nouveau jeu de dé
					data->dice[0] = dice[0];
					data->dice[1] = dice[1];
					data->dice[2] = -1;
					data->dice[3] = dice[3];

					// nouveau gameState  A PRENDRE SUR AURELIEN
				}
				// cas ou la cellule destination est le bar ET le mouvement est possible avec 4e dé
				else if( destCells[j] == 25 && canGoToBar(gameState, player, dice[3], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data* data = malloc(sizeof(Data));

					//ajout du 1er move
					data->moves[0].src_point = srcCells[i];
					data->moves[0].dest_point = destCells[j];
					data->nbMoves = 1;

					// nouveau jeu de dé
					data->dice[0] = dice[0];
					data->dice[1] = dice[1];
					data->dice[2] = dice[2];
					data->dice[3] = -1;

					// nouveau gameState  A PRENDRE SUR AURELIEN
				}
			}
		}
	}
	return movesPossible;
}



/**
 * Fonction qui supprime les celulles contenant moins de nbMoves mouvements
 * @param SList* movesPossible
 *   liste delaquelle on veut supprimer les cellules ne contenant pas assez de mouvements
 * @param int nbMoves
 *   nombre de mouvement minimal pour qu'une cellule ne soit pas supprimée
 */

void deleteCellsLessMoves(SList* movesPossible, int nbMoves){

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
!!!!!!!!!!!!!!!!!!!!!!!!!
COMMENTAIRE A MODIFIER
!!!!!!!!!!!!!!!!!!!!!!!!!!!
principe de fillIn_2_MovesPossible :
	on copie le gameState et pour chaque cellule de la liste ( contenant pour l'instant 1 et un seul mouvement)
	on le change pour appliquer ce mouvement. On va ensuite déterminer quels sont les mouvements ( n°2 ) possibles
	pour chacun des mouvements n°2 possibles on créer une nouvelle cellule dans la liste avec move1 ET mov2
	a la fin de l'analyse des move2 possibles, on détruit la cellule ne contenant que mov1
*/

/**
 * Fonction qui remplie les mouvements possibles dans movesPossible
 * @param Player player
 *   joueur pour lequel les mouvements possibles sont calculés
 * @param SList* movesPossible
 *   liste chainée dont les cellules contiennent un tableau de mouvements possible
 * @param : int dice[4]
 *   tableau contenant les valeurs disponibles de dé ( -1 = dé non disponible )
 * @param SGameState gameState
 *   état du jeu courant
 * @param int nbMovesInCells
 *   nombre de mouvements contenus dans chaque cellule de movesPossible ( ce nombre est constant suivant les cellules)
 * @return SList* 
 *   pointeur vers la liste movesPossible actualisé
 */
SList* fillIn_2_MovesPossible( Player player, SList* movesPossible, int nbMovesInCells ){

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	int movAdd =0; // pour savoir si on a rajouté des movesPossible ou pas
	
	int i,j;

	while ( cellEnTraitement != GetLastElement(movesPossible)){ // parcours des cellules de la liste
		
		/*
		!!!!!!!!!!!!!!!!!!!!!!!!!!!
		REMPLACER GAMESTATE PAR CELLENTRAITEMENT->VALUE.GAMESTATE  	ET DICE PAR CELENTRAITEMENT->VALUE.DICE
		!!!!!!!!!!!!!!!!!!!!!!!!!!
		*/

		// etat du jeu courant de cette cellule ( appliquant les mouvement précédents contenus dans la celulle)
		SGameState gameStateCell = cellEnTraitement->value.gameState;
		int diceCell[4];
		diceCell[0] = cellEnTraitement->value.dice[0];
		diceCell[1] = cellEnTraitement->value.dice[1];
		diceCell[2] = cellEnTraitement->value.dice[2];
		diceCell[3] = cellEnTraitement->value.dice[3];
		

		// création des listes contenant les cases de départ et d'arrivée possibles
		int srcCells[25];
		int indexSrc = getSrcCells(gameStateCell, player, srcCells);

		int destCells[25];
		int indexDest= getDestCells(gameStateCell, player, destCells);

		int movAddCell =0; // pour savoir si on a rajouté des moves dans la cellule (combinaison de moves) ou pas

		for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
			for (j=0; j < indexDest; j++){ // j --> parcours de destCells


				// vérification que le mouvement est dans le bon sens suivant le joueur
				// cas particulier : si la cellule de départ est out[]  ou celle d'arrivée le bar[] alors on ne compte pas le sens
				if ( (player == WHITE && destCells[j] < srcCells[i]) || (player == BLACK && destCells[j] > srcCells[i]) || srcCells[i] == 0 
						|| destCells[i] == 25 ){
					
					// cas où la cellule destination n'est pas le bar
					if ( destCells[i] != 25){

						// vérification que le mouvement correspond à un dé
						if ( (fabs(destCells[j] - srcCells[i]) == diceCell[0]) || (fabs(destCells[j] - srcCells[i]) == diceCell[1])  // cas "normal" 
							|| (fabs(destCells[j] - srcCells[i]) == diceCell[2]) || (fabs(destCells[j] - srcCells[i]) == diceCell[3])
							|| 25-destCells[j] == diceCell[0] || 25-destCells[j] == diceCell[1] // cas le joueur blanc sort un pion du out
							|| 25-destCells[j] == diceCell[2] || 25-destCells[j] == diceCell[3] 
							){  // remarque : cas ou un pion noir sort du out est traité dans "normal"
							
							// remplissage de la liste chainée avec une nouvelle cellule
							Data* data = malloc(sizeof(data));
	
							//copie des moves déjà présents
							int x;
							for (x=0; x < nbMovesInCells; x++){
								data->moves[i] = cellEnTraitement->value.moves[i];
							}
							
	
							// ajout du dernier move
							data->moves[nbMovesInCells].src_point = srcCells[i];
							data->moves[nbMovesInCells].dest_point = destCells[i];
							data->nbMoves ++;
	
							// nouveau jeu de dé
							data->dice[0] = diceCell[0];
							data->dice[1] = diceCell[1];
							data->dice[2] = diceCell[2];
							data->dice[3] = diceCell[3];
	
							if ( (fabs(destCells[j] - srcCells[i]) == diceCell[0]) || (player == WHITE && 25-destCells[j] == diceCell[0]) 
									|| (player == BLACK && destCells[j] == diceCell[0]) ){ // cas 1 : premier dé utilisé
	
								data->dice[0] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[1]) || (player == WHITE && 25-destCells[j] == diceCell[1])
										|| (player == BLACK && destCells[j] == diceCell[1]) ){ // cas 2 : 2e dé utilisé
	
								data->dice[1] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[2]) || (player == WHITE && 25-destCells[j] == diceCell[2]) 
										|| (player == BLACK && destCells[j] == diceCell[2]) ){ // cas 3 : 3e dé utilisé
	
								data->dice[2] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[3]) || (player == WHITE && 25-destCells[j] == diceCell[3]) 
										|| (player == BLACK && destCells[j] == diceCell[3]) ){ // cas 4 : 4e dé utilisé
	
								data->dice[3] = -1; // "supprime " le dé utilisé
							}
	
	
							// nouveau GameState  						PRENDRE CE BOUT DE CODE SUR AURELIEN
							/*data->gameState = gameState;
							data->GameState.board[srcCells[i]].nbDames --; // retirer le pion de la case de départ
		
							if ( data->GameState.board[srcCells[i]].nbDames ){ // s'il n'y a plus de pion sur la case de départ alors on change l'owner à personne
								data->GameState.board[srcCells[i]].owner = NOBODY;
							}
		
							data->GameState.board[destCells[j]].nbDames ++; // ajout du pion sur la case d'arrivée
							data->GameState.board[destCells[j]].owner = player;// actualisation de l'owner de la cellule ( au cas ou le joueur ne la possédait pas déjà)
							*/
	
							//ajout de la cellule
							AddElementBegin(movesPossible, *data);
							movAddCell = 1;
						}
					}
					// cas ou la cellule destination est le bar ET le mouvement est possible avec 1er dé
					else if ( destCells[j] == 25 && canGoToBar(gameStateCell, player, diceCell[0], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data* data = malloc(sizeof(data));

						//copie des moves déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data->moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier move
						data->moves[nbMovesInCells].src_point = srcCells[i];
						data->moves[nbMovesInCells].dest_point = destCells[j];
						data->nbMoves ++;

						// nouveau jeu de dé
						data->dice[0] = -1;
						data->dice[1] = diceCell[1];
						data->dice[2] = diceCell[2];
						data->dice[3] = diceCell[3];

						//nouveau gameState
						// A VOIR AVEC AURELIEN

					}
					// cas ou la cellule destination est le bar ET le mouvement est possible avec le 2e dé
					else if ( destCells[j] == 25 && canGoToBar(gameStateCell, player, diceCell[1], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data* data = malloc(sizeof(data));

						//copie des moves déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data->moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier move
						data->moves[nbMovesInCells].src_point = srcCells[i];
						data->moves[nbMovesInCells].dest_point = destCells[j];
						data->nbMoves ++;

						// nouveau jeu de dé
						data->dice[0] = diceCell[0];
						data->dice[1] = -1;
						data->dice[2] = diceCell[2];
						data->dice[3] = diceCell[3];

						//nouveau gameState
						// A VOIR AVEC AURELIEN
					}
					// cas ou la cellule destination est le bar ET le mouvement est possible avec le 3e dé
					else if ( destCells[j] == 25 && canGoToBar(gameStateCell, player, diceCell[2], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data* data = malloc(sizeof(data));

						//copie des moves déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data->moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier move
						data->moves[nbMovesInCells].src_point = srcCells[i];
						data->moves[nbMovesInCells].dest_point = destCells[j];
						data->nbMoves ++;

						// nouveau jeu de dé
						data->dice[0] = diceCell[0];
						data->dice[1] = diceCell[1];
						data->dice[2] = -1;
						data->dice[3] = diceCell[3];

						//nouveau gameState
						// A VOIR AVEC AURELIEN
					}
					// cas ou la cellule destination est le bar ET le mouvement est possible avec le 4e dé
					else if ( destCells[j] == 25 && canGoToBar(gameStateCell, player, diceCell[3], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data* data = malloc(sizeof(data));

						//copie des moves déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data->moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier move
						data->moves[nbMovesInCells].src_point = srcCells[i];
						data->moves[nbMovesInCells].dest_point = destCells[j];
						data->nbMoves ++;

						// nouveau jeu de dé
						data->dice[0] = diceCell[0];
						data->dice[1] = diceCell[1];
						data->dice[2] = diceCell[2];
						data->dice[3] = -1;

						//nouveau gameState
						// A VOIR AVEC AURELIEN
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
		movesPossible = fillIn_2_MovesPossible(player, movesPossible, nbMovesInCells); 
	}

	return movesPossible;
}


/**
 * Fontion qui renvoie la liste de tous les mouvements possibles en fonction de l'état du jeu, du joueur et des dés
 * @param SGameState gameSate
 *   état du jeu courant
 * @param Player player
 *   joueur pour lequel on souhaite avoir une liste de mouvements possibles
 * @param unsigned char diceGiven[2]
 *   tableau contenant les valeurs des dés 
 * @return SList* movesPossible
 *   liste de tous les mouvements possibles suivant le contexte de jeu
 */ 

SList* getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2]){

	//transformation du dé en en un tableau de 4 cases
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

	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//         ATTENTION
	// -- si le joueur ne peut pas jouer tous ces dés il doit jouer le + élevé --> nouvelle fonction
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	// liste chainée de combinaisons de mouvements possibes
	SList* movesPossible; 

	//remplissage des premiers mouvements possibles
	movesPossible = fillIn_1_MovesPossible( player, dice, gameState);


	// remplissage des mouvements suivants:
	movesPossible = fillIn_2_MovesPossible( player, movesPossible, 1);


	// A FAIRE --> SI LE JOUEUR NE PEUT PAS JOUER TOUS CES DES IL DOIT JOUER LE PLUS ELEVE

	return movesPossible;
}



/**
 * Fonction qui renvoie un booléen indiquant si un tableau de mouvement est correcte
 * @param nbMoves
 *   nombre de mouvements contenus dans le tableau de mouvements fourni
 * @param SMove moves[4]
 *   tableau contenant la suite de mouvements à analyser
 * @param SGameState gameState
 *   état courant du jeu
 * @param unsigned char dice[2]
 *   tableau contenant les valeurs des dés
 * @param Player player
 *    joueur pour lequel on veut tester le tableau de mouvements
 */

int validMoves( int nbMoves, SMove moves[4], SGameState gameState, unsigned char dices[2], Player player){ // retourne 0 ( pas de pb) ou 1 ( au moins un mouvement n'est pas correct)
	

// Smove movesPossibles[512][4]

	int valide = 0; // initialisation à " mouvements correctes"


	return valide;
	
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