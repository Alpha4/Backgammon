#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "liste.h"
#include "bot.h"

int TARGET;
int COLOR;
int COLORADV;



void InitLibrary(char name[50])
{
    name[0] = 'A';
    name[1] = 'l';
    name[2] = 't';
    name[3] = 'a';
    name[4] = '\0';
}

void StartMatch(const unsigned int target_score)
{
    TARGET = target_score;
    printf("%d",TARGET);
}

void StartGame(Player p)
{
    COLOR = p;
    printf("%d",COLOR);
    if (COLOR == 1) 
    	COLORADV = 0;
    else 
    	COLORADV = 1;
}

void EndGame()
{
   printf("Fin de la manche.\n"); 
}

void EndMatch()
{
    printf("Fin du match.\n");
}

int DoubleStack(const SGameState * const gameState)
{
	if (getDistanceToWin(gameState, COLOR)*2 < getDistanceToWin(gameState, COLORADV))//on regarde si notre total de deplacement est plus petit que celui de l'adversaire fois 1.5
	{
		return 1; //si c'est le cas, on demande le double
	}
	else
	{
	    return 0; //sinon on ne fait rien
	}
	return 0;
}

int TakeDouble(const SGameState * const gameState)
{
	printf("\ndistanceToWin bot : %d \ndistanceToWin joueur : %d\n",getDistanceToWin(gameState, COLOR),getDistanceToWin(gameState, COLORADV));
	if (getDistanceToWin(gameState, COLOR) < ((getDistanceToWin(gameState, COLORADV)*3)/2))//on regarde si notre total de deplacement est plus petit que celui de l'adversaire fois 1.5
	{
		return 1; //si c'est le cas, on accepte le double
	}
	else
	{
	    return 0; //sinon on refuse
	    printf("\n\nOn refuse de doubler !!\n\n");
	}
}

int getDistanceToWin(const SGameState * const gameState, Player p)
{
	int distanceToWin = 0;
	int i;
	if (p == WHITE)
	{
		for (i=0;i<24;i++)
		{
			if (gameState->board[i].owner == p)
			{
				distanceToWin += gameState->board[i].nbDames*(24-i); //on multiplie le nombre de pions sur la case avec la distance qu'il reste à parcourir (24 cases, moins la case où on est arrivé)
			}
		}
	}
	else
	{
		for (i=0;i<24;i++)
		{
			if (gameState->board[i].owner == p)
			{
				distanceToWin += gameState->board[i].nbDames*(i+1);
			}
		}
	}
	return distanceToWin;
}


/**
 * Fonction qui choisie l'action du bot
 * @param SGameState gameState
 *	l'état du jeu courant
 * @param char dices[2]
 * les deux dés tirés
 * @param SMove moves[4]
 * le tableau des 4 mouvements que l'on va choisir (donc remplir)
 * @param int *nbMoves
 * Le nombre de mouvements
 * @param int tries
 * le nombre de fautes encore possibles
 */

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
    // Copie du game state dans une variable modifiable
    printf("\n\n\n\n Playturn begin !! \n\n");
    SGameState* currentGameState;
    currentGameState = gameState;
    
    // copie des dices dnas une variable modifiable
    unsigned char currentDices[2];
    int* nbMvmnt = (int* )nbMove;
    int i;
    for (i=0;i<2;i++) currentDices[i]=dices[i];
    
    SList* possibleMovements = CreateList(); // ICI PROBLEME
    possibleMovements = getMovesPossible( *currentGameState, COLOR, currentDices, nbMvmnt);
    printf("\n\nGet moves possible ok.\n\n");
    SList* attMoves = CreateList();
    SList* defMoves = CreateList();
    SList* neutralMoves = CreateList();
    SList* harmingMoves = CreateList();
    SCell* currentCell = GetFirstElement(possibleMovements);
    SMove chosenMove;
    for (i=0;i<*nbMove;i++)
    {	
    	printf("\n\n Traitement rang %d \n\n",i);
    	int test = 0;
	    while (currentCell != NULL)
	    {
	    	printf("\n\n While : %d \n\n", test);
	    	//printList(possibleMovements); PROBLEME : SUREMENT MEME CELLULE COPIEE DANS PLUSIEURS
	        triMoves(i, *currentGameState, currentCell,attMoves,defMoves,neutralMoves,harmingMoves); //On trie dans les differentes categories
	        printf("trimoves");
	        currentCell = currentCell->next;
	        test++;
	    }
	    printf("\n\n\n\n Après Tri moves \n\n");
	    if (IsEmpty(attMoves) == 1)
	    {
	    	if (IsEmpty(defMoves) == 1)
	    	{
	    		if (IsEmpty(neutralMoves) ==1)
	    		{
	    			chosenMove = choseMove(harmingMoves,i);
	    			deleteUnchosen(chosenMove,possibleMovements,i);
	    		}
	    		else
	    		{
	    			chosenMove = choseMove(neutralMoves,i);
					deleteUnchosen(chosenMove,possibleMovements,i);
	    		}
	    	}
	    	else
	    	{
	    		chosenMove = choseMove(defMoves,i);
	    		deleteUnchosen(chosenMove,possibleMovements,i);
	    	}
	    }
	    else
	    {
	    	chosenMove = choseMove(attMoves,i);
	    	deleteUnchosen(chosenMove,possibleMovements,i);
	    }
	    moves[i] = chosenMove;

	    currentCell = GetFirstElement(possibleMovements);
	    while (currentCell != NULL)
	    {
	    	//printf("\n\n While : %d \n\n", test);
	        triMoves(i, *currentGameState, currentCell,attMoves,defMoves,neutralMoves,harmingMoves); //On trie dans les differentes categories
	        currentCell = currentCell->next;
	        //test++;
	    }
    }
    //printf("\n\ndes : %d, %d | move 1  src : %d dest : %d |move 2  src : %d dest : %d |move 3  src : %d dest : %d |move 4  src : %d dest : %d \n", dices[0], dices[1],moves[0].src_point,moves[0].dest_point,moves[1].src_point,moves[1].dest_point,moves[2].src_point,moves[2].dest_point,moves[3].src_point,moves[3].dest_point);
    printf("\n\nfin de la fonction \n\n");
    FILE *fp;
	fp = fopen("Output.txt", "a");// "w" means that we are going to write on this file
	fprintf(fp,"\n\ndes : %d, %d | nbmove : %d | move 1  src : %d dest : %d |move 2  src : %d dest : %d |move 3  src : %d dest : %d |move 4  src : %d dest : %d \n", dices[0], dices[1], *nbMove, moves[0].src_point,moves[0].dest_point,moves[1].src_point,moves[1].dest_point,moves[2].src_point,moves[2].dest_point,moves[3].src_point,moves[3].dest_point);
    fclose(fp);
    //sleep(10);
}

//////////////////////////////////////////
//    Fonction de choix d'un move       //
//////////////////////////////////////////

/**
 * Fonction qui choisit le meilleur move dans une liste
 * @param SList* listMove
 *	la liste de moves (harming, neutral, def, att)
 * @param int i
 * indice du move à regarder
 * @return SMove
 * le meilleur move
 */
SMove choseMove(SList* listMoves, int i)
{
	SCell* chosenCell = listMoves->head;
	SCell* currentCell = chosenCell->next;
	SMove chosenMove;
	while (currentCell != NULL)
	{
		if (COLOR == WHITE) // cas des blancs : de 1->24
		{
			if (currentCell->value.moves[i].src_point < chosenCell->value.moves[i].src_point)
			{
				chosenCell = currentCell; //la current étant meilleur, c'est la nouvelle chosen
			}
			currentCell = currentCell->next;
		}
		if (COLOR == BLACK) //cas des noirs : de 24->1
		{
			if (currentCell->value.moves[i].src_point > chosenCell->value.moves[i].src_point)
			{
				chosenCell = currentCell; //la current étant meilleur, c'est la nouvelle chosen
			}
			currentCell = currentCell->next;	
		}
	}
	chosenMove = chosenCell->value.moves[i];
	return chosenMove;
}


//////////////////////////////////////////
//          Fonction de delete          //
//////////////////////////////////////////
/**
 * Fonction qui enleve tous les moves dans la liste des moves possibles qui ne correspondent pas au move choisi
 * @param SMove move
 * le move que l'on veut garder (on supprime les autres)
 * @param SList* liste
 * liste de tous les moves possibles (la liste à updater)
 * @param int i
 * indice du move à regarder
 */
void deleteUnchosen(SMove move, SList* liste, int i)
{
	printf("\n\n Debut de deleteUnchosen\n\n");
	//printList(liste);
	SCell* currentCell;
	SCell* nextCell;

	currentCell = GetFirstElement(liste);
	//nextCell = currentCell->next;
	int test =0;
	
	while (currentCell != NULL)
	{
		nextCell = currentCell->next;
		printf("\n\nWhile num : %d\n",test);
		if((movesAreEquals(currentCell->value.moves[i],move) == 0))
		{
			DeleteCell(liste, currentCell);
			printf("\n\nCOUCOU JE PASSE LA DEDANS \n\n");
			//printList(liste);
		}
		currentCell = nextCell;
		test++;
	}
	printf("fin du while");
}


/**
 * Fonction qui verifie si un move est egal a un autre
 * @param SMove move1
 * premier move a tester
 * @param SMove move2
 * deuxieme move a tester
 * @return int
 * pseudo-booléen
 */
int movesAreEquals(SMove move1, SMove move2)
{
	int result = 0;
	if (move1.src_point == move2.src_point && move1.dest_point == move2.dest_point)
	{
		result = 1;
	}
	return result;
}
//////////////////////////////////////////
//            Fonction de tri           //
//////////////////////////////////////////

/**
 * Fonction qui tri les moves en fonction de leur nature
 * @param int i
 * le rang du move a trier
 * @param SGameState gameState
 * l'etat du jeu
 * @param SCell* currentCell
 * le tableau contenant le move a trier au rang i
 * @param SList* somethingMoves
 * les 4 listes a remplir (harming, neutral, def, att)
 */

void triMoves (int i, SGameState gameState, SCell* currentCell,SList* attMoves, SList* defMoves,SList* neutralMoves,SList* harmingMoves)
{
    SMove *currentMoves;
    currentMoves = currentCell->value.moves;
    if (currentMoves[i].src_point == 0)
    {
    	if (gameState.board[currentMoves[i].dest_point-1].nbDames == 0)
	        {
	           AddElementEnd(harmingMoves,currentCell->value);
	        }
	        else 
	        {
	           if (gameState.board[currentMoves[i].dest_point-1].nbDames == 1 && gameState.board[currentMoves[i].dest_point-1].owner != COLOR)
	           {
	               AddElementEnd(attMoves,currentCell->value);
	           }
	           else 
	           {
	               if (gameState.board[currentMoves[i].dest_point-1].nbDames == 1 && gameState.board[currentMoves[i].dest_point-1].owner == COLOR)
	               {
	                   AddElementEnd(defMoves,currentCell->value); 
	               }
	               else
	               {
	                   AddElementEnd(neutralMoves,currentCell->value);
	               }
	            }
	        }
    }
	else
	{
	    if (gameState.board[currentMoves[i].src_point-1].nbDames == 2)
	    {
	        AddElementEnd(harmingMoves,currentCell->value);
	    }
	    else 
	    {
	        if (gameState.board[currentMoves[i].dest_point-1].nbDames == 0)
	        {
	           AddElementEnd(harmingMoves,currentCell->value);
	        }
	        else 
	        {
	           if (gameState.board[currentMoves[i].dest_point-1].nbDames == 1 && gameState.board[currentMoves[i].dest_point-1].owner != COLOR)
	           {
	               AddElementEnd(attMoves,currentCell->value);
	           }
	           else 
	           {
	               if (gameState.board[currentMoves[i].dest_point-1].nbDames == 1 && gameState.board[currentMoves[i].dest_point-1].owner == COLOR)
	               {
	                   AddElementEnd(defMoves,currentCell->value); 
	               }
	               else
	               {
	                   AddElementEnd(neutralMoves,currentCell->value);
	               }
	            }
	        }
	    }
	}
}            
    


/////////////////////////////////////////////////////
//           Fonctions Moves Possible              //
/////////////////////////////////////////////////////

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

	// cas du joueur blanc
	if (player == WHITE ){
		int add = 0; // nb de pionts dans les 6 dernieres cases
		for ( i=18; i < 24; i++){
			if (gameState.board[i].owner == player){
				add += gameState.board[i].nbDames;
			}
		}
		if (add == 15){
			destCells[index] = 25; // out
			index ++;
		}
	}

	// cas du joueur noir
	if ( player == BLACK ){	
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
int canGoToOut(SGameState gameState, Player player, int numDice, int numSrcCell){

	int canGoToOut = 0; // initialisation à ne peut pas aller dans le out
	
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
 * @param SGameState* gameState
 *   pointeur vers l'état du jeu courant
 * @param Player player
 *   joueur effectuant le mouvement
 */

void actualizeGameState(int numSrcCell, int numDestCell, SGameState* gameState, Player player){

	printf("\n\nDebut actualizeGameSate\n\n");

	// si le pion sort du bar
	if (numSrcCell == 0){ 
		printf("le pion est sorti du bar\n");

		//La case sur laquelle on arrive était vide
		if ( gameState->board[numDestCell-1].nbDames == 0 )
		{
			// changement de l'owner de la case
			gameState->board[numDestCell-1].owner = player; 
		}

		//Cas de pions pris
		if ( (gameState->board[numDestCell-1].owner != player) && (gameState->board[numDestCell-1].owner != NOBODY) )
		{
			Player p = gameState->board[numDestCell-1].owner; //Ancien owner de la case prise
			gameState->board[numDestCell-1].owner = player; //Changement d'owner
			gameState->bar[p]++; // L'adversaire a une dame supplémentaire de sortie du jeu
			gameState->board[numDestCell-1].nbDames = 0; // Le placement de la dame est géré dans le cas général
		}

		// actualisation du nombre de pions sur les cases de départ et d'arrivée 
		gameState->bar[player] --;
		gameState->board[numDestCell-1].nbDames ++;
	}

	// si le pion va dans le out
	else if (numDestCell == 25){ 

		//La case de départ est maintenant vide
		if ( gameState->board[numSrcCell-1].nbDames == 1)
		{
			gameState->board[numSrcCell-1].owner = NOBODY;
		}

		//Cas général
		gameState->board[numSrcCell-1].nbDames --;
		gameState->out[player] ++;

	}

	// cas d'un mouvement basique
	else{ 
		
		//La case sur laquelle on arrive était vide
		if ( gameState->board[numDestCell-1].nbDames == 0 )
		{
			gameState->board[numDestCell-1].owner = player; 
		}

		//La case de départ est maintenant vide
		if ( gameState->board[numSrcCell-1].nbDames == 1)
		{
			gameState->board[numSrcCell-1].owner = NOBODY;
		}

		//Cas de pions pris
		if ( (gameState->board[numDestCell-1].owner != player) && (gameState->board[numDestCell-1].owner != NOBODY) )
		{
			Player p = gameState->board[numDestCell-1].owner; //Ancien owner de la case prise
			gameState->board[numDestCell-1].owner = player; //Changement d'owner
			gameState->bar[p]++; // L'adversaire a une dame supplémentaire de sortie du jeu
			gameState->board[numDestCell-1].nbDames = 0; // Le placement de la dame est géré dans le cas général
		}

		//Cas général
		gameState->board[numSrcCell-1].nbDames --;
		gameState->board[numDestCell-1].nbDames ++;
	}
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

	printf("\n\nDebut fill in 1\n\n");

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
							printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
							actualizeGameState(srcCells[i], destCells[j], &gameState, player);
							
		
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
						data.gameState = gameState; 
						printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
						actualizeGameState(srcCells[i], destCells[j], &gameState, player);
						
	
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
					printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
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
					printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
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
					printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
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
					printf("fill in 1 lance : actualiezGameState avec src : %i | dest : %i\n", srcCells[i], destCells[j]);
					actualizeGameState(srcCells[i], destCells[j], &data.gameState, player);

					// ajout de la cellule
					AddElementBegin(movesPossible, data);
				}
			}
		}
	}
	printf("Fill in 1 returne :\n");
	//printList(movesPossible);
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
	while ( cellEnTraitement != NULL){ 
		
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

	printf("\n\ndebut fill in 2\n\n");

	printf("fill in 2 recoit : movesPossible :\n");
	//printList(movesPossible);

	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	//printf("\n\nDebut Fill in 2\n\n");

	//printf("liste movesPossible recue : \n");
	//printList(movesPossible);

	int movAdd =0; // pour savoir si on a rajouté des mouvements dans movesPossible ou non
	
	int i,j;

	while ( cellEnTraitement != NULL){ // parcours des cellules de la liste

		printf("\n\nNouvelle cellule en traitement\n\n");
		printf("first move : %i->%i\n", cellEnTraitement->value.moves[0].src_point, cellEnTraitement->value.moves[0].dest_point);

		//etat du jeu courant de cette cellule ( appliquant au gameState les mouvement précédents contenus dans la celulle)
		SGameState gameStateCell = cellEnTraitement->value.gameState;

		//printf("nb de pions dans ")

		int diceCell[4];
		diceCell[0] = cellEnTraitement->value.dice[0];
		diceCell[1] = cellEnTraitement->value.dice[1];
		diceCell[2] = cellEnTraitement->value.dice[2];
		diceCell[3] = cellEnTraitement->value.dice[3];
		
		// création des listes contenant les cases de départ et d'arrivée possibles
		int srcCells[25];
		int indexSrc = getSrcCells(gameStateCell, player, srcCells);
		
		printf("srcCells : \n");
		int a;
		for (a=0; a<indexSrc; a++)
		{
			printf("   src cell : %i\n", srcCells[a]);
		}


		int destCells[25];
		int indexDest= getDestCells(gameStateCell, player, destCells);

		printf("desCells : \n");
		for (a=0; a<indexDest; a++)
		{
			printf("   destCell : %i\n", destCells[a]);
		}


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
						}/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

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

							printf("Move add : %i-->%i\n", srcCells[i], destCells[j]);
	
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


						/*
						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}
						*/

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						printf("Move add : %i-->%i\n", srcCells[i], destCells[j]);

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


						/*
						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}*/

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						printf("Move add : %i-->%i\n", srcCells[i], destCells[j]);

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


						/*
						//copie des mouvements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}*/

						// ajout du dernier mouvement
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						printf("Move add : %i-->%i\n", srcCells[i], destCells[j]);

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

						/*
						//copie des movements déjà présents
						int x;
						for (x=0; x < nbMovesInCells; x++){
							data.moves[i] = cellEnTraitement->value.moves[i];
						}*/

						// ajout du dernier mouvementS
						data.moves[nbMovesInCells].src_point = srcCells[i];
						data.moves[nbMovesInCells].dest_point = destCells[j];
						data.nbMoves ++;

						printf("Move add : %i-->%i\n", srcCells[i], destCells[j]);

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
		printf("Move add\n nbMoves = %i\n", nbMovesInCells);
		deleteCellsLessMoves( movesPossible , nbMovesInCells); // suppression des cellules ne contenant pas assez de mouvements
		// ( si une cellules contient 2 mouvements par exemples, alors il sera interdit au joueur de ne faire qu'un mouvement)

		if (nbMovesInCells != 4){
			printf("nbMoves != 4 --> on retourne dans fill in 2\n");
			printf("move add + nbMoves !=4 --> rappel de fill in 2\n");
			nbMovesInCells = fillIn_2_MovesPossible(player, movesPossible, nbMovesInCells); 
		}
	}
	printf("fin fill in 2, nbMoves = %i", nbMovesInCells);
	return nbMovesInCells;
}



/*
int nbMoves;
SList* moves = getMovesPossible( ;................., &nbMoves)
*/


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
	printf("\n\n\nDébut getMovesPossible\n\n\n");

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

	//AFFICHAGE CONSOLE
	int a;
	printf("Dés :\n");
	for (a=0; a<4; a++)
	{
		printf("%i\n", dice[a]);
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
		printf("fill in 2 dit: nbMoves = %i\n", *nbMovesPossible);
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

		// suppression des cellule dont le mouvement n'utilise pas le dé le plus élevé: 

		SCell* cellEnTraitement = GetFirstElement(movesPossible);
		SCell* cellNext;
		// parcours des cellules de la liste
		while ( cellEnTraitement != NULL){ 

			// cellule suivante 										
			cellNext = cellEnTraitement->next;

			// si le dé la plus élevé n'est pas utilisé alors on supprime la cellule des movesPossible
			if ( cellEnTraitement->value.dice[diceToPlay] != -1 ){ 					
				DeleteCell(movesPossible, cellEnTraitement);
			}
			cellEnTraitement = cellNext;
		}
	}
	return movesPossible;
}