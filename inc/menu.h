#ifndef DEF_MENU
#define DEF_MENU

/* Déclaration des constantes */

//Catalogue TXT : "catalogue.txt" /  CSV : "puissance_electromenagers.csv" 
#define CATALOG "../data/puissance_electromenagers.csv" 
#define DELIMITER ",\n"  // délimiteurs des données du fichier à lire : strtok(line,DELIMITER)


#define MAX_STR 200 // Taille max. chaine de caractere



/* Déclaration des structures */

/* Equipement du Catalogue */
typedef struct Equipement Equipement;   // Equiment du catalogue (principale ou utilisateur)
struct Equipement {
	char categorie[MAX_STR];
	char nom[MAX_STR];
	float puissance;
	Equipement *suivant;
};


/* Structure de contrôle */
typedef struct Catalogue Catalogue;  // Définit la liste du catalogue
struct Catalogue{
	Equipement *premier;
};


/* Déclaration des prototypes des fonctions */


/* Fonctions principales */ 

/**************************************/
Catalogue *initialisation();  // Initialise la liste du catalogue

void creationCatalogue(Catalogue *catalogue);  // Création du catalogue principale avec le fichier

void afficherCatalogue(Catalogue *catalogue);  // Affiche le catalogue (principale ou utilisateur

void modifierCatalogue(Catalogue *catalogue, Catalogue *catalogueUtilisateur, int choice); // Permet de modifier le catalogue utilisateur ou principale(par le fichier catalogue)

float modifierSurface(float surface); // Surface du toit

/***************************************/

/* Fonctions annexes  */ 

void writeInCatalog();  //Ecrit des données dans le fichier du catalogue

void insertion(Catalogue *catalogue, char categorie[], char nom[],float puissance); // Insertion d'un equipement dans la liste chainee (en LIFO = pile)

void suppression_tete(Catalogue *catalogue); //Supprime la tete de la pile

void suppresion_num(Catalogue *catalogue, int num_suppr);  // Supprime un élement numéroté de la pile

#endif

