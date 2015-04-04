#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*Bye bye le main bonjour les fonctions !*/

typedef struct Context
{
	SDL_Window* pWindow; //La fenêtre
	SDL_Renderer* pRenderer; //Le renderer


	SDL_Texture* board; //Texture du board
	SDL_Texture* dice[6]; //Textures de dé
	SDL_Texture* pawn[2]; //Texture des pions

}Context;

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
SDL_Texture* loadTexture(char* file, SDL_Renderer *pRenderer)
{
	SDL_Texture *texture = IMG_LoadTexture(pRenderer,file);

	if(texture==NULL)
		logSDLError("IMG_LoadTexture");

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






int Init(Context *C, int x, int y, char* title)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		logSDLError("SDL_Init");
		return -1;
	}

	if (IMG_Init(IMG_INIT_PNG) != 0 )
	{
		logSDLError("IMG_Init");
		return -1;
	}

	if (TTF_Init()!= 0 )
	{
		logSDLError("TTF_Init");
		return -1;
	}

	return 1;
	//A COMPLETER
}









/*TEST DE CRÉATION D'UNE BÊTE FENÊTRE! */
int main(int argc, char** argv)
{

	const int SCREEN_WIDTH  = 800;
	const int SCREEN_HEIGHT = 600;
	//We'll just be using square tiles for now
	const int TILE_SIZE = 100   ;
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		logSDLError("SDL_Init");
		return -1;
	}

	
	/* Création de la fenêtre */
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if (pWindow==NULL)
	{
		logSDLError("SDL_CreateWindow");
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		return 1;
	}

	/*Renderer*/
	SDL_Renderer *ren = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED);
	if (ren==NULL)
	{
		logSDLError("SDL_CreateRenderer");
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		return 1;
	}


	SDL_Texture *fond = loadTexture("img/test.bmp",ren);

	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;
	int i;
	//Draw the tiles by calculating their positions
	for (i= 0; i < xTiles * yTiles; ++i)
	{
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(fond, ren, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,TILE_SIZE);
	}


	/*Render*/
	SDL_RenderPresent(ren);

	SDL_Delay(3000);

	SDL_DestroyTexture(fond);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}