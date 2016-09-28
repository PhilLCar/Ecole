/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    11 Feb 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP1 Num. A                      */
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

// VARIABLES GLOBALES
float slr;
int axp;

// Vide le buffer après scanf
// Enlève les unités après les données
void emptybuffer()
{
  while (getchar() != '\n');
}

float prime()
{
  return 1.0275 * slr - slr;
}

float surprime()
{
  if (axp < 5)
    return 350;
  else if (axp < 10)
    return 765;
  else
    return 1230;
}

// Affiche une entête agréable
void printheader()
{
  printf("****************************************************************\n");
  printf("*                     Société LA GÉNÉREUSE                     *\n");
  printf("*                  Outil de calcul de la prime                 *\n");
  printf("****************************************************************\n");
}

/* getinfo()
 * Cette fonction gère l'ajout d'un nouvel employé
 */
void getinfo()
{
  system(CLEAR);
  printheader();
  printf("\nEntrez les informations suivantes sur l'employé...\n");
  printf("================================================================\n\n");
  printf("Quel est le salaire de l'employé?                ");
  scanf("%f", &slr);
  emptybuffer();
  printf("Combien d'années d'expérience a-t-il accumulées? ");
  scanf("%d", &axp);
  emptybuffer();
}

// Affiche un string de taille uniforme pour une valeur monétaire
void displaycash(char* cat, float val)
{
  printf("%-32s", cat);
  printf("%30.2f $\n", val);
}

// Affiche un string de taille uniforme pour une donnée de temps
void displayyears(char* cat, int val)
{
  printf("%-32s", cat);
  if (val > 1)
    printf("%30d ans\n", val);
  else
    printf("%30d  an\n", val);
}

// Afficher toutes les informations connues sur l'employé
void displayall()
{
  system(CLEAR);
  printheader();
  printf("\n                -  BILAN GÉNÉRAL DE L'EMPLOYÉ  -                \n");
  printf("================================================================\n\n");
  printf("Données\n");
  printf("----------------------------------------------------------------\n");
  displayyears("Années d'expériences", axp);
  displaycash("Salaire de base", slr);
  printf("\nBonus\n");
  printf("----------------------------------------------------------------\n");
  displaycash("Prime", prime());
  displaycash("Surprime", surprime());
  printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
  displaycash("Total", prime() + surprime());
  printf("\nSalaire\n");
  printf("----------------------------------------------------------------\n");
  displaycash("Salaire de base", slr);
  displaycash("Bonus", prime() + surprime());
  printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
  displaycash("Total", 1.0275 * slr + surprime());
}

// Affiche le menu
int menu()
{
  char c;
  do {
    system(CLEAR);
    printheader();
    printf("Que voulez-vous afficher?\n");
    printf("\t(A) Le salaire avant prime\n");
    printf("\t(P) La prime\n");
    printf("\t(S) La surprime\n");
    printf("\t(T) La prime totale\n");
    printf("\t(B) Le salaire bonifié\n");
    printf("\t(E) Le nombre d'année d'expérience\n");
    printf("\t(G) Un bilan général de l'employé\n\n");
    printf("Faites (M) pour revenir au menu principal\n");
    printf("ou (Q) pour quitter ...\n");
  badinput:
    scanf("%c", &c);
    emptybuffer();
    switch(toupper(c)) {
    case 'A':
      displaycash("Salaire avant la prime :", slr);
      break;
    case 'P':
      displaycash("Prime :", prime());
      break;
    case 'S':
      displaycash("Surprime :", surprime());
      break;
    case 'T':
      displaycash("Prime totale :", prime() + surprime());
      break;
    case 'B':
      displaycash("Salaire total :", 1.0275 * slr + surprime());
      break;
    case 'E':
      displayyears("Années d'expériences :", axp);
      break;
    case 'G':
      displayall();
      break;
    case 'M':
      return 1;
    case 'Q':
      return 0;
    default:
      // Le texte change pour alerter l'usager d'un mauvais usage
      system(CLEAR);
      printheader();
      printf("Assurez-vous d'entrer une option parmis les choix suivants:\n");
      printf("\t(A) Le salaire avant prime\n");
      printf("\t(P) La prime\n");
      printf("\t(S) La surprime\n");
      printf("\t(T) La prime totale\n");
      printf("\t(B) Le salaire bonifié\n");
      printf("\t(E) Le nombre d'année d'expérience\n");
      printf("\t(G) Un bilan général de l'employé\n\n");
      printf("Faites (M) pour revenir au menu principal\n");
      printf("ou (Q) pour quitter ...\n");
      goto badinput;
    }
    printf("\nAppuyez sur (C) pour revenir aux choix,");
    printf("\nsinon sur (Entrer) pour terminer...");
    c = getchar();
    if (c != '\n') emptybuffer();
  } while(toupper(c) == 'C');
}

int main()
{
  char c;
  do {
    getinfo();
    if (!menu()) break;
    system(CLEAR);
    printheader();
    printf("\n\nVoulez-vous calculer la prime d'un autre employé? (O/n)");
    c = getchar();
    if (c != '\n') emptybuffer();
  } while(toupper(c) != 'N');
}
