/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    19 Mar 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP2 Num. B                      */
/*------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Pour compatibilité avec Windows
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// Utilisée comme fonction d'attente
void emptybuffer()
{
  while (getchar() != '\n');
}

/*
 * Fonction qui prend en paramètre un entier et retourne le
 * Nombre de diviseurs de cet entier
 */
float getdivisors(int num) {
  int i, divisors = 1;

  // Zéro peut être divisé par tout les nombre et le
  // résultat est toujours entier
  if (num == 0) return INFINITY;
  
  for (i = 1; i < num / 2 + 1; i++)
    if ((float)num / i == num / i) divisors++;
  
  return divisors;
}

// Fonction qui répond au sous-numéro 1
void sub1()
{
  int sum1 = 10, sum2 = 10;
  
  do {
    sum1 += sum1;
  } while (sum1 < 640);

  do {
    sum2 += 5;
  } while (sum2 < 100);

  printf("\nSous-numéro 1:\n");
  printf("======================================\n");
  printf("\tSomme 1: %d\n", sum1);
  printf("\tSomme 2: %d\n\n", sum2);
}

// Fonction qui répond au sous-numéro 2
void sub2()
{
  float sum3 = 0;
  int sum4 = 0;
  int denom = 3;
  int number = 1234567;

  while (denom < 100) {
    sum3 += 1.0 / denom;
    denom += 2;
  }

  while (number > 0) {
    sum4 += number % 10;
    number /= 10;
  }

  printf("\nSous-numéro 2:\n");
  printf("======================================\n");
  printf("\tSomme 3: %f\n", sum3);
  printf("\tSomme 4: %d\n\n", sum4);
}

// Fonction qui répond au sous-numéro 3
void sub3()
{
  printf("\nSous-numéro 3:\n");
  printf("======================================\n");
  printf("\t20 a: %d diviseurs\n", (int)getdivisors(20));
  printf("\t6  a: %d diviseurs\n", (int)getdivisors(6));
  printf("\t40 a: %d diviseurs\n\n", (int)getdivisors(40));
}

int main()
{
  system(CLEAR);
  printf("NUMÉRO B:\n");
  printf("***********************************************\n");
  sub1();
  sub2();
  sub3();
  printf("Faites (Enter) pour quitter...\n");
  emptybuffer();
  system(CLEAR);
  return 0;
}
