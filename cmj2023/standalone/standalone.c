
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avalam.h>
#include <topologie.h>


#define PATH_REFRESH "../build/web/data/refresh-data.js"

int main(){
    T_Position p;
    T_Coup coup;

    p = getPositionInitiale();

    octet flag[3], var;

    CreerBonus(p, p.evolution.bonusJ, JAU, 100);
    CreerBonus(p, p.evolution.bonusR, ROU, 100);
    CreerBonus(p, p.evolution.bonusJ, JAU, p.evolution.bonusJ);
    CreerBonus(p, p.evolution.bonusJ, ROU, p.evolution.bonusR);

    while (getCoupsLegaux(p).nb != 0){
        printf("\nAu tout de %s\n", COLNAME(p.trait));

        printf('\nChoisissez l origine du coup: ');
        scanf("%d", coup.origine);

        printf("\n\nChoisissez la destination du coup : ");
        scanf("%d", coup.destination);
        printf("\n");

        p = jouerCoup(p, coup.origine, coup.destination);
    }

return 0;
}











int CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee){

    octet var, test = 0;


    printf ("\nEn quelle position placez-vous le bonus/malus ? ");
    scanf("%d", var);

    while(test == 0){

            test = 1;

            if (var < 0 || var > NBCASES){
                printf("\nCase hors du plateau, redonnez une nouvelle case : ");
                scanf("%d", var);
                print("\n");
                test = 0;
            }
            if (test == 1 && equipe != p.cols[var].couleur){
                printf("\nCase de mauvaise couleur, redonnez une nouvelle case : ");
                scanf("%d", var);
                print("\n");
                test = 0;
            }
            if (test == 1 && var == case_bloquee){
                printf("\nCase  deja prise pour le bonus, redonnez une nouvelle case : ");
                scanf("%d", var);
                print("\n");
                test = 0;
            } 
    }

    *bonus = var;

    return var;
}
