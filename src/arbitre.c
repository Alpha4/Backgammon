/*
!!!!!!!!!!!!!!
A FAIRE 
!!!!!!!!!!!!!!

--> FAIRE LES FREES POUR LES MALLOCS !
*/


// pour l'interface
// make -f nomMakeFile(makefile_test_interface)

/*include de SDL*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"
#include "arbitre.h"
#include "liste.h"
#include "interface.h"






/**
 * Fonction qui renvoie le joueur ayant gagné ou NOBODY sinon
 * @param SGameState gameState
 *	l'état du jeu courant
 * @return Player
 *	le joueur gagnant
 * Le cas de gain par abandon est traité par le main.c
 */
Player isGameFinished(SGameState gameState,int penalty[2])
{
	if (gameState.out[BLACK] == 15)
		return BLACK;
	else if (gameState.out[WHITE] == 15)
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
  *   index de la premiere case vide du tableau srcCells
 **/
int getSrcCells(SGameState gameState, Player player, int* srcCells ){

	int index = 0; // indice de la premiere case vide du tableau
	int i;

	// si le joueur a au moins 1 pion dans le bar il ne peut que partir du bar
	if ( gameState.bar[player] > 0){
		srcCells[0] = 25;
		index ++;
	}
	// sinon on prend les cellules possédées par le joueur
	else{
		for (i=0; i < 24; i++){ 
			if ( gameState.board[i].owner == player){
				srcCells[index] = i+1;
				index ++;
			}
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
		//cellules possédées par le joueur ou personne
		if ( ( gameState.board[i].owner == player) || (gameState.board[i].owner == NOBODY) ){
			destCells[index] = i+1;
			index ++;
		}

		//cellules ou l'ennemi n'a qu'un pion
		else if ( gameState.board[i].nbDames == 1){
			destCells[index] = i+1;
			index ++;
		}
	}

	// si tous les pions du joueur sont dans les 6 dernieres cases (du joueur) --> il peut mettre des pions dans out[]

	// cas du joueur noir
	if (player == BLACK ){
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for ( i=18; i < 34; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[index] = 25; // out
			index ++;
		}
	}

	// cas du joueur blanc
	if ( player == WHITE ){	
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for (i=0; i < 6; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[index] = 25; //out
			index ++;
		}	
	}
	return index;
}



/**
 * Fonction qui renvoie un booléen indiquant si le joueur possède des pions plus loin du out que la source du mouvement
 * @param int numSrcCell 
 *   numéro de la cellule sur laquelle se trouve le pion pour lequel on veut vérifier qu'il peut aller dans le out
 * @param Player player
 *   joueur qui veut effectuer le mouvement
 * @param SGameState gameState
 *   état du jeu courant
 * @return int thereIsFartherPiece
 *   booléen indiquant si le joueur possède ou non des pions plus loin du out que la source du mouvement
 */
int thereIsFartherPiece(int numSrcCell, Player player, SGameState gameState){
	int thereIsFartherPiece = 0;

	// cas du joueur noir
	if ( player == BLACK){
		int i;
		// de la 6e case jusqu'a la case source du mouvement voulu (exclue) ( sens décroissant)
		// on regarde si le joueur possède des pions
		for (i=6; i>numSrcCell; i--){
			if (gameState.board[numSrcCell-1].owner == player){
				thereIsFartherPiece = 1;
			}
		}
	}

	//cas du joueur blanc
	if (player == WHITE){
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
 * Fonction qui renvoie un booléen indiquant si un pion peut etre déplacé dans le out ou non
 * @param SGameState
 *   état du jeu courant
 * @param Player player
 *   joueur qui veut effectuer le mouvement
 * @param int numDice
 *   valeur d'un dé testée
 * @param int numSrcCell
 *   cellule sur laquelle se trouve le pion pour lequel on veut savoir si il peut être déplacé dans le out ou non
 * @return int canGoToOut
 *   booléen indiquant si un pion peut etre déplacé dans le out ou non
 */
int canGoToOut(SGameState gameState, Player player, int numDice, int numSrcCell){

	int canGoToOut = 0; // initialisation à ne peut pas aller dans le out
	
	int i;

	// vérification que tous les pions du joueur sont dans ses 6 dernieres cases
	int piecesWellPlaced = 0;

	// cas du joueur noir
	if (player == BLACK ){

		int add = 0; // nombre de pions dans les 6 dernieres cases

		for ( i=18; i < 34; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			piecesWellPlaced = 1;
		}
	}

	// cas du joueur blanc
	if ( player == WHITE ){	
		int add = 0; // nombre de pions dans les 6 dernieres cases
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
		if ( ((player = BLACK) && (numSrcCell == numDice)) || ((player = WHITE) &&  ((25-numSrcCell) == numDice) )){
			canGoToOut = 1;
		}
		// sinon s'il n'y pas de pions plus loin et que le dé est plus grand que la distance entre le pion et le out, le mouvement est également autorisé
		else if( (!(thereIsFartherPiece(numSrcCell, player, gameState))) && ( (player == BLACK && numDice>numSrcCell) || (player == WHITE && numDice > (25-numSrcCell)) ) ){
			canGoToOut = 1;
		}
	}

	return canGoToOut;
}


/**
 * Fonction qui initialise les composantes d'un élément de la structure Data
 * @param Data* data
 *   pointeur vers l'élément de structure Data à initialiser
 */

void initData(Data* data){
	SMove emptyMove;
	emptyMove.src_point = 100;
	emptyMove.dest_point = 100;

	int i;
	for (i=0; i<4; i++){
		data->moves[i] = emptyMove;
		data->dice[i] = -1;
	}
	data->nbMoves = 0;

}

/**
 * Fonction qui actualise le gameState donné en appliquant le mouvement d'un pion
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int numDestCell
 *   numéro de la cellule d'arrivée du mouvement
 * @param SGameState gameState
 *   état du jeu courant
 * @param Player player
 *   joueur effectuant le mouvement
 * @return SGameState gameState
 *   état du jeu actualisé 
 */

SGameState actualizeGameState(int numSrcCell, int numDestCell, SGameState gameState, Player player){


	// si le pion sort du bar
	if (numSrcCell == 0){ 
		
		gameState.bar[player] --;

		//La case sur laquelle on arrive était vide
		if ( gameState.board[numDestCell-1].nbDames == 0 )
		{
			gameState.board[numDestCell-1].owner = player; 
		}

		//Cas de pions pris
		if ( (gameState.board[numDestCell-1].owner != player) && (gameState.board[numDestCell-1].owner != NOBODY) )
		{
			Player p = gameState.board[numDestCell-1].owner; //Ancien owner de la case prise
			gameState.board[numDestCell-1].owner = player; //Changement d'owner
			gameState.bar[p]++; // L'adversaire a une dame supplémentaire de sortie du jeu
			gameState.board[numDestCell-1].nbDames = 0; // Le placement de la dame est géré dans le cas général
		}

		//Cas général
		gameState.board[numSrcCell-1].nbDames --;
		gameState.board[numDestCell-1].nbDames ++;
	}

	// si le pion va dans le out
	else if (numDestCell == 25){ 
		
		gameState.out[player] ++;

		//La case de départ est maintenant vide
		if ( gameState.board[numSrcCell-1].nbDames == 1)
		{
			gameState.board[numSrcCell-1].owner = NOBODY;
		}

		//Cas général
		gameState.board[numSrcCell-1].nbDames --;
		gameState.board[numDestCell-1].nbDames ++;

	}

	// cas d'un mouvement basique
	else{ 
		
		//La case sur laquelle on arrive était vide
		if ( gameState.board[numDestCell-1].nbDames == 0 )
		{
			gameState.board[numDestCell-1].owner = player; 
		}

		//La case de départ est maintenant vide
		if ( gameState.board[numSrcCell-1].nbDames == 1)
		{
			gameState.board[numSrcCell-1].owner = NOBODY;
		}

		//Cas de pions pris
		if ( (gameState.board[numDestCell-1].owner != player) && (gameState.board[numDestCell-1].owner != NOBODY) )
		{
			Player p = gameState.board[numDestCell-1].owner; //Ancien owner de la case prise
			gameState.board[numDestCell-1].owner = player; //Changement d'owner
			gameState.bar[p]++; // L'adversaire a une dame supplémentaire de sortie du jeu
			gameState.board[numDestCell-1].nbDames = 0; // Le placement de la dame est géré dans le cas général
		}

		//Cas général
		gameState.board[numSrcCell-1].nbDames --;
		gameState.board[numDestCell-1].nbDames ++;


	}

	return gameState;
}

/**
 * Fonction qui créer et remplit les premiers mouvements possibles dans movesPossible
 * @param Player player
 *   joueur pour lequel les mouvements possibles sont calculés
 * @param : int dice[4]
 *   tableau contenant les valeurs disponibles de dé ( -1 = dé non disponible )
 * @param SGameState gameState
 *   état du jeu courant
 * @return SList* movesPossible
 *   pointeur vers la liste chainée dont les cellules contiennent, entre autres, un tableau de mouvements possible (SMove) (actualisé)
 */
SList* fillIn_1_MovesPossible( Player player, int dice[4], SGameState gameState){

	// création des listes contenant les cases de départ et d'arrivée possibles
	int srcCells[25];
	int indexSrc = getSrcCells(gameState, player, srcCells);

	int destCells[25];
	int indexDest= getDestCells(gameState, player, destCells);


	// création de la liste chainée contenant les mouvements possibles
	SList* movesPossible;
	movesPossible = CreateList();

	int i, j;
	for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
		for (j=0; j < indexDest; j++){ // j --> parcours de destCells

			// vérification que le mouvement est dans le bon sens suivant le joueur
			// cas particulier : si la cellule de départ est bar[]  ou celle d'arrivée le out[] alors on ne compte pas le sens
			if ( (player == BLACK && destCells[j] < srcCells[i]) || (player == WHITE && destCells[j] > srcCells[i]) || srcCells[i] == 0 
					|| destCells[j] == 25 ) {

				// cas où la cellule destination n'est pas le out
				if ( destCells[j] != 25){					

					// vérification que le mouvement correspond à un dé
					if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || (fabs(destCells[j] - srcCells[i]) == dice[1])  // cas "normal" 
						|| (fabs(destCells[j] - srcCells[i]) == dice[2]) || (fabs(destCells[j] - srcCells[i]) == dice[3])
						|| (srcCells[i]==0 && 25-destCells[j] == dice[0])|| (srcCells[j]==0 && 25-destCells[j] == dice[1]) // cas le joueur noir sort un pion du bar
						|| (srcCells[i]==0 && 25-destCells[j] == dice[2]) || (srcCells[j]==0 && 25-destCells[j] == dice[3]) 
						){  // remarque : cas ou un pion blanc sort du bar est traité dans "normal"
	
	
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier mouvement et l'état du jeu
						//initialisation de data
						//Data* data = malloc(sizeof(Data));
						Data data;
						initData(&data);
	
						//ajout du 1er mouvement
						SMove newMove;
						data.moves[0] = newMove;
						data.moves[0].src_point = srcCells[i];
						data.moves[0].dest_point = destCells[j];
						data.nbMoves = 1;
	
						// nouveau jeu de dé
	
						data.dice[0] = dice[0];
						data.dice[1] = dice[1];
						data.dice[2] = dice[2];
						data.dice[3] = dice[3];
	
						if ( (fabs(destCells[j] - srcCells[i]) == dice[0]) || ( (player == BLACK) && (25-destCells[j] == dice[0])) 
									|| ( (player == WHITE) && (destCells[j] == dice[0])) ){ // cas 1 : premier dé utilisé
	
							data.dice[0] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[1]) || (player == BLACK && 25-destCells[j] == dice[1]) 
									|| (player == WHITE && destCells[j] == dice[1]) ){ // cas 2 : 2e dé utilisé
	
							data.dice[1] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[2]) || (player == BLACK && 25-destCells[j] == dice[2]) 
									|| (player == WHITE && destCells[j] == dice[2]) ){ // cas 3 : 3e dé utilisé
	
							data.dice[2] = -1; // "supprime " le dé utilisé
						}
						else if ( (fabs(destCells[j] - srcCells[i]) == dice[3]) || (player == BLACK&& 25-destCells[j] == dice[3]) 
									|| (player == WHITE && destCells[j] == dice[3]) ){ // cas 4 : 4e dé utilisé
	
							data.dice[3] = -1; // "supprime " le dé utilisé
						}
	
	
						//nouveau gameState 
						data.gameState = actualizeGameState(srcCells[i], destCells[j], gameState, player);
						
	
						// ajout de la cellule
						AddElementBegin(movesPossible, data);
					}
				}
				// cas ou la cellule destination est le out ET le mouvement est possible avec le 1er dé
				else if ( destCells[j] == 25 && canGoToOut(gameState, player, dice[0], srcCells[i]) ){

					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier mouvement et l'état du jeu
					Data data;
					initData(&data);

					//ajout du 1er mouvement
					data.moves[0].src_point = srcCells[i];
					data.moves[0].dest_point = destCells[j];
					data.nbMoves = 1;

					// nouveau jeu de dé
					data.dice[0] = -1;
					data.dice[1] = dice[1];
					data.dice[2] = dice[2];
					data.dice[3] = dice[3];

					// nouveau gameState 
					data.gameState = actualizeGameState(srcCells[i], destCells[j], gameState, player);

					// ajout de la cellule
					AddElementBegin(movesPossible, data);
				}
				// cas ou la cellule destination est le out ET le mouvement est possible avec le 2e dé
				else if ( destCells[j] == 25 && canGoToOut(gameState, player, dice[1], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data data;
					initData( &data);

					//ajout du 1er mouvemnt
					data.moves[0].src_point = srcCells[i];
					data.moves[0].dest_point = destCells[j];
					data.nbMoves = 1;

					// nouveau jeu de dé
					data.dice[0] = dice[0];
					data.dice[1] = -1;
					data.dice[2] = dice[2];
					data.dice[3] = dice[3];

					// nouveau gameState 
					data.gameState = actualizeGameState(srcCells[i], destCells[j], gameState, player);

					// ajout de la cellule
					AddElementBegin(movesPossible, data);
				}
				// cas ou la cellule destination est le out ET le mouvement est possible avec le 3e dé
				else if( destCells[j] == 25 && canGoToOut(gameState, player, dice[2], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data data;
					initData(&data);

					//ajout du 1er mouvement
					data.moves[0].src_point = srcCells[i];
					data.moves[0].dest_point = destCells[j];
					data.nbMoves = 1;

					// nouveau jeu de dé
					data.dice[0] = dice[0];
					data.dice[1] = dice[1];
					data.dice[2] = -1;
					data.dice[3] = dice[3];

					// nouveau gameState
					data.gameState = actualizeGameState(srcCells[i], destCells[j], gameState, player);

					// ajout de la cellule
					AddElementBegin(movesPossible, data);
				}
				// cas ou la cellule destination est le out ET le mouvement est possible avec le 4e dé
				else if( destCells[j] == 25 && canGoToOut(gameState, player, dice[3], srcCells[i]) ){
					// remplissage de la liste chainée avec une nouvelle cellule contenant le premier move et l'état du jeu
					Data data;
					initData(&data);

					//ajout du 1er mouvement
					data.moves[0].src_point = srcCells[i];
					data.moves[0].dest_point = destCells[j];
					data.nbMoves = 1;

					// nouveau jeu de dé
					data.dice[0] = dice[0];
					data.dice[1] = dice[1];
					data.dice[2] = dice[2];
					data.dice[3] = -1;

					// nouveau gameState
					data.gameState = actualizeGameState(srcCells[i], destCells[j], gameState, player);

					// ajout de la cellule
					AddElementBegin(movesPossible, data);
				}
			}
		}
	}

	return movesPossible;
}



/**
 * Fonction qui supprime les celulles contenant moins de nbMoves mouvements
 * @param SList* movesPossible
 *   liste de laquelle on veut supprimer les cellules ne contenant pas assez de mouvements
 * @param int nbMoves
 *   nombre de mouvements minimal pour qu'une cellule ne soit pas supprimée
 */
void deleteCellsLessMoves(SList* movesPossible, int nbMoves){

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	// parcours des cellules de la liste
	while ( cellEnTraitement != GetLastElement(movesPossible)){ 
		
		SCell* cellNext = cellEnTraitement->next;

		// test si la cellule contient assez de mouvements
		if ( cellEnTraitement->value.nbMoves < nbMoves ){
			DeleteCell(movesPossible, cellEnTraitement);
		}
		cellEnTraitement = cellNext;
	}

}



/**
 * Fonction qui remplie les mouvements possibles dans movesPossible (les cellules de movesPossible doivent contenir au moins un mouvement)
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
 * @return int nbMovesInCells 
 *   nombre de mouvements contenus dans les cellules de movesPossible
 */
int fillIn_2_MovesPossible( Player player, SList* movesPossible, int nbMovesInCells ){

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	int movAdd =0; // pour savoir si on a rajouté des mouvements dans movesPossible ou non
	
	int i,j;

	while ( cellEnTraitement != GetLastElement(movesPossible)){ // parcours des cellules de la liste



		
		
		// etat du jeu courant de cette cellule ( appliquant au gameState les mouvement précédents contenus dans la celulle)
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



		for (i=0; i < indexSrc; i++){ // i --> parcours de srcCells
			for (j=0; j < indexDest; j++){ // j --> parcours de destCells


				// vérification que le mouvement est dans le bon sens suivant le joueur
				// cas particulier : si la cellule de départ est bar[]  ou celle d'arrivée le out[] alors on ne compte pas le sens
				if ( (player == BLACK && destCells[j] < srcCells[i]) || (player == WHITE && destCells[j] > srcCells[i]) || srcCells[i] == 0 
						|| destCells[i] == 25 ){
					
					// cas où la cellule destination n'est pas le out
					if ( destCells[i] != 25){

						// vérification que le mouvement correspond à un dé
						if ( (fabs(destCells[j] - srcCells[i]) == diceCell[0]) || (fabs(destCells[j] - srcCells[i]) == diceCell[1])  // cas "normal" 
							|| (fabs(destCells[j] - srcCells[i]) == diceCell[2]) || (fabs(destCells[j] - srcCells[i]) == diceCell[3])
							|| (srcCells[i]==0 && 25-destCells[j] == diceCell[0])|| (srcCells[j]==0 && 25-destCells[j] == diceCell[1]) // cas le joueur noir sort un pion du bar
							|| (srcCells[i]==0 && 25-destCells[j] == diceCell[2]) || (srcCells[j]==0 && 25-destCells[j] == diceCell[3]) 
							){  // remarque : cas ou un pion blanc sort du bar est traité dans "normal"

							// remplissage de la liste chainée avec une nouvelle cellule
							Data data;

							initData(&data);
							data.nbMoves = nbMovesInCells;

	

							//copie des mouvements déjà présents
							if (nbMovesInCells == 1){
								SMove move1;
								move1.src_point = cellEnTraitement->value.moves[0].src_point;
								move1.dest_point = cellEnTraitement->value.moves[0].dest_point;
								data.moves[0] = move1;
							}
							else if (nbMovesInCells == 2){
								SMove move1,move2;
								move1.src_point = cellEnTraitement->value.moves[0].src_point;
								move1.dest_point = cellEnTraitement->value.moves[0].dest_point;
								move2.src_point = cellEnTraitement->value.moves[1].src_point;
								move2.dest_point = cellEnTraitement->value.moves[1].dest_point;

								data.moves[0] = move1;
								data.moves[1] = move2;
							}
							else if (nbMovesInCells == 3){
								SMove move1,move2,move3;
								move1.src_point = cellEnTraitement->value.moves[0].src_point;
								move1.dest_point = cellEnTraitement->value.moves[0].dest_point;
								move2.src_point = cellEnTraitement->value.moves[1].src_point;
								move2.dest_point = cellEnTraitement->value.moves[1].dest_point;
								move3.src_point = cellEnTraitement->value.moves[2].src_point;
								move3.dest_point = cellEnTraitement->value.moves[2].dest_point;

								data.moves[0] = move1;
								data.moves[1] = move2;
								data.moves[2] = move3;
							}





							/*
							//copie des mouvements déjà présents
							int x;
							for (x=0; x < nbMovesInCells; x++){
								data.moves[i] = cellEnTraitement->value.moves[i];
							}
							*/
							
	
							// ajout du dernier mouvement
							data.moves[nbMovesInCells].src_point = srcCells[i];
							data.moves[nbMovesInCells].dest_point = destCells[i];
							data.nbMoves ++;
	
							// nouveau jeu de dé
							data.dice[0] = diceCell[0];
							data.dice[1] = diceCell[1];
							data.dice[2] = diceCell[2];
							data.dice[3] = diceCell[3];
	
							if ( (fabs(destCells[j] - srcCells[i]) == diceCell[0]) || (player == BLACK && 25-destCells[j] == diceCell[0]) 
									|| (player == WHITE && destCells[j] == diceCell[0]) ){ // cas 1 : premier dé utilisé
	
								data.dice[0] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[1]) || (player == BLACK && 25-destCells[j] == diceCell[1])
										|| (player == WHITE && destCells[j] == diceCell[1]) ){ // cas 2 : 2e dé utilisé
	
								data.dice[1] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[2]) || (player == BLACK && 25-destCells[j] == diceCell[2]) 
										|| (player == WHITE && destCells[j] == diceCell[2]) ){ // cas 3 : 3e dé utilisé
	
								data.dice[2] = -1; // "supprime " le dé utilisé
							}
							else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[3]) || (player == BLACK && 25-destCells[j] == diceCell[3]) 
										|| (player == WHITE && destCells[j] == diceCell[3]) ){ // cas 4 : 4e dé utilisé
	
								data.dice[3] = -1; // "supprime " le dé utilisé
							}
	
	
							// nouveau GameState
							data.gameState = actualizeGameState(srcCells[i], destCells[j], cellEnTraitement->value.gameState, player);
							

							//ajout de la cellule
							AddElementBegin(movesPossible, data);
							movAdd = 1;

						}
					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec 1er dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[0], srcCells[i]) ){

						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);

						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						// nouveau jeu de dé
						data.dice[0] = -1;
						data.dice[1] = diceCell[1];
						data.dice[2] = diceCell[2];
						data.dice[3] = diceCell[3];

						//nouveau gameState
						data.gameState = actualizeGameState(srcCells[i], destCells[j], cellEnTraitement->value.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;

					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 2e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[1], srcCells[i]) ){

						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);

						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						// nouveau jeu de dé
						data.dice[0] = diceCell[0];
						data.dice[1] = -1;
						data.dice[2] = diceCell[2];
						data.dice[3] = diceCell[3];

						//nouveau gameState
						data.gameState = actualizeGameState(srcCells[i], destCells[j], cellEnTraitement->value.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;
					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 3e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[2], srcCells[i]) ){
						
						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);

						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						// nouveau jeu de dé
						data.dice[0] = diceCell[0];
						data.dice[1] = diceCell[1];
						data.dice[2] = -1;
						data.dice[3] = diceCell[3];

						//nouveau gameState
						data.gameState = actualizeGameState(srcCells[i], destCells[j], cellEnTraitement->value.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;
					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 4e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[3], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);

						//copie des movements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}

						// ajout du dernier mouvementS
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						// nouveau jeu de dé
						data.dice[0] = diceCell[0];
						data.dice[1] = diceCell[1];
						data.dice[2] = diceCell[2];
						data.dice[3] = -1;

						//nouveau gameState
						data.gameState = actualizeGameState(srcCells[i], destCells[j], cellEnTraitement->value.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;
					}
				}
			}
			
		}
		cellEnTraitement = cellEnTraitement->next;
	}
	// si on a rajouté au moins un mouvement alors il faut voir si on peut encore en rajouter
	if ( movAdd != 0 ){ 

		nbMovesInCells ++;
		deleteCellsLessMoves( movesPossible , nbMovesInCells); // suppression des cellules ne contenant pas assez de mouvements
		// ( si une cellules contient 2 mouvements par exemples, alors il sera interdit au joueur de ne faire qu'un mouvement)

		if (nbMovesInCells != 4){
			nbMovesInCells = fillIn_2_MovesPossible(player, movesPossible, nbMovesInCells); 
		}
	}

	return nbMovesInCells;
}



/**
 * Fontion qui renvoie la liste de tous les mouvements possibles en fonction de l'état du jeu, du joueur et des dés
 * @param SGameState gameState
 *   état du jeu courant
 * @param Player player
 *   joueur pour lequel on souhaite avoir une liste de mouvements possibles
 * @param unsigned char diceGiven[2]
 *   tableau contenant les valeurs des dés 
 * @rparam SList* movesPossible
 *   liste de tous les mouvements possibles suivant le contexte de jeu
 * @return int nbMovesPossible
 *   nombre de mouvements contenus dans les cellule de movesPossible
 */ 


int getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2], SList* movesPossible){

	//transformation du dé en en un tableau de 4 entiers
	// pour pouvoir traiter le cas d'un double --> 4 dés pourront être utilisés
	int dice[4];

	 // si cest un double
	if (diceGiven[0] == diceGiven[1]){
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

	// nombre de mouvements contenus dans les cellules de movesPossible
	int nbMovesPossible = 0;


	//remplissage des premiers mouvements possibles
	movesPossible = fillIn_1_MovesPossible( player, dice, gameState);


	// si movesPossible n'est pas vide
	if (!(IsEmpty(movesPossible)) ){

		// remplissage des mouvements suivants:
		nbMovesPossible = fillIn_2_MovesPossible( player, movesPossible, 1);
	}

	// si le joueur ne peut jouer qu'un dé ( ne concerne pas le cas d'un double)
	// alors il est obligé de jouer le dé le plus élevé
	if (nbMovesPossible == 1){

		// dé le plus élevé :
		int diceToPlay;

		if (dice[0] > dice [1]){
			diceToPlay = 0;
		}
		else{
			diceToPlay = 1;
		}

		// suppression des cellule dont le mouvement n'utilise pas le dé le plus élevé: 

		SCell* cellEnTraitement = GetFirstElement(movesPossible);
		SCell* cellNext = cellEnTraitement->next;

		// parcours des cellules de la liste
		while ( cellEnTraitement != GetLastElement(movesPossible)){ 

			// si le dé la plus élevé n'est pas utilisé alors on supprime la cellule des movesPossible
			if ( cellEnTraitement->value.dice[diceToPlay] != -1 ){ 					
				DeleteCell(movesPossible, cellEnTraitement);
			}
			cellEnTraitement = cellNext;
		}
	}
	printList(movesPossible);
	return nbMovesPossible;
}



/**
 * Fonction qui renvoie un booléen indiquant si un tableau de mouvement est correcte ( suivant l'état du jeu)
 * @param SMove moves[4]
 *   tableau contenant la suite de mouvements à analyser
 * @param SGameState gameState
 *   état courant du jeu
 * @param unsigned char dice[2]
 *   tableau contenant les valeurs des dés
 * @param Player player
 *   joueur pour lequel on veut tester le tableau de mouvements
 */

int validMoves(int nbMoves, SMove moves[4], SGameState gameState, unsigned char dice[2], Player player){
	
	int valide = 0; // initialisation à "mouvements non correctes"

	//récupération des mouvements possibles :
	SList* movesPossible = NULL;
	int nbMovesPossible;
	nbMovesPossible = getMovesPossible(gameState, player, dice, movesPossible);

	// si le joueur ne peut jouer aucun des 2 dés et qu'il donne un tableau de mouvement vide, celui ci est accepté
	if ( (nbMovesPossible == nbMoves) && (nbMoves == 0) ){
		valide = 1;
	}

	// parcours des mouvements possibles (correctes ) :
	SCell* cellEnTraitement = GetFirstElement(movesPossible);
	while ( cellEnTraitement != GetLastElement(movesPossible)){ 

		// vérification que le nombre de mouvements est correcte ( = vérification que le joueur ne doit pas effectuer + de coups)
		if (nbMovesPossible == nbMoves){
			
			// vérification que chaque mouvement des tableaux sont identiques
			int same = 1; // initialisation à " mouvements identiques"
			int i;
			for (i=0; i<nbMoves; i++){
				if ( !(cellEnTraitement->value.moves[i].src_point == moves[i].src_point) && (cellEnTraitement->value.moves[i].dest_point == moves[i].dest_point) ){
					same = 0;
				}
			}
			//si tous les mouvements sont identiques alors le tableau de mouvements donné est correcte
			if ( same == 1 ){
				valide =1;
			}
		}
		cellEnTraitement = cellEnTraitement->next;
	}

	// libération de la mémoire allouée pour la liste movesPossible
	DeleteList(movesPossible);

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
 * @return char*
 *	le nom du gagnant
 *	FORMAT
 *	nomGagnant	points	nomPerdant	points\n
 */
char* saveMatch(SGameState gs, char* p1Name,char* p2Name,Player player1){

	FILE *file = NULL;  // Pointeur vers le fichier
	char* winner=NULL;
    file = fopen("result.txt", "a");  // Ouverture du fichier en mode "ajout" (on ajoute du contenu à la fin du fichier)

    if (file != NULL)  // Le fichier s'est bien ouvert
    {
    	if (gs.whiteScore>gs.blackScore) //BLANC 
    	{
    		if(player1==WHITE)
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p1Name,gs.whiteScore,p2Name,gs.blackScore);
    			winner=p1Name;
    		}
    		else
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p2Name,gs.blackScore,p1Name,gs.whiteScore);
    			winner=p2Name;
    		}
    	}
    	else
    	{
    		if(player1==WHITE)
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p2Name,gs.blackScore,p1Name,gs.whiteScore);
    			winner=p2Name;
    		}
    		else
    		{
    			fprintf(file, "%s\t%d\t%s\t%d\n",p1Name,gs.whiteScore,p2Name,gs.blackScore);
    			winner=p1Name;
    		}
    	}
    	fclose(file);  // On ferme le fichier
    }
    return winner;

}