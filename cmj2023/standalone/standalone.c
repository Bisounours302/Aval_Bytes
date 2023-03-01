#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avalam.h>
#include <topologie.h>


#define PATH_REFRESH "../build/web/data/refresh-data.js"

void EcrireJS(T_Position p, char* chemin);
void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee);

int main(){
    T_Position p;
    T_Coup coup;

    p = getPositionInitiale();

    EcrireJS(p, PATH_REFRESH);

    octet flag[3], var;

    CreerBonus(p, p.evolution.bonusJ, JAU, 100);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, p.evolution.bonusR, ROU, 100);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, p.evolution.bonusJ, JAU, p.evolution.bonusJ);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, p.evolution.bonusJ, ROU, p.evolution.bonusR);
    EcrireJS(p, PATH_REFRESH);

    while (getCoupsLegaux(p).nb != 0){
        printf("\nAu tout de %s\n", COLNAME(p.trait));

        printf('\nChoisissez l origine du coup: ');
        scanf("%d", coup.origine);

        printf("\n\nChoisissez la destination du coup : ");
        scanf("%d", coup.destination);
        printf("\n");

        p = jouerCoup(p, coup.origine, coup.destination);
        EcrireJS(p, PATH_REFRESH);
    }

return 0;
}

void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee){

    octet var, test = 0;


    printf ("\nEn quelle position placez-vous le bonus/malus ? ");
    scanf("%d", &var);

    while(test == 0){

            test = 1;

            if (var < 0 || var > NBCASES){
                printf("\nCase hors du plateau, redonnez une nouvelle case : ");
                scanf("%d", &var);
                printf("\n");
                test = 0;
            }
            if (test == 1 && equipe != p.cols[var].couleur){
                printf("\nCase de mauvaise couleur, redonnez une nouvelle case : ");
                scanf("%d", &var);
                printf("\n");
                test = 0;
            }
            if (test == 1 && var == case_bloquee){
                printf("\nCase  deja prise pour le bonus, redonnez une nouvelle case : ");
                scanf("%d", &var);
                printf("\n");
                test = 0;
            } 
    }

    *bonus = var;
}


void EcrireJS(T_Position p, char* chemin){
    FILE *f;
    f = fopen(chemin, "w");
    T_Score scores = evaluerScore(p);


    if (f != NULL){
        fprintf(f, "traiterJson({\n
                    \"trait\":%d,\n
                    \"scoreJ\":%d,\n
                    \"scoreJ5\":%d0,\n
                    \"scoreR\":%d,\n
                    \"scoreR5\":%d,\n
                    \"bonusJ\":%d,\n
                    \"malusJ\":%d,\n
                    \"bonusR\":%d,\n
                    \"malusR\":%d,\n
                    \"cols\":[\n", p.trait, scores.nbJ , scores.nbJ5, scores.nbR, scores.nbR5, 
                                p.evolution.bonusJ, p.evolution.malusJ, p.evolution.bonusR, p.evolution.malusR);
        T_Colonne col;
        for (int i = 0; i <= NBCASES - 2; i++){
            col = p.cols[i];
            fprintf(f,"\t{\"nb\":%d, \"couleur\":%d},\n", col.nb, col.couleur);
        }
        fprintf(f, "\t {\"nb\":%d, \"couleur\":%d}]});",p.cols[NBCASES-1].nb, p.cols[NBCASES-1].couleur);
    }
    fclose(f);
}

 