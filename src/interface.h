#ifndef _INTERFACE_H
    #define _INTERFACE_H


typedef struct Context Context;
/*	Structure contenant les variables utiles à l'affichage
 */
struct Context
{
	SDL_Window* pWindow; //La fenêtre
	SDL_Renderer* pRenderer; //Le renderer


	SDL_Texture* board; //Texture du board
	SDL_Texture* dice[6]; //Textures de dé
	SDL_Texture* pawn[2]; //Texture des pions
	SDL_Texture* pawnOut[2]; //Texture des pions Out
	SDL_Texture* doublingCube[6]; //Texture du videau

};

/**
 * Quitte SDL en détruisant le context
 * @param Context* c
 *  Context pour l'affichage
 */
void cleanup(Context* c);


/**
 * Log les erreurs SDL
 * @param char* msg
 *  le message d'erreur
 */
void logSDLError(char* msg);


/**
 * Chargement d'image BMP (pour l'instant)
 * @param char* file
 *  nom du fichier
 * @param SDL_Renderer* pRenderer
 *  Pointeur sur le renderer
 * @return SDL_Texture*
 *  la texture chargée 
 */
SDL_Texture* loadTexture(char* file,Context* c);

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
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);

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
void renderTextureAsIs(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

/**
 * Load les images en tant que texture dans le context c
 * @param Context* c
 *  Context dans lequel loadé les images
 */
int loadImages(Context* c);

/**
 * Charge SDL et initialise le context(pWindow,pRenderer et textures)
 * @param Context* c
 *  Context à initialisé
 * @param char* title
 *	titre de la fenetre
 */
int init(Context *c, char* title);

/**
 * Mise à jour de l'affichage
 * @param Context* c
 *  Context pour l'affichage
 * @param SGameState gs
 *	gamestate à afficher
 */
int update(Context *c, SGameState gs);




#endif