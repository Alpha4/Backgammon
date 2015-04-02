/*
Programme principal gérant l'interface, l'arbitre et les interface
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>

#include "backgammon.h"
#include "libs.h"
#include "arbitre.h" // Fonction de l'arbitre




int main (int arc, char *argv[])
{
	/*
	*	argv[1] Nombres de Game pour le match
	*	argv[2] Chemin vers la lib de l'IA 1 si nécessaire
	*	argv[3] Chemin vers la lib de l'IA 2 si nécessaire
	*/


	/*Librairies des IA*/
	AI ai1,ai2;


	/* Variables pour le jeu*/
	Player player1,player2;
	PLayer current=WHITE;
	SGameState gameState;
	SGameState gameStateCopy;


	int nbHumanPlayers;

	char p1Name[50], p2Name[50]; // Nom des joueurs
	int penalty[2]={0,0}; // Pénalité pour chacun des joueurs

	unsigned int nbMoves;
	unsigned char dices[2];

	SMove moves[4];


	/*Chargement des librairies*/
	switch (argc)
	{
		case 2 : //2 joueurs humains
			nbHumanPlayers=2;
		break;
 
		case 3 : //1 IA 1 humain
			nbHumanPlayers=1;
			if (LoadLib(&ai1, argv[2]) == -1)
            	return EXIT_FAILURE;
		break;

		case 4 : //2 IA
			nbHumanPlayers=0;
			if (LoadLib(&ai1, argv[2]) == -1)
            	return EXIT_FAILURE;
			if (LoadLib(&ai2, argv[3]) == -1)
				return EXIT_FAILURE;
		break;

		default : // Pas assez d'argument
			fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
        	return EXIT_FAILURE;
		break;

	}

	/* Jeu */

	/**
     * Randomisation de la couleur des joueurs
     */

    srand(time(NULL));
    player1 = (rand() % 2);
    if (player1 == WHITE)
    {
        player2 = BLACK;
    }
    else
    {
        player2=WHITE;
    }


	if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
	{
		ai1.InitLibrary(p1Name);  // On lui demande son nom
		ai1.StartMatch();  // On l'initialise

		if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
		{
			ai2.InitLibrary(p2Name);  // On lui demande son nom
			ai2.StartMatch();  // On l'initialise
		}
	}


	int g; //Nombre de games
	for (g=0;g<argv[1];g++)
	{
		if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
		{
			ai1.StartGame();

			if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
			{
				ai2.StartGame();
			}
		}

		/*Initialisation du plateau*/

		//WHITE
		gameState.board[0].owner=WHITE;
		gameState.board[0].nbDames=2;
		gameState.board[11].owner=WHITE;
		gameState.board[11].nbDames=5;
		gameState.board[16].owner=WHITE;
		gameState.board[16].nbDames=3;
		gameState.board[18].owner=WHITE;
		gameState.board[18].nbDames=5;

		//BLACK
		gameState.board[23].owner=BLACK;
		gameState.board[23].nbDames=2;
		gameState.board[12].owner=BLACK;
		gameState.board[12].nbDames=5;
		gameState.board[7].owner=BLACK;
		gameState.board[7].nbDames=3;
		gameState.board[5].owner=BLACK;
		gameState.board[5].nbDames=5;

		gameState.out={0,0};
		gameState.bar={0,0};
		gameState.turn=1;
		gameState.stake=1;



		int result=-1;
		while (result==-1) //Boucle pour chaque tour (result est à -1 si pas de gagnant)
		{
			// Tirage des dés
			srand(time(NULL));
			dices[0]=rand()%6+1;
			dices[1]=rand()%6+1;

			if(dices[0]==dices[1])
				nbMoves=4;
			else
				nbMoves=2;

			//Tableau vide pour les moves
			SMove vide;
			vide->src_point=0;
			vide->dest_point=0;
			moves={vide,vide,vide,vide};


			if (player1==current)
			{
				if (ai1.DoubleStack(&gameState))
					if(!(ai2.TakeDouble(&gameState)))
					{
						result=current;
						break;//Sortie de la boucle while
					}
					else
						gameState.stake*=2;
				ai1.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
			}
			else
			{
				if (ai2.DoubleStack(&gameState))
					if(!(ai1.TakeDouble(&gameState)))
					{
						result=current;
						break; //Sortie de la boucle while
					}
					else
						gameState.stake*=2;
				ai2.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
			}

			/* Calcul du nombre de moves effectués*/
			int n,nbMovesDone=4;
			for (n=0;n<nbMoves;n++)
			{
				if(moves[n].dest==0 && moves[n].src==0)
					nbMovesDone--;
			}

			memcpy(copy,game,sizeof(SGameState));
			if(validMoves(nbMovesDone,moves,gameStateCopy,dices,current))//Fonction de l'arbitre
			{
				for (n=0;n<nbMovesDone;n++) 
				{

					//Cas de pions pris
					if (gameState->board[moves[n].dest].owner!=current)
					{
						Player p=gameState->board[moves[n].dest].owner; //Ancien owner de la case prise
						gameState->board[moves[n].dest].owner=current; //Changement d'owner
						gameState->out[p]++; // L'adversaire a une dame supplémentaire de sortie du jeu
					}

					//Cas général
					gameState->board[moves[n].src].nbDames--;
					gameState->board[moves[n].dest].nbDames++;

					//Une dame est remise en jeu
					if (moves[n].src==25)
						gameState->out[current]--;

					//Une dame est placée dans le bar
					if (moves[n].dest==0)
						gameState->bar[current]++;
				}
			}
			else // mouvement(s) non valide(s)
			{
				penalty[current]++; //Pénalité pour le joueur 
			}

			result=isGameFinished(gameState,current,penalty); // Fonction de l'arbitre renvoyant le joueur gagnant(WHITE, BLACK) ou NOBODY
			
			//Prochain joueur
			if (current==WHITE)
				current=BLACK;
			else
				current=WHITE;

			gameState.turn++;
		}

		//MAJ du score du gagnant
		if(result==WHITE)
			gameState.whiteScore+=gameState.stake;
		else
			gameState.blackScore+=gameState.stake;

		//On sauvegarde le résultat
		if (result==player1)
			saveResult(p1Name,gameState.stake);
		else
			saveResult(p2Name,gameState.stake);
	}

	saveMatch(gameState,p1Name,p2Name,player1);


	return 0;
}
