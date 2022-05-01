#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "../inc/menu.h"
#include "../inc/simulation.h"

int main(){
	int choice  = 0;
	float surface = 0;
	float surface1Panneau = 1.7;
	int nPanneaux = 0;
	int maxChoice = 8;

	int actualisation = 0; //Sert a actualiser une fois le catalogue principal apres modification
	Catalogue *catalogue = initialisation();
	creationCatalogue(catalogue);
	Catalogue *catalogueUtilisateur = initialisation();


	system("clear");
	Irradiation *listeVille[DIR_SIZE] = {NULL}; //Initialisation 
    creerListeVille(listeVille);  // Table de hachage : chaque élément pointe vers une structure Irradiation (liste d'irradiation d'une ville)
	Irradiation *listeIrradiation = choixVille(listeVille); // Choix + recherche d'une ville dans la table de hachage

	
	char *chaine;
	int sub_choice;

	
	while(choice != maxChoice)
	{	
		nPanneaux = (int) surface / surface1Panneau;  // Valeur entière = valeur concrète
		printf("**************************************************\n");
		printf("*                                                *\n");
		printf("*              + Selection Menu +                *\n");
		printf("*                                                *\n");
		printf("* [1] : Afficher le catalogue des appareils      *\n");
		printf("* [2] : Modifier le catalogue principale         *\n");
		printf("* [3] : Modifier la surface de la maison         *\n");
		printf("* [4] : Afficher le catalogue de l'utilisateur   *\n");
		printf("* [5] : Modifier le catalogue de l'utilisateur   *\n");
		printf("* [6] : Changer de ville 		         *\n");
		printf("* [7] : Lancer la simulation                     *\n");
		printf("* [8] : Quitter                                  *\n");
		printf("*                                                *\n");
		printf("**************************************************\n");
		printf("\nVille actuelle : %s\n", listeIrradiation->nomVille);
		printf("Surface exploitable  : %.2f m2           	  \n",surface);
		printf("Panneaux solaires utilisables: %d       	  \n",nPanneaux);
		printf("\nTapez le numéro de la commande souhaitée: ");
		
		scanf("%d",&choice);

		// **** Mise a jour du catalogue principale *** //
		if(actualisation == 1){
			catalogue = initialisation();
			creationCatalogue(catalogue);
			actualisation == 0;
		}
		// ******************************************** //
		
		switch(choice)
		{
			case 1: 
				system("clear");
				afficherCatalogue(catalogue);
				break;

			case 2:
				//chaine = entreeStr();
				modifierCatalogue(catalogue,catalogueUtilisateur,0);
				//system("clear");
				actualisation = 1;
				break;

			case 3: 
				system("clear");
				surface = modifierSurface(surface);
				system("clear");
				break;

			case 4: 
				system("clear");
				afficherCatalogue(catalogueUtilisateur);
				break;

			case 5: 
				printf("Voulez-vous :\n");
				printf("[1] Suprimmer un element \n");
				printf("[2] : Ajouter un élément\n");
				printf("Sélection de l'option: ");
				scanf("%d",&sub_choice);
				
				system("clear");
				modifierCatalogue(catalogue,catalogueUtilisateur,sub_choice);
				break;

			case 6:
				system("clear");
				fgetc(stdin); // Pour éviter les bugs dans creerIrradiation(), fgetc récupère l'entrée du scanf(choice) 
				listeIrradiation = choixVille(listeVille); // Changement de ville => changement de pointeurs dans la Hash table
				break;

			case 7: 
				lancerSimulation(listeIrradiation,nPanneaux,catalogueUtilisateur);
				break;
			

		}
		if(choice > maxChoice){
			system("clear");
		}
	}
	system("clear");
	printf("Sortie du programme...						\n\n");
	return EXIT_SUCCESS;
}
