/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    19 Mar 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP2 Num. C                      */
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

int main()
{
  char poste[8]  = { 'A', 'P', 'O', 'P', 'A', 'P', 'A', 'P' };
  int  nbCafe[8] = {  3 ,  1 ,  5 ,  1 ,  2 ,  1 ,  0 ,  3  };
  int  age[8]    = { 25 , 18 , 23 , 20 , 50 , 24 , 52 , 29  };
  int i, j, k;
  char c;
  
  system(CLEAR);
  printf("NUMÉRO C:\n");
  printf("***********************************************************\n\n");

  ///// DÉBUT IMPRESSION /////
  printf("Tableau (1):\n\tPoste, consommation de café, et âge des employés\n");
  printf("==========");
  for (i = 0; i < 8; i++) printf("======");
  printf("=\n");
  
  // Imprime les postes
  printf("| Postes |");
  for (i = 0; i < 8; i++) printf("|  %c  ", poste[i]);
  printf("|\n");
  printf("----------");
  for (i = 0; i < 8; i++) printf("------");
  printf("-\n");
  
  // Imprime le nombre de cafés
  printf("| Cafés  |");
  for (i = 0; i < 8; i++) printf("| %2d  ", nbCafe[i]);
  printf("|\n");
  printf("----------");
  for (i = 0; i < 8; i++) printf("------");
  printf("-\n");
  
  // Imprime les ages
  printf("| Age    |");
  for (i = 0; i < 8; i++) printf("| %2d  ", age[i]);
  printf("|\n");
  printf("==========");
  for (i = 0; i < 8; i++) printf("======");
  printf("=\n");
  ///// FIN IMPRESSION /////

  // Calcul du nombre de programmeur
  for (i = 0, j = 0; i < 8; i++) {
    if (poste[i] == 'P') j++;
  }
  printf("\nIl y a au total %d programmeurs.\n", j);

  // Calcul du nombre de programmeur
  for (i = 0, j = 0; i < 8; i++) {
    if (poste[i] == 'S') j++;
  }
  printf("\nIl y a %d secrétaires.\n", j);

  // Calcul de la plus faible consommation de café
  for (i = 0, j = 100; i < 8; i++) {
    if (poste[i] == 'P' && nbCafe[i] < j)
      j = nbCafe[i];
  }
  printf("\nLe programmeur qui consomme le moins de café en consomme %d.\n", j);

  // Calcul de l'age de l'analyste le plus vieux
  for (i = 0, j = 0; i < 8; i++) {
    if (poste[i] == 'A' && age[i] > j)
      j = age[i];
  }
  printf("\nL'analyste le plus âgé a %d ans.\n", j);

  // Calcul de la consommation moyenne des programmeurs
  for (i = 0, j = 0, k = 0; i < 8; i++)
    if (poste[i] == 'P') {
      j++;
      k += nbCafe[i];
    }
  printf("\nLes programmeurs consomment en moyenne %0.1f cafés.\n", ((float)k / j));

  // Calcul de l'age moyen des employés
  for (i = 0, j = 0; i < 8; i++) {
    j += age[i];
  }
  printf("\nLa moyenne d'âge de tout les employés est %0.1f ans.\n\n", ((float)j / 8.0));

  // Swap sort
  for (i = 0; i < 7; i++)
    for (j = 0; j < 7; j++) {
      if (age[j] > age[j + 1]) {
	
	// Swap poste
	c = poste[j];
	poste[j] = poste[j + 1];
	poste[j + 1] = c;

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

  ///// DÉBUT IMPRESSION /////
  printf("Tableau (2):\n\tPremier organisé selon l'âge par ordre croissant\n");
  printf("==========");
  for (i = 0; i < 8; i++) printf("======");
  printf("=\n");
  
  // Imprime les postes
  printf("| Postes |");
  for (i = 0; i < 8; i++) printf("|  %c  ", poste[i]);
  printf("|\n");
  printf("----------");
  for (i = 0; i < 8; i++) printf("------");
  printf("-\n");
  
  // Imprime le nombre de cafés
  printf("| Cafés  |");
  for (i = 0; i < 8; i++) printf("| %2d  ", nbCafe[i]);
  printf("|\n");
  printf("----------");
  for (i = 0; i < 8; i++) printf("------");
  printf("-\n");
  
  // Imprime les ages
  printf("| Age    |");
  for (i = 0; i < 8; i++) printf("| %2d  ", age[i]);
  printf("|\n");
  printf("==========");
  for (i = 0; i < 8; i++) printf("======");
  printf("=\n");
  ///// FIN IMPRESSION /////

  printf("\nFaites (Enter) pour quitter...");
  
  // Attend que l'usager veuille quitter
  while(getchar() != '\n');

  // Retourne la console propre
  system(CLEAR);
    
  return 0;
}
