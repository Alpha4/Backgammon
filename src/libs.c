/*

Fonctions utilisées pour la gestion des libs chargées

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "backgammon.h"
#include "libs.h"

int LoadLib(AI *ai, char* file)
{
    // On charge toutes les fonctions de la lib
    if ((ai->lib = dlopen(file, RTLD_LAZY)) == NULL)
    {
        // Erreur de chargement de la lib
        fprintf(stderr, "Erreur de chargement de la lib : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }

    // Chargement des pointeurs de fonctions pour chaque fonction de la lib
    if ((ai->InitLibrary = (pfInitLibrary) dlsym(ai->lib, "InitLibrary")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->StartMatch = (pfStartMatch) dlsym(ai->lib, "StartMatch")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->StartGame = (pfStartGame) dlsym(ai->lib, "StartGame")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->EndGame = (pfEndGame) dlsym(ai->lib, "EndGame")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->EndMatch = (pfEndMatch) dlsym(ai->lib, "EndMatch")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->DoubleStack = (pfDoubleStack) dlsym(ai->lib, "DoubleStack")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->TakeDouble = (pfTakeDouble) dlsym(ai->lib, "TakeDouble")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->PlayTurn = (pfPlayTurn) dlsym(ai->lib, "PlayTurn")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }

    return 0;
}