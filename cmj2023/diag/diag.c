//---------------------------------------------------------------------------------------------------------------------------------
//Auteurs :  Noé Mejdoub / Mathieu Retourné / Mathis Pollet--Dassonval / Geoffrey Ramos
//---------------------------------------------------------------------------------------------------------------------------------
/* OBJECTIFS :  - optimiser le code
                - faire en sorte qu'une modification n'en entraine pas une autre
                - faire un meilleur affichage de la description lue
                - commenter tout
*/
//---------------------------------------------------------------------------------------------------------------------------------
// BIBLIOTHEQUES
//---------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <avalam.h>
//---------------------------------------------------------------------------------------------------------------------------------
// MODE DEBUG
//---------------------------------------------------------------------------------------------------------------------------------
#ifdef __DEBUG__
  #define DBG 1
#else
  #define DBG 0
#endif
//---------------------------------------------------------------------------------------------------------------------------------
// CONSTANTES SYMBOLIQUE
//---------------------------------------------------------------------------------------------------------------------------------
#define MAXCAR 200              // au max a l'initialisation -> 24 u + 24 U + b + B + m + M + ' ' + j + '\0' = 55
#define TAILLE_NDIAG 10
#define TAILLE_NOM_FICHIER 50
#define TAILLE_DESCRIPTION 1000
#define NOM_PAR_DEFAUT "../web/exemples/diag_test.js"
#define NDIAG_DEFAULT "1"
//---------------------------------------------------------------------------------------------------------------------------------
typedef char Tchaine[MAXCAR];
typedef char Tformat[MAXCAR - 2];
typedef char Tdiag[TAILLE_NDIAG];
typedef char Tfichier[TAILLE_NOM_FICHIER];
typedef char Tdescription[TAILLE_DESCRIPTION];
//---------------------------------------------------------------------------------------------------------------------------------
// PROTOTYPES DES FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
int  lire_arguments(Tdiag Ndiag, int argc, char *argv[]);                               //lit les arguments passés dans la commande et retourne
void verif_Ndiag(Tdiag Ndiag);                                                          //vérifie que le numéro de diagramme est correct
int  lire_fen(Tchaine chaine, char *argv2, int nbMAXcaracAsaisir);                      //lit la chaine, fait un pré-formatage et retourne le nombre d'espace dans la chaine
void formater_fen(Tchaine chaine, int nbesp, Tchaine copie);                            //formate la chaine
void supprimer_zero(Tchaine chaine);                                                    //supprime les 0 inutiles dans la chaine
void supprimer_char(Tchaine chaine, int pos);                                           //supprime un caractere dans la chaine
void afficher_chaine(Tchaine chaine);                                                   //affiche la chaine fen
int  rechercher_esp(Tchaine chaine);                                                    //retourne l'indice du caractere ' ' dans la chaine
void validation_1(Tchaine chaine, int nbesp);                                           //vérifie le trait   
void validation_2(Tchaine chaine);                                                      //vérifie la saisie des bonus/malus 
void validation_3(Tchaine chaine);                                                      //vérifie que la quantite de bonus/malus ne depasse pas le nombre de pions dans la tour
int  validation_4(Tchaine chaine, int *modifA, int *modifB);                            //vérifie que le nombre de pions soit inférieur ou égal à 48 et que le nombre de piles égal 48
char def_trait(Tchaine fen);                                                            //retourne le numéro équivalent au trait
void def_bonus_malus(Tchaine fen, int *bonusJ, int *malusJ, int *bonusR, int *malusR);  //définit les positions des bonus/malus
void Nommer_fichier(Tfichier nom);                                                      //demande du nom du fichier de sortie
void Lire_description(Tdescription notes);                                              //lit la description
void formater_description(Tdescription notes);                                          //formate la description
void ajouter_caractere(Tdescription notes, char caractere, int indice);                 //permet d'ajouter un caractère dans une chaine
void create_Tab(Tchaine chaine, Tchaine Tabnb, Tchaine Tabcouleur);                     //créer les tableaux Tabnb et Tabcouleur
void create_Json(Tformat Tabnb,Tformat Tabcouleur,char trait,char description[],char numdiag[], Tchaine numfen, int bonusJ, int bonusR, int malusJ, int malusR, char *Nomfichier);
//---------------------------------------------------------------------------------------------------------------------------------
// DEBUT DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //-----------------------------------------------------------------------------------------------------------------------------
    // DECLARATIONS ET INITIALISATION
    Tchaine fen;                //tableau de type Tchaine contenant la chaine fen
    Tformat Tabnb;              //tableau de type Tformat contenant le nombre de pions sur chaque case
    Tformat Tabcouleur;         //tableau de type Tformat contenant la couleur de la pile sur chaque case
    Tdiag Ndiag;                //stocke le numero diagramme
    Tfichier Nomfichier;        //stocke le nom de fichier
    Tdescription Description;   //stocke la description
    char trait;                 //stocke le trait
   	int nbespace = 0;           //stocke le nombre de caractere ' ' dans la chaine
    int len_argv2;              //stocke la longueur du 2eme argument (fen)
    int bonusJ=-1;              //stocke la position du bonus jaune
    int malusJ=-1;              //stocke la position du malus jaune
    int bonusR=-1;              //stocke la position du bonus rouge
    int malusR=-1;              //stocke la position du malus rouge     
    //----------------------------------------------------------------------------------------------------------------------------- 
    
    if( !(len_argv2 = lire_arguments(Ndiag, argc, argv)) )  
        return 0;
    
    nbespace = lire_fen(fen, argv[2], len_argv2);    

    formater_fen(fen, nbespace, argv[2]);
    
    trait = def_trait(fen);

    def_bonus_malus(fen, &bonusJ, &malusJ, &bonusR, &malusR);

    Nommer_fichier(Nomfichier);
    
    Lire_description(Description);      
    
    create_Tab(fen, Tabnb, Tabcouleur); 
    
    create_Json(Tabnb, Tabcouleur, trait, Description, Ndiag, fen, bonusJ, bonusR, malusJ, malusR, Nomfichier); 
}
//---------------------------------------------------------------------------------------------------------------------------------
// FIN DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
// FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
int lire_arguments(Tdiag Ndiag, int argc, char *argv[])
{
    int len;

    if (argc > 4 || argc < 3) 
    {
        fprintf(stderr, "\n\033[31m* Erreur : la synthaxe attendue est : diag.exe <numero_de_diagramme> \"<position_type_FEN>\"");
        fprintf(stderr, "\n                                 ou : diag.exe <numero_de_diagramme> \"<position_type_FEN>\" < <nom_fichier> cat <nom_fichier>\033[0m\n\n");
        return 0;
    }
    
    strcpy(Ndiag, argv[1]);
    len = strlen(argv[2])+1;

    printf("\n");

    verif_Ndiag(Ndiag);

    return len;
}

void verif_Ndiag(Tdiag Ndiag)
{
    for(int i=0; i < strlen(Ndiag); i++)
        if( !isdigit(Ndiag[i]) )
        {
            if(DBG)
            {
                printf("\033[35m# Warning : le numero de diagramme est le premier argument et il doit ne doit contenir que des chiffres.\n");
                printf("            un numero a été defini par défaut.\n\n");
            }
            strcpy(Ndiag, NDIAG_DEFAULT);
        }
}

int lire_fen(Tchaine chaine, char *argv2, int nbMAXcaracAsaisir)
{   
    //-----------------------------------------------------------------------------------------------------------------------------
    char car;                   //car est un caractere
    int i=0,j=0,l=0,nbesp=0;    //compteurs
    //-----------------------------------------------------------------------------------------------------------------------------

    do  // boucle qui permet d'avoir au maximum 1 seul caractere ' ' dans la chaine
    {
 	    car=argv2[j++];
	    
	    if (car!='\n')
	    {	
            if(l || j==nbMAXcaracAsaisir)
            {
                l++;
                if( (car=='j') || (car=='r') )
                    chaine[i++]=car;
            }
            else
                //si c'est un caractere valide
                if( (car=='u') || (car=='d') || (car=='t') || (car=='q') || (car=='c') || (car=='U') || (car=='D') || (car=='T') || (car=='Q') || (car=='C') || (car=='0') || (car=='1') || (car=='2') || (car=='3') || (car=='4') || (car=='5') || (car=='6') || (car=='7') || (car=='8') || (car=='9') || (car=='0') || (car=='b') || (car=='B') || (car=='m') || (car=='M') || (car==' ') )
                    if(car!=' ')  // ce n'est pas un espace
                        chaine[i++]=car;
                    else
                    {
                        if (nbesp==0)	
                        {
                            chaine[i++]=car;
                            l++;
                        }
                        nbesp++;
                    }
	    }
    }
    while(car!='\0' && i<nbMAXcaracAsaisir && l<2);

    // le caractere \n est rangé dans lachaine !!
    chaine[i]='\0';

    if(strcmp(chaine, argv2))
    {
        if(DBG)
        {
            printf("\033[35m# Warning : Des caractères invalides ont étés saisis.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }

    supprimer_zero(chaine);

    return nbesp;
}

void supprimer_zero(Tchaine chaine)
{
    //-----------------------------------------------------------------------------------------------------------------------------
    char car;       //car est un caractere
    int i=0;        //compteurs
    Tchaine copie;  //copie pour vérifier s'il y a eu des modifs
    //-----------------------------------------------------------------------------------------------------------------------------
    
    strcpy(copie, chaine);

    do  // boucle qui permet d'avoir au maximum 1 seul caractere ' ' dans la chaine
    {
 	    car=chaine[i];

	    if(car=='0')
            if(!isdigit(chaine[i-1]))
                supprimer_char(chaine, i--);
        
        i++; 
    }
    while(car!='\0');

    if(strcmp(chaine, copie))
    {
        if(DBG)
        {
            printf("\033[35m# Warning : Suppression des 0 inutiles.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }
}

void supprimer_char(Tchaine chaine, int pos)
{
    // Supprime le caractère à l'index i et décale les autres
    for (int j = pos; chaine[j] != '\0'; j++) {
        chaine[j] = chaine[j + 1];
    }
}

void afficher_chaine(Tchaine chaine)
{ 
    printf("\"");
    for(int i = 0; i < strlen(chaine); i++)
        printf("%c",chaine[i]);
    printf("\".\n\n");
}

void formater_fen(Tchaine chaine, int nbesp, Tchaine copie)
{
    int modificationA = 0;
    int modificationB = 0;
    //si les 4 validations retournent 1 la fonction format retournera aussi 1 !
    validation_1(chaine, nbesp);
    validation_2(chaine);
    validation_3(chaine);
    while(!validation_4(chaine, &modificationA, &modificationB)); 
    if(!strcmp(copie, chaine))
    {
        printf("\033[32m- Format valide, voici la chaine fen : ");
        afficher_chaine(chaine);
    }
}

int rechercher_esp(Tchaine chaine)
{
    for(int i=0; i < strlen(chaine); i++)
        if(chaine[i]==' ')
            return i;
    return 0;
}

void validation_1(Tchaine chaine, int nbesp)
{   
    int indice_esp;     //indice_esp est la valeur de l'indice ou se trouve le caractere ' '

    if(nbesp >= 1)  //si la chaine contient 1 espace
    {
        indice_esp = rechercher_esp(chaine);    //on recherche l'indice du caractere ' ' dans la chaine

        //si le caractere ' ' se situe en avant derniere position 
        if( strlen(chaine) - 2 == indice_esp )
        {   //et que le dernier caractere est 'r' ou 'j' -> validation numero 1
            if( !(chaine[indice_esp + 1] == 'r' || chaine[indice_esp + 1] == 'j') )
            {
                chaine[indice_esp + 1] = 'j';

                if(DBG)
                {
                    printf("\033[35m# Warning : Le trait a mal été défini, le trait jaune est appliqué par défaut.\n");
                    printf("            Après modification, fen : ");
                    afficher_chaine(chaine);
                }
            }
        }
        else
        {
            chaine[indice_esp + 1] = 'j';

            if(DBG)
            {
                printf("\033[35m# Warning : Le trait n'a pas été défini après l'espace, le trait jaune est appliqué par défaut.\n");
                printf("            Après modification, fen : ");
                afficher_chaine(chaine);
            }
        }
    }
    //si la chaine ne contient pas d'espace -> la chaine est modifiee puis validee
    else
    {
        if(chaine[strlen(chaine)-1] == 'j' || chaine[strlen(chaine)-1] == 'r')
        {
            chaine[strlen(chaine)] = chaine[strlen(chaine)-1];
            chaine[strlen(chaine)-2] = ' ';
            chaine[strlen(chaine)] = '\0';
            
            if(DBG)
            {
                printf("\033[35m# Warning : Le trait a été défini sans l'espace devant.\n");
                printf("            Après modification, fen : ");
                afficher_chaine(chaine);
            }
        }
        else
        {
            chaine[strlen(chaine)] = ' ';
            chaine[strlen(chaine)] = 'j';
            if(chaine[strlen(chaine)-1] != 'j')
                supprimer_char(chaine, strlen(chaine)-1);
            chaine[strlen(chaine)] = '\0';

            if(DBG)
            {
                printf("\033[35m# Warning : Le trait n'a pas été défini, le trait jaune est appliqué par défaut.\n");
                printf("            Après modification, fen : ");
                afficher_chaine(chaine);
            }
        }
    }
}

void validation_2(Tchaine chaine)
{
    Tchaine copie;
    int b=0, B=0, m=0, M=0; //compteur pour chaque bonus/malus

    strcpy(copie, chaine);

    for(int i=1; i < strlen(chaine) - 2; i++)   //ajoute 1 au compteur associé
    {
        if(chaine[i] == 'b')
            if(b)
                supprimer_char(chaine, i--);
            else
                b++;
        if(chaine[i] == 'B')
            if(B)
                supprimer_char(chaine, i--);
            else
                B++;
        if(chaine[i] == 'm')
            if(m)
                supprimer_char(chaine, i--);
            else
                m++;
        if(chaine[i] == 'M')
            if(M)
                supprimer_char(chaine, i--);
            else
                M++;
    }

    if((strcmp(copie, chaine)))
    {
        if(DBG)
        {
            printf("\033[35m# Warning : Un même bonus et/ou malus est présent plusieurs fois.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }
}

void validation_3(Tchaine chaine)
{
    int nb_bonus=0; //compteur de bonus/malus present dans une pile

    for(int i = strlen(chaine) - 3; i>=-1; i--)  //lecture des caracteres de droite a gauche
    {
        if( (chaine[i] == 'b') || (chaine[i] == 'B') || (chaine[i] == 'm') || (chaine[i] == 'M') )
            nb_bonus++; //si le caractere est un bonus/malus, on ajoute 1 au compteur
        else
        {
            //si ce n'est pas un bonus/malus 
            //on verifie que le nombre de pions dans la pile soit superieur au nombre de bonus/malus
            if(nb_bonus >= 1)
            {
                //comparaison grâce au switch
                //si nb_bonus est plus grand que le nombre de pion dans la pile alors la chaine n'est pas validée
                switch(chaine[i])
                {
                    case 'u' :
                    case 'U' : 
                        if(nb_bonus > 1)
                        {
                            for(int j=i+nb_bonus; j > i+1; j--)
                                supprimer_char(chaine, j);
                            
                            if(DBG)
                            {
                                printf("\033[35m# Warning : Plusieurs bonus/malus attribues sur un pion seul, suppression des bonus/malus en trop.\n");
                                printf("            Après modification, fen : ");
                                afficher_chaine(chaine);
                            }
                        }
                        break;
                    
                    case 'd' :
                    case 'D' :
                        if(nb_bonus > 2)
                        {
                            for(int j=i+nb_bonus; j > i+2; j--)
                                supprimer_char(chaine, j);
                            
                            if(DBG)
                            {
                                printf("\033[35m# Warning : Trop de malus/bonus attribues sur une pile de 2 pions, suppression des bonus/malus en trop.\n");
                                printf("            Après modification, fen : ");
                                afficher_chaine(chaine);
                            }
                        }
                        break;

                    case 't' :
                    case 'T' :
                        if(nb_bonus > 3)
                        {
                            for(int j=i+nb_bonus; j > i+3; j--)
                                supprimer_char(chaine, j);
                            
                            if(DBG)
                            {
                                printf("\033[35m# Warning : Trop de malus/bonus attribues sur une pile de 3 pions, suppression des bonus/malus en trop.\n");
                                printf("            Après modification, fen : ");
                                afficher_chaine(chaine);
                            }
                        }
                        break;

                    case 'q' :
                    case 'Q' :
                    case 'c' :
                    case 'C' :
                        break;
                    
                    default :
                        for(int j=i+nb_bonus; j > i; j--)
                            supprimer_char(chaine, j);
                        
                        if(DBG)
                        {
                            printf("\033[35m# Warning : Des bonus/malus sont attribués sur une pile vide, suppression des bonus/malus.\n");
                            printf("            Après modification, fen : ");
                            afficher_chaine(chaine);
                        }
                        break;
                }
            }
            nb_bonus = 0;
        }
    }
}

int validation_4(Tchaine chaine, int *modifA, int *modifB)
{
    //-----------------------------------------------------------------------------------------------------------------------------
    // declarations et initialisations
    int Tabpion[MAXCAR] = {0};  //Tabpion est un tableau dans lequel est indique le nombre de pions pour chaque caractere de la chaine fen 
    int Tabpile[MAXCAR] = {0};  //Tabpile est un tableau dans lequel est indique le nombre de piles pour chaque caractere de la chaine fen 
    int nb_element = 0;         //est le nombre d'elements dans Tabpion et Tabpile
    int somme = 0;              //est la somme des elements de Tabpion (qui doit etre egale a 48)
    int aux;                    //variable temporaire pour permettre de modifier la chaine fen lorsqu'elle est trop courte
    int dizaine;                //variable utilisee lors de la modification de la chaine
    int unite;                  //idem
    int indice_esp;             //indice du caractere ' '
    int j;
    int pos = strlen(chaine)-3;
    //-----------------------------------------------------------------------------------------------------------------------------
    
    for(int i=0; i < strlen(chaine) - 2; i++) // pour tous les caracteres sauf les 2 derniers qui ont deja ete verifies
    {
        //si le caractere est un chiffre
        if( (chaine[i]=='0') || (chaine[i]=='1') || (chaine[i]=='2') || (chaine[i]=='3') || (chaine[i]=='4') || (chaine[i]=='5') || (chaine[i]=='6') || (chaine[i]=='7') || (chaine[i]=='8') || (chaine[i]=='9') )
        {
            //et que le suivant l'est aussi
            if( (chaine[i + 1]=='0') || (chaine[i + 1]=='1') || (chaine[i + 1]=='2') || (chaine[i + 1]=='3') || (chaine[i + 1]=='4') || (chaine[i + 1]=='5') || (chaine[i + 1]=='6') || (chaine[i + 1]=='7') || (chaine[i + 1]=='8') || (chaine[i + 1]=='9') )
            {
                //si il y a 3 chiffres a la suite -> la chaine n'est pas validee 
                if( (chaine[i + 2]=='0') || (chaine[i + 2]=='1') || (chaine[i + 2]=='2') || (chaine[i + 2]=='3') || (chaine[i + 2]=='4') || (chaine[i + 2]=='5') || (chaine[i + 2]=='6') || (chaine[i + 2]=='7') || (chaine[i + 2]=='8') || (chaine[i + 2]=='9') )
                {
                    j = i + 2;
                    while(isdigit(chaine[j]))
                        supprimer_char(chaine, j);
                    
                    if(DBG)
                    {
                        printf("\033[35m# Warning : Un nombre de cases vides est beaucoup trop élevé, seuls les 2 premiers chiffres du nombre seront pris.\n");
                        printf("            Après modification, fen : ");
                        afficher_chaine(chaine);
                    }
                }
                //il y a 2 chiffres a la suite, il faut regrouper les 2 pour n'en former qu'1
                //on prend le premier chiffre pour celui des dizaines et le suivant pour celui des unites afin de stocker le resultat dans Tabpion et Tabpile
                Tabpion[i] = 0;   
                Tabpion[i + 1] = 0;
                Tabpile[i] = (chaine[i] - 48) * 10 + chaine[i + 1] - 48;  // on fait -48 a cause du code ascii du caractere
                Tabpile[i + 1] = 0;
                i++;
                nb_element += 2;
            }
            //il n'y a qu'un seul chiffre
            else
            {
                //on le stocke dans Tabpion et Tabpile
                Tabpion[i] = 0;
                Tabpile[i] = chaine[i] - 48;
                nb_element++;
            }
        } 
        else
        {
            //le caractere est un bonus/malus
            if((chaine[i] == 'b') || (chaine[i] == 'B') || (chaine[i] == 'm') || (chaine[i] == 'M'))
            {
                //on stocke 0 dans Tabpion et Tabpile
                Tabpion[i] = 0;
                Tabpile[i] = 0;
                nb_element++;
            }
            else //le caractere est une lette
            {
                //on stocke 1 dans Tabpile
                Tabpile[i] = 1;
                //on stocke sa valeur dans Tabpion
                switch(chaine[i])
                {
                    case 'u' :
                    case 'U' :
                        Tabpion[i] = 1;
                        nb_element++;
                        break;
                    
                    case 'd' :
                    case 'D' :
                        Tabpion[i] = 2;
                        nb_element++;
                        break;
                    
                    case 't' :
                    case 'T' :
                        Tabpion[i] = 3;
                        nb_element++;
                        break;
                    
                    case 'q' :
                    case 'Q' :
                        Tabpion[i] = 4;
                        nb_element++;
                        break;
                    
                    case 'c' :
                    case 'C' :
                        Tabpion[i] = 5;
                        nb_element++;
                        break;
                }
            }
        }
    }

    for(int i=0; i < nb_element; i++)   //calcul de la sommme des elements de Tabpion
        somme += Tabpion[i];
    
    if(somme > 48) 
    {
        if(isdigit(chaine[pos]) || (chaine[pos]=='b' || chaine[pos]=='B' || chaine[pos]=='m' || chaine[pos]=='M'))
        {
            supprimer_char(chaine, pos);
            pos--;
        }
        else
        {
            switch(chaine[pos])
            {
                case 'u' :
                case 'U' :
                    chaine[pos]='0';
                    break;
                
                case 'd' :
                    chaine[pos]='u';
                    break;

                case 'D' :
                    chaine[pos]='U';
                    break;

                case 't' :
                    chaine[pos]='d';
                    break;

                case 'T' :
                    chaine[pos]='D';
                    break;

                case 'q' :
                    chaine[pos]='t';
                    break;

                case 'Q' :
                    chaine[pos]='T';
                    break;

                case 'c' :
                    chaine[pos]='q';
                    break;

                case 'C' :
                    chaine[pos]='Q';
                    break;
            }
        }
        *modifA=1;
        return 0;
    }
    
    if(*modifA)
    {
        if(DBG)
        {
            printf("\033[35m# Warning : Le nombre de pions est trop grand. Rappel : Le nombre de pions doit être inférieur ou égal à 48.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }

    somme=0;

    for(int i=0; i < nb_element; i++)   //calcul de la sommme des elements de Tabpile
        somme += Tabpile[i];

    if(somme > 48) //si somme > 48 alors la chaine n'est pas validee
    {
        supprimer_char(chaine, strlen(chaine)-3);
        *modifB=1;
        return 0;
    }
    
    if(*modifB)
    {
        if(DBG)
        {
            printf("\033[35m# Warning : Le nombre de piles est trop grand. Rappel : Le nombre de piles doit être égal à 48.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }

    if(somme < 48)    //si somme < 48 alors la modification de la chaine commence...
    {
        indice_esp = rechercher_esp(chaine); //tout d'abord on recherche la position du caractere ' '

        //on verifie si le dernier caractere avant l'espace est un chiffre
        if( (chaine[indice_esp - 1] == '0') || (chaine[indice_esp - 1] =='1') || (chaine[indice_esp - 1] == '2') || (chaine[indice_esp - 1] == '3') || (chaine[indice_esp - 1] == '4') || (chaine[indice_esp - 1] == '5') || (chaine[indice_esp - 1] == '6') || (chaine[indice_esp - 1] == '7') || (chaine[indice_esp - 1] == '8') || (chaine[indice_esp - 1] == '9') )
        {
            //on verifie si le caractere encore avant est aussi un chiffre
            if( (chaine[indice_esp - 2] == '0') || (chaine[indice_esp - 2] =='1') || (chaine[indice_esp - 2] == '2') || (chaine[indice_esp - 2] == '3') || (chaine[indice_esp - 2] == '4') || (chaine[indice_esp - 2] == '5') || (chaine[indice_esp - 2] == '6') || (chaine[indice_esp - 2] == '7') || (chaine[indice_esp - 2] == '8') || (chaine[indice_esp - 2] == '9') )
            {
                //dans ce cas il y a 2 chiffres, on verifie le nombre manquant de pions
                //la division par 10 du nombre manquant de pions va nous donner le chiffre des dizaines a remplacer dans la chaine
                //le modulo par 10 nous donne le reste, donc le chiffre des unites
                //ATTETION A NE PAS CONFONDRE LE 48 POUR LE NOMBRE DE PIONS ET LE 48 POUR LA CONVERTION DU AU CODE ASCII !!!
                dizaine = (48 - somme + (chaine[indice_esp - 2] - 48 )*10 + chaine[indice_esp - 1] - 48) / 10;
                unite = (48 - somme + (chaine[indice_esp - 2] - 48 )*10 + chaine[indice_esp - 1] - 48) % 10;

                //on remplace par la valeur de pions restants
                chaine[indice_esp - 2] = dizaine + 48;
                chaine[indice_esp - 1] = unite + 48;
            }
            //il n'y a qu'un seul chiffre
            else
            {
                dizaine = (48 - somme + chaine[indice_esp - 1]- 48) / 10;
                unite = (48 - somme + chaine[indice_esp - 1] - 48) % 10;

                //on remplace par la valeur de pions restants
                //on stocke le dernier caractere dans une variable aux car la longeur de la chaine va etre modifiee
                //puis on rajoute le caractere ' ' ainsi que celui qui a ete stocke
                aux = chaine[indice_esp + 1]; 
                chaine[indice_esp - 1] = dizaine + 48;
                chaine[indice_esp] = unite + 48;
                chaine[indice_esp + 1] = ' ';
                chaine[indice_esp + 2] = aux;
                chaine[indice_esp + 3] = '\0';  
            }
        }
        //le dernier caractere avant l'espace n'est pas un chiffre
        else
        {
            //si le nombre restants de pions est superieur a 10, il faut rajouter 2 chiffres dans la chaine
            if(48 - somme >= 10)
            {
                dizaine = (48 - somme) / 10;
                unite = (48 - somme) % 10;

                aux = chaine[indice_esp + 1];
                chaine[indice_esp] = dizaine + 48;
                chaine[indice_esp + 1] = unite + 48;
                chaine[indice_esp + 2] = ' ';
                chaine[indice_esp + 3] = aux;
                chaine[indice_esp + 4] = '\0';
            }
            //si le nombre de pions est inferieur a 10 alors on peut ajouter un seul chiffre
            else
            {
                aux = chaine[indice_esp + 1];
                chaine[indice_esp] = 48 - somme + 48;
                chaine[indice_esp + 1] = ' ';
                chaine[indice_esp + 2] = aux;
                chaine[indice_esp + 3] = '\0';
            }  
        }
        
        if(DBG)
        {
            printf("\033[35m# Warning : Le nombre de piles est trop petit. Rappel : Le nombre de piles doit être égal à 48.\n");
            printf("            Après modification, fen : ");
            afficher_chaine(chaine);
        }
    }
    supprimer_zero(chaine);
    if(somme == 48)     //si somme vaut 48 -> validation numero 4 !
        return 1;
}

char def_trait(Tchaine fen)
{
    char trait = fen[strlen(fen) - 1];

    if(trait == 'j')
        trait = '1';
    else
        trait = '2';

    return trait;
}

void def_bonus_malus(Tchaine fen, int *bonusJ, int *malusJ, int *bonusR, int *malusR)
{
    int nb_piles = 0;   //stocke le nombre de piles
    
    for(int i=0; i < strlen(fen) - 2; i++) // pour tous les caracteres sauf les 2 derniers qui ont deja ete verifies
    {
        if(isdigit(fen[i]))
        {
            //et que le suivant l'est aussi
            //on ajoute le nombre au nombre de pile
            if(isdigit(fen[i+1]))
            {
                nb_piles += (fen[i] - 48) * 10 + fen[i + 1] - 48;
                i++;
            }
            //il n'y a qu'un seul chiffre
            else
            {
                //on ajoute le chiffre au nombre de pile
                nb_piles += fen[i] - 48;
            }
        } 
        else
        {
            //si le caractere est un bonus/malus
            if((fen[i] == 'b') || (fen[i] == 'B') || (fen[i] == 'm') || (fen[i] == 'M'))
            {
                switch(fen[i])
                {
                    case 'b' :
                        *bonusJ = nb_piles-1;
                        break;
                
                    case 'B' :
                        *bonusR = nb_piles-1;
                        break;

                    case 'm' :
                        *malusJ = nb_piles-1;
                        break;
                
                    case 'M' :
                        *malusR = nb_piles-1;
                        break;
                }
            }
            else
            {
                nb_piles++;
            }
        }
    }
}

void Nommer_fichier(Tfichier nom)
{
    printf("\033[0m- Entrez le nom du fichier de sortie (répertoire courant : Diag).\n");
    printf("\033[0m- Ne rien entrer pour nommer le fichier par défaut (\"../web/exemples/diag_test.js\") :\n->");
    fgets(nom,50,stdin);
    nom[strlen(nom)-1] = '\0';
    
    if(!strcmp(nom,""))
        strcpy(nom, NOM_PAR_DEFAUT);

    printf("\n");
    
    if(DBG)
    {
        printf("\033[94m. Debug : Nom du fichier : ");
        afficher_chaine(nom);
    }
}

void Lire_description(Tdescription notes)
{
    char lignelu[TAILLE_DESCRIPTION];
    char car;
    int i=0;

    strcpy(notes, " ");
    printf("\033[0m- Entrez une description de moins de %d caractères, puis validez avec Ctrl+D :\n", TAILLE_DESCRIPTION);
    
    while(fgets(lignelu,1000,stdin) != NULL)
        strcat(notes, lignelu);

    if(notes[strlen(notes)-1] == '\n')
        notes[strlen(notes)-1] = '\0';
    
    supprimer_char(notes, 0);

    if(DBG)
    {
        printf("\n\033[94m. Debug : Description lue :\n");
        afficher_chaine(notes);
    }
    
    formater_description(notes);
}

void formater_description(Tdescription notes)
{
    //-----------------------------------------------------------------------------------------------------------------------------
    char car;       //car est un caractere
    int i=0;        //compteur
    //-----------------------------------------------------------------------------------------------------------------------------
    
    do  // boucle qui permet d'avoir au maximum 1 seul caractere ' ' dans la chaine
    {
 	    car=notes[i];

	    if(car=='\n')
        {
            supprimer_char(notes, i);
            ajouter_caractere(notes, '<', i);
            i++;
            ajouter_caractere(notes, 'b', i);
            i++;
            ajouter_caractere(notes, 'r', i);
            i++;
            ajouter_caractere(notes, '>', i);
        }
        
        i++; 
    }
    while(car!='\0');
}

void ajouter_caractere(Tdescription chaine, char caractere, int indice) 
{
    int longueur_chaine = strlen(chaine);
    int i;
    
    // Décale les caractères de la chaîne à partir de l'indice spécifié
    for (i = longueur_chaine; i >= indice; i--) {
        chaine[i+1] = chaine[i];
    }
    
    // Ajoute le caractère à la position spécifiée
    chaine[indice] = caractere;
}

void create_Tab(Tchaine chaine, Tchaine Tabnb, Tchaine Tabcouleur)
{
    int j=0, l=0;

    for(int i=0; i < strlen(chaine); i++)
    {
        if( (chaine[i]=='0') || (chaine[i]=='1') || (chaine[i]=='2') || (chaine[i]=='3') || (chaine[i]=='4') || (chaine[i]=='5') || (chaine[i]=='6') || (chaine[i]=='7') || (chaine[i]=='8') || (chaine[i]=='9') )
        {
            //et que le suivant l'est aussi
            if( (chaine[i + 1]=='0') || (chaine[i + 1]=='1') || (chaine[i + 1]=='2') || (chaine[i + 1]=='3') || (chaine[i + 1]=='4') || (chaine[i + 1]=='5') || (chaine[i + 1]=='6') || (chaine[i + 1]=='7') || (chaine[i + 1]=='8') || (chaine[i + 1]=='9') )
            {
                //on prend le premier chiffre pour celui des dizaines et le suivant pour celui des unites afin de stocker le resultat dans Tab48
                j = (chaine[i] - 48) * 10 + chaine[i + 1] - 48;  // on fait -48 a cause du code ascii du caractere 

                for(int k=0; k < j; k++)
                {
                    Tabnb[l] = 0;
                    Tabcouleur[l] = 0;
                    l++;
                }

                i++;
            }
            //il n'y a qu'un seul chiffre
            else
            {
                j = chaine[i] - 48 ;

                for(int k=0; k < j; k++)
                {
                    Tabnb[l] = 0;
                    Tabcouleur[l] = 0;
                    l++;
                }
            }
        }
        else
        {
            switch(chaine[i])
            {
                case 'u' :
                    Tabnb[l] = 1;
                    Tabcouleur[l] = 1;
                    l++;
                    break;
                
                case 'U' :
                    Tabnb[l] = 1;
                    Tabcouleur[l] = 2;
                    l++;
                    break;
                
                case 'd' :
                    Tabnb[l] = 2;
                    Tabcouleur[l] = 1;
                    l++;
                    break;
                
                case 'D' :
                    Tabnb[l] = 2;
                    Tabcouleur[l] = 2;
                    l++;
                    break;

                case 't' :
                    Tabnb[l] = 3;
                    Tabcouleur[l] = 1;
                    l++;
                    break;
                
                case 'T' :
                    Tabnb[l] = 3;
                    Tabcouleur[l] = 2;
                    l++;
                    break;
                
                case 'q' :
                    Tabnb[l] = 4;
                    Tabcouleur[l] = 1;
                    l++;
                    break;
                
                case 'Q' :
                    Tabnb[l] = 4;
                    Tabcouleur[l] = 2;
                    l++;
                    break;

                case 'c' :
                    Tabnb[l] = 5;
                    Tabcouleur[l] = 1;
                    l++;
                    break;
                
                case 'C' :
                    Tabnb[l] = 5;
                    Tabcouleur[l] = 2;
                    l++;
                    break;
            }
        }
    }
}

void create_Json(Tformat Tabnb,Tformat Tabcouleur,char trait,char description[],char numdiag[], Tchaine numfen, int bonusJ, int bonusR, int malusJ, int malusR, char *Nomfichier)
{
	FILE* json=fopen(Nomfichier, "w"); // on ouvre le fichier avec w+ pour supprimer le contenu au préalable et récrire à chaque fois, utile car on veut générer un fichier à chaque coup
	
    if(DBG)
        printf("\033[94m. Debug : Appel de la fonction json export réussi !\n");
	
    if(json != NULL)
    {
		// tous les STR_.. sont dans avalam.h je n'ai fais que suivre ce que le prof a déjà écrit pour la structure json, ca évite d'écrire par exemple \"nb\" on met juste STR_NB avec un %s dans le fprintf et c'est réglé
		fprintf(json,"traiterJson({\n\"%s\":%d,","trait",trait-48);
		fprintf(json,"\n\"numDiag\":%s,",numdiag);
		fprintf(json,"\n\"notes\":\"%s\",",description);
		fprintf(json,"\n\"fen\":\"%s\",", numfen);
        fprintf(json,"\n\"bonusJ\":%d,", bonusJ);
        fprintf(json,"\n\"malusJ\":%d,", malusJ);
        fprintf(json,"\n\"bonusR\":%d,", bonusR);
        fprintf(json,"\n\"malusR\":%d,", malusR);
        fprintf(json,"\n%s:[",STR_COLS);
		
        for (int i = 0; i < NBCASES; ++i)
			fprintf(json,"\n\t{%s:%d, %s:%d},",STR_NB,Tabnb[i],STR_COULEUR,Tabcouleur[i]);//boucle des positions
		
        fprintf(json,"\n]\n});");
	    fclose(json);
        
        if(DBG) 
	        printf("\n\033[94m. Debug : Export Json terminé (chemin de fichier : \"%s\") ! \n", Nomfichier);
        
	}
	else
        fprintf(stderr, "\n\033[31m* Erreur : Ouverture du fichier Json impossible !\033[0m\n");
    
    printf("\n");
}