/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    25 Mar 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP3 Num. A                      */
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

#define MIN 1
#define MAX 0

// VARIABLES GLOBALES
int tabcounter = 0;
char poste[8]  = { 'A', 'P', 'O', 'P', 'A', 'P', 'A', 'P' };
int  nbCafe[8] = {  3 ,  1 ,  5 ,  1 ,  2 ,  1 ,  0 ,  3  };
int  age[8]    = { 25 , 18 , 23 , 20 , 50 , 24 , 52 , 29  };

void printheader()
{
  system(CLEAR);
  printf("NUMÉRO A:\n");
  printf("***********************************************\n\n");
}

void printseparator(char sep)
{
  int i;
  for (i = 0; i < 67; i++) printf("%c", sep);
  printf("\n");
}

void prettyprint(char *descr)
{
  int i;
  // Titre
  printf("Tableau (%d):\n\t%s\n", ++tabcounter, descr);
  
  printseparator('=');
  
  // Imprime les postes
  printf("| Postes |");
  for (i = 0; i < 8; i++) printf("|   %c  ", poste[i]);
  printf("|\n");
  printseparator('-');
  
  // Imprime le nombre de cafés
  printf("| Cafés  |");
  for (i = 0; i < 8; i++) printf("|  %2d  ", nbCafe[i]);
  printf("|\n");
  printseparator('-');
  
  // Imprime les ages
  printf("| Age    |");
  for (i = 0; i < 8; i++) printf("|  %2d  ", age[i]);
  printf("|\n");
  printseparator('=');
}

char* getname(char poste)
{
  switch (poste) {
  case 'P':
    return "programmeur";
  case 'O':
    return "opérateur";
  case 'A':
    return "analyste";
  case 'S':
    return "secrétaire";
  default:
    return "poste avec cet attribu";
  }
}

void printnumberof(char key)
{
  int i, j;
  for (i = 0, j = 0; i < 8; i++) {
    if (poste[i] == key) j++;
  }
  printf("\nIl y a au total %d %s%s.\n", j, getname(key), j > 1 ? "s" : "");
}

int getminmax(int comp[], char key, int func)
{
  int i, j = func * 100;
  for (i = 0; i < 8; i++) {
    if (poste[i] == key) {
      if (func) {
	if (comp[i] < j) j = comp[i];
      } else if (comp[i] > j) j = comp[i];
    }
  }
  return j;
}

float getmean(int comp[], char key)
{
  int i, sum = 0, num = 0;
  for (i = 0; i < 8; i++) {
    if (poste[i] == key || key == '*') {
      sum += comp[i];
      num++;
    }
  }
  return (float)sum / (float)num;
}

void swapsort()
{
  int i, j, k;
  
   // Swap sort
  for (i = 0; i < 7; i++)
    for (j = 0; j < 7; j++) {
      if (age[j] > age[j + 1]) {
	
	// Swap poste
	k = poste[j];
	poste[j] = poste[j + 1];
	poste[j + 1] = (char)k;

	// Swap nbCafe
	k = nbCafe[j];
	nbCafe[j] = nbCafe[j + 1];
	nbCafe[j + 1] = k;

	// Swap age
	k = age[j];
	age[j] = age[j + 1];
	age[j + 1] = k;
      }
    }
}

void waitquit()
{
  printf("\nFaites (Enter) pour quitter...");
  
  // Attend que l'usager veuille quitter
  while(getchar() != '\n');

  // Retourne la console propre
  system(CLEAR);
}

int main()
{
  printheader();
  prettyprint("Poste, consommation de café, et âge des employés");

  printnumberof('P');
  printnumberof('S');

  printf("\nLe programmeur qui consomme le moins de café en consomme %d.\n",
	 getminmax(nbCafe, 'P', MIN));
  
  printf("\nL'analyste le plus âgé a %d ans.\n", getminmax(age, 'A', MAX));

  printf("\nLes programmeurs consomment en moyenne %0.1f cafés.\n", getmean(nbCafe, 'P'));
  
  printf("\nLa moyenne d'âge de tout les employés est %0.1f ans.\n\n", getmean(age, '*'));

  swapsort();
  
  ///// DÉBUT IMPRESSION /////
  prettyprint("Premier tableau organisé selon l'âge par ordre croissant");

  waitquit();
    
  return 0;
}
