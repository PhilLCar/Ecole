/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    11 Feb 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP1 Num. B                      */
/*------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Pour compatibilité avec Windows
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// CONSTANTES
// Facteur de conversion
const float LBTOKG = 0.454;

// VARIABLES GLOBALES
int weight, count = 0, number = 0;
float temp, mean = 0;
char status;

// Fonction de conversion livre vers kilo
float lbtokg(int lbweight)
{
  return lbweight * LBTOKG;
}

// Fonction de conversion Farenheit vers Celsius
float FtoC(float Ftemp)
{
  return 5.0 / 9.0 * (Ftemp - 32);
}

// Vide le buffer après scanf
// Enlève les unités après les données
void emptybuffer()
{
  while(getchar() != '\n');
}

// Fonction spécifiquement dédiée au reccueil du statut civil avec
// une possibilité de demander de l'aide pour se rappeler des statuts
char getstatus()
{
  char c;
  do {
    printf("Quel est le statut civil du patient? Pesez sur (H) pour de l'aide...\n");
    scanf("%c", &c);
    emptybuffer();
    // Choix disponible
    if (toupper(c) == 'H') {
      system(CLEAR);
      printf("Entrez une valeur parmis les suivantes:\n");
      printf("\t- Célibataire (C)\n");
      printf("\t- Marié       (M)\n");
      printf("\t- Divorcé     (D)\n");
      printf("\t- Séparé      (S)\n");
      printf("\t- Autre       (A)\n\n");
      printf("Faites (Entrer) pour continuer");
      emptybuffer();
    }
    system(CLEAR);
    // Réimprime l'écran d'avant pour ne pas désorienter l'usager
    printf("AJOUT D'UN NOUVEAU PATIENT :\n");
    printf("============================\n\n");
    printf("Quel est le poids du patient? [En livres]: %d\n", weight);
    printf("[En kilos]: %0.3f kg\n\n", lbtokg(weight));
    printf("Quel est la température du patient? [En Fareinheits]: %0.2f\n", temp);
    printf("[En Celcius]: %0.3f °C\n\n", FtoC(temp));
  } while(toupper(c) == 'H');
  c = toupper(c);
  if (c != 'C' && c != 'M' && c != 'D' && c != 'S') c = 'A';
  return c;
}

// Indique l'ajout (ou non) d'un patient et affiche les statistiques
void displayresults(int added)
{
  system(CLEAR);
  if (added) printf("Patient ajouté avec succès..\n\n");
  else printf("Le patient n'a pas été ajouté..\n\n");
  printf("Nombre total de patient compatbilisés :           %7d\n", number);
  printf("Moyenne de poids [kg] des patients célibataires : %7.4f\n", lbtokg(mean));
}

// Affiche les informations pour assurer à l'usager qu'il n'a pas d'erreur
void review()
{
  system(CLEAR);
  printf("VOULEZ VOUS AJOUTER CE PATIENT À LA LISTE?\n");
  printf("==========================================\n");
  printf("  Valeurs   |   Métrique   |   Impérial   \n");
  printf("------------------------------------------\n");
  printf("Poids       |");
  printf("%10.3f kg |", lbtokg(weight));
  printf("%6d    lbs \n", weight);
  printf("Température |");
  printf("%10.3f °C |", FtoC(temp));
  printf("%9.2f  °F \n", temp);
  printf("\nStatut : ");
  switch (status) {
  case 'C':
    printf("Célibataire\n");
    break;
  case 'M':
    printf("Marié\n");
    break;
  case 'D':
    printf("Divorcé\n");
    break;
  case 'S':
    printf("Séparé\n");
    break;
  case 'A':
    printf("Autre\n");
    break;
  default:
    displayresults(0);
    return;
  }
  printf("\n\nFaites (O) pour l'ajouter ou (n) pour annuler...\n");
  char c;
  c = getchar();
  if (c != '\n') emptybuffer();
  if (toupper(c) != 'N') {
    number++;
    if (status == 'C')
      mean = (mean * count + weight) / (float)++count;
    displayresults(1);
  } else displayresults(0);
}


// Fonction qui prend en charge l'ajout de patient
int addpatient()
{
  system(CLEAR);
  printf("AJOUT D'UN NOUVEAU PATIENT :\n");
  printf("============================\n\n");
  printf("Quel est le poids du patient? [En livres]: ");
  scanf("%d", &weight);
  emptybuffer();
  printf("[En kilos]: %0.3f kg\n\n", lbtokg(weight));
  printf("Quel est la température du patient? [En Fareinheits]: ");
  scanf("%f", &temp);
  emptybuffer();
  printf("[En Celcius]: %0.3f °C\n\n", FtoC(temp));
  status = getstatus();
  review();
}


int main()
{
  do {
    addpatient();
    printf("Faites (Q) pour quitter, (Entrer) pour ajouter un autre patient...\n");
  } while (toupper(getchar()) != 'Q');
}
