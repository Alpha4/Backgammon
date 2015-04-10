#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "arbitre.h"


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
  * Fonction qui remplit le tableau srcCells contenant les numéros de cellules pouvant être source d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être source d'un mouvement
  * @return int index
  *   index de la premiere case vide du tableau srcCells
 **/
int getSrcCells(SGameState gameState, Player player, int* srcCells )
{
	int index = 0; // indice de la premiere case vide du tableau
	int i;

	// si le joueur a au moins 1 pion dans le bar il ne peut que partir du bar
	if ( gameState.bar[player] > 0){
		srcCells[index] = 0;
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
  * Fonction qui remplit le tableau destCells contenant les numéros de cellules pouvant être destination d'un mouvement
  * @param SGameState gameState
  *   l'état du jeu courant
  * @param Player player
  *   le joueur qui effectue le mouvement
  * @param int* srcCells
  *   tableau contenant les numéros de cellules pouvant être destination d'un mouvement
  * @return int index
  *   index de la premiere case vide du tablea destCells
  */
int getDestCells( SGameState gameState, Player player, int* destCells )
{	
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

	// cas du joueur blanc
	if (player == WHITE ){
		int add = 0; // nb de pions dans les 6 dernieres cases
		for ( i=18; i < 24; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		add+=gameState.out[WHITE];
		if (add == 15){
			destCells[index] = 25; // out
			index ++;
		}
	}

	// cas du joueur noir
	if ( player == BLACK ){	
		int add = 0; // nb de pions dans les 6 dernieres cases
		for (i=0; i < 6; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		add+=gameState.out[BLACK];
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
int thereIsFartherPiece(int numSrcCell, Player player, SGameState gameState)
{
	int thereIsFartherPiece = 0;

	// cas du joueur noir
	if ( player == BLACK){
		int i;
		// de la 6e case jusqu'a la case source du mouvement voulu (exclue) ( sens décroissant)
		// on regarde si le joueur possède des pions
		for (i=6; i>numSrcCell; i--){
			if (gameState.board[i-1].owner == player){
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
			if (gameState.board[i-1].owner == player){
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
int canGoToOut(SGameState gameState, Player player, int numDice, int numSrcCell)
{
	int canGoToOut = 0; // initialisation à "ne peut pas aller dans le out"
	
	int i;

	// vérification que tous les pions du joueur sont dans ses 6 dernieres cases
	int piecesWellPlaced = 0;

	// cas du joueur blanc
	if (player == WHITE ){

		int add = 0; // nombre de pions dans les 6 dernieres cases

		for ( i=18; i < 24; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		add+= gameState.out[player];
		if (add == 15){
			piecesWellPlaced = 1;
		}	
	}

	// cas du joueur noir
	if ( player == BLACK ){	
		int add = 0; // nombre de pions dans les 6 dernieres cases
		for (i=0; i < 6; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		add+= gameState.out[player];
		if (add == 15){
			piecesWellPlaced = 1;
		}	
	}


	// si tous les pions sont dans les 6 dernieres cases du joueur : on regarde les conditions suivantes
	if (piecesWellPlaced){

		// si le mouvement correspond pile au dé le mouvement est autorisé
		if ( ((player == BLACK) && (numSrcCell == numDice)) || ((player == WHITE) &&  ((25-numSrcCell) == numDice) )){
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

void initData(Data* data)
{
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
 * @param SGameState* gameState
 *   pointeur vers l'état du jeu courant
 * @param Player player
 *   joueur effectuant le mouvement
 */

void actualizeGameState(int numSrcCell, int numDestCell, SGameState* gameState, Player player)
{
	Square *dest;
	Square *src;
	Square nul;

	if (numSrcCell!=0)
		src=&gameState->board[numSrcCell-1];
	else // si la source est le bar les opération habituelles seront appliquées sur un Square vide
		src=&nul;

	if (numDestCell!=25)
		dest=&gameState->board[numDestCell-1];
	else // si la destination est le out les opération habituelles seront appliquées sur un Square vide
		dest=&nul; 

	//La case sur laquelle on arrive était vide
	if(dest->nbDames==0)
	{
		dest->owner=player; 
	}

	//La case de départ est maintenant vide
	if(src->nbDames==1)
	{
		src->owner=NOBODY;
	}

	//Cas de pions pris
	if (dest->owner!=player && dest->owner!=NOBODY && numDestCell!=25)
	{
		Player p=dest->owner; //Ancien owner de la case prise
		dest->owner=player; //Changement d'owner
		gameState->bar[p]++; // L'adversaire a une dame supplémentaire dans le bar
		dest->nbDames=0; // Le placement de la dame est géré dans le cas général
	}

	//Cas général
	src->nbDames--;
	dest->nbDames++;

	//Une dame est remise en jeu
	if (numSrcCell==0)
		gameState->bar[player]--;

	//Une dame est sortie
	if (numDestCell==25)
		gameState->out[player]++;
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
SList* fillIn_1_MovesPossible( Player player, int dice[4], SGameState gameState)
{
	// création des listes contenant les cases de départ et d'arrivée possibles
	int srcCells[30];
	int indexSrc = getSrcCells(gameState, player, srcCells);

	
	int destCells[30];
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

					// la cellule source est le bar
					if (srcCells[i]==0)
					{
						// vérification que le mouvement correspond à un dé
						if((player==BLACK && (25-destCells[j] == dice[0] || 25-destCells[j] == dice[1] || 25-destCells[j] == dice[2] 
														|| 25-destCells[j] == dice[3]))
							|| (player==WHITE && ( (destCells[j] - srcCells[i]) == dice[0] || (destCells[j] - srcCells[i]) == dice[1] 
													|| (destCells[j] - srcCells[i]) == dice[2] || (destCells[j] - srcCells[i]) == dice[3]))
							)
						{
							
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

							if ((player==BLACK && 25-destCells[j] == dice[0]) || (player==WHITE && (destCells[j] - srcCells[i]) == dice[0]))
							{ // cas 1 : premier dé utilisé
	
								data.dice[0] = -1; // "supprime " le dé utilisé
							}
							else if ((player==BLACK && 25-destCells[j] == dice[1]) || (player==WHITE && (destCells[j] - srcCells[i]) == dice[1]))
							{ // cas 2 : 2e dé utilisé
		
								data.dice[1] = -1; // "supprime " le dé utilisé
							}
							else if ((player==BLACK && 25-destCells[j] == dice[2]) || (player==WHITE && (destCells[j] - srcCells[i]) == dice[2]))
							{ // cas 3 : 3e dé utilisé
		
								data.dice[2] = -1; // "supprime " le dé utilisé
							}
							else if ((player==BLACK && 25-destCells[j] == dice[3]) || (player==WHITE && (destCells[j] - srcCells[i]) == dice[3]))
							{ // cas 4 : 4e dé utilisé
		
								data.dice[3] = -1; // "supprime " le dé utilisé
							}

							//nouveau gameState
							data.gameState = gameState; 
							actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);
							
		
							// ajout de la cellule
							AddElementBegin(movesPossible, data);
						}
					}

					// vérification que le mouvement correspond à un dé
					else if ((fabs(destCells[j] - srcCells[i]) == dice[0]) || (fabs(destCells[j] - srcCells[i]) == dice[1])  // cas "normal" 
						|| (fabs(destCells[j] - srcCells[i]) == dice[2]) || (fabs(destCells[j] - srcCells[i]) == dice[3]))
					{
							
						// remplissage de la liste chainée avec une nouvelle cellule contenant le premier mouvement et l'état du jeu
						//initialisation de data
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
						data.gameState = gameState; 
						//printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
						actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);
						
	
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
					data.gameState = gameState;
					actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

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
					data.gameState = gameState;
					actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);


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
					data.gameState = gameState;
					actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

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
					data.gameState = gameState;
					actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

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
void deleteCellsLessMoves(SList* movesPossible, int nbMoves)
{
	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	// parcours des cellules de la liste
	while ( cellEnTraitement != NULL){ 
		
		SCell* cellNext = cellEnTraitement->next;

		// teste si la cellule contient assez de mouvements
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
int fillIn_2_MovesPossible( Player player, SList* movesPossible, int nbMovesInCells )
{
	SCell* cellEnTraitement = GetFirstElement(movesPossible);
	int movAdd =0; // pour savoir si on a rajouté des mouvements dans movesPossible ou non
	int i,j;

	while ( cellEnTraitement != NULL){ // parcours des cellules de la liste

		//etat du jeu courant de cette cellule ( appliquant au gameState les mouvement précédents contenus dans la celulle)
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
						|| destCells[j] == 25 ){
					
					// cas où la cellule destination n'est pas le out
					if ( destCells[j] != 25){

						/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
						// la cellule source est le bar
						if (srcCells[i]==0)
						{
							// vérification que le mouvement correspond à un dé
							if((player==BLACK && (25-destCells[j] == diceCell[0] || 25-destCells[j] == diceCell[1] || 25-destCells[j] == diceCell[2] 
															|| 25-destCells[j] == diceCell[3]))
								|| (player==WHITE && ( (destCells[j] - srcCells[i]) == diceCell[0] || (destCells[j] - srcCells[i]) == diceCell[1] 
														|| (destCells[j] - srcCells[i]) == diceCell[2] || (destCells[j] - srcCells[i]) == diceCell[3]))
								)
							{
								
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

								// ajout du dernier mouvement
								data.moves[nbMovesInCells].src_point = srcCells[i];
								data.moves[nbMovesInCells].dest_point = destCells[j];
								data.nbMoves ++;
	

								// nouveau jeu de dé
								data.dice[0] = diceCell[0];
								data.dice[1] = diceCell[1];
								data.dice[2] = diceCell[2];
								data.dice[3] = diceCell[3];
	
								if ((player==BLACK && 25-destCells[j] == diceCell[0]) || (player==WHITE && (destCells[j] - srcCells[i]) == diceCell[0]))
								{ // cas 1 : premier dé utilisé
		
									data.dice[0] = -1; // "supprime " le dé utilisé
								}
								else if ((player==BLACK && 25-destCells[j] == diceCell[1]) || (player==WHITE && (destCells[j] - srcCells[i]) == diceCell[1]))
								{ // cas 2 : 2e dé utilisé
			
									data.dice[1] = -1; // "supprime " le dé utilisé
								}
								else if ((player==BLACK && 25-destCells[j] == diceCell[2]) || (player==WHITE && (destCells[j] - srcCells[i]) == diceCell[2]))
								{ // cas 3 : 3e dé utilisé
			
									data.dice[2] = -1; // "supprime " le dé utilisé
								}
								else if ((player==BLACK && 25-destCells[j] == diceCell[3]) || (player==WHITE && (destCells[j] - srcCells[i]) == diceCell[3]))
								{ // cas 4 : 4e dé utilisé
			
									data.dice[3] = -1; // "supprime " le dé utilisé
								}
	
								// nouveau GameState
								data.gameState = cellEnTraitement->value.gameState;
								actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);
							

								//ajout de la cellule
								AddElementBegin(movesPossible, data);
								movAdd = 1;
							}
						}

						// vérification que le mouvement correspond à un dé
						else if ( (fabs(destCells[j] - srcCells[i]) == diceCell[0]) || (fabs(destCells[j] - srcCells[i]) == diceCell[1])  // cas "normal" 
							|| (fabs(destCells[j] - srcCells[i]) == diceCell[2]) || (fabs(destCells[j] - srcCells[i]) == diceCell[3]) 
							)
						{ 

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
							
							// ajout du dernier mouvement
							data.moves[nbMovesInCells].src_point = srcCells[i];
							data.moves[nbMovesInCells].dest_point = destCells[j];
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
							data.gameState = cellEnTraitement->value.gameState;
							actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);
							

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
						data.nbMoves = nbMovesInCells;
						
						// copie des mouvements déjà présents
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
						data.gameState = cellEnTraitement->value.gameState;
						actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;

					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 2e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[1], srcCells[i]) ){

						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);
						data.nbMoves = nbMovesInCells;

						// copie des mouvements déjà présents
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
						data.gameState = cellEnTraitement->value.gameState;
						actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;
					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 3e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[2], srcCells[i]) ){
						
						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);
						data.nbMoves = nbMovesInCells;

						// copie des mouvements déjà présents
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
						data.gameState = cellEnTraitement->value.gameState;
						actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

						//ajout de la cellule
						AddElementBegin(movesPossible, data);
						movAdd = 1;
					}
					// cas ou la cellule destination est le out ET le mouvement est possible avec le 4e dé
					else if ( destCells[j] == 25 && canGoToOut(gameStateCell, player, diceCell[3], srcCells[i]) ){
						// remplissage de la liste chainée avec une nouvelle cellule
						Data data;
						initData(&data);
						data.nbMoves = nbMovesInCells;


						// copie des mouvements déjà présents
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
						data.gameState = cellEnTraitement->value.gameState;
						actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

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
 * @rparam int nbMovesPossible
 *   nombre de mouvement contenu dans les tableaux de movesPossible
 * @return SList* movesPossible
 *   liste chainée contenant tous les tableaux de mouvements possible
 */ 
SList* getMovesPossible(SGameState gameState, Player player, unsigned char diceGiven[2], int* nbMovesPossible){

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
	*nbMovesPossible = 0;

	//remplissage des premiers mouvements possibles
	SList* movesPossible;
	movesPossible = fillIn_1_MovesPossible( player, dice, gameState);


	// si movesPossible n'est pas vide
	if (!(IsEmpty(movesPossible)) ){

		// remplissage des mouvements suivants:
		*nbMovesPossible = fillIn_2_MovesPossible( player, movesPossible, 1);
	}

	// si le joueur ne peut jouer qu'un dé ( ne concerne pas le cas d'un double)
	// alors il est obligé de jouer le dé le plus élevé						
	if (*nbMovesPossible == 1){

		// dé le plus élevé :
		int diceToPlay;

		if (dice[0] > dice [1]){
			diceToPlay = 0;
		}
		else{
			diceToPlay = 1;
		}

		// c'est possible d'utiliser le dé le plus élevé ?
		int isPossible = 0; // initialisation à non

		// parcours de movesPossible
			SCell* cellEnTraitement = GetFirstElement(movesPossible);
			while(cellEnTraitement != NULL)
			{
				int src_point = cellEnTraitement->value.moves[0].src_point;
				int dest_point = cellEnTraitement->value.moves[0].dest_point;
				if ( diceUsed(dice, player, src_point, dest_point) == diceToPlay)
				{
					isPossible =1;
				}
				cellEnTraitement = cellEnTraitement->next;
			}

			// c'est possible d'utiliser le dé le plus élevé
			if (isPossible)
			{
				// suppression des cellules dont le mouvement n'utilise pas le dé le plus élevé
				cellEnTraitement = GetFirstElement(movesPossible);
				SCell* cellNext;
				// parcours des cellules de la liste
				while ( cellEnTraitement != NULL){ 

					// cellule suivante 										
					cellNext = cellEnTraitement->next;

					// si le dé le plus élevé n'est pas utilisé alors on supprime la cellule des movesPossible
					if ( cellEnTraitement->value.dice[diceToPlay] != -1 ){ 					
						DeleteCell(movesPossible, cellEnTraitement);
					}
					cellEnTraitement = cellNext;
				}

			}
	}
	return movesPossible;
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
	SList* movesPossible;
	int nbMovesPossible;
	movesPossible = getMovesPossible(gameState, player, dice, &nbMovesPossible);

	// si le joueur ne peut jouer aucun des 2 dés et qu'il donne un tableau de mouvement vide, celui ci est accepté
	if ( (nbMovesPossible == nbMoves) && (nbMoves == 0) )
	{
		valide = 1;
	}

	// parcours des mouvements possibles (correctes ) :
	SCell* cellEnTraitement = GetFirstElement(movesPossible);
	while ( cellEnTraitement != NULL)
	{
		
		// vérification que le nombre de mouvements est correct ( = vérification que le joueur ne doit pas effectuer + de coups)
		if (nbMovesPossible == nbMoves)
		{
			
			// vérification que chaque mouvement des tableaux sont identiques
			int same = 1; // initialisation à " mouvements identiques"
			int i;
			for (i=0; i<nbMoves; i++)
			{
				
				if ( !((cellEnTraitement->value.moves[i].src_point == moves[i].src_point) && (cellEnTraitement->value.moves[i].dest_point == moves[i].dest_point)) )
				{
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
 * Fonction qui ne garde que les cellules de movesPossible dont le mouvement au rang 'rank' correspond au mouvement donné
 * @param SList* movesPossible
 *    liste contenant les mouvements possibles
 * @param int rank
 *    numéro du mouvement que l'on doit traiter dans le tableau de mouvement contenu dans chaque celulle de movesPossible
 * @ parma int numSrcCell
 *     numéro de la cellule du départ du mouvement que l'on veut conserver
 * @parma int numDestCell
 *     numéor de la cellule d'arrivée du mouvement que l'on veut conserver
 */
void keepCells(SList* movesPossible, int rank, int numSrcCell, int numDestCell)
{
	// parcours de movesPossible
	SCell* cellEnTraitement = GetFirstElement(movesPossible);
	SCell* next;
	while (cellEnTraitement != NULL)
	{
		next = cellEnTraitement->next;
		if ( cellEnTraitement->value.moves[rank].src_point != numSrcCell || cellEnTraitement->value.moves[rank].dest_point != numDestCell )
		{
			DeleteCell(movesPossible, cellEnTraitement);
		}
		cellEnTraitement = next;
	}
}

/**
 * Fonction qui indique si un élément ( entier ) se trouve dans un tableau (d'entiers)
 * @param int elem
 *   élément pour lequel on veut savoir s'il se trouve dans le tableau
 * @param in index
 *   index de la premiere case vide dans le tableau
 * @param int* tab
 *   tableau dans lequel on cherche l'élément
 * @return int result
 *   0 : l'élément ne se trouve pas dans le tableau
 *   1 : l'élément se trouve dans le tableau
 */
int isIn(int elem, int index, int* tab)
{
	
	int result = 0; // initalisation à " l'élément n'est pas dans le tableau"

	// parcours du tableau
	int i;
	for (i=0; i<index; i++)
	{
		// si l'élément correspond à la case du tableau alors on change la valeur de result
		if ( elem == tab[i] )
		{
			result = 1; 
		}
	}
	return result;

}

/**
 * Fonction qui remplit le tableau des cellules d'arrivée possibles d'un mouvement
 * @param SLits* movesPossible
 *   liste de mouvements possibles
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int* destCells
 *   tableau contenant le numéro des cellules d'arrivée possibles à remplir
 */
int fillInDestCells(SList* movesPossible, int numSrcCell, int* destCells, int rank){
	// index de la premiere case vide du tableau
	int index = 0;

	// parcours de la liste movesPossible
	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	while ( cellEnTraitement != NULL)
	{
		// la cellule source du mouvement correspond à notre cellule numSrcCell
		if (cellEnTraitement->value.moves[rank].src_point == numSrcCell && !(isIn(cellEnTraitement->value.moves[rank].dest_point, index, destCells )))
		{
			destCells[index] = cellEnTraitement->value.moves[rank].dest_point; // on ajoute la cellule destination au tableau destCells
			index ++;
		}

		// prochaine cellule à traiter
		cellEnTraitement = cellEnTraitement->next;
	}
	return index;
}

/**
 * Fonction qui renvoie l'indice du dé utilisé pour le mouvement
 * @param int* dice
 *   jeu de dés
 * @param Player player
 *   joueur qui effectue le mouvement
 * @param int numSrcCell
 *    numéro de la cellule de départ du mouvement
 * @param int numDestCell
 *    numéro de la cellule d'arrivée du mouvement
 * @return int diceUsed
 *   indice du dé utilisé
 */
int diceUsed(int* dice, Player player, int numSrcCell, int numDestCell){
	
	int diceUsed = -1;

	int i;

	// cas "normal"
	if ( (numSrcCell != 0) &&  (numDestCell != 25) ){
		for (i=3; i>-1; i--){
			if (dice[i] == fabs(numDestCell - numSrcCell)){
				diceUsed = i;
			}
		}
	}

	// cas spécifiques pour le joueur blanc
	if ( player == WHITE ){
		// sort du bar
		if ( numSrcCell == 0 ){
			for (i=3; i>-1; i--){
				if (dice[i] == numDestCell){
					diceUsed = i;
				}
			}
		}
		// vas dans le out
		if ( numDestCell == 25 ){
			for (i=3; i>-1; i--){
				if ( dice[i] == (25-numSrcCell) ){
					diceUsed = i;
				}
			}
		}
	}

	// cas spécifique pour le joueur noir
	if ( player == BLACK ){
		// sort du bar
		if ( numSrcCell == 0 ){
			for (i=3; i>-1; i--){
				if (dice[i] == (25-numDestCell)){
					diceUsed = i;
				}
			}
		}
		// vas dans le out
		if ( numDestCell == 25 ){
			for (i=3; i>-1; i--){
				if ( dice[i] == numSrcCell ){
					diceUsed = i;
				}
			}
		}
	}


	return diceUsed;
}

/**
 * Fonction qui remplie les cellules sources possibles d'un mouvement avev movesPossible
 * @param SList* movesPossible
 *    liste chainée contenant les mouvements possibles
 * @param int rank
 *    numéro du mouvement que l'on doit traiter dans le tableau de mouvement contenu dans chaque cellule de movesPossible
 * @parma int srcCells
 *    tableau contenant les numéros des cellules sources, à remplir
 * @return int index
 *    index de la premiere case vide de srcCells
 */
int getRealSrcCells(SList* movesPossible, int rank, int* srcCells)
{
	int index = 0; // index de la premiere case vide de srcCells

	// parcours de la liste movesPossible
	SCell* cellEnTraitement = GetFirstElement(movesPossible);
	while ( cellEnTraitement != NULL)
	{
		// si la cellule source du mouvement n'est pas déjà dans srcCells, on l'ajoute
		if ( !(isIn(cellEnTraitement->value.moves[rank].src_point, index, srcCells)) )
		{
			srcCells[index] = cellEnTraitement->value.moves[rank].src_point ;
			index ++;
		}

		cellEnTraitement = cellEnTraitement->next;
	}
	return index;
}


