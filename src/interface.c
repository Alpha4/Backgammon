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
	//Images
	int i;
	for (i=0;i<6;i++)
	{
		if (c->dice[i]!=NULL)
			SDL_DestroyTexture(c->dice[i]);
		if(c->doublingCube[i]!=NULL)
			SDL_DestroyTexture(c->doublingCube[i]);
	}
	if(c->prompt!=NULL)
		SDL_DestroyTexture(c->prompt); //Texture pour les prompts
	if(c->button!=NULL)
		SDL_DestroyTexture(c->button); //Texture pour les boutons
	if(c->highlightUp!=NULL)
		SDL_DestroyTexture(c->highlightUp); //Texture pour highlight point
	if(c->highlightDown!=NULL)
		SDL_DestroyTexture(c->highlightDown);
	if(c->highlightPawn!=NULL)
		SDL_DestroyTexture(c->highlightPawn);
	if(c->highlightOut!=NULL)
		SDL_DestroyTexture(c->highlightOut);
	if(c->grayedOutDice!=NULL)
		SDL_DestroyTexture(c->grayedOutDice); //Texture pour griser les dés
	if(c->pawnOut[0]!=NULL)
		SDL_DestroyTexture(c->pawnOut[0]);
	if(c->pawnOut[1]!=NULL)
		SDL_DestroyTexture(c->pawnOut[1]);
	if(c->pawn[0]!=NULL)
		SDL_DestroyTexture(c->pawn[0]);
	if(c->pawn[1]!=NULL)
		SDL_DestroyTexture(c->pawn[1]);
	if(c->board!=NULL)
		SDL_DestroyTexture(c->board);

	//Textes
	if(c->msg!=NULL)
		SDL_DestroyTexture(c->msg); //Texte prompt et butto
	if(c->nbBarW!=NULL)
		SDL_DestroyTexture(c->nbBarW);
	if(c->nbBarB!=NULL)
		SDL_DestroyTexture(c->nbBarB);
	if(c->nbOutW!=NULL)
		SDL_DestroyTexture(c->nbOutW);
	if(c->nbOutB!=NULL)
		SDL_DestroyTexture(c->nbOutB);
	if(c->scoreW!=NULL)
		SDL_DestroyTexture(c->scoreW);
	if(c->scoreB!=NULL)
		SDL_DestroyTexture(c->scoreB);
	if(c->texturn!=NULL)
		SDL_DestroyTexture(c->texturn);
	if(c->oui!=NULL)
		SDL_DestroyTexture(c->oui);
	if(c->non!=NULL)
		SDL_DestroyTexture(c->non);
	if(c->doubler!=NULL)
		SDL_DestroyTexture(c->doubler);


	//else
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
	c->highlightUp=loadTexture("img/highlightUp.png",c);
	c->highlightDown=loadTexture("img/highlightDown.png",c);
	c->grayedOutDice=loadTexture("img/grayedOutDice.png",c);
	c->highlightOut=loadTexture("img/highlightOut.png",c);
	c->highlightPawn=loadTexture("img/highlightPawn.png",c);
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
 *	Fonction mettant en surbrillance les points / bar / out
 * @param Context* c
 *	le context pour l'affichage
 * @param int i
 *	numéro du point à highlight
 * @param Player p
 *	le joueur qui joue (nécessaire pour le bar et out)
*/
void highlight(Context* c,int i,Player p)
{
	//0 bar
	//25 out
	if(i==0)
	{
		if(p==BLACK)
			renderTextureAsIs(c->highlightPawn,c->pRenderer,450,280);
		else
			renderTextureAsIs(c->highlightPawn,c->pRenderer,50,280);
	}
	else if (i==25)
	{
		if(p==BLACK)
			renderTextureAsIs(c->highlightOut,c->pRenderer,682,116);
		else
			renderTextureAsIs(c->highlightOut,c->pRenderer,682,397);
	}
	else if(i<13)
	{
		renderTextureAsIs(c->highlightUp,c->pRenderer,560-50*(i-1),360);
	}
	else
	{
		renderTextureAsIs(c->highlightDown,c->pRenderer,10+50*(i-13),10);
	}
}

/**
 *	Fonction grisant le dé indiqué
 * @param Context* c
 *	le context pour l'affichage
 * @param int* dice
 *	tableau des dés
*/
void grayOut(Context* c,int* dice)
{
	if(dice[2]==dice[3]&&dice[2]==-1) // pas de double
	{
		if(dice[0]==-1)
		{
			renderTextureAsIs(c->grayedOutDice,c->pRenderer,250,285);
		}
		if(dice[1]==-1)
		{
			renderTextureAsIs(c->grayedOutDice,c->pRenderer,320,285);
		}
	}
	else //Double
	{
		int i;
		for (i=0;i<4;i++)
		{
			if (dice[i]==-1)
			{
				if(i%2==0)
				{
					renderTextureAsIs(c->grayedOutDice,c->pRenderer,250,285);
				}
				else if (i%2==1)
				{
					renderTextureAsIs(c->grayedOutDice,c->pRenderer,320,285);
				}
			}
		}
	}
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

	SDL_Color black={0,0,0,0};
	c->accept=renderText("Accepter ?",c,36,black);
	c->doubler=renderText("Doubler la mise ?",c,36,black);
	c->oui=renderText("Oui",c,24,black);
	c->non=renderText("Non",c,24,black);


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

	SDL_Color white={0,0,0,0};

	//Score cumulé
	char sW[20];
	char sB[20];
	sprintf(sW,"Score %d",gs.whiteScore);
	sprintf(sB,"Score %d",gs.blackScore);

	c->scoreB=renderText(sB,c,42,white);
	c->scoreW=renderText(sW,c,42,white);

	renderTextureAsIs(c->scoreB,c->pRenderer,620,10);
	renderTextureAsIs(c->scoreW,c->pRenderer,620,530);



	//Tour en cours
	char turn[20];
	sprintf(turn,"Tour %d",gs.turn);
	c->texturn=renderText(turn,c,36,white);
	renderTextureAsIs(c->texturn,c->pRenderer,620,580);
	renderTextureAsIs(c->pawn[((gs.turn)%2)],c->pRenderer,720,580);

	//mise à jour des out
	char out[4];
	
	sprintf(out,"%d",gs.out[WHITE]);
	c->nbOutW=renderText(out,c,42,white);
	renderTextureAsIs(c->nbOutW,c->pRenderer,682,397);
	c->nbOutB=renderText(out,c,42,white);
	sprintf(out,"%d",gs.out[BLACK]);
	renderTextureAsIs(c->nbOutB,c->pRenderer,682,116);


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
	c->nbBarB=renderText(bar,c,42,white);
	renderTextureAsIs(c->nbBarB,c->pRenderer,470,280);

	
	sprintf(bar,"%d",gs.bar[WHITE]);
	renderTextureAsIs(c->pawn[WHITE],c->pRenderer,110,280);
	c->nbBarW=renderText(bar,c,42,white);
	renderTextureAsIs(c->nbBarW,c->pRenderer,70,280);




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
	renderTextureAsIs(c->doublingCube[face],c->pRenderer,625,285);

	//Dés
	renderTextureAsIs(c->dice[dices[0]-1],c->pRenderer,250,285);
	renderTextureAsIs(c->dice[dices[1]-1],c->pRenderer,320,285);

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
 *	Fonction affichant un message sur l'écran et attend 2secondes
 * @param Context* c
 *	le context pour l'affichage
 * @param char* text
 *	le message à afficher
*/
void prompt(Context* c,char* text)
{
	SDL_Color white={0,0,0,0};
	renderTextureAsIs(c->prompt,c->pRenderer,85,260);
	c->msg=renderText(text,c,36,white);
	renderTextureAsIs(c->msg,c->pRenderer,xMiddleOf(c->prompt,c->msg,85),yMiddleOf(c->prompt,c->msg,260));
}

/**
 *	Fonction affichantun bouton et son nom
 * @param Context* c
 *	le context pour l'affichage
 * @param char* name
 *	le nom du bouton
*/
void button(Context* c,char* name)
{
	SDL_Color white={0,0,0,0};

	renderTextureAsIs(c->button,c->pRenderer,685,292);

	c->msg=renderText(name,c,24,white);

	renderTextureAsIs(c->msg,c->pRenderer,xMiddleOf(c->button,c->msg,685),yMiddleOf(c->button,c->msg,292));
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
	renderTextureAsIs(c->prompt,c->pRenderer,85,260);
	renderTextureAsIs(c->button,c->pRenderer,175,320);
	renderTextureAsIs(c->button,c->pRenderer,357,320);

	renderTextureAsIs(c->oui,c->pRenderer,xMiddleOf(c->button,c->oui,357),yMiddleOf(c->button,c->oui,320));
	renderTextureAsIs(c->non,c->pRenderer,xMiddleOf(c->button,c->non,175),yMiddleOf(c->button,c->non,320));
	
	if(!qoa)
	{	
		renderTextureAsIs(c->doubler,c->pRenderer,xMiddleOf(c->prompt,c->doubler,85),260);
	}
	else
	{	
		renderTextureAsIs(c->accept,c->pRenderer,xMiddleOf(c->prompt,c->accept,85),260);
	}

}

/**
 *	Fonction qui renvoie la valeur x pour afficher au milieu
 * @param Texture* texOut 
 *	texture englobante
 * @param Texture* texIn
 *	texture englboée
 * @param int xOut
 *	abscisse d'affihage de la texture Out
 */
int xMiddleOf(SDL_Texture* texOut,SDL_Texture* texIn,int xOut)
{
	int wOut,wIn;
	SDL_QueryTexture(texOut,NULL,NULL,&wOut,NULL);
	SDL_QueryTexture(texIn,NULL,NULL,&wIn,NULL);

	return xOut+(wOut-wIn)/2;

}

/**
 *	Fonction qui renvoie la valeur y pour afficher au milieu
 * @param Texture* texOut 
 *	texture englobante
 * @param Texture* texIn
 *	texture englboée
 * @param int yOut
 *	ordonnée d'affihage de la texture Out
 */
int yMiddleOf(SDL_Texture* texOut,SDL_Texture* texIn,int yOut)
{
	int hOut,hIn;
	SDL_QueryTexture(texOut,NULL,NULL,NULL,&hOut);
	SDL_QueryTexture(texIn,NULL,NULL,NULL,&hIn);

	return yOut+(hOut-hIn)/2;

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

	int x;
	int y;

	// déclaration de l'événement
	SDL_Event event;

	// traitement de l'événement
	while (pointClicked == -1){

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
							if (  isClicked(x, y, xMin, xMax, yMin, yMax) ){ 
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
							if (  isClicked(x, y, xMin, xMax, yMin, yMax) ){ 
								pointClicked = j;
							}

							// actualisation des coordonnées pour la surface de la prochaine case
							xMin += 50;
							xMax += 50;
						}

						// cas spécifiques du joueur blanc
						if ( player == WHITE ){

							// clic sur le bar
							if ( isClicked(x, y, 110, 160, 280, 330) ){
								pointClicked = 0;
							}

							// clic sur le out
							if ( isClicked(x, y, 622, 672, 397, 502)  ){
								pointClicked = 25;
							}

						}

						// cas spécifiques du joueur noir
						if ( player == BLACK ){

							// clic sur le bar
							if ( isClicked(x, y, 510, 560, 280, 330)  ){
								pointClicked = 0;
							}

							// clic sur le out
							if ( isClicked(x, y, 622, 672, 116, 221)  ){
								pointClicked = 25;				
							}
						}
						// undo
						if ( isClicked(x, y, 685, 773, 292, 322) )	
						{
							pointClicked = 26;
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
 * @param Player player
 *   joueur qui doit effectuer le mouvement
 * @param SGameState
 *   état du jeu courant
 * @param int* dice
 *   dés obtenus pour le mouvement
 * @return SMove move
 *    mouvement effectué par le joueur
 */
SMove getMoveDone(Player player, SGameState* gameState, int* dice, Context* c, unsigned char* diceGiven){
	
	// mouvement qui sera effectué par le joueur
	SMove move;


	// récupération des cellules qui peuvent etre source d'un mouvement
	int srcCells[30];
	int indexSrc = getSrcCells( *gameState, player, srcCells);
	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 APPELER FILLIN 1 POUR AVOIR LES VRAIES SOURCES 
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 */
	
	printf("getMovesDone : cellules sources possibles :\n");
	int a;
	for(a=0;a<indexSrc;a++)
	{
		printf("SrcPossible : %d\n",srcCells[a]);
	}

	// affichage en surbrillance des cellules qui peuvent etre source du mouvement
	int i;
	SDL_RenderClear(c->pRenderer);
		update(c,*gameState,diceGiven);
		for (i=0; i<indexSrc; i++)
		{
			highlight(c, srcCells[i] , player);
		}
		grayOut(c,dice);
	SDL_RenderPresent(c->pRenderer);
	

	// tant que la case voulue pour la source du mouvement n'est pas dans srcCells on la "redemande"
	printf("src?\n");
	unsigned int numSrcCell = 100;
	while ( !(isIn(numSrcCell, indexSrc, srcCells)) ){
		printf("LastPointClicked  %d\n", numSrcCell);
		numSrcCell = pointClicked(player); // on récupère la case source voulue pour le mouvement
	}
	printf("LastLastPointClicked  %d\n", numSrcCell);

	SDL_RenderClear(c->pRenderer);
		update(c,*gameState,diceGiven);
		grayOut(c,dice);
	SDL_RenderPresent(c->pRenderer);

	//récupération des mouvements possibles
	SList* movesPossible = fillIn_1_MovesPossible( player, dice, *gameState);
	printf("GetMoveDone\n");
	printf("liste des premiers mouvements dispo dans getMoveDone:\n");
	printList(movesPossible);

	// récupération des cellules qui peuvent etre destination du mouvement
	int destCells[30];
	int indexDest = fillInDestCells(movesPossible, numSrcCell, destCells );
	
	for(a=0;a<indexDest;a++)
	{
		printf("DestPossible : %d\n",destCells[a]);
	}

	//Libération mémoire allouée pour la liste movesPossible
	//DeleteList(movesPossible);

	// affichage en surbrillance des cellules qui peuvent etre destination du mouvement

	SDL_RenderClear(c->pRenderer);
		update(c,*gameState,diceGiven);
		for (i=0; i<indexDest; i++)
		{
			highlight(c, destCells[i] , player);
		}
		button(c, "Undo");
		grayOut(c,dice);
	SDL_RenderPresent(c->pRenderer);


	

	printf("dest?\n");
	// tant que la case voulue par le joueur pour la destination n'est pas dans destCells on la "redemande"
	unsigned int numDestCell = 100;
	while ( !(isIn(numDestCell, indexDest, destCells)) )
	{
		printf("LastPointClicked  %d\n", numDestCell);

		//si le joueur cliques sur undo : on reprend la fonction au début
		if (numDestCell == 26)
		{
			return getMoveDone(player, gameState, dice, c, diceGiven);
		}
		numDestCell = pointClicked(player);
	}
	printf("LastLastPointClicked  %d\n", numDestCell);
	// actualisation du gameSate par rapport au mouvement effectué
	actualizeGameState(numSrcCell, numDestCell, gameState, player);

	
	// recherche du dé qui a été utilisé :
	int numDiceUsed = diceUsed(dice, player, numSrcCell, numDestCell);
	// actualisation des dés par rapport au mouvement effectué
	dice[numDiceUsed] = -1;


	// on remplit le mouvement
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

/**
 * Fonction qui remplie une liste contenant l'ensemble des mouvements effectué par le joueur (pour 1 tour)
 * @param SMoves* moves
 *   liste de mouvements à remplir
 * @param SGameState gameState
 *   état du jeu courant
 * @param unsigned char* diceGvien
 *   dés obtenus par le joueur
 * @param Player player
 *   joueur dont c'est le tour
 * @param Context* c
 *   contexte pour l'affichage
 * @preturn int nbMoves
 *   nombre de mouvements contenus dans la liste
 */
int getArrayMoves(SMove* moves, SGameState gameState, unsigned char* diceGiven, Player player, Context* c){
		
	
	//SList* movesPossible;
	// nombre de mouvements que le joueur doit faire
	int nbMoves; 
	SList* movesPossible = getMovesPossible(gameState, player, diceGiven, &nbMoves);

	// AFFICHAGE CONSOLE	
	printf("\ngetArrayMoves :movesPossible : \n");
	printList(movesPossible);
	printf("nbMoves = %i\n", nbMoves);

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 A FAIRE RECUPERE GETMOVESPOSSIBLE POUR DELETE
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/


	/*//Libération mémoire allouée pour la liste movesPossible
	printList(movesPossible);
	printListTab(movesPossible);
	DeleteList(movesPossible);*/

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
	// récupération des mouvements
	if ( nbMoves == 0 )
	{
		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
			prompt(c, "Pas de coup possible"); // prévient le joueur qu'il n'a pas de coup possible

		SDL_RenderPresent(c->pRenderer);
		playerClicked();
	}
	else if ( nbMoves == 1 ){
		SMove move1;
		move1 = getMoveDone(player, &gameState, dice, c, diceGiven);


		//remplissage du tableau de mouvements:
		moves[0] = move1;
	}
	else if ( nbMoves==2 ){
		SMove move1, move2;
		move1 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué

		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);
		move2 = getMoveDone( player, &gameState, dice, c, diceGiven); 

		// remplissage du tableau de mouvements:
		moves[0] = move1;
		moves[1] = move2;
	}
	else if ( nbMoves == 3 ){
		SMove move1, move2, move3;
		move1 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué

		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);
		move2 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué
		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);
		move3 = getMoveDone(player, &gameState, dice, c, diceGiven); 

		//remplissage du tableau de mouvements:
		moves[0] = move1;
		moves[1] = move2;
		moves[2] = move3;
	}
	else if ( nbMoves == 4 ){
		SMove move1, move2, move3, move4;
		move1 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué

		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);

		move2 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué
		
		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);
		
		move3 = getMoveDone(player, &gameState, dice, c, diceGiven); // le gameState et les dés sont actualisés en fonction du mouvement effectué
		
		SDL_RenderClear(c->pRenderer);
			update(c,gameState,diceGiven);
			grayOut(c,dice);
		SDL_RenderPresent(c->pRenderer);
		
		move4 = getMoveDone(player, &gameState, dice, c, diceGiven); 

		//remplissage du tableau de mouvements:
		moves[0] = move1;
		moves[1] = move2;
		moves[2] = move3;
		moves[3] = move4;
	}

	SDL_RenderClear(c->pRenderer);
		update(c,gameState,diceGiven);
		grayOut(c,dice);
	SDL_RenderPresent(c->pRenderer);
	return nbMoves;
}

/**
 * Fonction qui renvoit l'indice du dé utilisé pour le mouvement
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
 * Fonction qui remplit le tableau des cellules d'arrivée possibles d'un mouvement
 * @param SLits* movesPossible
 *   liste de mouvements possibles
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int* destCells
 *   tableau contenant le numéro des cellules d'arrivée possibles à remplir
 */
int fillInDestCells(SList* movesPossible, int numSrcCell, int* destCells){
	// index de la premiere case vide du tableau
	int index = 0;

	// parcours de la liste movesPossible
	SCell* cellEnTraitement = GetFirstElement(movesPossible);

	while ( cellEnTraitement != NULL)
	{
		// la cellule source du mouvement correspond à notre cellule numSrcCell
		if (cellEnTraitement->value.moves[0].src_point == numSrcCell)
		{
			destCells[index] = cellEnTraitement->value.moves[0].dest_point; // on ajoute la cellule destination au tableau destCells
			index ++;
		}

		// prochaine cellule à traiter
		cellEnTraitement = cellEnTraitement->next;
	}
	return index;
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
 * Fonction qui indique si un clic se situe sur un bouton
 * @param int x
 *   abscisse du clic
 * @param int y
 *   ordonnée du clic
 * @param int xMin
 *   abscisse du bord gauche du bouton
 * @param int xMax
 *   abscisse du bord gauche du bouton
 * @param int yMin
 *   ordonnée du bord bas du bouton
 * @param int yMax
 *   ordonnée du bord haut du bouton
 * @return int result
 *   0 --> le clic n'est pas sur le bouton
 *   1 --> le clic est sur le bouton
 */
int isClicked(int x, int y, int xMin, int xMax, int yMin, int yMax){
	// initialisation à " bouton non cliqué"
	int result = 0;

	// test si le clic se trouve dans la surface du bouton
	if ( (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax) ){
		result = 1;
	}

	return result;
}


/**
 * Fonction qui attend que le joueur clique sur l'écran
 */
void playerClicked(){

	// déclaration de l'événement
	SDL_Event event;

	int i=0;
	while ( i==0 ){
		if (SDL_PollEvent(&event)){

			switch (event.type){
				// cas d'un clic ( souris)
				case SDL_MOUSEBUTTONDOWN: 

					// on ne traite que le cas d'un clic gauche
					if ( event.button.button == 1 ){ 
						i++; // on sort de la boucle et de la fonction
					}

				break;
			}
		}
	}

}