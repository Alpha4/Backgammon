#ifndef _INTERFACE_H
    #define _INTERFACE_H


#include "liste.h"
#include "arbitre.h"

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
	SDL_Texture* prompt; //Texture pour les prompts
	SDL_Texture* button; //Texture pour les boutons
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
 * @param unsigned char* dices
 *	tableau des dés
 */
int update(Context *c, SGameState gs,unsigned char* dices);

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
SDL_Texture* renderText(char* text,Context* c,int size,SDL_Color color);

/**
 *	Fonction qui affiche le prompt pour le doublage de la mise
 * @param Context* c
 *	le context de l'affichage
 * @param int qoa
 *	question or answer 
 */
void doubleQuery(Context* c,int qoa);

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
int isClicked(int x, int y, int xMin, int xMax, int yMin, int yMax);


/**
 * Fonction qui attend que le joueur clique sur l'écran
 */
void playerClicked();

/** Fonction qui attend que le joueur clic sur une des cases du plateau et renvoie le numéro de celle ci
 * @param Player player
 *    joueur qui doit cliquer 
 * @return int piontClicked
 *    numéro de la cellule sur laquelle le joueur a cliqué
 */
int pointClicked(Player player);


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
int isIn(int elem, int index, int* tab);


/**
 * Fonction qui remplit le tableau des cellules d'arrivée possibles d'un mouvement
 * @param SLits* movesPossible
 *   liste de mouvements possibles
 * @param int numSrcCell
 *   numéro de la cellule de départ du mouvement
 * @param int* destCells
 *   tableau contenant le numéro des cellules d'arrivée possibles à remplir
 */
int fillInDestCells(SList* movesPossible, int numSrcCell, int* destCells);


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
int diceUsed(int* dice, Player player, int numSrcCell, int numDestCell);



/**
 * Fonction qui renvoie le mouvement effectué par le joueur
 * @return SMove move
 *    mouvement effectué par le joueur
 */
SMove getMoveDone(Player player, SGameState* gameState, int* dice);


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
int getArrayMoves(SMove* moves, SGameState gameState, unsigned char* diceGiven, Player player, Context* c);


/**
 * Fonction qui renvoie si le joueur a répondu 'oui' ou 'non' à une question posée
 * @return int response
 *   réponse du joueur : 0-->oui   1-->non
 */
int yesOrNo();

/**
 *	Fonction qui renvoie la valeur x pour afficher au milieu
 * @param Texture* texOut 
 *	texture englobante
 * @param Texture* texIn
 *	texture englboée
 * @param int xOut
 *	abscisse d'affihage de la texture Out
 */
int xMiddleOf(SDL_Texture* texOut,SDL_Texture* texIn,int xOut);

/**
 *	Fonction qui renvoie la valeur y pour afficher au milieu
 * @param Texture* texOut 
 *	texture englobante
 * @param Texture* texIn
 *	texture englboée
 * @param int yOut
 *	ordonnée d'affihage de la texture Out
 */
int yMiddleOf(SDL_Texture* texOut,SDL_Texture* texIn,int yOut);


/**
 *	Fonction affichant un message sur l'écran et attend 2secondes
 * @param Context* c
 *	le context pour l'affichage
 * @param char* text
 *	le message à afficher
*/

void prompt(Context* c,char* text);

/**
 *	Fonction affichantun bouton et son nom
 * @param Context* c
 *	le context pour l'affichage
 * @param char* name
 *	le nom du bouton
*/
void button(Context* c,char* name);

#endif