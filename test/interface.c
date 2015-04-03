#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

/*Bye bye le main bonjour les fonctions !*/

/*
    TROUVER UN MOYEN DE CLEANUP
    WINDOW + RENDERER + SURFACE + ETC 

*/

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
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)
{
    int w,h;

    //On récupère la taille de la texture
    SDL_QueryTexture(tex,NULL,NULL,&w,&h);
    renderTexture(tex,ren,x,y,w,h);
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
















/*TEST DE CRÉATION D'UNE BÊTE FENÊTRE! NOW WITH A 100% MORE IMAGE ! WHICH SUMS UP TO 1*/
int main(int argc, char** argv)
{
    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    
    /* Création de la fenêtre */
    SDL_Window* pWindow = NULL;
    pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN);
    if (pWindow==NULL)
    {
        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    /*Renderer*/
    SDL_Renderer *ren = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED);
    if (ren==NULL)
    {
        fprintf(stderr,"Erreur de création du renderer: %s\n",SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }


    SDL_Texture *fond = IMG_LoadTexture(ren, "img/test.bmp");
    /*image
    SDL_Surface *bmp = SDL_LoadBMP("img/test.bmp");
    if (bmp==NULL)
    {
        fprintf(stderr,"Erreur de création du chargemet d'image BMP: %s\n",SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    /*Texture
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if(tex==NULL)
    {
        fprintf(stderr,"Erreur de création de texture: %s\n",SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }*/

    /*Render*/
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, fond, NULL, NULL);
    SDL_RenderPresent(ren);

    SDL_Delay(3000);

    SDL_DestroyTexture(fond);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}