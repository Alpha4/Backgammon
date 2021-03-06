#ifndef _INTERFACE_H
    #define _INTERFACE_H

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
	SDL_Texture* highlightUp; //Texture pour highlight point
	SDL_Texture* highlightDown;
	SDL_Texture* highlightPawn;
	SDL_Texture* highlightOut;
	SDL_Texture* grayedOutDice; //Texture pour griser les dés
	SDL_Texture* msg; //Texte prompt et button
	SDL_Texture* nbBarW;
	SDL_Texture* nbBarB;
	SDL_Texture* nbOutW;
	SDL_Texture* nbOutB;
	SDL_Texture* scoreW;
	SDL_Texture* scoreB;
	SDL_Texture* texturn;
	SDL_Texture* oui;
	SDL_Texture* non;
	SDL_Texture* doubler;
	SDL_Texture* accept;

	TTF_Font* fonts[3];
};

/***** FONCTIONS GRAPHIQUES *****/

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
 * Load les polices d'écriture dans le Context c
 * @param Context* c
 *  Context dans lequel loadé les images
 */
void loadFonts(Context *C);

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

/**
 *	Fonction mettant en surbrillance les points / bar / out
 * @param Context* c
 *	le context pour l'affichage
 * @param int i
 *	numéro du point à highlight
 * @param Player p
 *	le joueur qui joue (nécessaire pour le bar et out)
*/
void highlight(Context* c,int i,Player p);

/**
 *	Fonction grisant le dé indiqué
 * @param Context* c
 *	le context pour l'affichage
 * @param int i
 *	numéro du dé à griser
*/
void grayOut(Context* c,int *dice);

/***** FONCTIONS INTERACTIVES *****/

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
SList* getMoveDone(SMove* move, Player player, SGameState* gameState, int* dice, Context* c, unsigned char* diceGiven, int* srcCells, int indexSrc, SList* movesPossible, int rank );


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
 *   réponse du joueur : 0-->non   1-->oui
 */
int yesOrNo();

/**
 * Sauvegarde du résultat du round
 * @param char* winner
 *	nom du gagnant
 * @param int pointsWin
 *	le nombre de points gagnés
 * @param int round
 *	le round gagné
 *
 *	FORMAT
 *	nomGagnant	points\n<-- une game
 */
void saveResult(char* winner, int pointsWin);

/**
 * Sauvegarde du résultat du match
 * @param GameState gs
 *  l'état du jeu courant
 * @param char* p1Name
 *  nom joueur 1
 * @param char* p2Name
 *  nom joueur 2
 * @param Player player1
 *  couleur du joueur 1
 * @return char*
 *  le nom du gagnant
 *  FORMAT
 *  nomGagnant  points  nomPerdant  points\n
 */
char* saveMatch(SGameState gs, char* p1Name,char* p2Name,Player player1);


#endif
