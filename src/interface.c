#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*include de SDL*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*include du projet*/
#include "backgammon.h"
#include "interface.h"

/**
 * Quitte SDL en détruisant le context
 * @param Context* c
 *  Context pour l'affichage
 */
void cleanup(Context* c)
{
	int i;
	for (i=0;i<6;i++)
	{
		if (c->dice[i]!=NULL)
			SDL_DestroyTexture(c->dice[i]);
		if(c->doublingCube[i]!=NULL)
			SDL_DestroyTexture(c->doublingCube[i]);
	}
	if(c->pawn[0]!=NULL)
		SDL_DestroyTexture(c->pawn[0]);
	if(c->pawn[1]!=NULL)
		SDL_DestroyTexture(c->pawn[1]);
	if(c->pawnOut[0]!=NULL)
		SDL_DestroyTexture(c->pawnOut[0]);
	if(c->pawnOut[1]!=NULL)
		SDL_DestroyTexture(c->pawnOut[1]);
	if(c->board!=NULL)
		SDL_DestroyTexture(c->board);
	if(c->pRenderer!=NULL)
		SDL_DestroyRenderer(c->pRenderer);
	if(c->pWindow!=NULL)
		SDL_DestroyWindow(c->pWindow);
	SDL_Quit();
}

/**
 * Log les erreurs SDL
 * @param char* msg
 *  le message d'erreur
 */

void logSDLError(char* msg)
{
	fprintf(stdout,"%s error: %s\n",msg,SDL_GetError());
}


/**
 * Chargement d'image BMP (pour l'instant)
 * @param char* file
 *  nom du fichier
 * @param SDL_Renderer* pRenderer
 *  Pointeur sur le renderer
 * @return SDL_Texture*
 *  la texture chargée 
 */
SDL_Texture* loadTexture(char* file,Context* c)
{
	SDL_Texture *texture = IMG_LoadTexture(c->pRenderer,file);
	if(texture==NULL)
	{
		logSDLError("IMG_LoadTexture");
		cleanup(c);
	}
	return texture;
}



/**
 * Dessine la texture dans le renderer
 * @param SDL_Texture* tex
 *  Texture à dessiner
 * @param SDL_Renderer* ren
 *  Le renderer dans lequel dessiner
 * @param int x
 * @param int y
 *  coordonneés x,y où dessiner
 * @param int w
 * @param int h
 *  width et height
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
 * Dessine la texture dans le renderer
 * @param SDL_Texture* tex
 *  Texture à dessiner
 * @param SDL_Renderer* ren
 *  Le renderer dans lequel dessiner
 * @param int x
 * @param int y
 *  coordonneés x,y où dessiner
 */
void renderTextureAsIs(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)
{
	int w,h;

	//On récupère la taille de la texture
	SDL_QueryTexture(tex,NULL,NULL,&w,&h);
	renderTexture(tex,ren,x,y,w,h);
}


/**
 * Load les images en tant que texture dans le context c
 * @param Context* c
 *  Context dans lequel loadé les images
 */
int loadImages(Context* c)
{
	
	c->board=loadTexture("img/board.png",c);
	c->pawn[WHITE]=loadTexture("img/whitePawn.png",c);
	c->pawnOut[WHITE]=loadTexture("img/whitePawnOut.png",c);
	c->pawn[BLACK]=loadTexture("img/blackPawn.png",c);
	c->pawnOut[BLACK]=loadTexture("img/blackPawnOut.png",c);
	int i;
	for(i=0;i<6;i++)
	{
		char pathDice[40];
		char pathDoublingCube[40];
		sprintf(pathDice, "img/dice%d.png",i+1);
		sprintf(pathDoublingCube, "img/doublingCube%d.png",i+1);
		c->dice[i]=loadTexture(pathDice,c);
		c->doublingCube[i]=loadTexture(pathDoublingCube,c);
	}
	return 0;

}


/**
 * Charge SDL et initialise le context(pWindow,pRenderer et textures)
 * @param Context* c
 *  Context à initialisé
 * @param char* title
 *	titre de la fenetre
 */
int init(Context *c, char* title)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		logSDLError("SDL_Init");
		return -1;
	}

	/*if (IMG_Init(IMG_INIT_PNG) != 0 )
	{
		logSDLError("IMG_Init");
		return -1;
	}*/

	if (TTF_Init()!= 0 )
	{
		logSDLError("TTF_Init");
		return -1;
	}

	c->pWindow = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,820,620,SDL_WINDOW_SHOWN);

	if (c->pWindow==NULL)
	{
		logSDLError("SDL_CreateWindow");
		cleanup(c);
		return 1;
	}

	c->pRenderer= SDL_CreateRenderer(c->pWindow,-1,SDL_RENDERER_ACCELERATED);

	if (c->pRenderer==NULL)
	{
		logSDLError("SDL_CreateRenderer");
		cleanup(c);
		return 1;
	}
	loadImages(c);

	return 0;
}



/**
 * Mise à jour de l'affichage
 * @param Context* c
 *  Context pour l'affichage
 * @param SGameState gs
 *	gamestate à afficher
 */
int update(Context *c, SGameState gs)
{

	/*Update des pions*/
	int i,j,x,y;
	for(i=0;i<24;i++)
	{
		if(gs.board[i].owner!=NOBODY)
		{
			for(j=0;j<gs.board[i].nbDames;j++)
			{
				if(i<12)
				{
					x=560-50*i;
					if(j<5)
						y=560-30*j;
					else
					{
						y=440+30*(j-5);
						x+=5;
					}
				}
				else
				{
					x=10+50*(i-12);
					if(j<5)
						y=10+30*j;
					else
					{
						y=130-30*(j-5);
						x+=5;
					}
					
				}

				if(gs.board[i].owner==WHITE)
					renderTextureAsIs(c->pawn[WHITE],c->pRenderer,x,y);
				else
					renderTextureAsIs(c->pawn[BLACK],c->pRenderer,x,y);
			}
		}
	}
	//mise à jour des out

	for (i=0;i<gs.out[WHITE];i++)
	{
		renderTextureAsIs(c->pawnOut[WHITE],c->pRenderer,622,397+i*7);	
	}
	for (i=0;i<gs.out[BLACK];i++)
	{
		renderTextureAsIs(c->pawnOut[BLACK],c->pRenderer,622,116+i*7);
	}
	//mise à jour des bar (affichage du nombre de pions)

	renderTextureAsIs(c->pawn[BLACK],c->pRenderer,510,280);

	renderTextureAsIs(c->pawn[WHITE],c->pRenderer,110,280);


	//Videau
	int face;
	switch(gs.stake)
	{
		case 1:
			face=5;
		break;
		case 2:
			face=0;
		break;
		case 4:
			face=1;
		break;
		case 8:
			face=2;
		break;
		case 16:
			face=3;
		break;
		case 32:
			face=4;
		break;
		case 64:
			face=5;
		break;

	}
	renderTextureAsIs(c->doublingCube[face],c->pRenderer,710,310);

	return 0;
}