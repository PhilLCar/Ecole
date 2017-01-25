/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    19 Mar 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP2 Num. A                      */
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
const float PI = 3.141592653589793;

// VARIABLES GLOBALES
int   numfig   = 0;
int   numrect  = 0;
int   numsqr   = 0;
int   minset   = 0;
float maxarea  = 0;
float mincirc  = 0;
float meansqr  = 0;

// Vide le buffer après scanf
// Enlève les unités après les données
void emptybuffer()
{
  while (getchar() != '\n');
}

// Affiche une entête agréable
void printheader()
{
  system(CLEAR);
  printf("***********************************************************************************\n");
  printf("*                          Outil de calcul géométrique                            *\n");
  printf("*                                  (Numéro A)                                     *\n");
  printf("***********************************************************************************\n");
}

// Affiche les statistiques du programme au moment de l'appel
void getstats()
{
  printheader();
  printf("\n                            STATISTIQUES DU PROGRAMME                             \n");
  printf("===================================================================================\n\n");
  printf("\tNombre de figure calculée ............................... : %8d\n", numfig);
  printf("\tNombre de rectangle dont la longueur est supérieure a 7.8 : %8d\n", numrect);
  printf("\tMoyenne du côté des carrés calculés ..................... : %8.3fu\n", meansqr);
  printf("\tPlus petite circonférence calculée ...................... : %8.3fu\n", mincirc);
  printf("\tPlus grande aire calculée ............................... : %8.3fu²\n", maxarea);
  printf("\n\n\nFaites (Enter) revenir au menu principal...\n");
  emptybuffer();
}

// Récupère puis affiche les information sur un cercle
void getcircle()
{
  float r, circ, area;
  printheader();
  printf("\nEntrez le rayon du cercle : ");
  scanf("%f", &r);
  emptybuffer();
  circ = 2 * PI * r;
  area = PI * r * r;
  printf("\nRÉSULTATS:\n");
  printf("=====================================\n\n");
  printf("\tRayon         : %8.3fu\n", r);
  printf("\tCirconférence : %8.3fu\n", circ);
  printf("\tAire          : %8.3fu²\n", area);
  numfig++;
  
  // Le if suivant détermine si le minimum a été calculé
  // afin de supporter le cas ou un cercle aurait une
  // circonférence nulle
  if (circ < mincirc && minset) mincirc = circ;
  else {
    mincirc = circ;
    minset++;
  }
  if (area > maxarea) maxarea = area;
  printf("\n\n\n\nFaites (Enter) pour continuer...");
  emptybuffer();
}

// Récupère puis affiche les information sur un carré
void getsquare()
{
  float a, perim, area;
  printheader();
  printf("\nEntrez le côté du carré : ");
  scanf("%f", &a);
  emptybuffer();
  perim = 4 * a;
  area  = a * a;
  printf("\nRÉSULTATS:\n");
  printf("=====================================\n\n");
  printf("\tCôté      : %8.3fu\n", a);
  printf("\tPérimètre : %8.3fu\n", perim);
  printf("\tAire      : %8.3fu²\n", area);
  numfig++;
  meansqr = (meansqr * numsqr + a) / ++numsqr;
  if (area > maxarea) maxarea = area;
  printf("\n\n\n\nFaites (Enter) pour continuer...");
  emptybuffer();
}

// Récupère puis affiche les information sur un rectangle
void getrect()
{
  float a, b, perim, area;
  printheader();
  printf("\nEntrez la hauteur du rectangle : ");
  scanf("%f", &a);
  emptybuffer();
  printf("Entrez la largeur du rectangle : ");
  scanf("%f", &b);
  emptybuffer();
  perim = 2 * (a + b);
  area  = a * b;
  printf("\nRÉSULTATS:\n");
  printf("=====================================\n\n");
  printf("\tHauteur   : %8.3fu\n", a);
  printf("\tLargeur   : %8.3fu\n", b);
  printf("\tPérimètre : %8.3fu\n", perim);
  printf("\tAire      : %8.3fu²\n", area);
  numfig++;
  if (area > maxarea) maxarea = area;
  
  // Le if suivant détermine quel côté est la longueur, par convention
  // le plus long sera considéré comme tel. 
  if (a > b) {
    if (a > 7.8) numrect++; }
  else if (b > 7.8) numrect++;
  printf("\n\n\n\nFaites (Enter) pour continuer...");
  emptybuffer();
}

// Menu des figures
void menufigs()
{
  int mistake = 0;
  char c;
  while (1) {
    printheader();
    printf("\nAJOUT D'UNE FIGURE:\n");
    printf("=====================================\n");
    if (mistake) {
      printf("\nCe choix n'est pas disponible. ");
      mistake = 0;
    } else printf("\nQuelle figure voulez-vous calculer? ");
    printf("Les choix disponible sont:\n");//q po
    printf("\t- Un cercle    (C)\n");
    printf("\t- Un carré     (K)\n");
    printf("\t- Un rectangle (R)\n\n");
    printf("Ou faites (Q) pour retourner au menu principal...\n");
    c = toupper(getchar());
    if (c != '\n') emptybuffer();
    switch (c) {
    case 'C':
      getcircle();
      break;
    case 'K':
      getsquare();
      break;
    case 'R':
      getrect();
      break;
    case 'Q':
      return;
    case '\n':
      break;
    default:
      mistake = 1;
    }
  }
}

// Menu principal
int main()
{
  int mistake = 0;
  char c;
  while (1) {
    printheader();
    printf("\nMENU PRINCIPAL:\n");
    printf("=====================================\n");
    if (mistake) {
      printf("\nCe choix n'est pas disponible.\n");
      mistake = 0;
    } else printf("\nQue voulez vous faire?\n");
    printf("Choisissez parmis l'une des options suivantes:\n");
    printf("\t(F) Entrer une nouvelle figure\n");
    printf("\t(S) Consulter les statistiques sur les figures\n");
    printf("\t(Q) Quitter\n");
    c = toupper(getchar());
    if (c != '\n') emptybuffer();
    switch (c) {
    case 'F':
      menufigs();
      break;
    case 'S':
      getstats();
      break;
    case 'Q':
      system(CLEAR);
      return 0;
    default:
      mistake = 1;
    }
  }
}
