#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avalam.h>
#include <topologie.h>


#define PATH_REFRESH "../build/web/data/refresh-data.js"

#ifdef __DEBUG__
    #define DBG 1
#else
    #define DBG 0
#endif

void EcrireJS(T_Position p, char* chemin);
void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee);

int main(){
    unsetenv("GTK_MODULES");
    system ("xdg-open ../build/web/avalam-refresh.html");
    T_Position p;
    T_Coup coup;

    p = getPositionInitiale();

    EcrireJS(p, PATH_REFRESH);

    octet flag[3], var;

    CreerBonus(p, &p.evolution.bonusJ, JAU, 100);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, &p.evolution.bonusR, ROU, 100);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, &p.evolution.malusJ, JAU, p.evolution.bonusJ);
    EcrireJS(p, PATH_REFRESH);
    CreerBonus(p, &p.evolution.malusR, ROU, p.evolution.bonusR);
    EcrireJS(p, PATH_REFRESH);

    while (getCoupsLegaux(p).nb != 0){
        printf("\nAu tour de %s\n", COLNAME(p.trait));

        printf("\nChoisissez l origine du coup: ");
        scanf("%hhu", &coup.origine);

        printf("\n\nChoisissez la destination du coup : ");
        scanf("%hhu", &coup.destination);
        printf("\n");

        p = jouerCoup(p, coup.origine, coup.destination);
        EcrireJS(p, PATH_REFRESH);
    }
    if(evaluerScore(p).nbJ==evaluerScore(p).nbR)
	{
		if(evaluerScore(p).nbJ5<evaluerScore(p).nbR5) {
			printf("\nRouge gagnant\n"); 
		} else {
			printf("\nJaune gagnant\n");
		}
	}

    else if (evaluerScore(p).nbJ<evaluerScore(p).nbR) {
		printf("\nRouge gagnant\n");
	} 
    else { 
		printf("\nJaune gagnant\n");
	}
return 0;
}

void CreerBonus(T_Position p, octet *bonus, octet equipe, octet case_bloquee){

    octet var = 0;
    int test = 0;

    printf ("\nEn quelle position placez-vous le bonus/malus ? ");
    scanf("%hhu", &var);

    do{
        test = 1;
        if (var < 0 || var > NBCASES - 1){
            printf("\nCase hors du plateau, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        }
        if (test == 1 && equipe != p.cols[var].couleur){
            printf("\nCase de mauvaise couleur, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        }
        if (test == 1 && var == case_bloquee){
            printf("\nCase deja prise pour le bonus, redonnez une nouvelle case : ");
            scanf("%hhu", &var);
            printf("\n");
            test = 0;
        } 
    }while(test == 0);

    *bonus = var;
}


void EcrireJS(T_Position p, char* chemin){
    FILE *f;
    f = fopen(chemin, "w");
    T_Score scores = evaluerScore(p);


    if (f != NULL){
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
    fclose(f);
}
 