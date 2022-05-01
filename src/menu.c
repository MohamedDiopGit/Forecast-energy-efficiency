#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/menu.h"

/* Fonctions de gestions de la catalogue */


Catalogue *initialisation(){
	Catalogue *catalogue = malloc(sizeof(*catalogue)); //Allocation
	Equipement *equipement = malloc(sizeof(*equipement));

	if (catalogue == NULL || equipement == NULL) //verification allocation
	{
		printf("exit");
		exit(EXIT_FAILURE);
	}

	equipement = NULL;  //Le premier élement est initialisé en NULL

	catalogue->premier = equipement;
	return catalogue;
}



void creationCatalogue(Catalogue *catalogue){

	/* Ouverture du fichier */
	FILE *fichier = NULL;
	fichier = fopen(CATALOG,"r");
	
	/* Variables */
	char line[MAX_STR];
	char f_nom[MAX_STR];
	char f_categorie[MAX_STR];
	float f_puissance;
	char *token;
	
	/* Traitement du fichier */
	if(fichier != NULL){  // Vérification de l'ouverture du fichier
		fgets(line, sizeof(line), fichier); // Saut de la premiere ligne
		while(fgets(line, sizeof(line), fichier) != NULL){
			
			token = strtok(line,DELIMITER);
			strcpy(f_categorie,token);
			
			token = strtok(NULL,DELIMITER);
			strcpy(f_nom,token);
			
			token = strtok(NULL,DELIMITER);
			f_puissance = (float) atof(token);
			

			insertion(catalogue,f_categorie,f_nom,f_puissance);
		    
		}
		printf("\n");
		fclose(fichier);
	}
	
	else{
		printf("Impossible d'ouvrir le fichier\n");
	}
}





void afficherCatalogue(Catalogue *catalogue)
{
	if (catalogue == NULL)
	{
		exit(EXIT_FAILURE);
	}

	Equipement *equipement = catalogue->premier;
	int equipement_id =1;
	printf("-----	  Categorie | Nom | Puissance (W)     -----\n\n");
	while (equipement != NULL)
	{

		printf("[%d] : %s %s  %.0f\n",equipement_id, equipement->categorie,equipement->nom, equipement->puissance);
		equipement_id +=1;
		equipement = equipement->suivant;
	}
	printf("\n-----       	  Fin du catalogue 	      -----\n\n");
    
}







void modifierCatalogue(Catalogue *catalogue,Catalogue *catalogueUtilisateur,int choice)
{
	int numSuppr = 1; // Initialisation
	int numAjout = 1; //Initialisation pour l'affichage
	int k;
	int init = 0; // Pour l'affichage
	Equipement *equipementAjout;	
	
	switch(choice)
	{   
		case 0:
			writeInCatalog();
			break;
			
		case 1:
			// Suppression d'un élement numéroté
			while(numSuppr != 0){
				printf("*** Rappel du catalogue utilisateur ***\n");
				afficherCatalogue(catalogueUtilisateur);
				printf("[0] : Revenir au menu principal			\n");
				printf("\n--------------------------------------\n");
				printf("Numéro de l'équipement à supprimer: ");
				scanf("%d",&numSuppr);
				suppresion_num(catalogueUtilisateur,numSuppr);
				system("clear");
			}
			printf("Retour au menu principal...\n\n");
			break;
		
		case 2:	
			// Ajout d'un équipement dans le catalogue utilisateur
			while(numAjout != 0){
			
				system("clear");
				printf("*** Rappel du catalogue principal *** 	\n");
				afficherCatalogue(catalogue);  
				printf("[0] : Retour au menu principal	\n");
				printf("\n--------------------------------------\n");
				
				
				if(equipementAjout != NULL && init == 1){
					printf("Ajout de [%d] effectué avec succès.\n",numAjout);
				}
				
				if(equipementAjout == NULL && numAjout != 0 && init == 1){ //Numéro de l'equipement inexistant
					printf("Element [%d] inexistant.\n",numAjout);
				}
				init = 1; // Activation de la ligne (pour l'affichage)
				printf("Numéro à ajouter: ");
				
				scanf("%d",&numAjout);
				equipementAjout = catalogue->premier;
				k = 1;
				
				while(k<numAjout && equipementAjout != NULL){ //Parcours jusqu'a l'élement ciblé
					equipementAjout = equipementAjout->suivant;
					k++;
				}
				if(equipementAjout != NULL && numAjout != 0){ // Vérification élement puis insertion
					insertion(catalogueUtilisateur,equipementAjout->categorie,equipementAjout->nom,equipementAjout->puissance);
					
				}
				
				
			}
			system("clear");
			printf("Retour au menu principal...\n\n");
			break;
	}
}






float modifierSurface(float surface)
{
	float nouvelleSurface;
	printf("Votre surface actuelle est de : %.2f m2\n",surface);
	printf("Votre nouvelle surface : ");
	scanf("%f",&nouvelleSurface);
	return nouvelleSurface;
}

void writeInCatalog(){
	char buf[MAX_STR];
	FILE *fichier = fopen(CATALOG,"a");
	
	printf("Exemple (attention virgule-espace): Cuisine, Micro-Ondes, 1250\n");
	printf("Entrer les informations à ajouter : ");
	fgetc(stdin); //récupère l'entrée pour faire fonctionner correctement fgets
	fgets(buf, MAX_STR, stdin);
	
	/* Traitement du fichier */
	system("clear");
	if(fichier != NULL){
		
		fprintf(fichier, "%s", buf);
		fclose(fichier);
		printf("Ajout effectué avec succés.\n\n");
	}
	if(fichier == NULL){
		printf("Impossible d'écrire dans le fichier\n");
	}
}



void insertion(Catalogue *catalogue, char categorie[], char nom[],float puissance){ //Insertion en début de catalogue (pile)

	//Création du nouvel élément d'équipement
	Equipement *nouveau = malloc(sizeof(*nouveau));  

	if(catalogue == NULL || nouveau == NULL){  //Vérification allocation
		printf("exit");
		exit(EXIT_FAILURE);
	}
	//Attribution de l'information au nouvel élément
	strcpy(nouveau->categorie,categorie);
	strcpy(nouveau->nom,nom);
	nouveau->puissance = puissance;

	//Insertion de l'élément au début du catalogue
	nouveau->suivant = catalogue->premier;  //Le nouvel élement prend en suivant le catalogue à partir du début
	catalogue->premier = nouveau; //Le nouvel élement se place en premier du catalogue
}


void suppression_tete(Catalogue *catalogue){
	if(catalogue == NULL){  //Vérification du catalogue
		printf("exit");
		exit(EXIT_FAILURE);
	}
	if(catalogue->premier != NULL){ 
		Equipement *aSupprimer = catalogue->premier; //Premier élément du catalogue à supprimer
		catalogue->premier = catalogue->premier->suivant; //Décalage au suivant du premier du catalogue
		free(aSupprimer);  //Libération de la mémoire
	}
}


void suppresion_num(Catalogue *catalogue, int num_suppr){
	if(catalogue == NULL){
		printf("exit - catalogue inexistant");
		num_suppr = 0;
	}
	int k =1;
	if(num_suppr > 1){
		Equipement *aSupprimer = catalogue->premier;
		Equipement *prec_aSupprimer = malloc(sizeof(prec_aSupprimer)); //Précède l'élement à supprimer
		

		while(k<num_suppr && aSupprimer != NULL){
			prec_aSupprimer = aSupprimer;
			aSupprimer = aSupprimer->suivant;
			k++;
		}
			
		
		if(aSupprimer == NULL){
			printf("Element inexistant.\n");

		}

		else{
			prec_aSupprimer->suivant = aSupprimer->suivant;
			free(aSupprimer);
		}
	}
	if(num_suppr == 1){
		suppression_tete(catalogue);
	}
}