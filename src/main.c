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
#include "liste.h"

void initGS(SGameState* gameState)
{
	/*Initialisation du plateau*/
	int i;
	for (i = 0; i < 24; ++i)
	{	
		gameState->board[i].owner=NOBODY;
		gameState->board[i].nbDames=0;
	}
	//WHITE
	gameState->board[0].owner=WHITE;
	gameState->board[0].nbDames=2;
	gameState->board[11].owner=WHITE;
	gameState->board[11].nbDames=5;
	gameState->board[16].owner=WHITE;
	gameState->board[16].nbDames=3;
	gameState->board[18].owner=WHITE;
	gameState->board[18].nbDames=5;

	//BLACK
	gameState->board[23].owner=BLACK;
	gameState->board[23].nbDames=2;
	gameState->board[12].owner=BLACK;
	gameState->board[12].nbDames=5;
	gameState->board[7].owner=BLACK;
	gameState->board[7].nbDames=3;
	gameState->board[5].owner=BLACK;
	gameState->board[5].nbDames=5;

	gameState->out[0]=0;
	gameState->out[1]=0;
	gameState->bar[0]=0;
	gameState->bar[1]=0;
	gameState->turn=1;
	gameState->stake=1;
}

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
	char msg[40];

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
	int pts=3; //Points pour remporter le match
	unsigned char dices[2]={1,1};

	SMove moves[4];
	int videau=-1;


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

	if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
	{
		ai1.InitLibrary(p1Name);  // On lui demande son nom

		if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
		{
			ai2.InitLibrary(p2Name);  // On lui demande son nom
		}
	}

	
	/* Randomisation de la couleur des joueurs */

	srand(time(NULL));
	player1 = (rand() % 2);
	if (player1 == WHITE)
	{
		sprintf(msg,"%s joue les blancs",p1Name);
	    player2 = BLACK;
	}
	else
	{
		sprintf(msg,"%s joue les blancs",p2Name);
	    player2=WHITE;
	}

	initGS(&gameState);

	SDL_RenderClear(c.pRenderer);
		update(&c,gameState,dices);
		prompt(&c,msg);
	SDL_RenderPresent(c.pRenderer);
	playerClicked();

	/* JEU */
	int m; //Nombre de matchs
	for (m=0;m<atoi(argv[1]);m++)
	{
		sprintf(msg,"Match %d",m+1);

		SDL_RenderClear(c.pRenderer);
			update(&c,gameState,dices);
			prompt(&c,msg);
		SDL_RenderPresent(c.pRenderer);
		playerClicked();

		if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
		{
			ai1.StartMatch(pts);  // On l'initialise

			if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
			{
				ai2.StartMatch(pts);  // On l'initialise
			}
		}


		gameState.whiteScore=0;
		gameState.blackScore=0;
		int g=0;
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

			sprintf(msg,"Game %d",g+1);
			

			SDL_RenderClear(c.pRenderer);
				update(&c,gameState,dices);
				prompt(&c,msg);
			SDL_RenderPresent(c.pRenderer);
			playerClicked();

			initGS(&gameState);
			/*Initialisation du plateau
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
			gameState.stake=1;*/
			
			videau=-1;

			int result=-1;
			while (result==-1) //Boucle pour chaque tour (result est à -1 si pas de gagnant)
			{
				// Tirage des dés
				srand(time(NULL));
				dices[0]=rand()%6+1;
				dices[1]=rand()%6+1;

				SDL_RenderClear(c.pRenderer);
					update(&c,gameState,dices);
				SDL_RenderPresent(c.pRenderer);

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
					sprintf(msg,"A %s de jouer",p1Name);
					
					SDL_RenderClear(c.pRenderer);
						update(&c,gameState,dices);
						prompt(&c,msg);
					SDL_RenderPresent(c.pRenderer);
					playerClicked();

					if (nbHumanPlayers <= 1)  // Le joueur 1 est une IA
					{
						if(videau!=current)
						{
							if (ai1.DoubleStack(&gameState))
							{
								videau=current;
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
									SDL_RenderClear(c.pRenderer);
										update(&c,gameState,dices);
										doubleQuery(&c,1);
									SDL_RenderPresent(c.pRenderer);

									if(yesOrNo()==1)//VIDEAU_TAKE J2
									{
										result=current;
										break;//Sortie de la boucle while
									}
									else
										gameState.stake*=2;
								}
							}
						}

						SDL_RenderClear(c.pRenderer);
							update(&c,gameState,dices);
						SDL_RenderPresent(c.pRenderer);

						ai1.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
					}
					else //Le joueur 1 est humain -->2joueurs humains
					{	
						if(videau!=current)
						{
							SDL_RenderClear(c.pRenderer);
								update(&c,gameState,dices);
								doubleQuery(&c,0);
							SDL_RenderPresent(c.pRenderer);

							if(yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J1
							{
								videau=current;
								SDL_RenderClear(c.pRenderer);
									update(&c,gameState,dices);
									doubleQuery(&c,1);
								SDL_RenderPresent(c.pRenderer);
								if(yesOrNo()==1)//VIDEAU_TAKE HUMAIN J2
								{
										result=current;
										break;//Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}
						}

						SDL_RenderClear(c.pRenderer);
							update(&c,gameState,dices);
						SDL_RenderPresent(c.pRenderer);

						printf("BP1\n");
						memcpy(&gameStateCopy,&gameState,sizeof(SGameState));
						printf("BP2\n");
						nbMoves=getArrayMoves(moves,gameStateCopy,dices,current,&c);
						printf("BP3\n");
					}
				}

				else // Au joueur 2 de jouer
				{
					sprintf(msg,"A %s de jouer",p2Name);
					SDL_RenderClear(c.pRenderer);
						update(&c,gameState,dices);
						prompt(&c,msg);
					SDL_RenderPresent(c.pRenderer);
					playerClicked();
					
					if (nbHumanPlayers == 0)  // Le joueur 2 est une IA
					{
						if(videau!=current)
						{
							if (ai2.DoubleStack(&gameState))
							{
								videau=current;
								if(!(ai1.TakeDouble(&gameState)))
								{
									result=current;
									break; //Sortie de la boucle while
								}
								else
									gameState.stake*=2;
							}
						}
						SDL_RenderClear(c.pRenderer);
							update(&c,gameState,dices);
						SDL_RenderPresent(c.pRenderer);
						ai2.PlayTurn(&gameState,dices,moves,&nbMoves,3-penalty[current]);
					}
					else // Le joueur 2 est humain
					{
						if (nbHumanPlayers==1) // Le joueur 1 est IA
						{
							if(videau!=current)
							{
								SDL_RenderClear(c.pRenderer);
									update(&c,gameState,dices);
									doubleQuery(&c,0);
								SDL_RenderPresent(c.pRenderer);

								if (yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J2
								{
									videau=current;
									if(!(ai1.TakeDouble(&gameState)))
									{
										result=current;
										break; //Sortie de la boucle while
									}
									else
										gameState.stake*=2;
								}
							}
						}

						else // Le joueur 1 est humain
						{
							if(videau!=current)
							{
								SDL_RenderClear(c.pRenderer);
									update(&c,gameState,dices);
									doubleQuery(&c,0);
								SDL_RenderPresent(c.pRenderer);

								if(yesOrNo()==0)//VIDEAU_DOUBLE HUMAIN J2
								{
									videau=current;
									SDL_RenderClear(c.pRenderer);
										update(&c,gameState,dices);
										doubleQuery(&c,1);
									SDL_RenderPresent(c.pRenderer);
									if(yesOrNo()==1)//VIDEAU_TAKE HUMAIN J1
									{
										result=current;
										break; //Sortie de la boucle while
									}
									else
										gameState.stake*=2;
								}
							}

						}
						SDL_RenderClear(c.pRenderer);
							update(&c,gameState,dices);
						SDL_RenderPresent(c.pRenderer);
						printf("BP1J2\n");
						memcpy(&gameStateCopy,&gameState,sizeof(SGameState));
						printf("BP2J2\n");
						nbMoves=getArrayMoves(moves,gameStateCopy,dices,current,&c);
						printf("BP3J2\n");
					}
				}


				printf("BP4\n");
				int n;
				memcpy(&gameStateCopy,&gameState,sizeof(SGameState));
				if(validMoves(nbMoves,moves,gameStateCopy,dices,current))//Fonction de l'arbitre
				{
					printf("BP5\n");
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
				printf("BP6\n");
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
			{
				sprintf(msg,"%s gagne le game",p1Name);
				saveResult(p1Name,gameState.stake);
			}
			else
			{
				sprintf(msg,"%s gagne le game!",p2Name);
				saveResult(p2Name,gameState.stake);
			}
			SDL_RenderClear(c.pRenderer);
				update(&c,gameState,dices);
				prompt(&c,msg);
			SDL_RenderPresent(c.pRenderer);
			playerClicked();
			g++;
		}
		printf("BP6\n");
		sprintf(msg,"%s remporte le match !",saveMatch(gameState,p1Name,p2Name,player1));
		SDL_RenderClear(c.pRenderer);
			update(&c,gameState,dices);
			prompt(&c,msg);
		SDL_RenderPresent(c.pRenderer);
		playerClicked();

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
