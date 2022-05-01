#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>   // Utilisée pour parcourir le dossier des fichiers .csv
#include "math.h"
#include "../inc/simulation.h"


/* Fonctions principales */

Irradiation *choixVille(Irradiation *listeVille[])  // Recherche dans la table de hachage
{
    int k = 0;
    long int hashKey;
	system("clear");
	printf("Villes disponibles pour la simulation :\n");
    while(k<DIR_SIZE){
        if(listeVille[k] != NULL){
            hashKey = k;
            printf("%s\n",listeVille[k]->nomVille);
        }
        k++;
    }
    
    char villeSimulation[BUFFER];
    printf("\nChoisissez la ville : ");
    fgets( villeSimulation, BUFFER, stdin);
    villeSimulation[strlen(villeSimulation)-1] = '\0';
    hashKey = hashing(villeSimulation);
	system("clear");
	return listeVille[hashKey];
}

void creerListeVille(Irradiation *listeVille[]){
    struct dirent *de;  // Pointer for directory entry
  
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(DIR_PATH);
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
    }
  
    printf("Fichiers disponibles pour la simulation :\n");
	int id=0;
    int hashKey;
    FILE *fichier;
    while ((de = readdir(dr)) != NULL)
    {
		/* Ouverture du fichier */
		fichier = NULL;
	
		char nomDuFichier[BUFFER] = DIR_PATH;
        
          
        strcat(nomDuFichier,de->d_name);
        if(strstr(nomDuFichier,".csv") != NULL){
            char ville[BUFFER];
            int i = 0;
            
            while(i != strlen(de->d_name)-4)
            {
                ville[i] = de->d_name[i];
                i++;
            }
            ville[i++] = '\0';
            
            /* Variables */
            
            
            char line[MAX_STR];
            int year;
            char month[MAX_STR];
            float i_opt;
            

            Irradiation *listeIrradiation = malloc(sizeof(*listeIrradiation));
            listeIrradiation->premier = NULL;
            strcpy(listeIrradiation->nomVille,ville);
            MoisIrradiation *moisIrradiation = malloc(sizeof(*moisIrradiation));
            /* Traitement du fichier */
            fichier = fopen(nomDuFichier,"r");

            if(fichier != NULL){  // Vérification de l'ouverture du fichier
                
                for(int k=1;k<=5;k++){  // Saut des premieres lignes
                    fgets(line, sizeof(line), fichier); // Saut de la premiere ligne
                }
                
                while(fgets(line, sizeof(line), fichier) != NULL){
                    fscanf(fichier,"%d		%s		%f",&year,month,&i_opt);
                    
                    moisIrradiation->year = year;
                    moisIrradiation->i_opt = i_opt;
                    strcpy(moisIrradiation->month,month);
                    
                    
                    moisIrradiation->suivant = listeIrradiation->premier;
                    listeIrradiation->premier = moisIrradiation;
                    moisIrradiation = malloc(sizeof(*moisIrradiation));
                    
                }
                fclose(fichier);
                hashKey = hashing(ville);
                listeVille[hashKey] = malloc(sizeof(listeIrradiation));
                listeVille[hashKey] = listeIrradiation;
                
            }
        }
    }
    closedir(dr);
}


void afficherIrradiation(Irradiation *listeIrradiation){
	MoisIrradiation *moisIrradiation;

	int choice = 0;
	while(choice != 1){  // Permet de revenir au menu de la simulation
		moisIrradiation = listeIrradiation->premier;
		while(moisIrradiation != NULL)
		{
			
			printf("%d		%s		%.2f\n",moisIrradiation->year,moisIrradiation->month,moisIrradiation->i_opt);
			moisIrradiation = moisIrradiation->suivant;
			
		}
		printf("Appuyer sur 1 pour revenir au menu: ");
		scanf("%d",&choice);
		
	}
}


float prodAnnee(float surfaceReelle, Irradiation *listeIrradiation){
	float productionAnnuelle = 0; 
	int nombreMois = 1;
	MoisIrradiation *moisIrradiation = listeIrradiation->premier;
	
	
	while(moisIrradiation != NULL){
	
		productionAnnuelle += moisIrradiation->i_opt;
		nombreMois++;
		moisIrradiation = moisIrradiation->suivant;
	}
	productionAnnuelle = (productionAnnuelle*surfaceReelle) / nombreMois;  //Calcul de la moyenne sur un an
	return productionAnnuelle;
}

float besoinEnergetique(Catalogue *catalogueUtilisateur){
	float besoinEnerg = 0;
	int dureeQuot = 0;
	int nbJourUtile = 0;
	
	int equipement_id = 1;
	Equipement *equipement = catalogueUtilisateur->premier;
	if(equipement == NULL){
		printf("Veuillez remplir le catalogue utilisateur avec au moins un équipement.\n");
	}
	
	printf("Veuillez remplir le catalogue utilisateur avec les durées d'utilisations :\n\n");
	printf("******* Catégorie    |   Nom  de l'équipement    |   Puissance (W) *******\n\n");
	while(equipement != NULL){
	
		
		printf("[%d] : %s %s %.0f\n",equipement_id,equipement->categorie,equipement->nom,equipement->puissance);
		printf("Entrez la durée d'utilisation par jour (h) et nombre de jours/an : ");
		scanf("%d %d",&dureeQuot,&nbJourUtile);
		
		
		besoinEnerg += (equipement->puissance)*dureeQuot*nbJourUtile;
		equipement_id++;
		equipement = equipement->suivant;
	}
	besoinEnerg /= 1000.0; //conversion en kWh
	system("clear");
	return besoinEnerg;

}


void lancerSimulation(Irradiation *listeIrradiation, int nPanneaux, Catalogue *catalogueUtilisateur){
	
	system("clear");
	/* Variables de simulation de la production d'energie */
	float puissanceCrete = 0.3*  (float) nPanneaux;
	float surfaceReelle = 1.7 * (float) nPanneaux;
	float productionAnnuelle = prodAnnee(surfaceReelle,listeIrradiation);
	float coutPanneaux = 2500.0 * puissanceCrete; 
	float besoinEnerg = besoinEnergetique(catalogueUtilisateur);
	float kWhPrix = 0.1765;  // Prix du kWh
	int nPanneauxBesoin = (int) 1 + ((nPanneaux* besoinEnerg)/productionAnnuelle); // +1 pour arrondir si le float de gauche est entre 0 et 1
	float dureeRetourInvestissement = coutPanneaux/(productionAnnuelle*kWhPrix); //en Années 
	/******************************************************/
	
	
	/********************************************************************************/
	/* Remarque durreRetourInvestissement =  CSTE car :				*/
	/* coutPanneaux = A*nPanneaux et productionAnnuelle = B*nPanneaux par déf. 	*/
	/* donc cette durée est indépendante du nombre de panneaux 			*/
	/* mais dépendante de l'irradiation par déf de production annuelle (fichier)    */
	/*										*/
	/* proof : coutPanneaux = 2500 * 0.3 * nPanneaux				*/
	/* productionAnnuelle*kWhPrix = nPanneaux * 1.7 * moyenneIrradiation * kWhPrix	*/
	/*										*/	
	/* --> dureeRetourInvestissement = (2500 * 0.3) / (1.7* (environ 150) * 0.1765)	*/
	/*				 = 16 ans environ				*/
	/*Elle est inversionnellemnt proportionnel à l'irradiation moyenne (logique)    */
	/********************************************************************************/
	
	if(productionAnnuelle == 0){  //Gestion du cas d'une prod. annuelle nulle (division par 0)
		nPanneauxBesoin = 0;
		dureeRetourInvestissement = 0;
	}
	
	int dureeRetourInvestissementJour = dureeRetourInvestissement * 365;
	
	
	/************** Variables pour la gestion de l'affichage **************/
	int choice = 0;	//Option du menu de simulation
	int display = 0; //Gere la mise en forme de l'affichage de certaines options
	int maxChoice = 9; // Nombre maximum d'options du menu
	/**********************************************************************/
	
	
	/*Enregistrement des indicateurs liés au retour sur l'investissement */
	FILE *fichier = fopen(INDICATEURS_INV,"w"); // Initialisation du fichier = RAZ
	if(fichier != NULL){
		fprintf(fichier,"Production Annuelle (kWh) | Puissance crête du parc (kWc) | Durée retour sur investissement (ans)\n");
		fprintf(fichier,"%f 				%f 		  	%f",productionAnnuelle,puissanceCrete,dureeRetourInvestissement);
		printf("Les indicateurs retour investissement ont été enregistrés.\n");
		fclose(fichier);
	}
	/*********************************************************************/
	
	//Menu de la simulation
	while(choice != 1)
	{
		
		printf("***************************************************************\n");
		printf("*							      *\n");
		printf("*		Simulation de production d'énergie            *\n");
		printf("*							      *\n");
		printf("* Voulez-vous :                				      *\n");
		printf("* [1] : Retourner au menu principal                           *\n");
		printf("* [2] : Afficher la liste d'irradiation mensuel 	      *\n");
		printf("* [3] : Afficher la production moyenne annuelle 	      *\n");
		printf("* [4] : Afficher la puissance crête des panneaux 	      *\n");
		printf("* [5] : Afficher la surface exploitable réelle  	      *\n");  
		printf("* [6] : Coût de l’installation des panneaux solaires 	      *\n");
		printf("* [7] : Nombre de panneaux solaires pour besoin énergétique   *\n");
		printf("* [8] : Consommation moyenne de la maison par an 	      *\n");
		printf("* [9] : Durée du retour sur investissement 	     	      *\n");
		printf("*							      *\n");
		printf("***************************************************************\n");
		printf("Nombre de panneaux : %d			      		       \n",nPanneaux);
		
		/**************** Mise en forme de l'affichage certaines options ***************************/
		if(display == 1){
			printf("Les besoins énergétiques sont de %.1f kWh par an.\n",besoinEnerg);
			printf("Soit %d panneaux solaires.\n",nPanneauxBesoin);
			choice = 0;
			display = 0;
		}
		if(display == 2){
			//Consommation Moyenne/an = Besoin energetique/an (pour une année)
			printf("La consommation moyenne de la maison par an est de %.1f kWh.\n",besoinEnerg);
			printf("Soit %.2f euros TTC/an.\n",besoinEnerg*kWhPrix);
			choice = 0;
			display = 0;
		}
		/*********************************************************************************************/
		
		switch(choice)
		{
			case 1: 
				// Quitte la simulation
				break;
			
			case 2:
				afficherIrradiation(listeIrradiation);
				break;
			
			case 3:
				printf("La production moyenne annuelle des panneaux solaires est de %.1f kWh.\n",productionAnnuelle);
				printf("Equivalent de %.2f euros TTC/an.\n",productionAnnuelle*kWhPrix);
				break;
			
			case 4:
				printf("La puissance crete pour les %d panneaux est de %.1f kWc.\n",nPanneaux,puissanceCrete);
				break;
			
			case 5:
				printf("La surface exploitable réelle est de %.1f m2.	       \n",surfaceReelle);
				break;
				
			case 6:
				printf("Le coût d'installation de %d panneaux est de %.1f euros.\n",nPanneaux,coutPanneaux);
				break;
			
			case 7:
				display = 1;
				break;
			
			case 8:
				display = 2;
				break;
			case 9:
				printf("La durée associée au retour sur investissement est %.2f ans.\n",dureeRetourInvestissement);
				printf("Soit %d jours.\n",dureeRetourInvestissementJour);
				break;
				
		}
		
		if(choice != 2 && choice != 7 && choice != 8){  // Mise en forme de l'affichage
			printf("Votre sélection: ");
			scanf("%d",&choice);
		}
		else if(choice == 2){
			choice = 0;  //Permet de réinitialiser l'affichage pour l'option 2
		}

		system("clear");
		
	}
	system("clear"); // Reinitialise l'affichage pour le retour au menu principale
	
}


/*********************/
/*Fonctions annexes*/

long int hashing(char *chaine){   // Fonction de hachage
    long int hashKey = 0;
    int k =0;

    while(chaine[k] != '\0'){
        hashKey += chaine[k]*pow(BASE,k);
        k++;
    }
    hashKey %= DIR_SIZE; //Retourne un nombre entre 0 et DIR_SIZE
    return hashKey;
}