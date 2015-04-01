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
    	current=player1;
        player2 = BLACK;
    }
    else
    {
    	current=player2;
        player2 = WHITE;
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

		while (/*PAS DE GAGNANT*/)
		{

		}

	}


	return 0;
}
