//---------------------------------------------------------------------------------------------------------------------------------
//Auteurs :  Noé Mejdoub / Mathieu Retourné / Mathis Pollet--Dassonval / Geoffrey Ramos
//---------------------------------------------------------------------------------------------------------------------------------
// BIBLIOTHEQUES
//---------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "avalam.h"
//---------------------------------------------------------------------------------------------------------------------------------
// MODE DEBUG
//---------------------------------------------------------------------------------------------------------------------------------
#ifdef __DEBUG__
  #define DBG 1
#else
  #define DBG 0
#endif
//---------------------------------------------------------------------------------------------------------------------------------
// PRE PROCESSING
//---------------------------------------------------------------------------------------------------------------------------------
#define MAXCAR 55 // au max a l'initialisation -> 24 u + 24 U + b + B + m + M + ' ' + j + '\0'
#define NOM_PAR_DEFAUT "../web/exemples/diag_test.js"
#define TRAIT_DEFAULT '1'
#define NDIAG_DEFAULT "1"
#define FEN_DEFAULT "UBubUMumUuuUuUuUUuUuUuUuUuUuuUuUuUuUuUuUUuUuUuuUuU j"
#define DESCRIPTION_DEFAULT "Voici la position obtenue grace a la chaine FEN"
#define BJ_DEFAULT 1
#define BR_DEFAULT 0
#define MJ_DEFAULT 3
#define MR_DEFAULT 2
#define TAILLE_DESCRIPTION 1000
//---------------------------------------------------------------------------------------------------------------------------------
typedef char Tchaine[MAXCAR];
typedef char Tformat[MAXCAR - 2];
//---------------------------------------------------------------------------------------------------------------------------------
// PROTOTYPES DES FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
int lire(Tchaine lachaine, char *argv2, int nbMAXcaracAsaisir);     //lit une chaine, supprime les mauvais caracteres et retourne le nombre d'espace dans la chaine
void afficher(Tchaine chaine);                                      //affiche la chaine fen
int format(Tchaine chaine, int nbesp);                              //retourne 1 si la chaine est au bon format
int rechercher_esp(Tchaine chaine);                                 //retourne l'indice du caractere ' ' dans la chaine
int validation_1(Tchaine chaine, int nbesp);                        //verifie les 2 derniers caracteres (espace et r/j)   
int validation_2(Tchaine chaine);                                   //verifie la saisie des bonus/malus 
int validation_3(Tchaine chaine);                                   //verifie que la quantite de bonus ne depasse pas le nombre de pions dans la tour
int validation_4(Tchaine chaine);                                   //verifie que le nombre de pions vaut 48
void create_Tab(Tchaine chaine, Tchaine Tabnb, Tchaine Tabcouleur); //creer les tableaux Tabnb et Tabcouleur
void Json(Tformat Tabnb,Tformat Tabcouleur,char trait,char description[],char numdiag[], Tchaine numfen, int bonusJ, int bonusR, int malusJ, int malusR, char *Nomfichier);
//---------------------------------------------------------------------------------------------------------------------------------
// DEBUT DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //---------------------------------------------------------------------------------------------------------------------------------
    // DECLARATIONS ET INITIALISATION
    Tchaine fen;                            //tableau de type Tchaine contenant la chaine fen
    Tformat Tabnb;                          //tableau de type Tformat contenant le nombre de pions sur chaque case
    Tformat Tabcouleur;                     //tableau de type Tformat contenant la couleur de la pile sur chaque case
    char Ndiag[10];                         //stocke le numero diagramme
    char Nomfichier[50];                    //stocke le nom de fichier
    char Notes[TAILLE_DESCRIPTION];         //stocke la description
    char Lirenotes[TAILLE_DESCRIPTION];     //stocke la description (chaine auxiliaire)
    char trait;                             //stocke le trait
    char choix = 0;                         //stocke la reponse de l'utilisateur 
   	int nbespace = 0;                       //stocke le nombre de caractere ' ' dans la chaine
    int bonusJ, malusJ, bonusR, malusR;     //stockent les positions des bonus/malus
    int nb_piles = 0;                       //stocke le nombre de piles
    int len_argv2;                          //stocke la longueur du 2eme argument (fen)
    int isdigit = 0;                        //variable utilisee pour verifier que le 1er argument (numero diagramme) soit bien un nombre
    //---------------------------------------------------------------------------------------------------------------------------------   
    //LECTECTURE DU FEN ET DU NUMERO DE DIAGRAMME------------------------------------------------------------------------------------

    if (argc > 3 || argc < 2) {
        printf("\n* Erreur : la synthaxe attendue est : diag.exe <numero_de_diagramme> \"<position_type_FEN>\"\n\n");
        return 0;
    }
    strcpy(Ndiag, argv[1]);
    len_argv2 = strlen(argv[2]);
    printf("\n");
    //------------------------------------------------------------------------------------
    // VERIFICTION NUMERO DIAGRAMME------------------------------------------------------------------------------------

    do
    {
        isdigit = 1;

        for(int i=0; i < strlen(Ndiag); i++)
        {
            if( !((Ndiag[i]=='0') || (Ndiag[i]=='1') || (Ndiag[i]=='2') || (Ndiag[i]=='3') || (Ndiag[i]=='4') || (Ndiag[i]=='5') || (Ndiag[i]=='6') || (Ndiag[i]=='7') || (Ndiag[i]=='8') || (Ndiag[i]=='9')) )
            {
                isdigit = 0;
                printf("# Warning : le numero de diagramme est le premier argument et il doit ne doit contenir que des chiffres\n");
                printf("Voulez vous definir un nouveau numero de diagramme ? O/N :\n->");
                scanf("%c",&choix);
                getchar();
                while ((toupper(choix) != 'O') && (toupper(choix) != 'N')){
                    printf("* Réponse incorrect, voulez vous definir un nouveau numero de diagramme ? O/N :\n->");
                    scanf("%c",&choix);
                    getchar();
                }
                if(toupper(choix) == 'O'){
                    printf("- Nom du numero de diagramme :\n->");
                    fgets(Ndiag,10,stdin);
                    printf("\n");
                    Ndiag[strlen(Ndiag)-1] = '\0';
                } 
                else
                {
                    strcpy(Ndiag, NDIAG_DEFAULT);
                    printf("\n# Warning : un numero a ete defini par defaut\n");
                }
            } 
        }
    }
    while(isdigit == 0);   
    //------------------------------------------------------------------------------------
    //VERIFICATION FEN
   
    nbespace = lire(fen, argv[2], len_argv2);   //lit la chaine et retourne le nombre d'espace dans la chaine 

    if(format(fen, nbespace))       //verification du format de la chaine
    {
        //la chaine est valide
        printf("- Format valide, voici la chaine fen : ");
        afficher(fen);

        trait = fen[strlen(fen) - 1];

        if(trait == 'j')
            trait = '1';
        else
            trait = '2';

        for(int i=0; i < strlen(fen) - 2; i++) // pour tous les caracteres sauf les 2 derniers qui ont deja ete verifies
        {
            if( (fen[i]=='0') || (fen[i]=='1') || (fen[i]=='2') || (fen[i]=='3') || (fen[i]=='4') || (fen[i]=='5') || (fen[i]=='6') || (fen[i]=='7') || (fen[i]=='8') || (fen[i]=='9') )
            {
                //et que le suivant l'est aussi
                //on ajoute le nombre au nombre de pile
                if( (fen[i + 1]=='0') || (fen[i + 1]=='1') || (fen[i + 1]=='2') || (fen[i + 1]=='3') || (fen[i + 1]=='4') || (fen[i + 1]=='5') || (fen[i + 1]=='6') || (fen[i + 1]=='7') || (fen[i + 1]=='8') || (fen[i + 1]=='9') )
                {
                    nb_piles += (fen[i] - 48) * 10 + fen[i + 1] - 48;
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
                            bonusJ = nb_piles;
                            break;
                    
                        case 'B' :
                            bonusR = nb_piles;
                            break;

                        case 'm' :
                            malusJ = nb_piles;
                            break;
                    
                        case 'M' :
                            malusR = nb_piles;
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
    else
    {
        printf("--> La chaine FEN est donc celle par defaut <--\n");
        trait = TRAIT_DEFAULT;
        strcpy(fen,FEN_DEFAULT);
        bonusJ = BJ_DEFAULT;
        bonusR = BR_DEFAULT;
        malusJ = MJ_DEFAULT;
        malusR = MR_DEFAULT;
    }
    //------------------------------------------------------------------------------------
    // DEMANDE DU NOM DU FICHIER---------------------------------------------------------------------------------------------------------------------------------

    printf("\n- Voulez vous nommer le fichier de sortie ? O/N :\n->");
    scanf("%c",&choix);
    getchar();
    while ((toupper(choix) != 'O') && (toupper(choix) != 'N')){
        printf("* Réponse incorrect, voulez vous nommer le fichier de sortie ? O/N :\n->");
        scanf("%c",&choix);
        getchar();
    }
    if(toupper(choix) == 'O'){
        printf("- Nom du fichier :\n->");
        fgets(Nomfichier,50,stdin);
        Nomfichier[strlen(Nomfichier)-1] = '\0';
    } else 
    strcpy(Nomfichier, NOM_PAR_DEFAUT);
    //---------------------------------------------------------------------------------------------------------------------------------
    // DEMANDE DE LA DESCRIPTION---------------------------------------------------------------------------------------------------------------------------------
    printf("\n- Entrez une description de moins de %d caracteres :\n->", TAILLE_DESCRIPTION);
    if(strlen(fgets(Lirenotes, TAILLE_DESCRIPTION, stdin)) > 1){
    	printf("\n. Debug : Chaine lu : %s", Lirenotes);
    	strcpy(Notes, Lirenotes);
        Notes[strlen(Notes)-1] = '\0';   
    }else {
 		printf("\n# Warning : Aucune description entree, une description par defaut a ete definie\n");
		strcpy(Notes, DESCRIPTION_DEFAULT);
	}
    create_Tab(fen, Tabnb, Tabcouleur);
    Json(Tabnb, Tabcouleur, trait, Notes, Ndiag, fen, bonusJ, bonusR, malusJ, malusR, Nomfichier);

//---------------------------------------------------------------------------------------------------------------------------------
// FIN DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
}
//---------------------------------------------------------------------------------------------------------------------------------
// FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
int lire(Tchaine lachaine, char *argv2, int nbMAXcaracAsaisir)
{   //---------------------------------------------------------------------------------------------------------------------------------
    char car;               //car est un caractere
    char der = '\0';        //dernier caractere (le trait)
    int i=0,j=0,l=0,nbesp=0;    //i est un indice et nbesp est le nombre de caractere espace
    //---------------------------------------------------------------------------------------------------------------------------------
    //si le trait est bon, on le stocke
    if( (argv2[nbMAXcaracAsaisir-1] == 'j') || (argv2[nbMAXcaracAsaisir-1] == 'r') )
        der = argv2[nbMAXcaracAsaisir-1];

    do  // boucle qui permet d'avoir au maximum 1 seul caractere ' ' dans la chaine
    {
 	    car=argv2[j];
        j++;
	    
        if(l)
            l++;
	    if (car!='\n')
	    {	
            //si c'est un caractere valide
            if( (car=='u') || (car=='d') || (car=='t') || (car=='q') || (car=='c') || (car=='U') || (car=='D') || (car=='T') || (car=='Q') || (car=='C') || (car=='0') || (car=='1') || (car=='2') || (car=='3') || (car=='4') || (car=='5') || (car=='6') || (car=='7') || (car=='8') || (car=='9') || (car=='0') || (car=='b') || (car=='B') || (car=='m') || (car=='M') || (car==' ') )
            {
                if (car!=' ' ) 
                    // ce n'est pas un espace
	    		    lachaine[i++]=car;
	     	    else
	    	    { // c'est un espace
	    	        if (i!=0 && nbesp==0)	
	    	            {
                            lachaine[i++]=car;
                            nbesp++;
                            l++;
                        }
	            }
            }
	    }
    }
    while(car!='\0' && i<nbMAXcaracAsaisir && l<2);

    if(der!='\0')
        lachaine[(i++)-1]=der;
    else
        lachaine[i++]=der;
    // le caractere \n est rangé dans lachaine !!
    lachaine[i]='\0';

    return nbesp;
}

void afficher(Tchaine chaine)
{ 
    for(int i = 0; i < strlen(chaine); i++)
        printf("%c",chaine[i]);

    printf("\n");
}

int format(Tchaine chaine, int nbesp)
{
    //si les 4 validations retournent 1 la fonction format retournera aussi 1 !
    if( ( !validation_1(chaine, nbesp) ) || ( !validation_2(chaine) ) )
        return 0;
    else
        if( !validation_3(chaine) )
            return 0;
        else
        {
            if( !validation_4(chaine) )
                return 0;
            else
            {
                printf("- Apres ajustement,\n");
                return 1;
            }
        }
}

int rechercher_esp(Tchaine chaine)
{
    for(int i=0; i < strlen(chaine); i++)
        if(chaine[i]==' ')
            return i;
    return 0;
}

int validation_1(Tchaine chaine, int nbesp)
{   
    int indice_esp;     //indice_esp est la valeur de l'indice ou se trouve le caractere ' '

    if(nbesp == 1)  //si la chaine contient 1 espace
    {
        indice_esp = rechercher_esp(chaine);    //on recherche l'indice du caractere ' ' dans la chaine

        //si le caractere ' ' se situe en avant derniere position 
        if( strlen(chaine) - 2 == indice_esp )
        {   //et que le dernier caractere est 'r' ou 'j' -> validation numero 1
            if( (chaine[indice_esp + 1] == 'r') || (chaine[indice_esp + 1] == 'j') )
                return 1;
            else
                chaine[indice_esp + 1] = 'j';
        }
        else
            chaine[indice_esp + 1] = 'j';
    }
    //si la chaine ne contient pas d'espace -> la chaine est modifiee puis validee
    else
    {
        chaine[strlen(chaine)] = ' ';
        chaine[strlen(chaine)] = 'j';
        chaine[strlen(chaine)] = '\0';
    }
    return 1;
}

int validation_2(Tchaine chaine)
{
    int b=0, B=0, m=0, M=0; //compteur pour chaque bonus/malus

    for(int i=1; i < strlen(chaine) - 2; i++)   //ajoute 1 au compteur associé
    {
        if(chaine[i] == 'b')
            b++;
        if(chaine[i] == 'B')
            B++;
        if(chaine[i] == 'm')
            m++;
        if(chaine[i] == 'M')
            M++;
    }
    
    if(b && B && m && M)    //verifie que chaque bonus/malus n'est present qu'une seule fois
        return 1;   //si oui -> verifacaion numero 3
    else
    {
        //si non, la chaine n'est pas validée
        printf("# Warning : Erreur de saisie des bonus/malus (rappel : chacun des bonus/malus 'b', 'B', 'm' ou 'M' doit etre present UNE fois et le premier caractere ne doit pas en etre un)\n");
        return 0;
    }
}

int validation_3(Tchaine chaine)
{
    int nb_bonus=0; //compteur de bonus/malus present dans une pile

    for(int i = strlen(chaine) - 3; i>=0; i--)  //lecture des caracteres de droite a gauche
    {
        if( (chaine[i] == 'b') || (chaine[i] == 'B') || (chaine[i] == 'm') || (chaine[i] == 'M') )
            nb_bonus++; //si le caractere est un bonus/malus, on ajoute 1 au compteur
        else
        {
            //si ce n'est pas un bonus/malus 
            //on verifie que le nombre de pions dans la pile soit superieur au nombre de bonus/malus
            if(nb_bonus >= 2)   //ça ne sert a rien de verifier si nb_bonus vaut 1 ou 0
            {
                //comparaison grâce au switch
                //si nb_bonus est plus grand que le nombre de pion dans la pile alors la chaine n'est pas validée
                //ça ne sert a rien de verifier pour les piles de 4 et 5 car le nombre max de nb_bonus vaut 4
                switch(chaine[i])
                {
                    case 'u' :
                    case 'U' : 
                        printf("* Plusieurs bonus/malus attribues sur un pion seul\n");
                        return 0;
                    
                    case 'd' :
                    case 'D' :
                        if(nb_bonus > 2)
                        {
                            printf("* Trop de malus/bonus attribues sur une pile de 2 pions\n");
                            return 0;
                        }
                        break;

                    case 't' :
                    case 'T' :
                        if(nb_bonus > 3)
                        {
                            printf("* Trop de malus/bonus attribues sur une pile de 3 pions\n");
                            return 0;
                        }
                        break;
                }
            }
            nb_bonus = 0;
        }
    }
    //si aucune erreur n'est rencontée -> validation numéro 4
    return 1;
}

int validation_4(Tchaine chaine)
{
    //---------------------------------------------------------------------------------------------------------------------------------
    // declarations et initialisations
    int Tabpion[MAXCAR] = {0};    //Tabpion est un tableau dans lequel est indique le nombre de pions pour chaque caractere de la chaine fen 
    int Tabpile[MAXCAR] = {0};    //Tabpile est un tableau dans lequel est indique le nombre de piles pour chaque caractere de la chaine fen 
    int nb_element = 0;         //est le nombre d'elements dans Tabpion et Tabpile
    int somme = 0;              //est la somme des elements de Tabpion (qui doit etre egale a 48)
    int aux;                    //variable temporaire pour permettre de modifier la chaine fen lorsqu'elle est trop courte
    int dizaine;                //variable utilisee lors de la modification de la chaine
    int unite;                  //idem
    int indice_esp;             //indice du caractere ' '
    //---------------------------------------------------------------------------------------------------------------------------------

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
                    return 0;
                //il y a 2 chiffres a la suite, il faut regrouper les 2 pour n'en former qu'1
                else
                {
                    //on prend le premier chiffre pour celui des dizaines et le suivant pour celui des unites afin de stocker le resultat dans Tabpion et Tabpile
                    Tabpion[i] = (chaine[i] - 48) * 10 + chaine[i + 1] - 48;  // on fait -48 a cause du code ascii du caractere 
                    Tabpion[i + 1] = 0;
                    Tabpile[i] = (chaine[i] - 48) * 10 + chaine[i + 1] - 48;  
                    Tabpile[i + 1] = 0;
                    i++;
                    nb_element += 2;
                }
            }
            //il n'y a qu'un seul chiffre
            else
            {
                //on le stocke dans Tabpion et Tabpile
                Tabpion[i] = chaine[i] - 48;
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
        somme += Tabpile[i];
    
    if(somme > 48) //si somme > 48 alors la chaine n'est pas validee
    {
        printf("* Le nombre de pions est trop grand (rappel : nombre de pions <= 48)\n");
        return 0;
    }

    somme=0;

    for(int i=0; i < nb_element; i++)   //calcul de la sommme des elements de Tabpile
        somme += Tabpile[i];

    if(somme == 48)     //si somme vaut 48 -> validation numero 5 !
        return 1;
    else if(somme > 48) //si somme > 48 alors la chaine n'est pas validee
    {
        printf("* Le nombre de piles est trop grand (rappel : nombre de piles <= 48)\n");
        return 0;
    }
    else                //si somme < 48 alors la modification de la chaine commence...
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
                chaine[indice_esp] = 48 - somme + 48;
                chaine[indice_esp + 1] = ' ';
                chaine[indice_esp + 2] = aux;
                chaine[indice_esp + 3] = '\0';
            }  
        }
        
        //la chaine a ete modifiée, elle est donc validée
        return 1;
    }
    printf("\n");

    return 1;
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

void Json(Tformat Tabnb,Tformat Tabcouleur,char trait,char description[],char numdiag[], Tchaine numfen, int bonusJ, int bonusR, int malusJ, int malusR, char *Nomfichier)
{
	printf(". Debug : Appel de la fonction json export réussi ! \n");
	FILE* json=fopen(Nomfichier, "w"); // on ouvre le fichier avec w+ pour supprimer le contenu au préalable et récrire à chaque fois, utile car on veut générer un fichier à chaque coup
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
		{
			fprintf(json,"\n\t{%s:%d, %s:%d},",STR_NB,Tabnb[i],STR_COULEUR,Tabcouleur[i]);//boucle des positions
		}
		fprintf(json,"\n]\n});");
	    fclose(json); 
	    printf("\n=> Export Json terminé (chemin de fichier : %s) ! \n\n", Nomfichier);
        
	}
	else{
        printf("\n* Erreur lors de l'ouverture du fichier Json\n\n");
    }
}