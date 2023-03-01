#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------------------------------------------------------------
#define MAXCAR 50 // au max a l'initialisation -> 24 u + 24 U + espace + j
//---------------------------------------------------------------------------------------------------------------------------------
typedef char Tchaine[MAXCAR];
//---------------------------------------------------------------------------------------------------------------------------------
// PROTOTYPES DES FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
char *lire(char* lachaine,int nbMAXcaracAsaisir);   //lit une chaine et retourne le nombre d'espace dans la chaine
void afficher(Tchaine chaine);                      //affiche la chaine fen
int format(Tchaine chaine, int nbesp);              //retourne 1 si la chaine est au bon format
int rechercher_esp(Tchaine chaine);                 //retourne l'indice du caractere ' ' dans la chaine
int validation_1(Tchaine chaine, int nbesp);        //verifie les 2 derniers caracteres (espace et r/j)     
int validation_2(Tchaine chaine);                   //verifie la validite des caracteres                                    
int validation_3(Tchaine chaine);                   //verifie que les bonus /malus sont attribues           
int validation_4(Tchaine chaine);                   //verifie que la quantite de bonus ne depasse pas le nombre de pions dans la tour
int validation_5(Tchaine chaine);                   //verifie que le nombre de pions vaut 48
//---------------------------------------------------------------------------------------------------------------------------------
// DEBUT DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
int main()
{
    //---------------------------------------------------------------------------------------------------------------------------------
    // DECLARATIONS ET INITIALISATIONS
    Tchaine fen;        //declaration d'un tableau fen de type Tchaine
    int nbespace = 0;   //compteur d'occurrence du caractere ' ' dans la chaine
    //---------------------------------------------------------------------------------------------------------------------------------   
    nbespace = lire(fen, MAXCAR);   //lit la chaine et retourne le nombre d'espace dans la chaine 

    if(format(fen, nbespace))       //verification du format de la chaine
    {
        //la chaine est valide
        printf("format valide, voici la chaine fen : ");
        afficher(fen);
    }
    
    return 1;
}
//---------------------------------------------------------------------------------------------------------------------------------
// FIN DE LA FONCTION MAIN
//---------------------------------------------------------------------------------------------------------------------------------
// FONCTIONS
//---------------------------------------------------------------------------------------------------------------------------------
char *lire(char* lachaine,int nbMAXcaracAsaisir)
{   //---------------------------------------------------------------------------------------------------------------------------------
    char car;           //car est un caractere
    int i=0,nbesp=0;    //i est un indice et nbesp est le nombre de caractere espace
    //---------------------------------------------------------------------------------------------------------------------------------

    printf("\nentrez une chaine fen\n>");

    do  // boucle qui permet d'avoir au maximum 1 seul caractere ' ' dans la chaine
    {
 	    car=getchar();
	    
	    if (car!='\n')
	    {	
	     	if (car!=' ' ) 
                // ce n'est pas un espace
	    		lachaine[i++]=car;
	     	else
	    	{ // c'est un espace
	    	    if (i!=0 && nbesp==0)	
	    	        {lachaine[i++]=car;nbesp++;}
	        }
	    }
    }
    while(car!='\n' && i<nbMAXcaracAsaisir);
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
    //si les 5 validations retournent 1 la fonction format retournera aussi 1 !
    if( ( !validation_1(chaine, nbesp) ) || ( !validation_2(chaine) ) || ( !validation_3(chaine) ) )
        return 0;
    else
        if( !validation_4(chaine) )
            return 0;
        else
        {
            if( !validation_5(chaine) )
                return 0;
            else
            {
                printf("- Apres ajustement, ");
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

        //si le caractere ' ' se situe en avant derniere position et que le dernier caractere est 'r' ou 'j' -> validation numero 1
        if( ( strlen(chaine) - 2 == indice_esp ) && ( (chaine[indice_esp + 1] == 'r') || (chaine[indice_esp + 1] == 'j') ) )
            return 1;
        else
        {
            printf("- Erreur de saisie du dernier caractere (rappel : r ou j)\n");
            return 0;
        }
    }
    //si la chaine ne contient pas d'espace -> la chaine n'est pas validee
    else
    {
        printf("- Le trait n'est pas defini (rappel : les 2 derniers caracteres sont ' ' puis 'r' ou 'j')\n");
        return 0;
    }
}

int validation_2(Tchaine chaine)
{
    for(int i=0; i < strlen(chaine) - 2; i++)
    {
        //verifie si tous les caracteres sont parmi cette liste : ['u', 'U', 'd', 'D', 't', 'T', 'q', 'Q', 'c', 'C', 'b', 'B', 'm', 'M']
        if( (chaine[i]=='u') || (chaine[i]=='d') || (chaine[i]=='t') || (chaine[i]=='q') || (chaine[i]=='c') || (chaine[i]=='U') || (chaine[i]=='D') || (chaine[i]=='T') || (chaine[i]=='Q') || (chaine[i]=='C') || (chaine[i]=='0') || (chaine[i]=='1') || (chaine[i]=='2') || (chaine[i]=='3') || (chaine[i]=='4') || (chaine[i]=='5') || (chaine[i]=='6') || (chaine[i]=='7') || (chaine[i]=='8') || (chaine[i]=='9') || (chaine[i]=='b') || (chaine[i]=='B') || (chaine[i]=='m') || (chaine[i]=='M') )
            //si oui -> validation numero 2
            return 1;
        //si non -> la chaine n'est pas validee
        else
        {
            printf("- Au moins un caractere n'est pas correct (rappel : u, U, d, D, t, T, q, Q, c, C, b, B, m, M)\n");
            return 0;
        }
    }
}

int validation_3(Tchaine chaine)
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
        printf("- Erreur de saisie des bonus/malus (rappel : chacun des bonus/malus 'b', 'B', 'm' ou 'M' doit etre present UNE fois\n");
        return 0;
    }
}

int validation_4(Tchaine chaine)
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
                        printf("- Plusieurs bonus/malus attribues sur un pion seul\n");
                        return 0;
                    
                    case 'd' :
                    case 'D' :
                        if(nb_bonus > 2)
                        {
                            printf("- Trop de malus/bonus attribues sur une pile de 2 pions\n");
                            return 0;
                        }
                        break;

                    case 't' :
                    case 'T' :
                        if(nb_bonus > 3)
                        {
                            printf("- Trop de malus/bonus attribues sur une pile de 3 pions\n");
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

int validation_5(Tchaine chaine)
{
    //---------------------------------------------------------------------------------------------------------------------------------
    // declarations et initialisations
    int Tab48[MAXCAR] = {0};    //Tab48 est un tableau dans lequel est indique le nombre de pions pour chaque caractere de la chaine fen 
    int nb_element = 0;         //est le nombre d'elements dans Tab48
    int somme = 0;              //est la somme des elements de Tab48 (qui doit etre egale a 48)
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
                    //on prend le premier chiffre pour celui des dizaines et le suivant pour celui des unites afin de stocker le resultat dans Tab48
                    Tab48[i] = (chaine[i] - 48) * 10 + chaine[i + 1] - 48;  // on fait -48 a cause du code ascii du caractere 
                    Tab48[i + 1] = 0;
                    i++;
                    nb_element += 2;
                }
            }
            //il n'y a qu'un seul chiffre
            else
            {
                //on le stocke dans Tab48
                Tab48[i] = chaine[i] - 48;
                nb_element++;
            }
        } 
        else
        {
            //le caractere est un bonus/malus
            if((chaine[i] == 'b') || (chaine[i] == 'B') || (chaine[i] == 'm') || (chaine[i] == 'M'))
            {
                //on stocke 0 dans Tab48
                Tab48[i] = 0;
                nb_element++;
            }
            else
            {
                //on stocke 1 dans Tab48
                Tab48[i] = 1;
                nb_element++;
            }
        }
    }

    for(int i=0; i < nb_element; i++)   //calcul de la sommme des elements de Tab48
        somme += Tab48[i];

    if(somme == 48)     //si somme vaut 48 -> validation numero 5 !
        return 1;
    else if(somme > 48) //si somme > 48 alors la chaine n'est pas validee
    {
        printf("- Le nombre de pions est trop grand (rappel : nombre de pions <= 48)\n");
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