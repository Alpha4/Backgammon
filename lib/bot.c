#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"
#include "listebot.h"

int TARGET;
Player COLOR;
/*
void InitLibrary(char name[50])
{
    
}
*/
void StartMatch(const unsigned int target_score)
{
    TARGET = target_score;
    sprintf("%d",TARGET);
}

void StartGame(Player p)
{
    COLOR = p;
    printf("%d",COLOR);
}
/*
void EndGame()
{
    
}

void EndMatch()
{
    
}

int DoubleStack(const SGameState * const gameState)
{
    return 0;
}

int TakeDouble(const SGameState * const gameState)
{
    return 0;
}
*/
void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
    int canOut; // 1 si on peut sortir des dames, 0 sinon
    int diceValues[4]; // contient les valeurs possible de déplacement, modifié quand un mouvement est fait
    int mustBar; // si une dame dans le bar (1) , il faut la sortir
    SList* owned = CreateList();
    SList* ennemi = CreateList();
    SList* open = CreateList();
    SList* possibleMovements = CreateList();
    
    if  (dices[0] == dices[1]) // on remplit diceValues 
    {
        for (i=0,i<4,i++)
        {
            diceValues = dices[0];
        }
    }
    else
    {
        diceValues [0] =dices[0];
        diceValues [1] =dices[1];
        diceValues [2] =0;
        diceValues [3] =0;
    }
    
    
    if (gameState.bar[COLOR] == 0) // On vérifie le bar
    {
        mustBar = 0;
    }
    else
    {
        mustBar = 1;
    }
    
    
    for (i=0,i<24,i++) //on stocke les flèches nous appartenant et celles de l'ennemi, et les fleches ouvertes
    {
        nFleche = i;
        if (gamestate->board[i]->owner == COLOR) // Fleches à nous
        {
            if (COLOR == WHITE) // Si on est les blanc, on stocke en début de liste les dernières flèches
            {
                AddElementBegin_int(owned,nFleche);
                AddElementBegin_int(open,nFleche);
            }
            else // sinon on stockes au début
            {
                AddElementEnd_int(owned,nFleche);
                AddElementEnd_int(open,nFleche);
            }
        }
        else
        {
            if (gamestate->board[i]->owner == NOBODY) // Fleches neutres
            {
                if (COLOR == WHITE) // Si on est les blanc, on stocke en début de liste les dernières flèches
                {
                    AddElementBegin_int(open,nFleche);
                }
                else // sinon on stockes au début
                {
                    AddElementEnd_int(open,nFleche);
                }
            }
            else // Fleches ennemies
            {
                AddElementBegin_int(ennemi,nFleche);
            
                if (gamestate->board[i]->nbDames == 1)
                {
                    if (COLOR == WHITE) // Si on est les blanc, on stocke en début de liste les dernières flèches
                    {
                        AddElementBegin_int(open,nFleche);
                    }
                    else // sinon on stockes au début
                    {
                        AddElementEnd_int(open,nFleche);
                    }
                }
            }
        }
    }
    
    
    if (mustBar == 1)
    {
        SList* possibleMovements = CreateList();
        for (i=0,i<4,i++)
        {
            if (diceValues[i]!=0)
            {
                if ((open->head->value)+1 == diceValues[i])
                {
                    SMove* move;
                    move = malloc(sizeof(SMove));
                    move->src_point = 0;
                    move -> dest_point = open->head->value;
                    AddElementEnd_move(possibleMovements,move);
                }
            }
        }
        
    }
    else
    {
        
    }
    
}