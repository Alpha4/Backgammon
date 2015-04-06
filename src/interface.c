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
	if(c->pawnOut[0]!=NULL)
		SDL_DestroyTexture(c->pawnOut[0]);
	if(c->pawnOut[1]!=NULL)
		SDL_DestroyTexture(c->pawnOut[1]);
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
	TTF_Quit();
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
	c->prompt=loadTexture("img/prompt.png",c);
	c->button=loadTexture("img/button.png",c);
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
 * @param unsigned char* dices
 *	tableau des dés
 */
int update(Context *c, SGameState gs,unsigned char* dices)
{
	SDL_RenderClear(c->pRenderer);

	renderTextureAsIs(c->board,c->pRenderer,0,0);

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

	//Score cumulés à AJOUTER


	//Tour en cours

	//mise à jour des out
	char out[4];
	SDL_Color white={0,0,0,0};
	sprintf(out,"%d",gs.out[WHITE]);
	renderTextureAsIs(renderText(out,c,42,white),c->pRenderer,682,397);
	sprintf(out,"%d",gs.out[BLACK]);
	renderTextureAsIs(renderText(out,c,42,white),c->pRenderer,682,116);


	for (i=0;i<gs.out[WHITE];i++)
	{
		renderTextureAsIs(c->pawnOut[WHITE],c->pRenderer,622,397+i*7);

	}
	for (i=0;i<gs.out[BLACK];i++)
	{
		renderTextureAsIs(c->pawnOut[BLACK],c->pRenderer,622,116+i*7);
	}


	//mise à jour des bar (affichage du nombre de pions)
	char bar[4];
	sprintf(bar,"%d",gs.bar[BLACK]);
	renderTextureAsIs(c->pawn[BLACK],c->pRenderer,510,280);
	renderTextureAsIs(renderText(bar,c,42,white),c->pRenderer,470,280);
	sprintf(bar,"%d",gs.bar[WHITE]);
	renderTextureAsIs(c->pawn[WHITE],c->pRenderer,110,280);
	renderTextureAsIs(renderText(bar,c,42,white),c->pRenderer,70,280);




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
	renderTextureAsIs(c->doublingCube[face],c->pRenderer,685,285);

	//Dés
	renderTextureAsIs(c->dice[dices[0]-1],c->pRenderer,250,285);
	renderTextureAsIs(c->dice[dices[1]-1],c->pRenderer,370,285);

	SDL_RenderPresent(c->pRenderer);

	return 0;
}

/**
 * Render du texte
 * @param char* text
 *	le texte à afficher
 * @param Context c
 *	le context contient la police
 * @param int size
 *	la taille en pt du texte
 * @param SDL_Color color
 *	la couleur du texte en RGBa
 */
SDL_Texture* renderText(char* text,Context* c,int size,SDL_Color color)
{
	TTF_Font* font=TTF_OpenFont("img/GeosansLight.ttf",size);
	if (font==NULL)
	{
		logSDLError("TTF_OpenFont");
		TTF_CloseFont(font);
	}
	SDL_Surface* surf=TTF_RenderText_Blended(font,text,color);
	if (surf == NULL)
	{
		logSDLError("TTF_RenderText");
		return NULL;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(c->pRenderer, surf);
	if (texture == NULL)
	{
		logSDLError("CreateTexture");
	}
	SDL_FreeSurface(surf);

	return texture;
}

/**
 *	Fonction qui affiche le prompt pour le doublage de la mise
 * @param Context* c
 *	le context de l'affichage
 * @param int qoa
 *	question or answer 
 */
void doubleQuery(Context* c,int qoa)
{
	SDL_Color white={0,0,0,0};

	renderTextureAsIs(c->prompt,c->pRenderer,85,260);
	renderTextureAsIs(c->button,c->pRenderer,175,320);
	renderTextureAsIs(c->button,c->pRenderer,357,320);
	renderTextureAsIs(renderText("Oui",c,24,white),c->pRenderer,382,323);
	renderTextureAsIs(renderText("Non",c,24,white),c->pRenderer,200,320);
	if(!qoa)
	{
		renderTextureAsIs(renderText("Doubler la mise ?",c,36,white),c->pRenderer,100,260);
	}
	else
	{	
		renderTextureAsIs(renderText("Acceptez ?",c,36,white),c->pRenderer,100,260);
	}

	SDL_RenderPresent(c->pRenderer);

}

/** Fonction qui attend que le joueur clic sur une des cases du plateau et renvoie le numéro de celle ci
 * @param Player player
 *    joueur qui clic sur la case
 * @return int pointClicked
 *    numéro de la cellule sur laquelle le joueur a cliqué
 */
int pointClicked(Player player){


	// initialisation du numéro de case cliquée
	int pointClicked = -1;

	int i = -1;

	int x;
	int y;

	// déclaration de l'événement
	SDL_Event event;

	// traitement de l'événement
	while (i == 0){

		if (SDL_PollEvent(&event)){

			switch (event.type){
				// cas d'un clic ( souris)
				case SDL_MOUSEBUTTONDOWN: 

					// définition des coordonnées du clic :
					x = event.button.x;
					y = event.button.y;

					// on ne traite que le cas d'un clic gauche
					if ( event.button.button == 1 ){ 
						
						// surface représentant la premiere case
						int xMin = 560;
						int xMax = 610;
						int yMin = 360;
						int yMax = 610;

						// parcours des cases "basses" du plateau
						int j;
						for ( j=1; j<13; j++ ){ 

							// si le clic est sur cette case, on retourne le numéro de celle ci
							if (  (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){ 
								pointClicked = j;
							}

							// actualisation des coordonnées pour la surface de la prochaine case
							xMin -= 50;
							xMax -= 50;
						}

						// surface représentant la 13e case
						xMin = 10;
						xMax = 60;
						yMin = 10;
						yMax = 260;

						//parcours des cases "hautes" du plateau
						for ( j=13; j<25; j++){

							// si le clic est sur cette case, on retourne le numéro de celle ci
							if (  (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){ 
								pointClicked = j;
							}

							// actualisation des coordonnées pour la surface de la prochaine case
							xMin += 50;
							xMax += 50;
						}

						// cas spécifiques du joueur blanc
						if ( player == WHITE ){

							// clic sur le bar

							// surface du bar
							xMin = 110;
							xMax = 160;
							yMin = 280;
							yMax = 330;

							if ( (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){
								pointClicked = 0;
							}

							// clic sur le out

							// surface du out
							xMin = 622;
							xMax = 672;
							yMin = 397;
							yMax = 502;

							if ( (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){
								pointClicked = 25;
							}
						}

						// cas spécifiques du joueur noir
						if ( player == BLACK ){

							// clic sur le bar

							// surface du bar
							xMin = 510;
							xMax = 560;
							yMin = 280;
							yMax = 330;

							if ( (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){
								pointClicked = 0;
							}

							// clic sur le out

							// surface du out
							xMin = 622;
							xMax = 672;
							yMin = 116;
							yMax = 221;

							if ( (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){
								pointClicked = 25;				
							}
						}
					}
				break;
			}
		}
	}
	return pointClicked;
}

/**
 * Fonction qui renvoie le mouvement effectué par le joueur
 * @return SMove move
 *    mouvement effectué par le joueur
 */
SMove getMoveDone(Player player){
	SMove move;
	unsigned int numSrcCell = pointClicked(player);
	unsigned int numDestCell = pointClicked(player);
	move.src_point = numSrcCell;
	move.dest_point = numDestCell;
	return move;
}

/**
 * Fonction qui renvoie si le joueur a répondu 'oui' ou 'non' à une question posée
 * @return int response
 *   réponse du joueur : 0-->oui   1-->non
 */
int yesOrNo(){

	// déclaration de la réponse
	int response = -1;

	// déclaration de l'événement
	SDL_Event event;

	int x;
	int y;

	// traitement de l'événement
	int i = 0;
	while ( i == 0 ){

		if (SDL_PollEvent(&event)){

			switch (event.type){

				// cas d'un clic (souris)
				case SDL_MOUSEBUTTONDOWN:
				
					// définition des coordonnées du clic :
					x = event.button.x;
					y = event.button.y;

					// on ne traite que le cas d'un clic gauche
					if ( event.button.button == 1){

						// surface représentant les boutons "oui" et "non"
						int xMinNo = 175;
						int xMaxNo = 263;

						int yMin = 320;
						int yMax = 350;

						int xMinYes = 357;
						int xMaxYes = 445;

						// si le joueur clic sur le bouton "non", la réponse est 1
						if ( (x>xMinNo) && (x<xMaxNo) && (y>yMin) && (y<yMax) ){
							response = 1;
							i++; // on sort de la boucle --> l'événement souhaité est passé
						}

						// si le joueur clic sur le bouton "oui", la réponse est 0
						if ( (x>xMinYes) && (x<xMaxYes) && (y>yMin) && (y<yMax) ){
							response = 0;
							i++; // on sort de la boucle --> l'événement souahité est passé
						}

					}
				break;
			}
		}
	}
	return response;
}
