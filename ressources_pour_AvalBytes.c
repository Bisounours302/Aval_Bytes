AvalBytes.c :

/********* Moteur de tournoi : joueur ***************/
/*
	AvalBytes_V5 :
		- Si je peux faire une pile de 5 sur une couleur adverse, je le fais avant que ce soit mon adversaire qui le fasse
		- j'isole une de mes piles :
			- on cherche le coup qui permet d'écarter la pile voisine 
			- Faire en sorte que le nombre de pions dans la pile que l'on veut isoler + ceux dans la voisine > 5 en ajoutant des pions à soi sur la pile voisine
		- sinon je creer une pile sur une destination isolee :
			- je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui, je ne joue ce coup qu'en fin de partie
		- Si je peux faire une pile de 5 sur une de mes couleur, je le fais :
			- je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui, je ne joue ce coup qu'en fin de partie 
		- si je peux, je me deplace sur une pile entourée de pions de ma couleur :
			- je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui, je ne joue ce coup qu'en fin de partie
			- Je favorise cette creation sur la couleur adverse 
		- écarte le seul voisin de couleur adverse d’une de ses pile en le deplaçant sur une pile adverse
		- sinon cherche a faire une tour de 3 pions :
			- je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui, je ne joue ce coup qu'en fin de partie
			- si la destination a un voisin allié on peut jouer ce coup
			- on verifie qu'un voisin de la destination ne peut pas prendre la pile juste apres
			- Je favorise cette creation sur la couleur adverse
		- Sinon j'empile un pion sur un autre :
			- En favorisant le deplacement d'un de mes pions sur un pion adverse
		- sinon joue un coup aleatoire
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

	int i, n; 
	octet o, d, O, D; 
	octet myColor = currentPosition.trait; 
	int critere = 0;

	// afficherListeCoups(listeCoups);
	srand(time(NULL));
	ecrireIndexCoup(rand()%(listeCoups.nb-1));

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

		// Si je peux faire une pile de 5 sur une couleur adverse, je le fais avant que ce soit mon adversaire qui le fasse 
		if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5) ) 
			if(currentPosition.cols[d].couleur != myColor)
			{
				printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction car c'est le meilleur coup
			}
		//isoler ses piles
		if ( (nbVoisinsReels(currentPosition, o) == 1) && (currentPosition.cols[o].couleur == myColor) )
		{
			//on cherche le coup qui permet d'écarter la pile voisine 
			if ( (critere < 13) )
				for(int j=0; j<listeCoups.nb; j++)
				{
					O = listeCoups.coups[j].origine; 
					D = listeCoups.coups[j].destination;
				
					if ( (O == d) && (D != o) )
					{
						if( currentPosition.cols[O].couleur != myColor )
						{
							critere = 13;
							printf("On choisit ce coup ! \n");
							ecrireIndexCoup(j);
							// on continue quand meme les recherches
						}
						else
							if( (critere < 12) )
							{
								critere = 12;
								printf("On choisit ce coup ! \n");
								ecrireIndexCoup(j);
								// on continue quand meme les recherches
							}
					}
				}
			
			//Faire en sorte que le nombre de pions dans la pile que l'on veut isoler + ceux dans la voisine > 5
			//En ajoutant des pions à soi sur la pile voisine
			if ( (critere < 11) )
				for(int j=0; j<listeCoups.nb; j++)
				{
					O = listeCoups.coups[j].origine; 
					D = listeCoups.coups[j].destination;
				
					if ( (O != o) && (D == d) && currentPosition.cols[O].couleur == myColor && currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[O].nb > 5 && currentPosition.cols[d].nb + currentPosition.cols[O].nb != 4)
					{
						critere = 11;
						printf("On choisit ce coup ! \n");
						ecrireIndexCoup(j);
						// on continue quand meme les recherches
					}
				}
		}
		if ( (critere < 10) && (currentPosition.cols[o].couleur == myColor) )
			//je fais une pile sur une destination isolee
			if ( caseIsolee(currentPosition, o, d) ) 
				// je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui je ne joue ce coup qu'en fin de partie
				if( !( caseIsolee(currentPosition, d, o) && currentPosition.cols[d].couleur == myColor) )
				{
					critere = 10;
					printf("On choisit ce coup ! \n"); 
					ecrireIndexCoup(i);
					// on continue quand meme les recherches
				}
		// Si je peux faire une pile de 5 sur une de mes couleur, je le fais 
		if ( (critere < 9) && (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5) ) 
			// je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui je ne joue ce coup qu'en fin de partie
			if(currentPosition.cols[d].couleur == myColor)
				if ( !( caseIsolee(currentPosition, o, d) && ( caseIsolee(currentPosition, d, o) ) ) )
				{
					critere = 9;
					printf("On choisit ce coup ! \n"); 
					ecrireIndexCoup(i);
					// on continue quand meme les recherches
				}
		// si je peux, je me deplace sur une pile entourée de pions de ma couleur
		if( voisinsAllies(currentPosition, o, d, myColor) && (currentPosition.cols[o].couleur == myColor) )
		{
			// je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui je ne joue ce coup qu'en fin de partie
			if ( !(caseIsolee(currentPosition, o, d) && ( caseIsolee(currentPosition, d, o) && currentPosition.cols[d].couleur == myColor)) )
				// Je favorise cette creation sur la couleur adverse 
				if( (critere < 8) && currentPosition.cols[d].couleur != myColor )
					{
						critere = 8;
						printf("On choisit ce coup ! \n"); 
						ecrireIndexCoup(i);
						// on continue quand meme les recherches 
					}
					else if(critere < 7)
					{
						critere = 7;
						printf("On choisit ce coup ! \n"); 
						ecrireIndexCoup(i);
						// on continue quand meme les recherches 
					}
		}
		// écarte le seul voisin de couleur adverse d’une de ses pile en le deplaçant sur une pile adverse
		if( (critere < 6) && (currentPosition.cols[o].couleur == myColor) )
		{
			n = posVoisinAdverse(currentPosition, o, myColor);
			if ( n != -1 )
			{
				for(int j=0; j<listeCoups.nb; j++)
				{
					O = listeCoups.coups[j].origine; 
					D = listeCoups.coups[j].destination;
				
					if ( (O == n) && (currentPosition.cols[D].couleur != myColor) && (currentPosition.cols[O].nb + currentPosition.cols[D].nb <= 3) )
					{
						critere = 6;
						printf("On choisit ce coup ! \n");
						ecrireIndexCoup(j);
						// on continue quand meme les recherches
					}
				}
			}
		}
		// Si je peux faire une pile de 3 pions, je le fais
		if( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 3) ) 
		{
			// je vérifie si les 2 piles m'appartiennent et qu'elles sont toutes les 2 isolées, si oui je ne joue ce coup qu'en fin de partie
			if ( !(caseIsolee(currentPosition, o, d) && ( caseIsolee(currentPosition, d, o) && currentPosition.cols[d].couleur == myColor)) )
				//si la destination a un voisin allié on peut jouer ce coup
				if( unVoisinAllie(currentPosition, o, d, myColor) )
					// on verifie qu'un voisin de la destination ne peut pas prendre la pile juste apres
					if( !voisinDangereux(currentPosition, o, d, myColor) )
					{
						// Je favorise cette creation sur la couleur adverse 
						if( (critere < 5) && currentPosition.cols[d].couleur != myColor )
						{
							critere = 5;
							printf("On choisit ce coup ! \n"); 
							ecrireIndexCoup(i);
							// on continue quand meme les recherches 
						}
						else if(critere < 4)
						{
							critere = 4;
							printf("On choisit ce coup ! \n"); 
							ecrireIndexCoup(i);
							// on continue quand meme les recherches 
						}
					}
		}
		// Sinon j'empile un pion sur un autre
		if( (currentPosition.cols[d].nb + currentPosition.cols[o].nb != 4) )
		{
			if( unVoisinAllie(currentPosition, o, d, myColor) )
				if( !voisinDangereux(currentPosition, o, d, myColor) )
					// En favorisant le deplacement d'un de mes pions
					if ( (currentPosition.cols[o].couleur == myColor) )
						// Sur une couleur adverse
						if( (critere < 3) && (currentPosition.cols[d].couleur != myColor) )
						{
							critere = 3;
							printf("On choisit ce coup ! \n"); 
							ecrireIndexCoup(i);
							// on continue quand meme les recherches
						}
						else if( critere < 2 )
						{
							critere = 2;
							printf("On choisit ce coup ! \n"); 
							ecrireIndexCoup(i);
							// on continue quand meme les recherches
						}
					else
						if( (critere < 1) && (currentPosition.cols[d].couleur != myColor) )
						{
							critere = 1;
							printf("On choisit ce coup ! \n"); 
							ecrireIndexCoup(i);
							// on continue quand meme les recherches
						}
		}
	}
}



libavalam.c :

#include <stdio.h>
#include "avalam.h"

extern T_Voisins topologie[NBCASES]; 
extern T_Position positionInitiale; // ajout extern 20/03/2022

octet nbVoisins(octet numCase) {
	// renvoie le nombre de voisins d'une case
	return topologie[numCase].nb; 
}

T_Voisins getVoisins(octet numCase) {
	return topologie[numCase]; 
}

void listerVoisins(octet numCase) {
	int i; 
	T_Voisins v = getVoisins(numCase);
	for(i=0;i<v.nb;i++) {
		printf("%d ",v.cases[i]);
	}
	printf("\n");
}

T_Position getPositionInitiale() {
	return positionInitiale ;
}

void afficherPosition(T_Position p) {
	int i; int delta; 
	printf("numCoup : %d\n",p.numCoup);	// ajout 20/03/2022
	for(i=0;i<NBCASES;i++) {
		printf("%3d : %3d : %s", i, p.cols[i].nb, COLNAME(p.cols[i].couleur));
		delta = 0; 
		if (p.evolution.bonusR == i)  delta++; 
		if (p.evolution.bonusJ == i)  delta++;
		if (p.evolution.malusR == i)  delta--;
		if (p.evolution.malusJ == i)  delta--;
		if (delta>0) printf(" [Evolution:+%d]\n",delta);
		else if (delta<0) printf(" [Evolution:%d]\n",delta); 
		else printf("\n");
	}
}

T_ListeCoups getCoupsLegaux(T_Position p) {
	T_ListeCoups l ={0};

	// Pour chaque case, 
	// Si cette contient une colonne non vide, 
	// Pour chaque voisin de cette colonne, 
	// Si la somme colO+colA<= 5, 
	// ajouter ce coup 

	int cO, cD, iV; // case Origine, case Destination, indice Voisin
	T_Voisins v; 
	
	// 20/03/2022 : on prévoit les 4 premiers coups spécifiquement 
	// 0 => +Jaune 
	// 1 => +Rouge 
	// 2 => -Jaune 
	// 3 => -Rouge
	
	if (p.numCoup == 0) {
		// renvoyer les indices des cases jaunes 
		for(cO=0;cO<NBCASES;cO++) {
			if (p.cols[cO].couleur == JAU)	addCoup(&l, cO,cO); 
		}
		return l;
	}
	
	if (p.numCoup == 1) {
		// renvoyer les indices des cases rouges
		for(cO=0;cO<NBCASES;cO++) {
			if (p.cols[cO].couleur == ROU)	addCoup(&l, cO,cO); 
		}
		return l;
	}
	
	if (p.numCoup == 3) {
		// renvoyer les indices des cases rouges sauf le bonus rouge 
		for(cO=0;cO<NBCASES;cO++) {
			if ( (p.cols[cO].couleur == ROU)	&& (cO != p.evolution.bonusR) ) addCoup(&l, cO,cO); 
		}
		return l;
	}
	
	if (p.numCoup == 2) {
		// renvoyer les indices des cases jaunes sauf le bonus jaune 
		for(cO=0;cO<NBCASES;cO++) {
			if ( (p.cols[cO].couleur == JAU) &&  (cO != p.evolution.bonusJ) ) addCoup(&l, cO,cO); 
		}
		return l;
	}
	
	// cas général 
	
	for(cO=0;cO<NBCASES;cO++) {
		printf1("case %d\n",cO); 
		if (p.cols[cO].nb == VIDE ) {printf0("vide!\n"); continue;}
		v = getVoisins(cO); 
		for(iV=0;iV<v.nb;iV++) {
			cD = v.cases[iV]; 
			printf2("voisin %d : case %d\n",iV, cD);
			if (p.cols[cD].nb == VIDE ) {printf0("vide!\n"); continue;} // ajout par tomnab 21/03/2018
			if (p.cols[cO].nb + p.cols[cD].nb <= 5) {
				printf0("possible !\n");
				addCoup(&l, cO,cD); 
			}
		}
	}

	return l; 
}


void addCoup(T_ListeCoups * pL, octet origine, octet destination) {
	pL->coups[pL->nb].origine = origine; 
	pL->coups[pL->nb].destination = destination; 
	pL->nb++; 
}

void afficherListeCoups(T_ListeCoups l) {
	int i; 
	for(i=0;i<l.nb;i++) {
		printf("%3d : %3d -> %3d\n", i, l.coups[i].origine, l.coups[i].destination); 
	}
}

octet estValide(T_Position p, octet origine, octet destination) {

	T_Voisins v; int i;
	
	// 20/03/2022 = f(numCoup) 
	if (p.numCoup==0) {
		if (p.cols[origine].couleur != JAU) {
			printf("jouerCoup impossible : la colonne %d n'est pas de la bonne couleur ! \n", origine);
			return FAUX; 
		} 
		return VRAI;
	}
	
	if (p.numCoup==1) {
		if (p.cols[origine].couleur != ROU) {
			printf("jouerCoup impossible : la colonne %d n'est pas de la bonne couleur ! \n", origine);
			return FAUX; 
		} 
		return VRAI;
	}
	
	if (p.numCoup==3) {
		if (p.cols[origine].couleur != ROU) {
			printf("jouerCoup impossible : la colonne %d n'est pas de la bonne couleur ! \n", origine);
			return FAUX; 
		} 
		if (origine == p.evolution.bonusR) {
			printf("jouerCoup impossible : le bonus rouge est deja dans %d ! \n", origine);
			return FAUX; 
		} 
		return VRAI;
	}
		
	if (p.numCoup==2) {
		if (p.cols[origine].couleur != JAU) {
			printf("jouerCoup impossible : la colonne %d n'est pas de la bonne couleur ! \n", origine);
			return FAUX; 
		}
		if (origine == p.evolution.bonusJ) {
			printf("jouerCoup impossible : le bonus jaune est deja dans %d ! \n", origine);
			return FAUX;
		}
		return VRAI;
	}

	// vérifie la légalité d'un coup 
	if (p.cols[origine].nb == VIDE) { 
		printf("jouerCoup impossible : la colonne %d est vide ! \n", origine);
		return FAUX;  
	} 

	if (p.cols[destination].nb == VIDE) { 
		printf("jouerCoup impossible : la colonne %d est vide ! \n", destination);
		return FAUX;  
	} 

	if (p.cols[origine].nb + p.cols[destination].nb > 5) {
		printf("jouerCoup impossible : trop de jetons entre %d et %d ! \n", origine, destination);
		return FAUX;  		
	}

	// Il faut aussi vérifier accessibilité !!
	v= getVoisins(origine);
	for (i=0;i<v.nb;i++)
		if (v.cases[i]==destination) return VRAI;
	
	printf("jouerCoup impossible : cases %d et %d inaccessibles! \n", origine, destination);	
	return FAUX;
}

T_Position jouerCoup(T_Position p, octet origine, octet destination) {
	// NB : p est une copie de la position !!

	if (!estValide(p,origine,destination)) return p;
	
	// 20/03/2022 : gestion des coups evolution 
	// 0 => +Jaune 
	// 1 => +Rouge 
	// 2 => -Jaune
	// 3 => -Rouge 
	if (p.numCoup == 0) {
		p.evolution.bonusJ = origine; 
	}
	else if (p.numCoup == 1) {
		p.evolution.bonusR = origine; 
	}
	else if (p.numCoup == 3) {
		p.evolution.malusR = origine; 
	}
	else if (p.numCoup == 2) {
		p.evolution.malusJ = origine;
	}
	else {
		// Joue un coup 
		p.cols[destination].nb += p.cols[origine].nb; 
		p.cols[destination].couleur = p.cols[origine].couleur; 
		p.cols[origine].nb = VIDE; 
		p.cols[origine].couleur = VIDE; 
		
		// Il faut mettre à jour les pions évolution
		if (p.evolution.bonusR == origine) p.evolution.bonusR=destination;
		if (p.evolution.bonusJ == origine) p.evolution.bonusJ=destination;
		if (p.evolution.malusR == origine) p.evolution.malusR=destination;
		if (p.evolution.malusJ == origine) p.evolution.malusJ=destination;
	}
	
	// incrémenter numCoup (on compte en demi-coups) 
	// 0 <-> trait aux JAU
	p.numCoup++; // 20/03/2022
	
	// On inverse le trait
	p.trait = (p.trait == JAU) ? ROU : JAU;

	// renvoie la nouvelle position 
	return p; 
}



T_Score evaluerScore(T_Position p) {
	T_Score s ={0};
	int i;
 
	for(i=0;i<NBCASES;i++) {
		if (p.cols[i].nb != VIDE)  {
			if (p.cols[i].couleur == JAU) {
				s.nbJ++; 
				if (p.cols[i].nb == 5)  s.nbJ5++; 
			} else {
				s.nbR++; 
				if (p.cols[i].nb == 5)  s.nbR5++; 
			}
		}
	}
	
	// partie évolution 	
	if ( (p.cols[p.evolution.bonusR].nb != VIDE) && (p.cols[p.evolution.bonusR].couleur == JAU)) {s.nbJ++;}
	if ( (p.cols[p.evolution.bonusR].nb != VIDE) && (p.cols[p.evolution.bonusR].couleur == ROU)) {s.nbR++;} 
	if ( (p.cols[p.evolution.bonusJ].nb != VIDE) && (p.cols[p.evolution.bonusJ].couleur == JAU)) {s.nbJ++;}
	if ( (p.cols[p.evolution.bonusJ].nb != VIDE) && (p.cols[p.evolution.bonusJ].couleur == ROU)) {s.nbR++;}
	if ( (p.cols[p.evolution.malusR].nb != VIDE) && (p.cols[p.evolution.malusR].couleur == JAU)) {s.nbJ--;}
	if ( (p.cols[p.evolution.malusR].nb != VIDE) && (p.cols[p.evolution.malusR].couleur == ROU)) {s.nbR--;}
	if ( (p.cols[p.evolution.malusJ].nb != VIDE) && (p.cols[p.evolution.malusJ].couleur == JAU)) {s.nbJ--;}
	if ( (p.cols[p.evolution.malusJ].nb != VIDE) && (p.cols[p.evolution.malusJ].couleur == ROU)) {s.nbR--;}


	return s; 
}

void afficherScore(T_Score s) {
	printf("J: %d (%d piles de 5) - R : %d (%d piles de 5)\n", s.nbJ, s.nbJ5, s.nbR, s.nbR5);
}

int nbVoisinsReels(T_Position p, octet numCase)
{
	int cO = numCase, cD, iV; // case Origine, case Destination, indice Voisin
	T_Voisins v;
	int nbVoisinsReels = 0;

	v = getVoisins(cO); 
	for(iV=0;iV<v.nb;iV++) {
		cD = v.cases[iV]; 
		if (p.cols[cD].nb == VIDE ) {continue;}
		if (p.cols[cO].nb + p.cols[cD].nb <= 5) {
				nbVoisinsReels++;
		}
	}

	return nbVoisinsReels;
}

int voisinDangereux(T_Position p, octet o, octet d, octet myColor)
{
	int cO = d, cD, iV; // case Origine, case Destination, indice Voisin
	T_Voisins v;

	v = getVoisins(cO); 
	for(iV=0;iV<v.nb;iV++) {
		cD = v.cases[iV]; 
		if (p.cols[cD].nb == VIDE || cD == o || p.cols[cD].couleur == myColor) {continue;}
		if (p.cols[o].nb + p.cols[cO].nb + p.cols[cD].nb == 5) {
				return 1;
		}
	}

	return 0;
}

int caseIsolee(T_Position p, octet o, octet d)
{
	int cO = d, cD, iV; // case Origine, case Destination, indice Voisin
	T_Voisins v;

	v = getVoisins(cO); 
	for(iV=0;iV<v.nb;iV++) {
		cD = v.cases[iV]; 
		if (p.cols[cD].nb == VIDE || cD == o) {continue;}
		if (p.cols[o].nb + p.cols[cO].nb + p.cols[cD].nb <= 5) {
				return 0;
		}
	}

	return 1;
}

int posVoisinAdverse(T_Position p, octet o, octet myColor)
{
	int cO = o, cD, iV, pos; // case Origine, case Destination, indice Voisin
	T_Voisins v;
	int nbVoisinsAdverses = 0;

	v = getVoisins(cO); 
	for(iV=0;iV<v.nb;iV++) {
		cD = v.cases[iV]; 
		if (p.cols[cD].nb == VIDE ) {continue;}
		if (p.cols[cD].couleur != myColor) {
				nbVoisinsAdverses++;
				pos = cD;
		}
	}

	if(nbVoisinsAdverses == 1)
		return pos;
	return -1;
}

int voisinsAllies(T_Position p, octet o, octet d, octet myColor)
{
	int cO = d, cD, iV; // case Origine, case Destination, indice Voisin
	T_Voisins v;

	v = getVoisins(cO); 
	for(iV=0;iV<v.nb;iV++) {
		cD = v.cases[iV]; 
		if (p.cols[cD].nb == VIDE || cD == o) {continue;}
		if ( p.cols[o].nb + p.cols[cO].nb + p.cols[cD].nb <= 5 && p.cols[cD].couleur != myColor) {
				return 0;
		}
	}

	return 1;
}

int unVoisinAllie(T_Position p, octet o, octet d, octet myColor)
{
	int cO = d, cD1, cD2, iV1, iV2, allie, ennemi; // case Origine, case Destination, indice Voisin
	T_Voisins v;

	v = getVoisins(cO); 
	for(iV1=0;iV1<v.nb;iV1++) {
		cD1 = v.cases[iV1]; 
		if (p.cols[cD1].nb == VIDE || cD1 == o) {continue;}
		if (p.cols[cD1].couleur == myColor) {
			for(iV2=0;iV2<v.nb;iV2++) {
				cD2 = v.cases[iV2]; 
				if (p.cols[cD2].nb == VIDE || cD1 == o || cD1 == cD2 || p.cols[cD2].couleur == myColor) {continue;}
				if (p.cols[o].nb + p.cols[cO].nb + p.cols[cD1].nb + p.cols[cD2].nb <= 5) {
						return 1;
				}
			}
		}
	}

	return 0;
}


avalam.h :

#define FAUX 0
#define VRAI 1


// Vainqueurs des parties et joueurs au trait 
#define EGALITE 0
#define VIDE 0
#define JAU 1
#define ROU 2

#define NBCASES 48
#define UNKNOWN 255

#define COLNAME(c) ((c==ROU) ? "rouge" : "jaune")

// Pour les exports JSON ////////////////////////////////////////////
#define STR_NB "\"nb\""
#define STR_COULEUR "\"couleur\""
#define STR_TURN "\"trait\""
#define STR_COLS "\"cols\""

#define STR_FEN "\"fen\""
#define STR_BONUS_J "\"bonusJ\""
#define STR_BONUS_R "\"bonusR\""
#define STR_MALUS_J "\"malusJ\""
#define STR_MALUS_R "\"malusR\""
#define STR_SCORE_J "\"scoreJ\""
#define STR_SCORE_J5 "\"scoreJ5\""
#define STR_SCORE_R "\"scoreR\""
#define STR_SCORE_R5 "\"scoreR5\""

#define STR_NUMCOUP "\"numCoup\""

#define STR_NUMDIAG "\"numDiag\""
#define STR_NOTES "\"notes\""

#define STR_COUPS "\"coups\""
#define STR_ORIGINE "\"o\""
#define STR_DESTINATION "\"d\""
#define STR_J "\"j\""
#define STR_R "\"r\""
#define STR_JOUEURS "\"joueurs\""

#define STR_NOM "\"nom\""
#define STR_SCORE "\"score\""
#define STR_RONDES "\"rondes\""
#define STR_RONDE "\"ronde\""
#define STR_PARTIES "\"parties\""
#define STR_RESULTAT "\"resultat\""
#define STR_STATUT "\"statut\""

#ifdef __DEBUG__
	#define printf0(p) printf(p)
	#define printf1(p,q) printf(p,q)
	#define printf2(p,q,r) printf(p,q,r)
	#define printf3(p,q,r,s) printf(p,q,r,s)
	#define printf4(p,q,r,s,t) printf(p,q,r,s,t)
	#define whoamid(p) whoami(p)
	#define whopd(p) whop(p)
	#define whojd(p) whoj(p)
	#define whoamjd() whoamj()
#else
	#define printf0(p)
	#define printf1(p,q)
	#define printf2(p,q,r)
	#define printf3(p,q,r,s)
	#define printf4(p,q,r,s,t)
	#define whoamid(p)
	#define whoamjd()
	#define whopd(p)
	#define whojd(p)
#endif

//verif appels systèmes 

#define CHECK_IF(sts,val,msg) \
if ((sts) == (val)) {fprintf(stderr,"erreur appel systeme\n");perror(msg); exit(-1);}

#define CHECK_DIF(sts,val,msg) \
if ((sts) != (val)) {fprintf(stderr,"erreur appel systeme\n");perror(msg); exit(-1);}

typedef unsigned char octet; 

typedef struct {
	octet nbJ; 	octet nbJ5; // total et piles de 5 pour les jaunes 
	octet nbR;	octet nbR5; // total et piles de 5 pour les rouges 
} T_Score;

typedef struct {
	octet nb; 
	octet cases[8]; // il peut y en avoir moins... ils vaudront 0
} T_Voisins; 

typedef struct {
	octet nb; 			// nb d'elts dans la colonne 
	octet couleur;  // couleur du sommet 
} T_Colonne; 

typedef struct { 
	octet bonusJ;
	octet malusJ; 
	octet bonusR;
	octet malusR; 
} T_Evolution;

typedef struct { 
	octet trait; 
	octet numCoup; // ajoute 20/03/2022
	T_Colonne cols[NBCASES];
	T_Evolution evolution;
} T_Position;

typedef struct {
	octet origine; 
	octet destination;
} T_Coup;

typedef struct {
	int nb; 
	T_Coup coups[8*NBCASES]; 
} T_ListeCoups; 

octet nbVoisins(octet numCase); 
T_Voisins getVoisins(octet numCase); 
void listerVoisins(octet numCase); 
T_Position getPositionInitiale();
void afficherPosition(T_Position p); 
void addCoup(T_ListeCoups * pL, octet origine, octet destination);
void afficherListeCoups(T_ListeCoups l);
T_Position jouerCoup(T_Position p, octet origine, octet destination) ;
T_ListeCoups getCoupsLegaux(T_Position p) ; 
T_Score evaluerScore(T_Position p);
void afficherScore(T_Score s);
octet estValide(T_Position p, octet origine, octet destination); // ajout 12/05/2021
int nbVoisinsReels(T_Position p, octet numCase); // ajout par Mathieu
int voisinDangereux(T_Position p, octet o, octet d, octet myColor); // ajout par Mathieu
int caseIsolee(T_Position p, octet o, octet d); // ajout par Mathieu
int posVoisinAdverse(T_Position p, octet o, octet myColor); // ajout par Mathieu
int voisinsAllies(T_Position p, octet o, octet d, octet myColor); // ajout par Mathieu
int unVoisinAllie(T_Position p, octet o, octet d, octet myColor); // ajout par Mathieu
