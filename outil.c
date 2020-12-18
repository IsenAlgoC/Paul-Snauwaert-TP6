#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/* Compléter votre nom ici                                                */
/*   Nom : Snauwaert		       Prénom : Paul                          */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le répertoire stocké en mémoire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire *rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	// compléter code ici pour tableau

	if (rep->nb_elts < MAX_ENREG)						//Test si ajout d'un élément possible (ie place dans le tableau)
	{
		*((rep->tab) + rep->nb_elts) = enr;					//Si oui, ajout de l'élément à la nb_elts position
		(rep->est_trie) = false;							//Tableau passe à non trié
		modif = true;										//Modifications apportées donc modif => true
		(rep->nb_elts)++;									//Incrémentation du nombre d'éléments dans le tableau
		return(OK);											//Renvoi OK : Succès
	}
	else {												//Sinon
		return(ERROR);										//Renvoi ERROR : Echec		
	}

	
#else
#ifdef IMPL_LIST

	//Lors de l'insertion d'éléments dans la liste chainée, on procède au tri directement de sorte 
	//que les éléments soient dans l'ordre

	if (rep->nb_elts == 0) {												//Si chaine vide
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {			//Si opération d'ajout succès
			rep->nb_elts++;															//Incrémentation nombre éléments
			modif = true;															//Modifications apportée donc modif = true
			rep->est_trie = true;													//1 seul élément => tri OK
			return(OK);																//Renvoi OK : Succès
		}

	}
	else {													//Sinon
		int position = 0;										//Initialisation variable position à 0
		//Déclaration variable SLLE (ie maillon) qui vaut le "position-ième" élément de la liste
		SingleLinkedListElem* currentElement = GetElementAt(rep->liste, position);		
		if (rep->nb_elts < MAX_ENREG) {												//Si assez de place dans la chaine
			if (est_sup(enr, rep->liste->tail->pers) == true) {							//Si élément supérieur à la queue
																						//=> Cet élément va devenir la queue
				if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {			//Test si insertion succès
					rep->nb_elts++;			//Incrémentation nombre éléments													
					modif = true;			//Modif passe à true
					rep->est_trie = true;	//La chaine est triée
}
			}
			//Si élément inférieur à la queue, on cherche le premier élément qui lui est supérieur dans la chaine 
			//et on insère notre élément juste avant celui-ci
			else {
				while (est_sup(enr, currentElement->pers) == true) {	
					position++;
					currentElement = GetElementAt(rep->liste, position);
				}
				if (InsertElementAt(rep->liste, position, enr) != 0) {
					rep->nb_elts++;
					modif = true;
					rep->est_trie = true;
				}
			}
		}
		else 
			return(ERROR);
	}


#endif
	
#endif


	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du répertoire l'enregistrement dont l'indice est donné en */
  /*   paramètre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire *rep, int indice) {

	// compléter code ici pour tableau
	if (rep->nb_elts >= 1 && indice<=(rep->nb_elts) && indice>=0)		/* s'il y a au moins un element ds le tableau */
	{														/* et que l'indice pointe a l'interieur */
		for (int i = indice ; i < (rep->nb_elts) - 1 ; i++)
		{
			*((rep->tab)->nom + i) = *((rep->tab)->nom + i + 1);
			*((rep->tab)->prenom + i) = *((rep->tab)->prenom + i + 1);
			*((rep->tab)->tel + i) = *((rep->tab)->tel + i + 1);
		}
		rep->nb_elts -= 1;		/* ds ts les cas, il y a un element en moins */
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du répertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif à vrai                              */
  /************************************************************************/
  // complet

	int supprimer_un_contact_dans_rep_liste(Repertoire *rep, SingleLinkedListElem *elem) {
	
	int ret=DeleteLinkedListElem(rep->liste, elem);
	if (ret == 1) {
		rep->nb_elts--;
		modif = true;
	}

	return (0); 
	}
#endif
#endif


  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement sur une ligne à l'écran  */
  /* ex Dupont, Jean                 0320304050                         */
  /**********************************************************************/
void affichage_enreg(Enregistrement enr)
{
	printf("%s, %s\t\t%s", enr.nom, enr.prenom, enr.tel);
} /* fin affichage_enreg */

  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
void affichage_enreg_frmt(Enregistrement enr)
{
	printf("\n|%s", enr.nom);
	for (int i = strlen(enr.nom); i < MAX_NOM; i++)
		printf(" ");
	printf("|%s", enr.prenom);
	for (int i = strlen(enr.prenom); i < MAX_NOM; i++)
		printf(" ");
	printf("|%s", enr.tel);
} /* fin affichage_enreg */


  /**********************************************************************/
  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{
	//code à compléter ici
	int i = 0;
	while (i < MAX_NOM) {
		int tmp1 = (char)toupper(enr1.nom[i]);//on met la valeur des lettres en majuscule pour les comparer en ASCII 
		int tmp2 = (char)toupper(enr2.nom[i]);

		if (tmp1 > tmp2) {
			return (true);
		}
		else if (tmp2 > tmp1) {
			return false;
		}
		i++;
	}
	i = 0;
	while (i < MAX_NOM) {
		int tmp1 = (char)toupper(enr1.prenom[i]);//on met la valeur des lettres en majuscule pour les comparer en ASCII 
		int tmp2 = (char)toupper(enr2.prenom[i]);

		if (tmp1 > tmp2) {
			return (true);
		}
		else if (tmp2 > tmp1) {
			return false;
		}
		i++;
	}

	return(false);
}
 
/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire *rep)
{

#ifdef IMPL_TAB

	Enregistrement temp;
	for (int i = rep->nb_elts; i >0 ; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (est_sup(*(rep->tab+j), *(rep->tab + j+1)) == false)
			{
				temp = *(rep->tab + j);
				*(rep->tab + j) = *(rep->tab + j + 1);
				*(rep->tab + j + 1) = temp;
			}
		}
	}
	


	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien à faire !
	// la liste est toujours triée
#endif
#endif


	rep->est_trie = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le répertoire d'un enregistrement correspondant au  */
  /*   nom à partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au critère ou*/
  /*   un entier négatif si la recherche est négative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire *rep, char nom[], int ind)
{
	int i = ind;		    /* position (indice) de début de recherche dans tableau/liste rep */
	int ind_fin;			/* position (indice) de fin de tableau/liste rep */

	char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;	

	ind_fin = rep->nb_elts; // indice de fin à ne pas dépasser
	strncpy_s(tmp_nom, _countof(tmp_nom), nom, _TRUNCATE); //copie du nom à rechercher
	_strupr_s(tmp_nom, _countof(tmp_nom));



#ifdef IMPL_TAB
	while(i < ind_fin && !trouve)
	{
		strncpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom, _TRUNCATE);
		_strupr_s(tmp_nom2, _countof(tmp_nom2));
		if (strcmp(tmp_nom2, tmp_nom) == 0)
			trouve = true;
		else
			i++;
	}

#else
#ifdef IMPL_LIST
							// ajouter code ici pour Liste
	SingleLinkedListElem* currentElement = GetElementAt(rep->liste, ind);
	while ((i < ind_fin) && (!trouve))
	{

		while ((currentElement != NULL) && (!trouve)) {

			strncpy_s(tmp_nom2, _countof(tmp_nom2), currentElement->pers.nom, _TRUNCATE);
			_strupr_s(tmp_nom2, _countof(tmp_nom2));

			if (strcmp(tmp_nom, tmp_nom2) == 0)
				trouve = true;
			else {
				// si pas trouvé, on passe au suivant
				currentElement = currentElement->next;
				i++;
			}
		}
	}
#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non numériques de la chaines        */
  /*********************************************************************/
void compact(char *s)
{
	int i = 0, j = 0;
	while (s[i] != '\0')
	{
		if (s[i] > 47 && s[i] < 58)
		{
			s[j] = s[i];
			j++;
		}
		i++;
	}
	s[j] = '\0';
	return;
}

/**********************************************************************/
/* sauvegarde le répertoire dans le fichier dont le nom est passé en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionné ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire* rep, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
	errno_t err = fopen_s(&fic_rep, nom_fichier, "w"); //On ouvre le fichier en écriture pour pouvoir écrire ce que l'on souhaite sauvegarder à l'intérieur
#ifdef IMPL_TAB
	// ajouter code ici pour tableau
	//Si le fichier s'est ouvert correctement :
	if (err == 0 && fic_rep != NULL) {
		//Déclaration pointeur de caractère qui va contenir ce que l'on veut écrire dans le fichier, une ligne à la fois
		char buffer[sizeof(Enregistrement) + 1];

		for (int idx = 0; idx < rep->nb_elts; idx++) {
			//On copie d'abord tout dans le buffer
			sprintf_s(buffer, sizeof(Enregistrement) + 1, "%s;%s;%s\n", rep->tab[idx].nom, rep->tab[idx].prenom, rep->tab[idx].tel);
			//Et ensuite on l'écrit dans le fichier
			fputs(buffer, fic_rep);
		}
	}
	else {
		return ERROR;
	}
	fclose(fic_rep);//on ferme le fichier
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	int idx = 0;
	int i = 0;
	if (err == 0 && fic_rep != NULL) {
		SingleLinkedListElem* currentElement = rep->liste->head;
		char buffer[sizeof(Enregistrement) + 1];


		for (idx = 0; idx < rep->nb_elts; idx++) {
			//On copie d'abord tout dans le buffer
			sprintf_s(buffer, sizeof(Enregistrement) + 1, "%s;%s;%s\n", currentElement->pers.nom, currentElement->pers.prenom,
				currentElement->pers.tel);
			//Et ensuite on l'écrit dans le fichier
			fputs(buffer, fic_rep);
			currentElement = currentElement->next;
		}
	}
	else {
		return ERROR;
	}
	fclose(fic_rep);
#endif
#endif

	return(OK);
		} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le répertoire le contenu du fichier dont le nom est  */
  /*   passé en argument                                                */
  /*   retourne OK si le chargement a fonctionné et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char *char_nw_line;
	
	_set_errno(0);
	if ( ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL) )
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element à priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST
														// ajouter code implemention liste
				SingleLinkedListElem currentElement;
				if (lire_champ_suivant(buffer, &idx, currentElement.pers.nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, currentElement.pers.prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, currentElement.pers.tel, MAX_TEL, SEPARATEUR) == OK) {
							//Insertion des éléments
							InsertElementAt(rep->liste, num_rec, currentElement.pers);
							num_rec++;		/* element à priori correct, on le comptabilise */

						}
					}
					}
#endif
#endif




			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */