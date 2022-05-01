#ifndef DEF_SIMULATION
#define DEF_SIMULATION
#include "../inc/menu.h"

/* Déclaration des constantes */

 
#define DIR_PATH "../data/Irradiation/" // Répertoire des fichiers CSV
#define DIR_SIZE 10000
#define INDICATEURS_INV "../data/indicateurs_invest.txt"
#define DELIMITER ",\n"  // délimiteurs des données du fichier à lire : strtok(line,DELIMITER)
#define BUFFER 1000
#define BASE 128 // Pour la fonction de hachage

/* Déclaration des structures */

/* Mois d'irradiation */
typedef struct MoisIrradiation MoisIrradiation;  //Structure de données de chaque mois d'irradiation du fichier
struct MoisIrradiation {
	int year;
	char month[MAX_STR];
	float i_opt;
	MoisIrradiation *suivant;
};

/* Structure de contrôle */
typedef struct Irradiation Irradiation;
struct Irradiation{
	char nomVille[MAX_STR];
	MoisIrradiation *premier;
};


/* Déclaration des prototypes des fonctions */


/* Fonctions principales */ 

/**************************************/
void creerListeVille(Irradiation *listeVille[]); // Table de hachage avec pointeurs sur structure de type Irradiation (chaque fichier csv)

Irradiation *choixVille(Irradiation *listeVille[]);  // Choix d'une ville --> recherche d'un struct dans la hash table

void afficherIrradiation(Irradiation *listeIrradiation);  //Affiche les données du la ville correspondant

float prodAnnee(float surfaceReelle, Irradiation *listeIrradiation); //Production moyenne annuelle des panneaux solaires pour une ville donnée

float besoinEnergetique(Catalogue *catalogueUtilisateur); //Besoin énergétique de la maison

void lancerSimulation(Irradiation *listeIrradiation, int nPanneaux, Catalogue *catalogueUtilisateur);



/***************************************/

/* Fonctions annexes  */ 

/***************************************/
long int hashing(char *chaine);
/***************************************/

#endif
