/**************************************************************************************************************
 * Nom du programme : standalone.c
 * Auteur : Aval_Bytes (Ramos Geoffrey, Pollet--Dassonval Mathis, Mejdoub Noé et Retourné Mathieu)
 * Date de création : 15/02/2023
 * Dernière mise à jour : 02/03/2023
 * Version : 1.0
 * Description : Jeu Avalam à deux joueurs avec saisie des coups au clavier et affichage par avalam-refresh
**************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avalam.h>
#include <topologie.h>

//Variables globales :

//Chemin du fichier dans lequel écrire les informations sur l'état actuel du jeu
#define PATH_REFRESH "../build/web/data/refresh-data.js" 

//Variable qui définit si le programme est en mode débogage ou non
#ifdef __DEBUG__ 
    #define DBG 1
#else
    #define DBG 0
#endif

//Prototypes des fonctions :
void EcrireJS(T_Position p, char* chemin);
void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee);

int main(){ 
    //Suppression de la variable d'environnement GTK_MODULES qui peut causer des problèmes avec l'interface graphique
    unsetenv("GTK_MODULES");
    //Exécution d'une commande système qui ouvre le fichier "avalam-refresh.html" dans un navigateur
    system ("xdg-open ../build/web/avalam-refresh.html"); 

    T_Position p; //Variable qui stocke la position actuelle du jeu
    T_Coup coup; //Variable qui stocke le coup entré par l'utilisateur

    p = getPositionInitiale(); //Initialisation de la variable p avec la configuration de départ du jeu Avalam

    EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau

    octet flag[3], var; //Variables permettant de créer les bonus et malus

    CreerBonus(p, &p.evolution.bonusJ, JAU, 100); //Création du bonus Jaune
    EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau
    CreerBonus(p, &p.evolution.bonusR, ROU, 100); //Création du bonus Rouge
    EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau
    CreerBonus(p, &p.evolution.malusJ, JAU, p.evolution.bonusJ); //Création du malus Jaune
    EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau
    CreerBonus(p, &p.evolution.malusR, ROU, p.evolution.bonusR); //Création du malus Rouge
    EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau

    while (getCoupsLegaux(p).nb != 0){ //Tant qu'il reste des coups jouables 
        printf("\nAu tour de %s\n", COLNAME(p.trait)); //Affichage de la couleur de l'équipe qui doit jouer

        //Récupération de la position de départ du pion à bouger
        printf("\nChoisissez l origine du coup: "); 
        scanf("%hhu", &coup.origine);

        //Récupération de la position d'arrivée du pion à bouger
        printf("\n\nChoisissez la destination du coup : "); 
        scanf("%hhu", &coup.destination);
        printf("\n");

        p = jouerCoup(p, coup.origine, coup.destination); //Déplacement du pion
        EcrireJS(p, PATH_REFRESH); //Mise à jour du Plateau
    }
    if(evaluerScore(p).nbJ==evaluerScore(p).nbR) //Si le score de l'équipe Jaune et de l'équipe Rouge sont égaux
	{
        //Si le nombre de colonnes de 5 pions de l'équipe Jaune est inférieur à celui de l'équipe Rouge
		if(evaluerScore(p).nbJ5<evaluerScore(p).nbR5){ 
			printf("\nRouge gagnant\n"); //Alors l'équipe Rouge a gagné

		} else { //Sinon l'équipe jaune a gagné
			printf("\nJaune gagnant\n");
		}
	}

    //Si le score de l'équipe Jaune est inférieur à celui de l'équipe Rouge
    else if (evaluerScore(p).nbJ<evaluerScore(p).nbR) {  
		printf("\nRouge gagnant\n"); //Alors l'équipe Rouge a gagné
	} 
    else { //Sinon l'équipe Jaune a gagné
		printf("\nJaune gagnant\n"); 
	}
return 0;
}

/**
 * Nom de la fonction : CreerBonus
 * 
 * Description : Fonction qui permet de créer un bonus ou un malus pour une équipe donnée, tout en choisissant une case saisie par l'utilisateur.
 * 
 * @param p : Position actuelle du jeu
 * @param bonus : Pointeur vers une variable qui stockera la case où le bonus/malus sera créé
 * @param equipe : Equipe qui positionne le bonus/malus
 * @param case_bloque : case où un bonus/malus est présent, empêchant ainsi de poser un autre bonus/malus sur la même case
 * 
 * @return void
*/
void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee){

    octet var = 0;
    int test = 0;

    //Récupération de la position où l'utilisateur souhaite positionner son bonus/malus
    printf ("\nEn quelle position placez-vous le bonus/malus ? ");
    scanf("%hhu", &var);

    do{
        test = 1;

        //Si la case sélectionnée par l'utilisateur n'est pas sur le Plateau
        if (var < 0 || var > NBCASES - 1){
            //Alors on redemande à l'utilisateur de sélectionner une case du Plateau
            printf("\nCase hors du plateau, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        }

        //Si la case sélectionnée est différent de la couleur du pion
        if (test == 1 && equipe != p.cols[var].couleur){
            //Alors on redemande à l'utilisateur de sélectionner une case du Plateau de la bonne couleur
            printf("\nCase de mauvaise couleur, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        }
        //Si la case est déjà prise par un bonus/malus
        if (test == 1 && var == case_bloquee){
            //Alors on redemande à l'utilisateur de sélectionner une case du Plateau qui n'est pas déjà prise par un bonus/malus
            printf("\nCase deja prise pour le bonus, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        } 
    }while(test == 0);//Tant que l'utilisateur ne répond pas aux 3 conditions ci-dessus

    *bonus = var;
}

/**
 * Nom de la fonction : EcrireJS
 * 
 * Description : Fonction qui permet d'écrire les informations sur l'état actuelle de la partie dans un fichier JS afin de les afficher sur l'interface web
 * 
 * @param p : Position actuelle du jeu
 * @param chemin : Chemin du fichier dans lequel écrire les informations
 * @return void
*/
void EcrireJS(T_Position p, char* chemin){
    FILE *f; //Variable f : Pointeur vers une structure FILE utilisée pour gérer les fichiers
    f = fopen(chemin, "w"); //Ouvre un fichier en mode écriture
    T_Score scores = evaluerScore(p); //Variable qui stocke les scores de la partie


    if (f != NULL){ //Si l'ouverture du fichier f a réussi
        //Alors écrire les scores et les informations sur les bonus/malus de chaque équipe dans le fichier f ouvert
        fprintf(f, "traiterJson({\n\"trait\":%d,\n\"scoreJ\":%d,\n\"scoreJ5\":%d0,\n\"scoreR\":%d,\n\"scoreR5\":%d,\n\"bonusJ\":%d,\n\"malusJ\":%d,\n\"bonusR\":%d,\n\"malusR\":%d,\n\"cols\":[\n", 
        p.trait, scores.nbJ , scores.nbJ5, scores.nbR, scores.nbR5,p.evolution.bonusJ, 
        p.evolution.malusJ, p.evolution.bonusR, p.evolution.malusR);
        T_Colonne col;
        for (int i = 0; i <= NBCASES - 2; i++){
            col = p.cols[i];
            fprintf(f,"\t{\"nb\":%d, \"couleur\":%d},\n", col.nb, col.couleur);
        }
        fprintf(f, "\t {\"nb\":%d, \"couleur\":%d}]});",p.cols[NBCASES-1].nb, p.cols[NBCASES-1].couleur);
    }
    fclose(f); //Fermeture du fichier f
}
 