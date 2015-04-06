/*
Programme principal gérant l'interface, l'arbitre et les interface
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>

/*include de SDL*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*include du projet*/
#include "backgammon.h"
#include "libs.h"
#include "arbitre.h" 
#include "interface.h"




int main (int argc, char *argv[])
{
	/*
	*	argv[1] Nombres de Matchs pour le match
	*	argv[2] Chemin vers la lib de l'IA 1 si nécessaire
	*	argv[3] Chemin vers la lib de l'IA 2 si nécessaire
	*/


	/*Librairies des IA*/
	AI ai1,ai2;

	/*Variables pour l'affichage*/
	Context c;

	/* Variables pour le jeu*/
	Player player1,player2;
	Player current=WHITE;
	SGameState gameState;
	SGameState gameStateCopy;


	int nbHumanPlayers;

	char p1Name[50], p2Name[50]; // Nom des joueurs
	strcpy(p1Name,"Joueur1");// pour le cas de parties avec humain
	strcpy(p2Name,"Joueur2");
	int penalty[2]={0,0}; // Pénalité pour chacun des joueurs

	unsigned int nbMoves;
	int pts=1; //Points pour remporter le match
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

	/* Initialisation affichage*/
	init(&c,"Backgammon");

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

	int m; //Nombre de matchs
	for (m=0;m<atoi(argv[1]);m++)
	{


		if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
		{
			ai1.InitLibrary(p1Name);  // On lui demande son nom
			ai1.StartMatch(pts);  // On l'initialise

			if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
			{
				ai2.InitLibrary(p2Name);  // On lui demande son nom
				ai2.StartMatch(pts);  // On l'initialise
			}
		}


		gameState.whiteScore=0;
		gameState.blackScore=0;

		while(gameState.whiteScore<=pts && gameState.blackScore<=pts)
		{
			if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
			{
				ai1.StartGame(player1);

				if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
				{
					ai2.StartGame(player2);
				}
			}

			/*Initialisation du plateau*/
			int i;
			for (i = 0; i < 24; ++i)
			{	
				gameState.board[i].owner=NOBODY;
				gameState.board[i].nbDames=0;
			}
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

			gameState.out[0]=0;
			gameState.out[1]=0;
			gameState.bar[0]=0;
			gameState.bar[1]=0;
			gameState.turn=1;
			gameState.stake=1;
	

			int result=-1;
			while (result==-1) //Boucle pour chaque tour (result est à -1 si pas de gagnant)
			{

				// Tirage des dés
				srand(time(NULL));
				dices[0]=rand()%6+1;
				dices[1]=rand()%6+1;

				update(&c,gameState,dices);

				//Tableau vide pour les moves
				SMove vide;
				vide.src_point=0;
				vide.dest_point=0;
				int i;
				for (i=0;i<4;i++)
				{
					moves[i]=vide;
				}

				if (player1==current) // Au joueur 1 de jouer
				{
					if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
					{
						if (ai1.DoubleStack(&gameState))
						{
							if(nbHumanPlayers==0) //Le joueur 2 est une IA
							{
								if(!(ai2.TakeDouble(&gameState)))
								{
									result=current;
									break;//Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}
							else //Le joueur 2 est humain
							{
								doubleQuery(&c,1);
								if(yesOrNo()==1)//VIDEAU_TAKE J2
								{
									result=current;
									break;//Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}
						}
						update(&c,gameState,dices);
						ai1.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
					}
					else //Le joueur 1 est humain -->2joueurs humains
					{	
						doubleQuery(&c,0);
						if(yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J1
						{
							doubleQuery(&c,1);
							if(yesOrNo()==1)//VIDEAU_TAKE HUMAIN J2
							{
									result=current;
									break;//Sortie de la boucle while
							}
							else
								gameState.stake*=2;
						}
						update(&c,gameState,dices);
						//COUP HUMAIN J1
					}
				}

				else // Au joueur 2 de jouer
				{
					if (nbHumanPlayers == 0)  // Le joueur 2 est une IA
					{
						if (ai2.DoubleStack(&gameState))
						{
							if(!(ai1.TakeDouble(&gameState)))
							{
								result=current;
								break; //Sortie de la boucle while
							}
							else
								gameState.stake*=2;
						}
						update(&c,gameState,dices);
						ai2.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
					}
					else // Le joueur 2 est humain
					{
						if (nbHumanPlayers==1) // Le joueur 1 est IA
						{
							doubleQuery(&c,0);
							if (yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J2
							{
								if(!(ai1.TakeDouble(&gameState)))
								{
									result=current;
									break; //Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}
						}

						else // Le joueur 1 est humain
						{
							doubleQuery(&c,0);
							if(yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J2
							{
								doubleQuery(&c,1);
								if(yesOrNo()==1)//VIDEAU_TAKE HUMAIN J1
								{
									result=current;
									break; //Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}

						}
						update(&c,gameState,dices);
						//COUP HUMAIN J2
					}
				}

				int n;
				memcpy(&gameStateCopy,&gameState,sizeof(SGameState));
				if(validMoves(nbMoves,moves,gameStateCopy,dices,current))//Fonction de l'arbitre
				{
					for (n=0;n<nbMoves;n++) 
					{
						Square *dest;
						Square *src;
						Square nul;

						if (moves[n].src_point!=0)
							src=&gameState.board[moves[n].src_point-1];
						else // si la source est le bar les opération habituelles seront appliquées sur un Square vide
							src=&nul;

						if (moves[n].dest_point!=25)
							dest=&gameState.board[moves[n].dest_point-1];
						else // si la destination est le out les opération habituelles seront appliquées sur un Square vide
							dest=&nul; 

						//La case sur laquelle on arrive était vide
						if(dest->nbDames==0)
						{
							dest->owner=current; 
						}

						//La case de départ est maintenant vide
						if(src->nbDames==1)
						{
							src->owner=NOBODY;
						}

						//Cas de pions pris
						if (dest->owner!=current && dest->owner!=NOBODY)
						{
							Player p=dest->owner; //Ancien owner de la case prise
							dest->owner=current; //Changement d'owner
							gameState.bar[p]++; // L'adversaire a une dame supplémentaire dans le bar
							dest->nbDames=0; // Le placement de la dame est géré dans le cas général
						}

						//Cas général
						src->nbDames--;
						dest->nbDames++;

						//Une dame est remise en jeu
						if (moves[n].src_point==0)
							gameState.bar[current]--;

						//Une dame est sortie
						if (moves[n].dest_point==25)
							gameState.out[current]++;
					}
				}
				else // mouvement(s) non valide(s)
				{
					penalty[current]++; //Pénalité pour le joueur 
				}

				result=isGameFinished(gameState,penalty); // Fonction de l'arbitre renvoyant le joueur gagnant(WHITE, BLACK) ou NOBODY
				
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

		// Inversion des sides
		if (player1 == WHITE)
		{
			player1=BLACK;
		    player2=WHITE;
		}
		else
		{
			player1=WHITE;
		    player2=BLACK;
		}
	}

	cleanup(&c);
	return 0;
}
